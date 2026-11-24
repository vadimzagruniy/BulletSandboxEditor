#include "BulletEmitter.h"

#include "core/camera/Camera.h"
#include <core/property/IPropertyVisitor.h>
#include "core/utils/Math.h"
#include "nlohmann/json.hpp"

namespace game
{

namespace // unnamed
{

constexpr const char * BULLET_LIFE_TIME_TAG  = "BulletLifeTime";
constexpr const char * BULLET_DAMAGE_TAG     = "BulletDamage";
constexpr const char * RELOAD_TIME_TAG       = "ReloadTime";

} // namespace unnamed

//
// Constructor/Destructor
//

CBulletEmitter::CBulletEmitter():
  m_BulletLifeTime(10.0f),
  m_BulletDamage(500.0f),
  m_ReloadTime(2.0f),
  m_IsCharged(true),
  m_ReloadTimer(0.0f)
{
  // Empty
}

CBulletEmitter::~CBulletEmitter()
{
  // Empty
}

//
// CSceneNode
//

void CBulletEmitter::Update(float _DelataTime)
{
  CSceneNode::Update(_DelataTime);

  if (!m_IsCharged)
  {
    m_ReloadTimer += _DelataTime;
    if (m_ReloadTimer > m_ReloadTime)
    {
      m_ReloadTimer = 0.0f;
      m_IsCharged  = true;
    }
  }
}

nlohmann::ordered_json CBulletEmitter::Serialize()
{
  nlohmann::ordered_json Data = CSceneNode::Serialize();

  Data[BULLET_LIFE_TIME_TAG] = m_BulletLifeTime;
  Data[BULLET_DAMAGE_TAG]    = m_BulletDamage;
  Data[RELOAD_TIME_TAG]      = m_ReloadTime;

  return Data;
}

void CBulletEmitter::Deserialize(const nlohmann::ordered_json & _Data)
{
  CSceneNode::Deserialize(_Data);

  m_BulletLifeTime = _Data.value(BULLET_LIFE_TIME_TAG, m_BulletLifeTime);
  m_BulletDamage   = _Data.value(BULLET_DAMAGE_TAG, m_BulletDamage);
  m_ReloadTime     = _Data.value(RELOAD_TIME_TAG, m_ReloadTime);
}

std::string_view CBulletEmitter::GetTypeID() const
{
  return TYPE_ID;
}

void CBulletEmitter::InspectProperties(const std::shared_ptr<core::IPropertyInspector> & _PropertyInspector)
{
  CSceneNode::InspectProperties(_PropertyInspector);

  _PropertyInspector->BeginGroup(TYPE_ID.data());

  _PropertyInspector->Float(BULLET_LIFE_TIME_TAG, m_BulletLifeTime, 10.0f, 0.0f);
  _PropertyInspector->Float(BULLET_DAMAGE_TAG, m_BulletDamage, 1000.f, 0.1f);
  _PropertyInspector->Float(RELOAD_TIME_TAG, m_ReloadTime, 30.f, 0.0f);
}

//
// CSceneNode service
//

void CBulletEmitter::RenderContent(const core::CCamera & _Camera)
{
  CSceneNode::RenderContent(_Camera);

  TCorners GunCorners;
  GetGunCorners(GunCorners);

  for (auto & Corner : GunCorners)
    Corner = _Camera.ToScreen(Corner);

  auto * DrawList = ImGui::GetWindowDrawList();
  DrawList->AddQuadFilled(GunCorners[0], GunCorners[1], GunCorners[2], GunCorners[3], IM_COL32(200, 70, 70, 255));
}

//
// Interface
//

std::vector<TBullet> CBulletEmitter::FireBullets()
{
  const auto [GunOrigin, GunSize] = GetGunRect();

  ImVec2 GunOriginWorld    = LocalToWorld(GunOrigin);
  ImVec2 UpDirectionWorld  = LocalToWorld(ImVec2(0.0f, -1.0f));
  ImVec2 GunDirectionWorld = GunOriginWorld - UpDirectionWorld;

  TBullet Bullet;
  Bullet.Damage   = m_BulletDamage;
  Bullet.LifeTime = m_BulletLifeTime;
  Bullet.Position = GunOriginWorld;
  Bullet.Velocity = GunDirectionWorld * 100.f;

  m_IsCharged = false;

  return { Bullet };
}

bool CBulletEmitter::IsCharged() const
{
  return m_IsCharged;
}

void CBulletEmitter::GetGunCorners(TCorners & _Corners) const
{
  const auto [GunOrigin, GunSize] = GetGunRect();

  ImVec2 GunHalfSize = GunSize * 0.5f;

  TCorners LocalCorners =
  {
    ImVec2(-GunHalfSize.x + GunOrigin.x, -GunHalfSize.y + GunOrigin.y),
    ImVec2( GunHalfSize.x + GunOrigin.x, -GunHalfSize.y + GunOrigin.y),
    ImVec2( GunHalfSize.x + GunOrigin.x,  GunHalfSize.y + GunOrigin.y),
    ImVec2(-GunHalfSize.x + GunOrigin.x,  GunHalfSize.y + GunOrigin.y)
  };

  for (int i = 0; i < LocalCorners.size(); ++i)
    _Corners[i] = LocalToWorld(LocalCorners[i]);
}

std::tuple<ImVec2, ImVec2> CBulletEmitter::GetGunRect() const
{
  ImVec2 GunSize   = ImVec2(m_Size.x * 0.25f, m_Size.y * 0.5f);
  ImVec2 GunOrigin = ImVec2(0.0f, -(m_Size.y * 0.3f));

  return { GunOrigin, GunSize };
}


} // namespace game
