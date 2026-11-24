#include "SceneSerializer.h"

#include "core/scene/ObjectFactory.h"
#include "nlohmann/json.hpp"

#include <fstream>
#include <filesystem>

namespace core
{

namespace // unnamed
{

//
// Constants
//

constexpr const char * NAME_TAG       = "Name";
constexpr const char * TYPE_ID_TAG    = "TypeID";
constexpr const char * CHILDREN_TAG   = "Children";
constexpr const char * PROPERTIES_TAG = "Properties";
constexpr const char * SCENE_DATA_TAG = "SceneData";
constexpr const char * VERSION_TAG    = "Version";

//
// Utils
//

std::string LoadStringFromFile(const std::string & _FilePath)
{
  std::ifstream File(_FilePath);

  if (!File.is_open())
    return {};

  std::stringstream Buffer;
  Buffer << File.rdbuf();

  return Buffer.str();
}

bool SaveStringToFile(const std::string & _Data, const std::string & _FilePath)
{
  std::ofstream File(_FilePath);

  if (!File.is_open())
    return false;

  File << _Data;

  return File.good();
}

} // namespace unnamed

//
// Constructor
//

CSceneSerializer::CSceneSerializer() = default;

//
// Interface
//

bool CSceneSerializer::LoadScene(const std::shared_ptr<CScene> & _Scene, const std::string & _FilePath) const
{
  auto Root = nlohmann::ordered_json::parse(LoadStringFromFile(_FilePath), nullptr, false);
  if (Root.is_discarded() || !Root.is_object())
    return false;

  return LoadSceneData(_Scene, Root);
}

bool CSceneSerializer::SaveScene(const std::shared_ptr<CScene> & _Scene, const std::string & _FilePath) const
{
  nlohmann::ordered_json Root = SaveSceneData(_Scene);
  return SaveStringToFile(Root.dump(2), _FilePath);
}

bool CSceneSerializer::LoadSceneData(const std::shared_ptr<CScene> & _Scene, const nlohmann::ordered_json & _Root) const
{
  if (!_Root.is_object())
    return false;

  const auto   RootNode  = _Scene->GetRootNode();
  const auto & SceneData = _Root.at(SCENE_DATA_TAG);

  const auto   Name       = SceneData.at(NAME_TAG).get<std::string>();
  const auto & Children   = SceneData.at(CHILDREN_TAG);
  const auto & Properties = SceneData.at(PROPERTIES_TAG);

  RootNode->SetName(Name);
  RootNode->Deserialize(Properties);

  for (const auto & ChildData : Children)
    LoadChildNode(RootNode, ChildData);

  return true;
}

nlohmann::ordered_json CSceneSerializer::SaveSceneData(const std::shared_ptr<CScene> & _Scene) const
{
  nlohmann::ordered_json Root;

  Root[VERSION_TAG]    = 1;
  Root[SCENE_DATA_TAG] = SaveChildNode(_Scene->GetRootNode());

  return Root;
}

//
// Service
//

void CSceneSerializer::LoadChildNode(const std::shared_ptr<CSceneNode> & _Parent, const nlohmann::ordered_json & _Data) const
{
  const auto   Name       = _Data.at(NAME_TAG).get<std::string>();
  const auto   TypeID     = _Data.at(TYPE_ID_TAG).get<std::string>();
  const auto & Children   = _Data.at(CHILDREN_TAG);
  const auto & Properties = _Data.at(PROPERTIES_TAG);

  const auto Node = core::GetObjectFactory().Create(TypeID);
  Node->SetName(Name);
  Node->Deserialize(Properties);

  if (_Parent != nullptr)
    _Parent->AddChild(Node);

  for (const auto & ChildData : Children)
    LoadChildNode(Node, ChildData);
}

nlohmann::ordered_json CSceneSerializer::SaveChildNode(const std::shared_ptr<CSceneNode> & _Node) const
{
  nlohmann::ordered_json NodeData;
  nlohmann::ordered_json Children;

  for (const auto & Child : _Node->GetChildren(false))
    Children.push_back(SaveChildNode(std::static_pointer_cast<CSceneNode>(Child)));

  NodeData[NAME_TAG]       = _Node->GetName();
  NodeData[TYPE_ID_TAG]    = _Node->GetTypeID();
  NodeData[PROPERTIES_TAG] = _Node->Serialize();
  NodeData[CHILDREN_TAG]   = Children;

  return NodeData;
}

} // namespace core
