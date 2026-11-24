#pragma once

#include "nlohmann/json_fwd.hpp"
#include <memory>

namespace core
{

class CCamera;

class IGameRuntime
{
public: // Destructor

  virtual ~IGameRuntime() = default;

public: // Interface

  virtual bool Start(const nlohmann::ordered_json & _SceneData) = 0;

  virtual void Stop() = 0;

  virtual void Update(float _TimeDelta) = 0;

  virtual void Render(const CCamera & _Camere) = 0;
};

} // namespace core
