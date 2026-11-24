#pragma once

#include "BaseWindow.h"
#include <functional>
#include <memory>

namespace ui
{

class CModalWindow:
  public std::enable_shared_from_this<CModalWindow>,
  public CBaseWindow
{
private: // Types

  using TExitCallback = std::function<void(const std::shared_ptr<CModalWindow> &)>;

public: // Constructor/Destructor

  CModalWindow(std::string _Title);
  
  ~CModalWindow();

private: // CBaseWindow service

  bool BeginFrame() override final;

  void EndFrame() override final;

  ImGuiCond GetSizingCondition() const override;

public: // CBaseWindow events

  void OnUpdate(float _TimeDelta) override;

public: // Interface

  void SetExitCallback(const TExitCallback & _ExitCallback);

  void Close();

private: // Attr

  bool          m_IsExitRequired;
  TExitCallback m_OnExitCallback;
};

} // namespace ui
