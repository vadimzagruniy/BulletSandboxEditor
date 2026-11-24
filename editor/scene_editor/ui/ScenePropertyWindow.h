#pragma once

#include "editor/ui/windows/DockedWindow.h"

namespace core
{

class CSceneNode;

} // namespace core

namespace editor
{

class CSceneEditorWorkspace;
class CPropertyInspector;

} // namespace editor

namespace ui
{

class CScenePropertyWindow:
  public CDockedWindow 
{
public: // Constructor/Destructor

  CScenePropertyWindow();

private: // CBaseWindow events

  void OnRenderGui() override;

public: // Interface

  void SetWorkspace(std::weak_ptr<editor::CSceneEditorWorkspace> _SceneWorkspace);

private: // Service

  void RenderHeader(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _SceneNode);

private: // Attr

  std::weak_ptr<editor::CSceneEditorWorkspace> m_SceneWorkspace;
  std::shared_ptr<editor::CPropertyInspector>  m_PropertyInspector;
};



} // namespace ui