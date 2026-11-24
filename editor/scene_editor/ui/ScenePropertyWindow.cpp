#include "ScenePropertyWindow.h"

#include "core/scene/ObjectFactory.h"
#include "editor/scene_editor/property/PropertyInspector.h"
#include "editor/scene_editor/SceneEditorWorkspace.h"
#include "editor/ui/WindowIDs.h"
#include "imgui.h"

namespace ui
{

//
// Constructor/Destructor
//

CScenePropertyWindow::CScenePropertyWindow():
  CDockedWindow(SCENE_PROPERTY, true),
  m_PropertyInspector(std::make_shared<editor::CPropertyInspector>())
{
  // Empty
}

//
// CBaseWindow events
//

void CScenePropertyWindow::OnRenderGui()
{
  const auto SceneWorkspace = m_SceneWorkspace.lock();
  if (SceneWorkspace == nullptr)
    return;

  const auto SelectedNode = SceneWorkspace->GetSelectedNode();
  if (SelectedNode == nullptr)
    return;

  ImGui::BeginDisabled(SceneWorkspace->IsSimulating());

  RenderHeader(*SceneWorkspace, SelectedNode);
  ImGui::Separator();

  m_PropertyInspector->ResetChanges();

  SelectedNode->InspectProperties(m_PropertyInspector);

  if (m_PropertyInspector->HasChanges())
    SceneWorkspace->OnNodePropertyChanged();

  ImGui::EndDisabled();
}

//
// Interface
//

void CScenePropertyWindow::SetWorkspace(std::weak_ptr<editor::CSceneEditorWorkspace> _SceneWorkspace)
{
  m_SceneWorkspace = std::move(_SceneWorkspace);
}

//
// Service
//

void ui::CScenePropertyWindow::RenderHeader(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _SceneNode)
{
  const std::string TypeID(_SceneNode->GetTypeID());
  ImGui::TextDisabled("TypeID: %s", TypeID.c_str());
  ImGui::TextDisabled("Name: %s", _SceneNode->GetName().c_str());
}

} // namespace ui