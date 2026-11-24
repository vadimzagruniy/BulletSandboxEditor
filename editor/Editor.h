#pragma once

#include "imgui.h"
#include <memory>

namespace ui
{

class CWindowManager;
class CWorkspaceWindow;
class CContentBrowserWindow;

} // namespace ui

namespace editor
{

class CAssetManager;
class CAssetTypeRegistry;
class CWorkspaceManager;
class CSceneEditor;

class CEditor
{
public: // Constructor/Destructor

  CEditor();
  
  ~CEditor();

public: // Interface

  bool Init();

  void Shutdown();

  void Update(float _DeltaTime);

  void Render();

  const std::shared_ptr<CSceneEditor> & GetSceneEditor() const;

private: // Service

  void RenderMainMenu();

  void UpdateDockLoyaut(ImGuiID _DockspaceID);

  void HandleShortcuts();

private: // Attr

  std::shared_ptr<ui::CWindowManager>        m_WindowManager;
  std::shared_ptr<CAssetManager>             m_AssetManager;
  std::shared_ptr<CAssetTypeRegistry>        m_AssetTypeRegistry;
  std::shared_ptr<CWorkspaceManager>         m_WorkspaceManager;
  std::shared_ptr<ui::CWorkspaceWindow>      m_WorkspaceWindow;
  std::shared_ptr<ui::CContentBrowserWindow> m_ContentBrowserWindow;
  std::shared_ptr<CSceneEditor>              m_SceneEditor;
  bool                                       m_RequestResetLoyaut;
};

} // namespace editor
