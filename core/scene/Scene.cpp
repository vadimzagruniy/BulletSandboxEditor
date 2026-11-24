#include "Scene.h"
#include "SceneRoot.h"

namespace core
{

//
// Constructor/Destructor
//

CScene::CScene():
  m_RootNode(std::make_shared<CSceneRoot>())
{
  // Empty
}

CScene::~CScene() = default;

//
// Interface
//

std::shared_ptr<CSceneNode> CScene::GetRootNode() const
{
  return m_RootNode;
}

void CScene::Render(const core::CCamera & _Camera)
{
  m_RootNode->Render(_Camera);
}

void CScene::Update(float _DelataTime)
{
  m_RootNode->Update(_DelataTime);
}

} // namespace core
