#include "ContentBrowserController.h"

#include "editor/workspace/WorkspaceManager.h"

namespace ui
{

//
// Constructor
//

CContentBrowserController::CContentBrowserController(std::shared_ptr<editor::CAssetManager> _AssetManager, std::shared_ptr<editor::CWorkspaceManager> _WorkspaceManager):
  m_AssetManager(std::move(_AssetManager)),
  m_WorkspaceManager(std::move(_WorkspaceManager))
{
  RebuildItems();

  m_RefreshSubscription = m_AssetManager->SubscribeAssetChanged([this](){ RebuildItems(); });
}

//
// IContentBrowserController
//

const editor::TAssetNode & CContentBrowserController::GetRootNode() const
{
  return m_AssetManager->GetAssetRootNode();
}

const std::filesystem::path & CContentBrowserController::GetCurrentFolder() const
{
  return m_CurrentFolder;
}

const std::vector<TContentItem> & CContentBrowserController::GetContentItems() const
{
  return m_ContentItems;
}

const std::filesystem::path & CContentBrowserController::GetSelection() const
{
  return m_Selection;
}

bool CContentBrowserController::IsSelected(const std::filesystem::path & _Path) const
{
  return m_Selection == _Path;
}

void CContentBrowserController::ClearSelection()
{
  m_Selection.clear();
}

void CContentBrowserController::NavigateTo(const std::filesystem::path & _Path)
{
  m_PendindgActions.push_back([this, _Path](){ DoNavigateTo(_Path); });
}

void CContentBrowserController::Open(const std::filesystem::path & _Path)
{
  m_PendindgActions.push_back([this, _Path](){ DoOpen(_Path); });
}

void CContentBrowserController::ProcessPendingActions()
{
  std::vector<TAction> PendindgActions;

  PendindgActions.swap(m_PendindgActions);
  
  for (const auto & Action : PendindgActions)
    Action();
}

//
// Service
//

void CContentBrowserController::RebuildItems()
{
  m_ContentItems.clear();

  if (m_CurrentFolder.empty() || !m_AssetManager->HasNode(m_CurrentFolder))
    m_CurrentFolder = m_AssetManager->GetAssetRootDir();

  if (!m_AssetManager->HasNode(m_CurrentFolder))
    return;

  for (const auto & Child : m_AssetManager->GetAssetNode(m_CurrentFolder).Children)
  {
    TContentItem ContentItem;
    ContentItem.Path        = Child.Path; 
    ContentItem.Name        = Child.Name;
    ContentItem.IsDirectory = Child.IsDirectory;

    m_ContentItems.push_back(ContentItem);
  }

  std::sort(m_ContentItems.begin(), m_ContentItems.end(), [](const TContentItem & _Left, const TContentItem & _Right)
  {
    if (_Left.IsDirectory != _Right.IsDirectory)
      return _Left.IsDirectory;

    return false;
  });
}

void CContentBrowserController::DoNavigateTo(const std::filesystem::path & _Path)
{
  if (!m_AssetManager->HasNode(_Path) || !m_AssetManager->GetAssetNode(_Path).IsDirectory)
    return;

  m_CurrentFolder = _Path;
  m_Selection.clear();

  RebuildItems();
}

void CContentBrowserController::DoOpen(const std::filesystem::path & _Path)
{
  if (!m_AssetManager->HasNode(_Path))
    return;

  const editor::TAssetNode & Node = m_AssetManager->GetAssetNode(_Path);

  if (!Node.IsDirectory)
  {
    m_WorkspaceManager->OpenAsset(Node.Path.string());
    return;
  }

  DoNavigateTo(Node.Path);
}

} // namespace ui