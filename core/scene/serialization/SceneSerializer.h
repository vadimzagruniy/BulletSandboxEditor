#pragma once

#include "core/scene/Scene.h"

#include "nlohmann/json_fwd.hpp"

#include <string>
#include <memory>

namespace core
{

class CSceneSerializer
{
public: // Constructor

  CSceneSerializer();

public: // Interface

  bool LoadScene(const std::shared_ptr<CScene> & _Scene, const std::string & _FilePath) const;

  bool SaveScene(const std::shared_ptr<CScene> & _Scene, const std::string & _FilePath) const;

  bool LoadSceneData(const std::shared_ptr<CScene> & _Scene, const nlohmann::ordered_json & _Root) const;

  nlohmann::ordered_json SaveSceneData(const std::shared_ptr<CScene> & _Scene) const;

private: // Service

  void LoadChildNode(const std::shared_ptr<CSceneNode> & _Parent, const nlohmann::ordered_json & _Data) const;

  nlohmann::ordered_json SaveChildNode(const std::shared_ptr<CSceneNode> & _Node) const;
};

} // namespace core
