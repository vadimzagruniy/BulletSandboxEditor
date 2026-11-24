#include "SceneNodeTreeWindow.h"

#include "core/scene/SceneNode.h"
#include "editor/scene_editor/SceneEditorWorkspace.h"
#include "editor/ui/WindowIDs.h"
#include "core/scene/ObjectFactory.h"

#include "imgui.h"

namespace ui
{

namespace // unnamed
{
  
int CharFilter(
    ImGuiInputTextCallbackData * _Data
  )
{
  if (_Data->EventFlag != ImGuiInputTextFlags_CallbackCharFilter)
    return 0;

  const bool IsCharValid = (_Data->EventChar >= 'a' && _Data->EventChar <= 'z') ||
                           (_Data->EventChar >= 'A' && _Data->EventChar <= 'Z') ||
                           (_Data->EventChar >= '0' && _Data->EventChar <= '9') ||
                           (_Data->EventChar == '_');

  return IsCharValid ? 0 : 1;
}

} // namespace unnamed

//
// Constructor/Destructor
//

CSceneNodeTreeWindow::CSceneNodeTreeWindow():
  CDockedWindow(SCENE_NODE_TREE, true)
{
  // Empty
}

//
// CBaseWindow events
//

void CSceneNodeTreeWindow::OnRenderGui()
{
  const auto SceneWorkspace = m_SceneWorkspace.lock();
  if (SceneWorkspace == nullptr)
    return;

  ImGui::BeginDisabled(SceneWorkspace->IsSimulating());

  RenderToolBar(*SceneWorkspace);

  ImGui::Separator();

  RenderNode(*SceneWorkspace, SceneWorkspace->GetRootNode());

  if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Delete))
    SceneWorkspace->DeleteSelectedNode();

  ImGui::EndDisabled();
}

//
// Interface
//

void CSceneNodeTreeWindow::SetWorkspace(std::weak_ptr<editor::CSceneEditorWorkspace> _SceneWorkspace)
{
  m_SceneWorkspace = _SceneWorkspace;
}

//
// Service
//


void CSceneNodeTreeWindow::RenderNode(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _SceneNode)
{
  ImGuiTreeNodeFlags Flags    = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
  const auto         Children = _SceneNode->GetChildren(false);

  if (_Workspace.IsNodeSelected(_SceneNode))
    Flags |= ImGuiTreeNodeFlags_Selected;

  if (Children.empty())
    Flags |= ImGuiTreeNodeFlags_Leaf;

  const bool IsOpen = ImGui::TreeNodeEx(("###" + _SceneNode->GetName()).c_str(), Flags);

  if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen())
    _Workspace.SelectNode(_SceneNode);

  RenderNodeContectMenu(_Workspace, _SceneNode);

  ImGui::SameLine();
  RenderLabel(_Workspace, _SceneNode);

  if (!IsOpen)
    return;

  for (const auto & Child : Children)
    RenderNode(_Workspace, std::static_pointer_cast<core::CSceneNode>(Child));

  ImGui::TreePop();
}

void CSceneNodeTreeWindow::RenderAddNodeMenu(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _Parent)
{
  const auto TypeNames = core::GetObjectFactory().GetTypeNames();
  for (const auto & TypeName : TypeNames)
    if (ImGui::MenuItem(TypeName.c_str()))
      _Workspace.CreateNode(TypeName, _Parent);
}

void CSceneNodeTreeWindow::RenderNodeContectMenu(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _SceneNode)
{
  if (ImGui::BeginPopupContextItem((_SceneNode->GetName() + "NodePopup").c_str()))
  {
    _Workspace.SelectNode(_SceneNode);

    if (ImGui::BeginMenu("Add Child"))
    {
      RenderAddNodeMenu(_Workspace, _SceneNode);
      ImGui::EndMenu(); 
    }

    if (ImGui::MenuItem("Rename"))
      OnStartRename(_SceneNode->GetName());

    if (!_SceneNode->IsRoot() && ImGui::MenuItem("Delete"))
      _Workspace.DeleteNode(_SceneNode);

    ImGui::Separator();

    if (!_SceneNode->IsRoot() && ImGui::MenuItem("Copy"))
      _Workspace.CopyNode(_SceneNode);

    if (_Workspace.CanPaste() && ImGui::MenuItem("Paste"))
      _Workspace.PasteToNode(_SceneNode);

    ImGui::EndPopup();
  }
}

void CSceneNodeTreeWindow::RenderToolBar(editor::CSceneEditorWorkspace & _Workspace)
{
  const std::string PopUpID = "##AddNodePopUp";

  if (ImGui::Button("+ Add"))
    ImGui::OpenPopup(PopUpID.c_str());

  ImGui::SetItemTooltip("%s", "Add new node to scene");

  if (ImGui::BeginPopup(PopUpID.c_str()))
  {
    RenderAddNodeMenu(_Workspace, _Workspace.GetRootNode());
    ImGui::EndPopup();
  }

  ImGui::SameLine();

  const auto SelectedNode = _Workspace.GetSelectedNode();
  ImGui::BeginDisabled(SelectedNode == nullptr || SelectedNode->IsRoot());

  if (ImGui::Button("Delete"))
    _Workspace.DeleteNode(SelectedNode);

  ImGui::SetItemTooltip("%s", "Delete selected node");

  ImGui::EndDisabled();
}

void CSceneNodeTreeWindow::RenderLabel(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _SceneNode)
{
  if (_SceneNode->GetName() == m_RenameData.RenamingID)
  {
    RenderRename(_Workspace, _SceneNode);
  }
  else
  {
    ImGui::Text("%s", _SceneNode->GetName().c_str());
  }
}

void CSceneNodeTreeWindow::RenderRename(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _SceneNode)
{
  ImGui::SetKeyboardFocusHere();

  constexpr auto InputFlags    = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCharFilter;
  const     bool IsTextEntered = ImGui::InputText(("###Renaming" + m_RenameData.RenamingID).c_str(), m_RenameData.RenamingBuffer.data(), m_RenameData.RenamingBuffer.size(), InputFlags, CharFilter, this);

  m_RenameData.NeedStopRename &= !ImGui::IsItemHovered();

  const auto RenamingBuffer = std::string(m_RenameData.RenamingBuffer.data());
  const bool IsIDChanged    = RenamingBuffer != m_RenameData.RenamingID;
  const bool IsNodeExist    = IsIDChanged && !_SceneNode->CanBeIdentifiedAs(RenamingBuffer);

  if (IsNodeExist)
  {
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(255, 0, 0, 255));
    ImGui::SetTooltip("Name '%s' is already exist", m_RenameData.RenamingBuffer.data());
    ImGui::PopStyleColor();
  }

  const bool NeedApplyChanges = (m_RenameData.NeedStopRename || IsTextEntered) &&
                              !RenamingBuffer.empty()                          &&
                               IsIDChanged                                     &&
                              !IsNodeExist;

  if (NeedApplyChanges)
    _Workspace.RenameNode(m_RenameData.RenamingID, m_RenameData.RenamingBuffer.data());

  if (m_RenameData.NeedStopRename || ImGui::IsItemDeactivated() || NeedApplyChanges)
  {
    m_RenameData.NeedStopRename = false;
    m_RenameData.RenamingID.clear();
  }
}

//
// Events
//

void CSceneNodeTreeWindow::OnStartRename(const std::string & _CurrentName)
{
  m_RenameData.NeedStopRename = false;
  m_RenameData.RenamingID     = _CurrentName;

  strncpy_s(m_RenameData.RenamingBuffer.data(), m_RenameData.RenamingBuffer.size(), m_RenameData.RenamingID.c_str(), m_RenameData.RenamingID.size());
}

} // namespace ui