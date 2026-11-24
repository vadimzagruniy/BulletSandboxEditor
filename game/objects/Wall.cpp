#include "Wall.h"

#include "core/camera/Camera.h"
#include "core/property/IPropertyVisitor.h"
#include "game/runtime_objects/Bullet.h"
#include "nlohmann/json.hpp"

namespace game
{

namespace // unnamed
{

constexpr const char * STRENGTH_TAG = "Strength";

} // namespace unnamed

//
// Constructor/Destructor
//

CWall::CWall():
  m_Strength(1000.0f),
  m_IsDestroyed(false)
{
  m_Color = ImVec4(0.5f, 0.9f, 1.0f, 1.0f);
  m_Size  = ImVec2(300.0f, 100.0f);
}

CWall::~CWall() = default;

//
// CSceneNode
//

void CWall::Update(float _DelataTime)
{
  CSceneNode::Update(_DelataTime);
}

nlohmann::ordered_json CWall::Serialize()
{
  nlohmann::ordered_json Data = CSceneNode::Serialize();

  Data[STRENGTH_TAG] = m_Strength;

  return Data;
}

void CWall::Deserialize(const nlohmann::ordered_json & _Data)
{
  CSceneNode::Deserialize(_Data);

  m_Strength = _Data.value(STRENGTH_TAG, m_Strength);
}

std::string_view CWall::GetTypeID() const
{
  return TYPE_ID;
}

void CWall::InspectProperties(const std::shared_ptr<core::IPropertyInspector> & _PropertyInspector)
{
  CSceneNode::InspectProperties(_PropertyInspector);

  _PropertyInspector->BeginGroup(TYPE_ID.data());
  _PropertyInspector->Float(STRENGTH_TAG, m_Strength, std::numeric_limits<float>::max(), 1.0f);
}

//
// CSceneNode service
//

void CWall::RenderContent(const core::CCamera & _Camera)
{
  CSceneNode::RenderContent(_Camera);
}

//
// Interface
//

bool CWall::IsDestroyed() const
{
  return m_IsDestroyed;
}

//
// Service
//

void CWall::ReflectBullet(TBullet & _Bullet)
{
  const ImVec2 LocalPrevPos = WorldToLocal(_Bullet.PrevPosition);
  const ImVec2 NodeHalfSize = GetSize() * 0.5f;

  ImVec2 LocalVelocity = WorldToLocal(_Bullet.PrevPosition + _Bullet.Velocity) - LocalPrevPos;

  if (std::abs(LocalPrevPos.x) / NodeHalfSize.x > std::abs(LocalPrevPos.y) / NodeHalfSize.y)
    LocalVelocity.x = -LocalVelocity.x;
  else
    LocalVelocity.y = -LocalVelocity.y;

  _Bullet.Velocity = LocalToWorld(LocalPrevPos + LocalVelocity) - _Bullet.PrevPosition;
  _Bullet.Position = _Bullet.PrevPosition;
  _Bullet.LifeTime *= 0.5f;
}

//
// Events
//

void CWall::OnHit(TBullet & _Bullet)
{
  if (IsDestroyed())
    return;

  ReflectBullet(_Bullet);

  m_Strength -=_Bullet.Damage;
  if (m_Strength < 0.0f)
    m_IsDestroyed = true;
}

} // namespace game
