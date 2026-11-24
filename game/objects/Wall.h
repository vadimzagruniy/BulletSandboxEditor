#pragma once

#include "core/scene/SceneNode.h"

namespace game
{

struct TBullet;

class CWall:
  public core::CSceneNode
{
public: // Statics

  static constexpr std::string_view TYPE_ID = "Wall";

public: // Constructor/Destructor

  CWall();

  ~CWall();

public: // CSceneNode

  void Update(float _DelataTime) override;

  nlohmann::ordered_json Serialize() override;

  void Deserialize(const nlohmann::ordered_json & _Data) override;

  std::string_view GetTypeID() const override;

  void InspectProperties(const std::shared_ptr<core::IPropertyInspector> & _PropertyInspector) override;

 protected: // CSceneNode service

  void RenderContent(const core::CCamera & _Camera) override;

public: // Interface

  bool IsDestroyed() const;

private: // Service

  void ReflectBullet(TBullet & _Bullet);

public: // Events

  void OnHit(TBullet & _Bullet);

private: // Attr

  float m_Strength;
  bool  m_IsDestroyed;
};

} // namespace game
