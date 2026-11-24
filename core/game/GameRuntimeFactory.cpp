#include "GameRuntimeFactory.h"

namespace core
{

CGameRuntimeFactory::CGameRuntimeFactory():
  m_RuntimeFactory([](){ return nullptr; })
{
  // Empty
}

void CGameRuntimeFactory::Setup(TFactoryFn _Factory)
{
  m_RuntimeFactory = std::move(_Factory);
}

std::shared_ptr<IGameRuntime> CGameRuntimeFactory::CreateRuntime()
{
  return m_RuntimeFactory();
}

//
// Singlton
//

CGameRuntimeFactory & GetGameRuntimeFactory()
{
  static CGameRuntimeFactory Factory;
  return Factory;
}

} // namespace core