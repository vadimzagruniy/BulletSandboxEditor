#pragma once

#include "DockedWindow.h"
#include <memory>

namespace ui
{

class IContentBrowserController;
class CFolderTreeView;
class CContentItemsView;

class CContentBrowserWindow:
  public CDockedWindow
{
public: // Constructors/Desstructors

  CContentBrowserWindow(std::shared_ptr<IContentBrowserController> _Controller);
  
  ~CContentBrowserWindow();

private: // CBaseWindow events

  void OnRenderGui() override;

  void OnUpdate(float _TimeDelta) override;

private: // Service

  void RenderSplitter(float _AvailableWidth);

private: // Attr

  std::shared_ptr<IContentBrowserController> m_Controller;
  std::unique_ptr<CFolderTreeView>           m_FolderTreeView;
  std::unique_ptr<CContentItemsView>         m_ContentItemsView;
  float                                      m_SplitRatio;
};

} // namespace ui