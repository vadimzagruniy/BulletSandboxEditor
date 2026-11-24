#pragma once

#include "SceneNode.h"

namespace core
{

class CCamera;

class CScene
{
public: // Constructor/Destructor

  CScene();

  ~CScene();

public: // Interface

  std::shared_ptr<CSceneNode> GetRootNode() const;

  void Render(const core::CCamera & _Camera);

  void Update(float _DelataTime);

private: // Attr

  std::shared_ptr<CSceneNode> m_RootNode;
};

} // namespace core
