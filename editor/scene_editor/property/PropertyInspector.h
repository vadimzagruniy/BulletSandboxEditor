#pragma once

#include "core/property/IPropertyVisitor.h"

namespace editor
{

class CPropertyInspector:
  public core::IPropertyInspector
{
public: // Constructor

  CPropertyInspector();

public: // IPropertyInspector

  virtual void ResetChanges() override;

  virtual bool HasChanges() const override;

  void BeginGroup(const std::string & _Name) override;

  bool Float(const std::string & _Name, float & _Value, float _Max, float _Min) override;

  bool Int(const std::string & _Name, int & _Value, int _Max, int _Min) override;

  bool Vec2(const std::string & _Name, ImVec2 & _Value, float _Max, float _Min) override;

  bool Angle(const std::string & _Name, float & _Value, float _Max, float _Min) override;
  
  bool Color(const std::string & _Name, ImVec4 & _Value) override;

private: // Service

  bool TrackChanges(bool _IsChanged);

private: // Attr

  bool m_HasChanges;
};

} // namespace editor