#pragma once

#include "SceneNode.h"

#include <unordered_map>
#include <functional>

namespace core
{

class CObjectFactory
{
public: // Types

  using TFactoryFunc = std::function<std::shared_ptr<CSceneNode>()>;

public: //Constructor

  CObjectFactory();

  CObjectFactory(const CObjectFactory &) = delete;

  CObjectFactory(CObjectFactory &&) = delete;

public: // Operators

  CObjectFactory & operator = (const CObjectFactory &) = delete;

  CObjectFactory & operator = (const CObjectFactory &&) = delete;

public: // Interface

  template<typename Type>
  bool Register()
  {
    static_assert(std::is_base_of_v<CSceneNode, Type>);

    return Register(Type::TYPE_ID.data(), [](){ return std::make_shared<Type>(); });
  }

  bool Register(const std::string & _TypeName, TFactoryFunc _FactoryFunc);

  std::shared_ptr<CSceneNode> Create(const std::string & _TypeName) const;

  std::vector<std::string> GetTypeNames() const;

private: // Attr

  std::unordered_map<std::string, TFactoryFunc> m_Factory;
};

CObjectFactory & GetObjectFactory();

} // namespace core
