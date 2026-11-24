#pragma once

#include <string>
#include <memory>
#include <functional>

namespace ui
{

class CWorkspaceWindow;
class IWindowHost;

} // namespace ui

namespace editor
{

class CAssetTypeRegistry;
class CEditorWorkspace;

class CWorkspaceManager
{
public: // Constructor

  CWorkspaceManager(const std::weak_ptr<ui::IWindowHost> & _WindowHost, std::shared_ptr<CAssetTypeRegistry> _AssetTypes);

public: // Interface

  bool OpenAsset(const std::string & _FilePath);

  bool SeveActiveWorkspace();
  
  void CloseActiveWorkspace();

  const std::shared_ptr<CEditorWorkspace> & GetActiveWorkspace() const;

private: // Service

  bool CreateWorkspace(const std::string & _FilePath);

  void HandleUnsavedChanges(std::function<void()> _ConfirmCalback);

public: // Events

  void OnCloseWorkspaceConfirm();

private: // Attr

  std::shared_ptr<CEditorWorkspace>     m_ActiveWorkspace;
  std::shared_ptr<CAssetTypeRegistry>   m_AssetTypes;
  std::weak_ptr<ui::IWindowHost>        m_WindowHost;
};

} // namespace editor
