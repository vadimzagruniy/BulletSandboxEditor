#pragma once

#include <Imgui.h>
#include <string>

namespace ui
{

class CBaseWindow
{
public: // Constructor/Destructor

  CBaseWindow(std::string _Title, bool _IsVisible);
  
  virtual ~CBaseWindow();

public: // Interface

  void Render();

  void Update(float _TimeDelta);

  void SetVisible(bool _IsVisible);

  bool IsVisible() const;

protected: // Service

  virtual ImGuiCond GetSizingCondition() const = 0;

private: // Service

  virtual bool BeginFrame() = 0;

  virtual void EndFrame() = 0;

private: // Events

  virtual void OnRenderGui() = 0;

  virtual void OnUpdate(float _TimeDelta) = 0;

protected: // Attr

  std::string m_Title;
  bool        m_IsVisible;
  bool        m_IsActive;
  ImVec2      m_Size;
};

} // namespace ui
