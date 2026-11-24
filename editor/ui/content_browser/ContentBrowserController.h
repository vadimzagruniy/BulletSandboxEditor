#pragma once

#include "IContentBrowserController.h"
#include "editor/asset/AssetManager.h"

namespace editor
{

class CAssetManager;
class CWorkspaceManager;

} // namespace editor

namespace ui
{

class CContentBrowserController:
  public IContentBrowserController
{
private: // Types

  using TAction = std::function<void()>;

public: // Constructor

  CContentBrowserController(std::shared_ptr<editor::CAssetManager> _AssetManager, std::shared_ptr<editor::CWorkspaceManager> _WorkspaceManager);

public: // IContentBrowserController

  const editor::TAssetNode & GetRootNode() const override;

  const std::filesystem::path & GetCurrentFolder() const override;

  const std::vector<TContentItem> & GetContentItems() const override;

  const std::filesystem::path & GetSelection() const override;

  bool IsSelected(const std::filesystem::path & _Path) const override;

  void ClearSelection() override;

  void NavigateTo(const std::filesystem::path & _Path) override;

  void Open(const std::filesystem::path & _Path) override;

  void ProcessPendingActions() override;

private: // Service

  void RebuildItems();

  void DoNavigateTo(const std::filesystem::path & _Path);

  void DoOpen(const std::filesystem::path & _Path);

private: // Subscriptions

  editor::CAssetManager::TSubscription m_RefreshSubscription;

private: // Attr

  std::filesystem::path                      m_CurrentFolder;
  std::filesystem::path                      m_Selection;
  std::shared_ptr<editor::CAssetManager>     m_AssetManager;
  std::shared_ptr<editor::CWorkspaceManager> m_WorkspaceManager;
  std::vector<TContentItem>                  m_ContentItems;
  std::vector<TAction>                       m_PendindgActions;
};

} // namespace ui