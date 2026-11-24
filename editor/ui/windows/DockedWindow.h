#pragma once

#include "BaseWindow.h"
#include <memory>

namespace ui
{

class CDockedWindow:
  public std::enable_shared_from_this<CDockedWindow>,
  public CBaseWindow
{
public: // Constructor/Destructor

  CDockedWindow(std::string _Title, bool _IsVisible = true);
  
  ~CDockedWindow();

private: // CBaseWindow service

  bool BeginFrame() override final;

  void EndFrame() override final;

  ImGuiCond GetSizingCondition() const override;

public: // CBaseWindow events

  void OnUpdate(float _TimeDelta) override;

public: // Interface

  virtual std::string GetDisplayTitle() const;

};

} // namespace ui
