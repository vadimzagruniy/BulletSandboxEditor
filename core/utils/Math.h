#pragma once

#include <imgui.h>
#include <cmath>

namespace core::math
{

inline float Length(const ImVec2 & _Vector)
{
  return std::sqrtf(_Vector.x * _Vector.x + _Vector.y * _Vector.y);
}

inline ImVec2 Rotate(const ImVec2 & _Vector, float _Angle)
{
  float Cos = std::cos(_Angle);
  float Sin = std::sin(_Angle);

  return ImVec2(_Vector.x * Cos - _Vector.y * Sin, _Vector.x * Sin + _Vector.y * Cos);
}

inline float DotProduct(const ImVec2 & _Vector1, const ImVec2 & _Vector2)
{
  return _Vector1.x * _Vector2.x + _Vector1.y * _Vector2.y;
}

inline ImVec2 Reflect(const ImVec2 & _Vector, const ImVec2 & _Normal)
{
  const float Dot = DotProduct(_Vector, _Normal);

  return _Vector - _Normal * (2.0f * Dot);
}

inline ImVec2 Normalize(const ImVec2 & _Vector)
{
  const float Dot = DotProduct(_Vector, _Vector);

  const float Length = std::sqrt(Dot);
  return _Vector / Length;
}

} // namespace core
