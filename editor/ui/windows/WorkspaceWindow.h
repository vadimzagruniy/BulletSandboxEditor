#pragma once

#include "DockedWindow.h"

namespace editor
{

class CWorkspaceManager;

} // namespace editor

namespace ui
{

class CWorkspaceWindow:
  public CDockedWindow 
{
public: // Constructor/Destructor

  CWorkspaceWindow(std::shared_ptr<editor::CWorkspaceManager> _WorkspaceManager);
  
  ~CWorkspaceWindow();

public: // CDockedWindow

  std::string GetDisplayTitle() const override;

private: // CBaseWindow events

  void OnRenderGui() override;

  void OnUpdate(float _TimeDelta) override;

private: // Attr

  std::shared_ptr<editor::CWorkspaceManager> m_WorkspaceManager;
};



} // namespace ui