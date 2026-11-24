#pragma once

#include "IGameRuntime.h"
#include <memory>
#include <functional>

namespace core
{

class CGameRuntimeFactory
{
public: // Types

  using TFactoryFn = std::function<std::shared_ptr<core::IGameRuntime>()>;

public: // Constructor

  CGameRuntimeFactory();

  CGameRuntimeFactory(const CGameRuntimeFactory &) = delete;

  CGameRuntimeFactory(CGameRuntimeFactory &&) = delete;

public: // Operators

  CGameRuntimeFactory & operator = (const CGameRuntimeFactory &) = delete;

  CGameRuntimeFactory & operator = (const CGameRuntimeFactory &&) = delete;

public: // Interface

  void Setup(TFactoryFn _Factory);

  std::shared_ptr<core::IGameRuntime> CreateRuntime();

private: // Attr

  TFactoryFn m_RuntimeFactory;
};

//
// Singlton
//

CGameRuntimeFactory & GetGameRuntimeFactory();

} // namespace core
