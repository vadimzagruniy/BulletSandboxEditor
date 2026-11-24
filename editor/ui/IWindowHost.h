#pragma once

#include <vector>
#include <memory>

namespace ui
{

class CModalWindow;
class CDockedWindow;

class IWindowHost
{
public: // Destructor

  virtual ~IWindowHost() = default;

public: // Interrface

  virtual void AddModalWindow(const std::shared_ptr<CModalWindow> & _Window) = 0;

  virtual void RemoveModalWindow(const std::shared_ptr<CModalWindow> & _Window) = 0;

  virtual void AddDockedWindow(const std::shared_ptr<CDockedWindow> & _Window) = 0;
  
  virtual void RemoveDockedWindow(const std::shared_ptr<CDockedWindow> & _Window) = 0;
};

} // namespace ui