#pragma once

#include "imgui.h"
#include <string>

namespace core
{

class IPropertyInspector
{
public: // Destructor

  virtual ~IPropertyInspector() = default;

public: // Interfce

  virtual void ResetChanges() = 0;

  virtual bool HasChanges() const = 0;

  virtual void BeginGroup(const std::string & _Name) = 0;

  virtual bool Float(const std::string & _Name, float & _Value, float _Max, float _Min) = 0;

  virtual bool Int(const std::string & _Name, int & _Value, int _Max, int _Min) = 0;

  virtual bool Vec2(const std::string & _Name, ImVec2 & _Value, float _Max, float _Min) = 0;

  virtual bool Angle(const std::string & _Name, float & _Value, float _Max, float _Min) = 0;
  
  virtual bool Color(const std::string & _Name, ImVec4 & _Value) = 0;
};

} // namespace core
