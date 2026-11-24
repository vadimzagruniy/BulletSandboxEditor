#include "SceneSimulator.h"

#include "editor/scene_editor/SceneDocument.h"
#include "core/game/IGameRuntime.h"
#include "core/game/GameRuntimeFactory.h"
#include "nlohmann/json.hpp"

namespace editor
{

namespace // unnamed
{

constexpr float FIXED_DELATA_TIME = 1.0f / 60.0f;

} // namespace unnamed


//
// Constructor
//

CSceneSimulator::CSceneSimulator(std::shared_ptr<CSceneDocument> _SceneDocument):
  m_SceneDocument(std::move(_SceneDocument)),
  m_State(ESimulationState::Stoped)
{
  // Empty
}

//
// Interface
//

void CSceneSimulator::Update(float _TimeDelta)
{
  if (m_GameRuntime && m_State == ESimulationState::Playing)
    m_GameRuntime->Update(_TimeDelta);
}

void CSceneSimulator::Render(const core::CCamera & _Camera)
{
  if (m_GameRuntime)
    m_GameRuntime->Render(_Camera);
}

CSceneSimulator::ESimulationState CSceneSimulator::GetState() const
{
  return m_State;
}

bool CSceneSimulator::IsSimulating() const
{
  return m_State != ESimulationState::Stoped;
}

bool CSceneSimulator::Start()
{
  if (IsSimulating())
    return true;

  auto Runtime = core::GetGameRuntimeFactory().CreateRuntime();
  if (Runtime == nullptr || !Runtime->Start(m_SceneDocument->SerializeScene()))
    return false;

  m_GameRuntime = std::move(Runtime);
  m_State       = ESimulationState::Playing;

  return true;
}

void CSceneSimulator::Stop()
{
  if (!IsSimulating())
    return;

  m_GameRuntime->Stop();
  m_GameRuntime.reset();

  m_State = ESimulationState::Stoped;
}

void CSceneSimulator::PauseResume()
{
  if (!IsSimulating())
    return;

  const bool IsPlaying = m_State == ESimulationState::Playing;
  m_State = IsPlaying ? ESimulationState::Paused : ESimulationState::Playing;
}

} // namespace editor