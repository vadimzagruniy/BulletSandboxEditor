#pragma once

#include "editor/ui/windows/DockedWindow.h"

namespace core
{

class CSceneNode;

} // namespace core

namespace editor
{

class CSceneEditorWorkspace;

} // namespace editor

namespace ui
{

class CSceneObjectsWindow:
  public CDockedWindow 
{
public: // Constructor/Destructor

  CSceneObjectsWindow();

private: // CBaseWindow events

  void OnRenderGui() override;

};



} // namespace ui