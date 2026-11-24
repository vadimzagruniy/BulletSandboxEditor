#pragma once

#include "core/scene/SceneNode.h"
#include "game/runtime_objects/Bullet.h"

namespace game
{

class CBulletEmitter:
  public core::CSceneNode
{
public: // Statics

  static constexpr std::string_view TYPE_ID = "BulletEmitter";

public: // Constructor/Destructor

  CBulletEmitter();

  ~CBulletEmitter();

public: // CSceneNode

  void Update(float _DelataTime) override;

  nlohmann::ordered_json Serialize() override;

  void Deserialize(const nlohmann::ordered_json & _Data) override;

  std::string_view GetTypeID() const override;

  void InspectProperties(const std::shared_ptr<core::IPropertyInspector> & _PropertyInspector) override;

 protected: // CSceneNode service

  void RenderContent(const core::CCamera & _Camera) override;

public: // Interface

  virtual std::vector<TBullet> FireBullets();

  bool IsCharged() const;

private: // Service

  void GetGunCorners(TCorners & _Corners) const;

  std::tuple<ImVec2, ImVec2> GetGunRect() const;

private: // Attr

  float m_BulletLifeTime;
  float m_BulletDamage;
  float m_ReloadTime;

  bool  m_IsCharged;
  float m_ReloadTimer;
};

} // namespace game
