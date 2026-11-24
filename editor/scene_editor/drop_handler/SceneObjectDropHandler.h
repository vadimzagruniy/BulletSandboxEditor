#pragma once

#include "editor/workspace/drop_handler/IWorkspaceDropHandler.h"

namespace editor
{

class CSceneEditorWorkspace;

class CSceneObjectDropHandler:
  public IWorkspaceDropHandler
{
public: // Constructor

  CSceneObjectDropHandler(CSceneEditorWorkspace & _SceneWorkspace);

public: // IWorkspaceDropHandler

  bool CanHandle(const TWorkspaceDropContext & _Context) override;

  std::string GetToolTip(const TWorkspaceDropContext & _Context) override;

  void Drop(const TWorkspaceDropContext & _Context) override;

private: // Attr

  CSceneEditorWorkspace & m_SceneWorkspace;
};

} // namespace editor