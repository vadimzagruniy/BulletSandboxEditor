#include "EditorWorkspace.h"

#include "drop_handler/IWorkspaceDropHandler.h"
#include "core/camera/Camera.h"

namespace editor
{

namespace // unnmaed
{

const ImU32 AXIS_X_COLOR = IM_COL32(200, 70, 70, 255);
const ImU32 AXIS_Y_COLOR = IM_COL32(70, 200, 90, 255);

} // namespace unnamed

CEditorWorkspace::CEditorWorkspace() = default;

CEditorWorkspace::~CEditorWorkspace() = default;

//
// Interface
//

void CEditorWorkspace::Update(float _DeltaTime)
{
  // Empty
}

void CEditorWorkspace::Render()
{
  OnRenderToolBar();

  if (!ImGui::BeginChild("Workspace"))
  {
    ImGui::EndChild();
    return;
  }

  m_CanvasPosition = ImGui::GetCursorScreenPos();
  m_CanvasSize     = ImGui::GetContentRegionAvail();
  m_CanvasCenter   = m_CanvasPosition + m_CanvasSize * 0.5f;

  ImGui::InvisibleButton("##Canvas", m_CanvasSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

  const bool   IsCanvasHovered   = ImGui::IsItemHovered();
  const bool   IsCanvasActive    = ImGui::IsItemActive();
  const bool   IsCanvasClicked   = ImGui::IsItemClicked(ImGuiMouseButton_Left);
  const bool   IsLeftBtnDown     = IsCanvasActive && ImGui::IsMouseDown(ImGuiMouseButton_Left);
  const bool   IsLeftBtnReleased = ImGui::IsItemDeactivated() && ImGui::IsMouseReleased(ImGuiMouseButton_Left);
  const auto & IO                = ImGui::GetIO();

  m_Camera.SetPosition(m_CanvasPosition);

  RenderZeroAxes(m_Camera);
  HadleDropTarget();

  if (IsCanvasClicked)
    OnCanvasMouseDown(IO.MousePos);
  else
  if (IsLeftBtnDown)
    OnCanvasMouseDrag(IO.MousePos);

  if (IsLeftBtnReleased)
    OnCanvasMouseUp(IO.MousePos);

  if (IsCanvasActive && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
    m_Camera.UpdateOffset(IO.MouseDelta);

  if (IsCanvasHovered && IO.MouseWheel != 0.0f)
    m_Camera.UpdateZoom(IO.MouseWheel, IO.MousePos);

  OnRenderContent(m_Camera);

  ImGui::EndChild();
}

//
// Events
//

void CEditorWorkspace::OnRenderToolBar()
{
  // Empty
}

void CEditorWorkspace::OnActivate(const std::weak_ptr<ui::IWindowHost> & _WindowHost)
{
  m_WindowHost = _WindowHost;
}

void CEditorWorkspace::OnDeactivate()
{
  m_WindowHost.reset();
}

//
// Service
//

void CEditorWorkspace::HadleDropTarget()
{
  if (!ImGui::BeginDragDropTarget())
    return;

  const ImGuiPayload * Payload = ImGui::GetDragDropPayload();
  if (Payload == nullptr || Payload->Data == nullptr)
  {
    ImGui::EndDragDropTarget();
    return;
  }

  TWorkspaceDropContext Context;
  Context.PayloadType = Payload->DataType;
  Context.Value       = static_cast<const char *>(Payload->Data);
  Context.WorldPos    = m_Camera.ToWorld(ImGui::GetIO().MousePos);

  const auto DropHendler = GetDropHendler(Context);
  if (DropHendler == nullptr)
  {
    ImGui::EndDragDropTarget();
    return;
  }

  ImGui::SetTooltip("%s", DropHendler->GetToolTip(Context).c_str());

  if (ImGui::AcceptDragDropPayload(Payload->DataType))
    DropHendler->Drop(Context);

  ImGui::EndDragDropTarget();
}

std::shared_ptr<IWorkspaceDropHandler> CEditorWorkspace::GetDropHendler(const TWorkspaceDropContext & _Context) const
{
  for (const auto & DropHandler : m_DropHandlers)
    if (DropHandler->CanHandle(_Context))
      return DropHandler;

  return nullptr;
}

void CEditorWorkspace::RenderZeroAxes(const core::CCamera & _Camera) const
{
  ImVec2 RectMin = m_CanvasPosition;
  ImVec2 RectMax = m_CanvasPosition + m_CanvasSize;
  ImVec2 Origin  = _Camera.ToScreen(ImVec2(0.0f, 0.0f));

  auto * DrawList = ImGui::GetWindowDrawList();
  DrawList->AddLine(ImVec2(RectMin.x, Origin.y), ImVec2(RectMax.x, Origin.y), AXIS_X_COLOR);
  DrawList->AddLine(ImVec2(Origin.x, RectMin.y), ImVec2(Origin.x, RectMax.y), AXIS_Y_COLOR);
}

} // namespace editor
