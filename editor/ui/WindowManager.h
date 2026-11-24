#pragma once

#include "IWindowHost.h"
#include "windows/ModalWindow.h"
#include "windows/DockedWindow.h"

#include <vector>
#include <memory>

namespace ui
{

class CWindowManager:
  public IWindowHost
{
public: // Constructor/Destruvvtor

  CWindowManager();

  ~CWindowManager();

public: // IWindowHost

  void AddModalWindow(const std::shared_ptr<CModalWindow> & _Window) override;

  void RemoveModalWindow(const std::shared_ptr<CModalWindow> & _Window) override;

  void AddDockedWindow(const std::shared_ptr<CDockedWindow> & _Window) override;

  void RemoveDockedWindow(const std::shared_ptr<CDockedWindow> & _Window) override;

public: // Interface

  void Render();

  void Update(
      const float _TimeDelta
    );

  void ShowMenuItems();

private: // Attr

  std::vector<std::shared_ptr<CModalWindow>>  m_ModalWindows;
  std::vector<std::shared_ptr<CDockedWindow>> m_DockedWindows;

  std::vector<std::shared_ptr<CModalWindow>>  m_ModalWindowsToHide;
  std::vector<std::shared_ptr<CDockedWindow>> m_DockedWindowsToHide;
};

} // namespace ui