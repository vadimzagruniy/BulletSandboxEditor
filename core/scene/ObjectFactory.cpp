#include "ObjectFactory.h"

namespace core
{

//
// Constructor
//

CObjectFactory::CObjectFactory() = default;

//
// Interface
//

bool CObjectFactory::Register(const std::string & _TypeName, TFactoryFunc _FactoryFunc)
{
  if (!_FactoryFunc)
    return false;

  return m_Factory.emplace(_TypeName, std::move(_FactoryFunc)).second;
}

std::shared_ptr<CSceneNode> CObjectFactory::Create(const std::string & _TypeName) const
{
  const auto FuncIter = m_Factory.find(_TypeName);
  if (FuncIter == m_Factory.end())
    return nullptr;

  return FuncIter->second();
}

std::vector<std::string> CObjectFactory::GetTypeNames() const
{
  std::vector<std::string> TypeNames;
  for (const auto & ObjectItem : m_Factory)
    TypeNames.push_back(ObjectItem.first);

  return TypeNames;
}

CObjectFactory & GetObjectFactory()
{
  static CObjectFactory Factory;
  return Factory;
}

} // namespace core
