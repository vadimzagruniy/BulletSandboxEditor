#include "Camera.h"

#include <algorithm>

namespace core
{

//
// Constructor
//

CCamera::CCamera():
  m_Zoom(1.0)
{
  // Empty
}

//
// Interface
//

ImVec2 CCamera::ToScreen(const ImVec2 & _World) const
{
  ImVec2 ScreenPos;
  
  ScreenPos.x = m_Position.x + m_Offset.x + _World.x * m_Zoom;
  ScreenPos.y = m_Position.y + m_Offset.y + _World.y * m_Zoom;

  return ScreenPos;
}

ImVec2 CCamera::ToWorld(const ImVec2 & _Screen) const
{
  ImVec2 WorldPos;

  WorldPos.x = (_Screen.x - m_Position.x - m_Offset.x) / m_Zoom;
  WorldPos.y = (_Screen.y - m_Position.y - m_Offset.y) / m_Zoom;

  return WorldPos;
}

void CCamera::SetPosition(const ImVec2 & _Position)
{
  m_Position = _Position;
}

void CCamera::UpdateOffset(const ImVec2 & _OffsetDelta)
{
  m_Offset.x += _OffsetDelta.x;
  m_Offset.y += _OffsetDelta.y;
}

void CCamera::UpdateZoom(float _MouseWheel, const ImVec2 & _MousePos)
{
  ImVec2 MouseWorldPos = ToWorld(_MousePos);

  m_Zoom = std::clamp(m_Zoom * (_MouseWheel > 0.0f ? 1.1f : 1 / 1.1f), 0.1f, 10.f);

  m_Offset.x = _MousePos.x - m_Position.x - MouseWorldPos.x * m_Zoom;
  m_Offset.y = _MousePos.y - m_Position.y - MouseWorldPos.y * m_Zoom;
}

} // namespace core
