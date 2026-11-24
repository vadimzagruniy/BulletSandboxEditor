#pragma once

#include "imgui.h"
#include <memory>

namespace core
{

class CCamera;
class CSceneNode;

} // namespace core

namespace editor
{

class CTransformGizmo
{
public: // Types

enum class EDragMode
{
  None,
  Move,
  Rotate,
  Size
};

struct THandlePoint
{
  EDragMode  Mode = EDragMode::None;
  ImVec2     SizeDir;
};

public: // Constructor

  CTransformGizmo();

public: // Interface

  THandlePoint HandleHitTest(const core::CCamera & _Camera, const core::CSceneNode & _Node, const ImVec2 & _MauseScreenPos);

  void BeginDrag(const core::CCamera & _Camera, const std::shared_ptr<core::CSceneNode> & _Node, const THandlePoint & _HanldePoint, const ImVec2 & _MauseScreenPos);

  void ContinueDrag(const core::CCamera & _Camera, const ImVec2 & _MauseScreenPos);

  bool EndDrag();

  bool IsDragging() const;

  void Render(const core::CCamera & _Camera, const core::CSceneNode & _Node);

private: // Service

  void UpdateMove(core::CSceneNode & _Node, const ImVec2 & _MauseParentPos) const;

  void UpdateRotation(core::CSceneNode & _Node, const ImVec2 & _MauseParentPos) const;

  void UpdateSize(core::CSceneNode & _Node, const ImVec2 & _MauseParentPos) const;

  void RenderRectFrame(ImDrawList * DrawList, const core::CCamera & _Camera, const core::CSceneNode & _Node) const;

  void RenderSizeHandles(ImDrawList * DrawList, const core::CCamera & _Camera, const core::CSceneNode & _Node) const;

  void RenderRotationHandle(ImDrawList * DrawList, const core::CCamera & _Camera, const core::CSceneNode & _Node) const;

private: // Types

  struct TDragState
  {
    THandlePoint                    HandlePoint;
    std::weak_ptr<core::CSceneNode> DragNode;
    ImVec2                          StartMouseParentPos;
    ImVec2                          StartNodePos;
    ImVec2                          StartNodeSize;
    float                           StartNodeRotation = 0.0f;
    float                           HasChanges        = false;
  };

private: // Attr

  TDragState m_DragState;
};

} // namespace editor