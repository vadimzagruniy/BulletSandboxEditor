#pragma once

#include "core/game/IGameRuntime.h"
#include "game/runtime_objects/Bullet.h"
#include <memory>

namespace core
{

class CCamera;
class CScene;
class CSceneSerializer;

} // namespace core

namespace game
{

class CWall;
class CBulletEmitter;

class CGameRuntime:
  public core::IGameRuntime
{
public: // Constructor/Destructor

  CGameRuntime();

  ~CGameRuntime();

public: // IGameRuntime

  bool Start(const nlohmann::ordered_json & _SceneData) override;

  void Stop() override;

  void Update(float _TimeDelta) override;

  void Render(const core::CCamera & _Camere) override;

public: // Service

  void CollectOgjects();

  void Clear();

  void UpdateEmitters();

  void UpdateBullets(float _TimeDelta);

  void UpdateCollision();

  void RenderBullets(const core::CCamera & _Camere);

  void RemoveDestroyedObjects();

private: // Attr

  std::shared_ptr<core::CScene>           m_Scene;
  std::shared_ptr<core::CSceneSerializer> m_SceneSerializer;

  std::vector<std::shared_ptr<CWall>>          m_Walls;
  std::vector<std::shared_ptr<CBulletEmitter>> m_Emitters;
  std::vector<TBullet>                         m_Bullets;
};

} // namespace game
