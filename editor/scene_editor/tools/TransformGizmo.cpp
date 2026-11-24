#include "TransformGizmo.h"

#include <core/camera/Camera.h>
#include <core/scene/SceneNode.h>
#include <core/utils/Math.h>


namespace editor
{

namespace // unnamed
{
  
constexpr float HANDLE_HALF_SIZE   = 4.0f;
constexpr float HANDLE_HIT_RADIUS  = 7.0f;
constexpr float HANDLE_ROTATE_DIST = 25.0f;
constexpr float LINE_THICKNESS     = 1.5f;
constexpr float MIN_SIZE           = 1.0f;

const ImU32  GIZMO_COLOR         = IM_COL32(0, 160, 255, 255);
const ImU32  HANDLE_COLOR        = IM_COL32(255, 160, 255, 255);
const ImU32  HANDLE_ACTIVE_COLOR = IM_COL32(50, 255, 0, 255);
const ImVec2 TOP_CENTER_DIR      = ImVec2(0.0f, -1.0f); 

const ImVec2 FRAME_DIRS[4] =
{
  ImVec2(-1.0f, -1.0f),
  ImVec2( 1.0f, -1.0f),
  ImVec2( 1.0f,  1.0f),
  ImVec2(-1.0f,  1.0f),
};

const ImVec2 SIZE_DIRS[8] =
{
  ImVec2(-1.0f, -1.0f),
  ImVec2( 0.0f, -1.0f),
  ImVec2( 1.0f, -1.0f),
  ImVec2( 1.0f,  0.0f),
  ImVec2( 1.0f,  1.0f),
  ImVec2( 0.0f,  1.0f),
  ImVec2(-1.0f,  1.0f),
  ImVec2(-1.0f,  0.0f),
};

//
// Utils
//

ImVec2 GetScreenFramePoint(const core::CCamera & _Camera, const core::CSceneNode & _Node, const ImVec2 & _Direction)
{
  return _Camera.ToScreen(_Node.LocalToWorld(_Direction * _Node.GetSize() * 0.5f));
}

ImVec2 GetScreenRotatePoint(const core::CCamera & _Camera, const core::CSceneNode & _Node)
{
  const ImVec2 TopCenter = GetScreenFramePoint(_Camera, _Node, TOP_CENTER_DIR);
  const ImVec2 Center    = _Camera.ToScreen(_Node.LocalToWorld(ImVec2(0.0f, 0.0f)));
  const ImVec2 Up        = TopCenter - Center;
  const float  Lenght    = core::math::Length(Up);

  return Lenght > 0.0f ? TopCenter + Up * (HANDLE_ROTATE_DIST / Lenght) : TopCenter;
}

bool IsHanldleHit(const ImVec2 & _HandlerPos, const ImVec2 & _ScreenPos)
{
  return std::abs(_HandlerPos.x - _ScreenPos.x) <= HANDLE_HIT_RADIUS &&
         std::abs(_HandlerPos.y - _ScreenPos.y) <= HANDLE_HIT_RADIUS;
}

} // namespace unnamed

//
// Constructor
//

CTransformGizmo::CTransformGizmo() = default;

//
// Interface
//

CTransformGizmo::THandlePoint CTransformGizmo::HandleHitTest(const core::CCamera & _Camera, const core::CSceneNode & _Node, const ImVec2 & _MauseScreenPos)
{
  if (IsHanldleHit(GetScreenRotatePoint(_Camera, _Node), _MauseScreenPos))
    return THandlePoint{ .Mode = EDragMode::Rotate };

  for (const auto & SizeDir : SIZE_DIRS)
    if (IsHanldleHit(GetScreenFramePoint(_Camera, _Node, SizeDir), _MauseScreenPos))
      return THandlePoint{ .Mode = EDragMode::Size, .SizeDir = SizeDir };

  return THandlePoint{};
}

void CTransformGizmo::BeginDrag(const core::CCamera & _Camera, const std::shared_ptr<core::CSceneNode> & _Node, const THandlePoint & _HanldePoint, const ImVec2 & _MauseScreenPos)
{
  if (_Node == nullptr || _HanldePoint.Mode == EDragMode::None)
  {
    EndDrag();
    return;
  }

  m_DragState.DragNode            = _Node;
  m_DragState.HandlePoint         = _HanldePoint;
  m_DragState.StartMouseParentPos = _Node->WorldToParent(_Camera.ToWorld(_MauseScreenPos));
  m_DragState.StartNodePos        = _Node->GetOrigin();
  m_DragState.StartNodeSize       = _Node->GetSize();
  m_DragState.StartNodeRotation   = _Node->GetRotation();
}

void CTransformGizmo::ContinueDrag(const core::CCamera & _Camera, const ImVec2 & _MauseScreenPos)
{
  const auto DragNode = m_DragState.DragNode.lock();
  if (DragNode == nullptr)
  {
    EndDrag();
    return;
  }

  const ImVec2 MouseWorldPos = DragNode->WorldToParent(_Camera.ToWorld(_MauseScreenPos));

  switch (m_DragState.HandlePoint.Mode)
  {
    case EDragMode::Move:   UpdateMove(*DragNode, MouseWorldPos);     break;
    case EDragMode::Size:   UpdateSize(*DragNode, MouseWorldPos);    break;
    case EDragMode::Rotate: UpdateRotation(*DragNode, MouseWorldPos); break;

    default: break;
  }

  m_DragState.HasChanges = true;
}

bool CTransformGizmo::EndDrag()
{
  const bool HasChanges = m_DragState.HasChanges;
  m_DragState = TDragState();

  return HasChanges;
}

bool CTransformGizmo::IsDragging() const
{
  return m_DragState.HandlePoint.Mode != EDragMode::None;
}

void CTransformGizmo::Render(const core::CCamera & _Camera, const core::CSceneNode & _Node)
{
  auto * DrawList = ImGui::GetWindowDrawList();

  RenderRectFrame(DrawList, _Camera, _Node);
  RenderRotationHandle(DrawList, _Camera, _Node);
  RenderSizeHandles(DrawList, _Camera, _Node);
}

//
// Service
//

void CTransformGizmo::UpdateMove(core::CSceneNode & _Node, const ImVec2 & _MauseParentPos) const
{
  _Node.SetOrigin(m_DragState.StartNodePos + (_MauseParentPos - m_DragState.StartMouseParentPos));
}

void CTransformGizmo::UpdateRotation(core::CSceneNode & _Node, const ImVec2 & _MauseParentPos) const
{
  ImVec2 VecFrom = m_DragState.StartMouseParentPos - m_DragState.StartNodePos;
  ImVec2 VecTo   = _MauseParentPos - m_DragState.StartNodePos;

  const float Delta = std::atan2(VecTo.y, VecTo.x) - std::atan2(VecFrom.y, VecFrom.x);

  _Node.SetRotation(m_DragState.StartNodeRotation + Delta);
}

void CTransformGizmo::UpdateSize(core::CSceneNode & _Node, const ImVec2 & _MauseParentPos) const
{
  ImVec2 LocalDelta = core::math::Rotate(_MauseParentPos - m_DragState.StartMouseParentPos, -m_DragState.StartNodeRotation);
  ImVec2 Direction  = m_DragState.HandlePoint.SizeDir;
  ImVec2 StartSize  = m_DragState.StartNodeSize;
  ImVec2 Scale      = _Node.GetScale();

  ImVec2 NewSize = StartSize;

  if (Direction.x != 0.0f && Scale.x > 0.0f)
    NewSize.x = std::max(StartSize.x + LocalDelta.x * Direction.x / Scale.x, MIN_SIZE);

  if (Direction.y != 0.0f && Scale.y > 0.0f)
    NewSize.y = std::max(StartSize.y + LocalDelta.y * Direction.y / Scale.y, MIN_SIZE);

  ImVec2 LocalOffset = ImVec2((NewSize.x - StartSize.x) * 0.5f * Direction.x * Scale.x,
                              (NewSize.y - StartSize.y) * 0.5f * Direction.y * Scale.y);

  _Node.SetSize(NewSize);
  _Node.SetOrigin(m_DragState.StartNodePos + core::math::Rotate(LocalOffset, m_DragState.StartNodeRotation));
}

void CTransformGizmo::RenderRectFrame(ImDrawList * DrawList, const core::CCamera & _Camera, const core::CSceneNode & _Node) const
{
  ImVec2 Frames[4];

  for (int i = 0; i < 4; ++i)
    Frames[i] = GetScreenFramePoint(_Camera, _Node, FRAME_DIRS[i]);

  DrawList->AddPolyline(Frames, 4, GIZMO_COLOR, ImDrawFlags_Closed, LINE_THICKNESS);
}

void CTransformGizmo::RenderSizeHandles(ImDrawList * DrawList, const core::CCamera & _Camera, const core::CSceneNode & _Node) const
{
  ImVec2 HandleHalfSize = ImVec2(HANDLE_HALF_SIZE, HANDLE_HALF_SIZE);

  for (const auto SizeDir : SIZE_DIRS)
  {
    ImVec2 FramePoint = GetScreenFramePoint(_Camera, _Node, SizeDir);
    ImVec2 RectMin    = FramePoint - HandleHalfSize;
    ImVec2 RectMax    = FramePoint + HandleHalfSize;

    const ImVec2 ActiveSizeDir = m_DragState.HandlePoint.SizeDir;
    const auto   HandleColor    = SizeDir == ActiveSizeDir ? HANDLE_ACTIVE_COLOR : HANDLE_COLOR;

    DrawList->AddRectFilled(RectMin, RectMax, HANDLE_COLOR);
    DrawList->AddRect(RectMin, RectMax, HandleColor);
  }
}

void CTransformGizmo::RenderRotationHandle(ImDrawList * DrawList, const core::CCamera & _Camera, const core::CSceneNode & _Node) const
{
  ImVec2 TopCenter = GetScreenFramePoint(_Camera, _Node, TOP_CENTER_DIR);
  ImVec2 RotatePos = GetScreenRotatePoint(_Camera, _Node);

  DrawList->AddLine(TopCenter, RotatePos, GIZMO_COLOR, LINE_THICKNESS);
  DrawList->AddCircleFilled(RotatePos, HANDLE_HALF_SIZE + 1.0f, HANDLE_COLOR);
  DrawList->AddCircle(RotatePos, HANDLE_HALF_SIZE + 1.0f, GIZMO_COLOR);
}

} // namespace editor