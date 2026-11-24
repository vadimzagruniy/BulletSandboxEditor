#pragma once

#include "core/scene/SceneNode.h"

namespace core
{

class CSceneRoot:
  public core::CSceneNode
{
public: // Statics

  static constexpr std::string_view TYPE_ID = "SceneRoot";

public: // Constructor

  CSceneRoot();

public: // CSceneNode

  void Update(float _DelataTime) override;

  std::string_view GetTypeID() const override;
};

} // namespace core
