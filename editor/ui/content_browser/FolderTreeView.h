#pragma once

#include <memory>

namespace editor
{

struct TAssetNode;

} // namespace editor

namespace ui
{

class IContentBrowserController;

class CFolderTreeView
{
public: // Constructor

  CFolderTreeView(std::shared_ptr<IContentBrowserController> _Controller);

public: // Interface

  void Render();

private: // Service

  void DrawFolder(const editor::TAssetNode & _AssetNode);

private: // Attr

  std::shared_ptr<IContentBrowserController> m_Controller;
};

} // namespace ui