#include "SceneRoot.h"

#include "nlohmann/json.hpp"

namespace core
{

//
// Constructor
//

CSceneRoot::CSceneRoot()
{
  m_Color  = ImVec4(0.3f, 0.5f, 0.9f, 0.5f);
  m_Size   = ImVec2(1000.0f, 1000.0f);
  m_Origin = m_Size * 0.5f;
}

//
// CSceneNode
//

void CSceneRoot::Update(float _DelataTime)
{
  CSceneNode::Update(_DelataTime);
}

std::string_view CSceneRoot::GetTypeID() const
{
  return TYPE_ID;
}

} // namespace core
