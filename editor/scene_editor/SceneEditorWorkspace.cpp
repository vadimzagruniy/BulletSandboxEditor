#include "SceneEditorWorkspace.h"
#include "SceneDocument.h"

#include "ui/SceneNodeTreeWindow.h"
#include "ui/SceneObjectsWindow.h"
#include "ui/ScenePropertyWindow.h"
#include "drop_handler/SceneObjectDropHandler.h"
#include "core/scene/SceneNode.h"
#include "core/scene/ObjectFactory.h"
#include "editor/scene_editor/SceneSimulator.h"
#include "editor/ui/IWindowHost.h"

namespace editor
{

namespace // unnmaed
{

const ImU32 ROOT_BOUNDS_COLOR      = IM_COL32(0, 255, 255, 255);
const ImU32 SIMUlATION_PLAY_COLOR  = IM_COL32(0, 255, 125, 255);
const ImU32 SIMUlATION_PAUSE_COLOR = IM_COL32(255, 255, 0, 255);

constexpr float SIMULATION_RECT_THICKNESS = 5.0f;
constexpr float ROOT_BOUNDS_THICKNESS     = 4.0f;

} // namespace unnamed

//
// Constructor
//

CSceneEditorWorkspace::CSceneEditorWorkspace(std::shared_ptr<CSceneDocument> _AssetDocument):
  m_SceneDocument(std::move(_AssetDocument)),
  m_SceneSimulator(std::make_shared<CSceneSimulator>(m_SceneDocument)),
  m_NodeTreeWindow(std::make_shared<ui::CSceneNodeTreeWindow>()),
  m_SceneObjectsWindow(std::make_shared<ui::CSceneObjectsWindow>()),
  m_ScenePropertyWindow(std::make_shared<ui::CScenePropertyWindow>())
{
  m_DropHandlers.push_back(std::make_shared<CSceneObjectDropHandler>(*this));
}

//
// CEditorWorkspace
//

void CSceneEditorWorkspace::Update(float _DeltaTime)
{
  if (m_SceneSimulator->IsSimulating())
    m_SceneSimulator->Update(_DeltaTime);
  else
    m_SceneDocument->Update(_DeltaTime);
}

//
// CEditorWorkspace events
//

void CSceneEditorWorkspace::OnActivate(const std::weak_ptr<ui::IWindowHost> & _WindowHost)
{
  CEditorWorkspace::OnActivate(_WindowHost);

  m_NodeTreeWindow->SetWorkspace(shared_from_this());
  m_ScenePropertyWindow->SetWorkspace(shared_from_this());

  if (const auto WindowManager = m_WindowHost.lock())
  {
    WindowManager->AddDockedWindow(m_NodeTreeWindow);
    WindowManager->AddDockedWindow(m_SceneObjectsWindow);
    WindowManager->AddDockedWindow(m_ScenePropertyWindow);
  }
}

void CSceneEditorWorkspace::OnDeactivate()
{
  m_SceneSimulator->Stop();

  m_NodeTreeWindow->SetWorkspace({});
  m_ScenePropertyWindow->SetWorkspace({});

  if (const auto WindowManager = m_WindowHost.lock())
  {
    WindowManager->RemoveDockedWindow(m_NodeTreeWindow);
    WindowManager->RemoveDockedWindow(m_SceneObjectsWindow);
    WindowManager->RemoveDockedWindow(m_ScenePropertyWindow);
  }

  CEditorWorkspace::OnDeactivate();
}

void CSceneEditorWorkspace::OnRenderContent(const core::CCamera & _Camera)
{
  if (m_SceneSimulator->IsSimulating())
  {
    m_SceneSimulator->Render(_Camera);
    RenderSimulationBounds(_Camera);
    return;
  }

  RenderRootBounds(_Camera);

  m_SceneDocument->Render(_Camera);

  if (const auto Node = m_Selection.GetSelected())
  {
    const auto SceneNode = std::static_pointer_cast<core::CSceneNode>(Node);
    m_TransformGizmo.Render(_Camera, *SceneNode);
  }

  if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Delete))
    DeleteSelectedNode();

  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_C, ImGuiInputFlags_RouteGlobal))
    CopySelectedNode();

  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_V, ImGuiInputFlags_RouteGlobal))
    PasteToSelectedNode();
}

//
// Interface
//

bool CSceneEditorWorkspace::IsNodeSelected(const std::shared_ptr<core::CSceneNode> & _SceneNode) const
{
  return m_Selection.IsSelected(std::static_pointer_cast<core::CNode>(_SceneNode));
}

void CSceneEditorWorkspace::SelectNode(const std::shared_ptr<core::CSceneNode> & _SceneNode)
{
  m_Selection.Select(_SceneNode);
}

std::shared_ptr<core::CSceneNode> CSceneEditorWorkspace::GetRootNode() const
{
  return m_SceneDocument->GetRootNode();
}

std::shared_ptr<core::CSceneNode> CSceneEditorWorkspace::GetSelectedNode() const
{
  return std::static_pointer_cast<core::CSceneNode>(m_Selection.GetSelected());
}

bool CSceneEditorWorkspace::CreateNode(const std::string & _TypeID, const std::shared_ptr<core::CSceneNode> & _Parent)
{
  return CreateNode(_TypeID, _Parent, m_Camera.ToWorld(m_CanvasCenter));
}

bool CSceneEditorWorkspace::CreateNode(const std::string & _TypeID, const std::shared_ptr<core::CSceneNode> & _Parent, const ImVec2 & _WorldPos)
{
  const auto Node = m_SceneDocument->CreateNode(_TypeID, _Parent, _WorldPos);
  if (Node == nullptr)
    return false;

  SelectNode(Node);

  return true;
}

void CSceneEditorWorkspace::DeleteNode(const std::shared_ptr<core::CSceneNode> & _Node)
{
  if (m_SceneDocument->DeleteNode(_Node))
    SelectNode(nullptr);
}

void CSceneEditorWorkspace::RenameNode(const std::string & _Name, const std::string & _NewName)
{
  m_SceneDocument->RenameNode(_Name, _NewName);
}

void CSceneEditorWorkspace::DeleteSelectedNode()
{
  DeleteNode(GetSelectedNode());
}

void CSceneEditorWorkspace::CopyNode(const std::shared_ptr<core::CSceneNode> & _Node)
{
  m_NodeCopyBuffer.MakeSnapshot(_Node);
}

void CSceneEditorWorkspace::CopySelectedNode()
{
  if (const auto SelectedNode = GetSelectedNode())
    CopyNode(SelectedNode);
}

void CSceneEditorWorkspace::PasteToNode(const std::shared_ptr<core::CSceneNode> & _Node)
{
  m_SceneDocument->PasteNode(m_NodeCopyBuffer, _Node);
}

void CSceneEditorWorkspace::PasteToSelectedNode()
{
  if (const auto SelectedNode = GetSelectedNode())
    m_SceneDocument->PasteNode(m_NodeCopyBuffer, SelectedNode);
}

bool CSceneEditorWorkspace::IsSimulating() const
{
  return m_SceneSimulator->IsSimulating();
}

bool CSceneEditorWorkspace::CanPaste() const
{
  return !m_NodeCopyBuffer.IsEmpty();
}

//
// CEditorWorkspace Events
//

void CSceneEditorWorkspace::OnRenderToolBar()
{
  const auto SimState  = m_SceneSimulator->GetState();
  const bool IsPaused  = SimState == CSceneSimulator::ESimulationState::Paused;
  const bool IsPlaying = SimState != CSceneSimulator::ESimulationState::Stoped;

  ImGui::BeginDisabled(IsPlaying);
  
  if (ImGui::Button("Play"))
    m_SceneSimulator->Start();

  ImGui::SetItemTooltip("%s", "Start simulation");

  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::BeginDisabled(!IsPlaying);

  if (ImGui::Button(IsPaused ? "Resume" : "Pause"))
    m_SceneSimulator->PauseResume();

  ImGui::SetItemTooltip("%s", "Pause|Resume simulation");

  ImGui::SameLine();

  if (ImGui::Button("Stop"))
    m_SceneSimulator->Stop();

  ImGui::SetItemTooltip("%s", "Stop simulation");

  ImGui::EndDisabled();
  ImGui::Separator();
}

void CSceneEditorWorkspace::OnCanvasMouseDown(const ImVec2 & _ScreenPos)
{
  if (m_SceneSimulator->IsSimulating())
    return;

  if (TryBeginDragGizmo(_ScreenPos))
    return;

  const auto Node = m_SceneDocument->FindNodeByPosition(m_Camera.ToWorld(_ScreenPos));
  SelectNode(Node);

  if (Node != nullptr)
    m_TransformGizmo.BeginDrag(m_Camera, Node, CTransformGizmo::THandlePoint{ CTransformGizmo::EDragMode::Move }, _ScreenPos);
}

void CSceneEditorWorkspace::OnCanvasMouseUp(const ImVec2 & _ScreenPos)
{
  if (m_TransformGizmo.EndDrag())
    OnNodePropertyChanged();
}

void CSceneEditorWorkspace::OnCanvasMouseDrag(const ImVec2 & _ScreenPos)
{
  if (m_TransformGizmo.IsDragging() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    m_TransformGizmo.ContinueDrag(m_Camera, _ScreenPos);
}

std::shared_ptr<CAssetDocument> CSceneEditorWorkspace::GetAssetDocument()
{
  return m_SceneDocument;
}

bool CSceneEditorWorkspace::TryBeginDragGizmo(const ImVec2 & _ScreenPos)
{
  const auto SelectedNode = GetSelectedNode();
  if (SelectedNode == nullptr)
    return false;

  const auto Handle     = m_TransformGizmo.HandleHitTest(m_Camera, *SelectedNode, _ScreenPos);
  const bool DragActive = Handle.Mode != CTransformGizmo::EDragMode::None;

  if (DragActive)
    m_TransformGizmo.BeginDrag(m_Camera, SelectedNode, Handle, _ScreenPos);

  return DragActive;
}

//
// Service
//

void CSceneEditorWorkspace::RenderRootBounds(const core::CCamera & _Camera) const
{
  core::CSceneNode::TCorners Corners;
  GetRootNode()->GetCorners(Corners);

  for (auto & Corner : Corners)
    Corner = _Camera.ToScreen(Corner);

  auto * DrawList = ImGui::GetWindowDrawList();
  DrawList->AddQuad(Corners[0], Corners[1], Corners[2], Corners[3], ROOT_BOUNDS_COLOR, ROOT_BOUNDS_THICKNESS);
}

void CSceneEditorWorkspace::RenderSimulationBounds(const core::CCamera & _Camera) const
{
  if (!m_SceneSimulator->IsSimulating())
    return;

  const ImVec2 RectMin = m_CanvasPosition;
  const ImVec2 RectMax = m_CanvasPosition + m_CanvasSize;

  const bool IsPaused = m_SceneSimulator->GetState() == CSceneSimulator::ESimulationState::Paused;

  auto * DrawList = ImGui::GetWindowDrawList();
  DrawList->AddRect(RectMin, RectMax, IsPaused ? SIMUlATION_PAUSE_COLOR : SIMUlATION_PLAY_COLOR, 1.0f, SIMULATION_RECT_THICKNESS);
}

//
// Events
//

void CSceneEditorWorkspace::OnNodePropertyChanged()
{
  m_SceneDocument->MarkDirty();
}

} // namespace editor