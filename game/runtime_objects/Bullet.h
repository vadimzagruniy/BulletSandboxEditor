#pragma once

#include "imgui.h"

namespace core
{

class CCamera;
class CSceneNode;

} // namespace core

namespace game
{

struct TBullet
{
  ImVec2 Velocity;
  ImVec2 Position;
  ImVec2 PrevPosition;
  float  LifeTime = 0;
  float  Damage   = 0;

public: // Interface

  bool IsAlive() const;

  void Render(const core::CCamera & _Camera);

  void Update(float _TimeDelta);
};

} // namespace game