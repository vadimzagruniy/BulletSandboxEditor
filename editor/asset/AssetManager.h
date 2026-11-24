#pragma once

#include "AssetNode.h"

#include <filesystem>
#include <memory>
#include <functional>

namespace editor
{

using TAssetNodeOptionalRef = std::optional<std::reference_wrapper<const TAssetNode>>;

class CAssetManager
{
public: // Types

  using TAssetListener = std::function<void()>;
  using TSubscription  = std::shared_ptr<TAssetListener>;

public: // Conststrucctor/Destructor

  CAssetManager();

  ~CAssetManager();

public: // Interface

  bool OpetnProject(const std::filesystem::path & _RootDirectory);

  void Refresh();

  const TAssetNode & GetAssetRootNode() const;

  const std::filesystem::path & GetAssetRootDir() const;

  bool HasNode(const std::filesystem::path & _Path);

  const TAssetNode & GetAssetNode(const std::filesystem::path & _Path) const;

  [[nodiscard]] TSubscription SubscribeAssetChanged(TAssetListener _Listener);

  void NotifyAssetChanged();

private: // Service

  void ScanFolder(TAssetNode & _Folder);

  TAssetNodeOptionalRef FindNode(const std::filesystem::path & _Path) const;

private: // Attr

  std::filesystem::path                      m_AssetRootDir;
  TAssetNode                                 m_AssetRootNode;
  std::vector<std::weak_ptr<TAssetListener>> m_Listeners;
};

} // namespace editor