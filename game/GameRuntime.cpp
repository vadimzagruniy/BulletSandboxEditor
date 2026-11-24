#include "GameRuntime.h"

#include "objects/Wall.h"
#include "objects/BulletEmitter.h"

#include "core/scene/Scene.h"
#include "core/scene/ObjectFactory.h"
#include "core/scene/serialization/SceneSerializer.h"

namespace game
{

//
// Constructor/Destructor
//

CGameRuntime::CGameRuntime():
  m_SceneSerializer(std::make_shared<core::CSceneSerializer>())
{
  // Empty
}

CGameRuntime::~CGameRuntime() = default;

//
// IGameRuntime
//

bool CGameRuntime::Start(const nlohmann::ordered_json & _SceneData)
{
  m_Scene = std::make_shared<core::CScene>();

  if (!m_SceneSerializer->LoadSceneData(m_Scene, _SceneData))
  {
    m_Scene.reset();
    return false;
  }
  
  CollectOgjects();

  return true;
}

void CGameRuntime::Stop()
{
  Clear();

  m_Scene.reset();
}

void CGameRuntime::Update(float _TimeDelta)
{
  if (m_Scene)
    m_Scene->Update(_TimeDelta);

  UpdateEmitters();
  UpdateBullets(_TimeDelta);

  UpdateCollision();

  RemoveDestroyedObjects();
}

void CGameRuntime::Render(const core::CCamera & _Camere)
{
  if (m_Scene)
    m_Scene->Render(_Camere);

  RenderBullets(_Camere);
}

//
// Service
//

void CGameRuntime::CollectOgjects()
{
  Clear();

  const auto Root     = m_Scene->GetRootNode();
  const auto Children = Root->GetChildren(true);

  for (const auto & Child : Children)
  {
    const auto SceneChild = std::static_pointer_cast<core::CSceneNode>(Child);
    if (SceneChild->GetTypeID() == CWall::TYPE_ID)
      m_Walls.push_back(std::static_pointer_cast<CWall>(SceneChild));

    if (SceneChild->GetTypeID() == CBulletEmitter::TYPE_ID)
      m_Emitters.push_back(std::static_pointer_cast<CBulletEmitter>(SceneChild));
  }

}

void CGameRuntime::Clear()
{
  m_Walls.clear();
  m_Emitters.clear();
  m_Bullets.clear();
}

void CGameRuntime::UpdateEmitters()
{
  for (const auto & Emmiter : m_Emitters)
  {
    if (Emmiter->IsCharged())
    {
      const auto Bullets = Emmiter->FireBullets();
      m_Bullets.insert(m_Bullets.cbegin(), Bullets.cbegin(), Bullets.cend());
    }
  }
}

void CGameRuntime::UpdateBullets(float _TimeDelta)
{
  const auto Root = m_Scene->GetRootNode();
  for (auto & Bullet : m_Bullets)
  {
    Bullet.Update(_TimeDelta);
    if (!Root->HitTest(Bullet.Position))
      Bullet.LifeTime = -1.0f;
  }
}

void CGameRuntime::UpdateCollision()
{
  for (TBullet & Bullet : m_Bullets)
  {
    for (const auto & Wall : m_Walls)
    {
      if (Wall->HitTest(Bullet.Position))
        Wall->OnHit(Bullet);
    }
  }
}

void CGameRuntime::RenderBullets(const core::CCamera & _Camere)
{
  for (auto & Bullet : m_Bullets)
    Bullet.Render(_Camere);
}

void CGameRuntime::RemoveDestroyedObjects()
{
  std::erase_if(m_Bullets, [](const TBullet & _Bullet){ return !_Bullet.IsAlive(); });
  
  std::erase_if(m_Walls, [](const auto & _Wall)
    {
      if (!_Wall->IsDestroyed())
        return false;

      if (const auto Parent = _Wall->GetParent().lock())
        Parent->DeleteChild(_Wall->GetName());

      return true;
    });
}

} // namespace game
