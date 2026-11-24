#pragma once

#include <string>
#include <memory>

namespace ui
{

class CWindowManager;

} // namespace uis

namespace editor
{

class CSceneAssetType;
class CAssetManager;
class CAssetTypeRegistry;
class CWorkspaceManager;
class CSceneNodeToolsRegistry;

class CSceneEditor
{
public: // Constructor

  CSceneEditor(std::shared_ptr<ui::CWindowManager> _WindowManager, std::shared_ptr<CWorkspaceManager> _WorkspaceManager, std::shared_ptr<CAssetManager> _AssetManager);

public: // Interface

  bool Init(const std::shared_ptr<CAssetTypeRegistry> & _AssetTypeRegistry);

private: // Service

  void CreateNewScene(const std::string & _FilePath) const;

public: // Event

  void OnCreateNewScene();

private: // Attr

  std::shared_ptr<CSceneAssetType>         m_AssetType;
  std::shared_ptr<CAssetManager>           m_AssetManager;
  std::shared_ptr<ui::CWindowManager>      m_WindowManager;
  std::shared_ptr<CWorkspaceManager>       m_WorkspaceManager;
};

} // namespace editor