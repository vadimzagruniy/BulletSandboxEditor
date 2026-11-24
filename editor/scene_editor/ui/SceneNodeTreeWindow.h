#pragma once

#include "editor/ui/windows/DockedWindow.h"
#include <array>

namespace core
{

class CSceneNode;

} // namespace core

namespace editor
{

class CSceneEditorWorkspace;

} // namespace editor

namespace ui
{

class CSceneNodeTreeWindow:
  public CDockedWindow 
{
private: // Types

  struct TRenameData
  {
    std::string           RenamingID;
    std::array<char, 128> RenamingBuffer = { 0 };
    bool                  NeedStopRename = false;
  };

public: // Constructor/Destructor

  CSceneNodeTreeWindow();

private: // CBaseWindow events

  void OnRenderGui() override;

public: // Interface

  void SetWorkspace(std::weak_ptr<editor::CSceneEditorWorkspace> _SceneWorkspace);

private: // Service

  void RenderNode(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _SceneNode);

  void RenderAddNodeMenu(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _Parent);

  void RenderNodeContectMenu(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _SceneNode);

  void RenderToolBar(editor::CSceneEditorWorkspace & _Workspace);

  void RenderLabel(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _SceneNode);

  void RenderRename(editor::CSceneEditorWorkspace & _Workspace, const std::shared_ptr<core::CSceneNode> & _SceneNode);

private: // Events

  void OnStartRename(const std::string & _CurrentName);

private: // Attr

  std::weak_ptr<editor::CSceneEditorWorkspace> m_SceneWorkspace;
  TRenameData                                  m_RenameData;
};



} // namespace ui