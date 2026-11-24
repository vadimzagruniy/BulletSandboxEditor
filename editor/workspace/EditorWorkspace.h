#pragma once

#include "core/camera/Camera.h"

#include <memory>
#include <vector>

namespace ui
{

class IWindowHost;

} // namespace ui

namespace editor
{

struct TWorkspaceDropContext;
class IWorkspaceDropHandler;
class CAssetDocument;

class CEditorWorkspace
{
public: // Constructor/Destructor

  CEditorWorkspace();

  virtual ~CEditorWorkspace();

public: // Interface

  virtual void Update(float _DeltaTime);

  void Render();

public: // Events

  virtual void OnRenderToolBar();

  virtual void OnActivate(const std::weak_ptr<ui::IWindowHost> & _WindowHost);

  virtual void OnDeactivate();

  virtual void OnRenderContent(const core::CCamera & _Camera) = 0;

  virtual void OnCanvasMouseDown(const ImVec2 & _ScreenPos) = 0;

  virtual void OnCanvasMouseUp(const ImVec2 & _ScreenPos) = 0;

  virtual void OnCanvasMouseDrag(const ImVec2 & _ScreenPos) = 0;
  
  virtual std::shared_ptr<CAssetDocument> GetAssetDocument() = 0;

private: // Service

  void HadleDropTarget();

  std::shared_ptr<IWorkspaceDropHandler> GetDropHendler(const TWorkspaceDropContext & _Context) const;

  void RenderZeroAxes(const core::CCamera & _Camera) const;

protected: // Attr

  core::CCamera                                       m_Camera;
  std::weak_ptr<ui::IWindowHost>                      m_WindowHost;
  ImVec2                                              m_CanvasPosition;
  ImVec2                                              m_CanvasSize;
  ImVec2                                              m_CanvasCenter;
  std::vector<std::shared_ptr<IWorkspaceDropHandler>> m_DropHandlers;
};

} // namespace editor
