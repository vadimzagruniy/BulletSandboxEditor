#include "AssetManager.h"

#include <algorithm>

namespace editor
{

//
// Conststrucctor/Destructor
//

CAssetManager::CAssetManager()
{
  // Empty
}

CAssetManager::~CAssetManager()
{
  // Empty
}

//
// Interface
//

bool CAssetManager::OpetnProject(const std::filesystem::path & _RootDirectory)
{
  m_AssetRootDir = std::filesystem::absolute(_RootDirectory) / "assets";

  if (!std::filesystem::is_directory(m_AssetRootDir))
    return false;

  Refresh();

  return true;
}

void CAssetManager::Refresh()
{
  if (!std::filesystem::is_directory(m_AssetRootDir))
    return;

  m_AssetRootNode = TAssetNode();

  m_AssetRootNode.Name         = m_AssetRootDir.filename().string();
  m_AssetRootNode.Path = m_AssetRootDir;
  m_AssetRootNode.IsDirectory  = true;

  ScanFolder(m_AssetRootNode);

  NotifyAssetChanged();
}

const TAssetNode & CAssetManager::GetAssetRootNode() const
{
  return m_AssetRootNode;
}

const std::filesystem::path & CAssetManager::GetAssetRootDir() const
{
  return m_AssetRootDir;
}

bool CAssetManager::HasNode(const std::filesystem::path & _Path)
{
  return FindNode(_Path).has_value();
}

const TAssetNode & CAssetManager::GetAssetNode(const std::filesystem::path & _Path) const
{
  static TAssetNode Empty;
  
  const auto NodeOpt = FindNode(_Path);
  if (NodeOpt.has_value())
    return NodeOpt->get();

  // TODO: add assert, log error

  return Empty;
}

CAssetManager::TSubscription CAssetManager::SubscribeAssetChanged(TAssetListener _Listener)
{
  const auto Subscription = std::make_shared<TAssetListener>(std::move(_Listener));
  m_Listeners.push_back(Subscription);

  return Subscription;
}

void CAssetManager::NotifyAssetChanged()
{
  const auto Listeners = m_Listeners;
  for (const auto & WeakListener : Listeners)
    if (auto Listener = WeakListener.lock())
      (*Listener)();

  m_Listeners.erase(std::remove_if(m_Listeners.begin(), m_Listeners.end(), [](const auto & _WeakListener){ return _WeakListener.expired() ;}), m_Listeners.end());
}

//
// Service
//

void CAssetManager::ScanFolder(TAssetNode & _Folder)
{
  for (const auto & FileEntry : std::filesystem::directory_iterator(_Folder.Path))
  {
    const bool IsDirectory = FileEntry.is_directory();
    if (!IsDirectory && !FileEntry.is_regular_file()) // TODO: Add maintained extention check 
      continue;

    TAssetNode AssetNode;

    AssetNode.Name         = FileEntry.path().filename().string();
    AssetNode.Path         = FileEntry.path();
    AssetNode.IsDirectory  = IsDirectory;

    if (AssetNode.IsDirectory)
      ScanFolder(AssetNode);

    _Folder.Children.push_back(AssetNode);
  }
}

TAssetNodeOptionalRef CAssetManager::FindNode(const std::filesystem::path & _Path) const
{
  TAssetNodeOptionalRef       Current      = m_AssetRootNode;
  const std::filesystem::path RelativePath = std::filesystem::relative(_Path, m_AssetRootNode.Path);

  if (RelativePath.empty() || RelativePath == ".")
    return m_AssetRootNode;

  for (const auto & PathElement : RelativePath)
  {
    const auto & Children  = Current->get().Children;
    const auto   ChildIter = std::find_if(Children.begin(), Children.end(), [&PathElement](const TAssetNode & _Node)
      {
        return PathElement.string() == _Node.Name;
      });

    if (ChildIter == Children.end())
      return std::nullopt;

    Current = *ChildIter;
  }

  return Current;
}

} // namespace editor