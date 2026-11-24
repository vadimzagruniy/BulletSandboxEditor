#pragma once

#include "imgui.h"

namespace core
{

class CCamera
{
public: // Constructor

  CCamera();

public: // Interface

  ImVec2 ToScreen(const ImVec2 & _World) const;

  ImVec2 ToWorld(const ImVec2 & _Screen) const;

  void SetPosition(const ImVec2 & _Position);

  void UpdateOffset(const ImVec2 & _OffsetDelta);

  void UpdateZoom(float _MouseWheel, const ImVec2 & _MousePos);

private: // Attr

  ImVec2 m_Position;
  ImVec2 m_Offset;
  float  m_Zoom;
};

} // namespace core
