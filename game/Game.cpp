#include "Game.h"

#include "GameRuntime.h"

#include "objects/Wall.h"
#include "objects/BulletEmitter.h"

#include "core/scene/ObjectFactory.h"
#include "core/game/GameRuntimeFactory.h"


namespace game
{

//
// Constructor/Destructor
//

CGame::CGame() = default;

CGame::~CGame() = default;

//
// Interface
//

bool CGame::Init()
{
  RegisterGameObjects();

  core::GetGameRuntimeFactory().Setup([](){ return std::make_shared<CGameRuntime>(); });

  return true;
}

void CGame::Shutdown()
{
  // Empty
}

//
// Service
//

void CGame::RegisterGameObjects()
{
  auto & ObjectFactory = core::GetObjectFactory();

  ObjectFactory.Register<CWall>();
  ObjectFactory.Register<CBulletEmitter>();
}

} // namespace game
