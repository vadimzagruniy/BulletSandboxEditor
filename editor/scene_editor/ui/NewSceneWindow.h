#pragma once

#include <editor/ui/windows/ModalWindow.h>

namespace ui
{

class CNewSceneWindow:
  public CModalWindow 
{
public: // Types

  using TCreateSceneCallback = std::function<void(const std::string &)>;

public: // Constructor/Destructor

  CNewSceneWindow(std::string _CurrentPath, std::string _SceneFileExt, TCreateSceneCallback _CreateSceneCallback);

private: // CBaseWindow events

  void OnRenderGui() override;

private: // Events

  void OnSelectFolder();

private: // Attr

  std::string          m_CurrentPath;
  std::string          m_SceneFileExt;
  TCreateSceneCallback m_CreateSceneCallback;
  char                 m_SceneName[256];
};



} // namespace ui