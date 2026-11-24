#include "Bullet.h"

#include "core/camera/Camera.h"
#include "core/scene/SceneNode.h"
#include "core/utils/Math.h"

namespace game
{

//
// Interface
//

bool TBullet::IsAlive() const
{
  return LifeTime > 0;
}

void TBullet::Render(const core::CCamera & _Camera)
{
  if (!IsAlive())
    return;

  auto * DrawList = ImGui::GetWindowDrawList();
  DrawList->AddCircleFilled(_Camera.ToScreen(Position), 8.f, IM_COL32(200, 70, 70, 255));
}

void TBullet::Update(float _TimeDelta)
{
  if (!IsAlive())
    return;

  PrevPosition = Position;
  Position += Velocity * _TimeDelta;
  LifeTime -= _TimeDelta;
}

} // namespace game
