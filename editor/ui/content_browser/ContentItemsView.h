#pragma once

#include <memory>
#include <vector>

struct ImVec2;

namespace ui
{

struct TContentItem;

class IContentBrowserController;

class CContentItemsView
{
public: // Constructor

  CContentItemsView(std::shared_ptr<IContentBrowserController> _Controller);

public: // Interface

  void Render();

private: // Service

  void RenderItems(const std::vector<TContentItem> & _ContentItems);

  void RenderItemButton(const TContentItem & _Item, const ImVec2 & _Position, const ImVec2 & _Size);

  void RenderItemIcon(const TContentItem & _Item, const ImVec2 & _Position, float _Size) const;

  void RenderItemLabel(const TContentItem & _Item, const ImVec2 & _Position, float _Width,  bool _IsCentered);

private: // Attr

  std::shared_ptr<IContentBrowserController> m_Controller;
};

} // namespace ui
