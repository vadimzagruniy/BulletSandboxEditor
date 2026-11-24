#include "PropertyInspector.h"
#include <imgui_internal.h>
#include <algorithm>

namespace editor
{

//
// Constructor
//

CPropertyInspector::CPropertyInspector():
  m_HasChanges(false)
{
  // Empty
}

//
// IPropertyInspector
//

void CPropertyInspector::ResetChanges()
{
  m_HasChanges = false;
}

bool CPropertyInspector::HasChanges() const
{
  return m_HasChanges;
}

void CPropertyInspector::BeginGroup(const std::string & _Name)
{
  ImGui::SeparatorText(_Name.c_str());
}

bool CPropertyInspector::Float(const std::string & _Name, float & _Value, float _Max, float _Min)
{
  return TrackChanges(ImGui::DragFloat(_Name.c_str(), &_Value, 1.0f, _Min, _Max));
}

bool CPropertyInspector::Int(const std::string & _Name, int & _Value, int _Max, int _Min)
{
  return TrackChanges(ImGui::DragInt(_Name.c_str(), &_Value, 1.0f, _Min, _Max));
}

bool CPropertyInspector::Vec2(const std::string & _Name, ImVec2 & _Value, float _Max, float _Min)
{
  return TrackChanges(ImGui::DragFloat2(_Name.c_str(), &_Value.x, 1.0f, _Min, _Max));
}

bool CPropertyInspector::Angle(const std::string & _Name, float & _Value, float _Max, float _Min)
{
        float Degrees    = _Value * 180.0f / IM_PI;
  const float MinDegrees = _Min   * 180.0f / IM_PI;
  const float MaxDegrees = _Max   * 180.0f / IM_PI;

  const bool IsChanged = ImGui::DragFloat(_Name.c_str(), &Degrees, 1.0f, MinDegrees, MaxDegrees, "%.1f deg");
  if (IsChanged)
  {
    _Value = Degrees * IM_PI / 180.0f;
  }

  return TrackChanges(IsChanged);
}

bool CPropertyInspector::Color(const std::string & _Name, ImVec4 & _Value)
{
  return TrackChanges(ImGui::ColorEdit4(_Name.c_str(), &_Value.x));
}

//
// Service
//

bool CPropertyInspector::TrackChanges(bool _IsChanged)
{
  m_HasChanges |= _IsChanged;
  return _IsChanged;
}

} // namespace editor