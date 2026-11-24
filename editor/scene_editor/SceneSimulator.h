#pragma once

#include <memory>

namespace core
{

class IGameRuntime;
class CCamera;

} // namespace core

namespace editor
{

class CSceneDocument;

class CSceneSimulator
{
public: // Types

  enum class ESimulationState
  {
    Stoped,
    Playing,
    Paused
  };

public: // Constructor

  CSceneSimulator(std::shared_ptr<CSceneDocument> _SceneDocument);

public: // Interface

  void Update(float _TimeDelta);

  void Render(const core::CCamera & _Camera);

  ESimulationState GetState() const;

  bool IsSimulating() const;

  bool Start();

  void Stop();

  void PauseResume();

private: // Attr

  std::shared_ptr<CSceneDocument>     m_SceneDocument;
  std::shared_ptr<core::IGameRuntime> m_GameRuntime;
  ESimulationState                    m_State;
};

} // namespace editor