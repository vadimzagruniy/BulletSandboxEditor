#pragma once

#include <../../editor/asset/AssetNode.h>

#include <string>

namespace ui
{

struct TContentItem
{
  std::filesystem::path Path;
  std::string           Name;
  bool                  IsDirectory = false;
};

class IContentBrowserController
{
public: // Destructor

  virtual ~IContentBrowserController() = default;

public: // Interface

  virtual const editor::TAssetNode & GetRootNode() const = 0;

  virtual const std::filesystem::path & GetCurrentFolder() const = 0;

  virtual const std::vector<TContentItem> & GetContentItems() const = 0;

  virtual const std::filesystem::path & GetSelection() const = 0;

  virtual bool IsSelected(const std::filesystem::path & _Path) const = 0;

  virtual void ClearSelection() = 0;

  virtual void NavigateTo(const std::filesystem::path & _Path) = 0;

  virtual void Open(const std::filesystem::path & _Path) = 0;

  virtual void ProcessPendingActions() = 0;
};

} // namespace ui