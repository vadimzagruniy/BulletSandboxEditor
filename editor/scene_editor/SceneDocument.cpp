
#include "SceneDocument.h"

#include "core/scene/Scene.h"
#include "core/scene/serialization/SceneSerializer.h"
#include "core/scene/ObjectFactory.h"
#include "editor/scene_editor/tools/NodeCopyBuffer.h"
#include "nlohmann/json.hpp"

namespace editor
{

//
// Constructor/Destructor
//

CSceneDocument::CSceneDocument(std::string _FilePath):
  CAssetDocument(_FilePath),
  m_Scene(std::make_shared<core::CScene>()),
  m_Serializer(std::make_shared<core::CSceneSerializer>())
{
  // Empty
}

CSceneDocument::~CSceneDocument() = default;

//
// CAssetDocument
//

bool CSceneDocument::Load()
{
  return m_Serializer->LoadScene(m_Scene, m_FilePath);
}

bool CSceneDocument::Save()
{
  if (!m_Serializer->SaveScene(m_Scene, m_FilePath))
    return false;

  m_IsDirty = false;

  return true;
}

void CSceneDocument::Update(float _DeltaTime)
{
  m_Scene->Update(_DeltaTime);
}

void CSceneDocument::Render(const core::CCamera & _Camera)
{
  m_Scene->Render(_Camera);
}

//
// Interface
//

std::shared_ptr<core::CSceneNode> CSceneDocument::GetRootNode() const
{
  return m_Scene->GetRootNode();
}

std::shared_ptr<core::CSceneNode> CSceneDocument::FindNodeByPosition(const ImVec2 & _WorldPos) const
{
  const auto Root = GetRootNode();
  auto Children   = GetRootNode()->GetChildren(true);

  Children.insert(Children.cbegin(), Root);
  for (auto Iterator = Children.crbegin(); Iterator != Children.crend(); ++Iterator)
  {
    const auto & Node = std::static_pointer_cast<core::CSceneNode>(*Iterator);
    if (Node->HitTest(_WorldPos))
      return Node;
  }

  return nullptr;
}

std::shared_ptr<core::CSceneNode> CSceneDocument::CreateNode(const std::string & _TypeID, const std::shared_ptr<core::CSceneNode> & _Parent, const ImVec2 & _WorldPos)
{
  const auto Node = core::GetObjectFactory().Create(_TypeID);
  if (Node == nullptr)
    return nullptr;

  const auto Parent = _Parent != nullptr ? _Parent : GetRootNode();
  
  Node->SetName(core::CNode::FindAvailableNameForNode(Parent, _TypeID));
  Node->SetOrigin(_Parent->WorldToLocal(_WorldPos));
  Parent->AddChild(Node);

  m_IsDirty = true;

  return Node;
}

void CSceneDocument::PasteNode(const CNodeCopyBuffer & _Buffer, const std::shared_ptr<core::CSceneNode> & _Parent)
{
  if (!_Buffer.IsEmpty())
    CreateNodeFromSnapshot(_Buffer.GetSnapshot(), _Parent);
}

bool CSceneDocument::DeleteNode(const std::shared_ptr<core::CSceneNode> & _Node)
{
  if (_Node == nullptr || _Node->IsRoot())
    return false;

  auto Parent = _Node->GetParent().lock();
  if (Parent == nullptr)
    return false;

  if (!Parent->DeleteChild(_Node->GetName()))
    return false;

  MarkDirty();

  return true;
}

void CSceneDocument::RenameNode(const std::string & _Name, const std::string & _NewName)
{
  const auto Root = GetRootNode();
  const auto Node = Root->FindChild(_Name);

  if (Node != nullptr && Node->CanBeIdentifiedAs(_NewName))
  {
    Node->SetName(_NewName);
    MarkDirty();
  }
}

void CSceneDocument::MarkDirty()
{
  m_IsDirty = true;
}

nlohmann::ordered_json CSceneDocument::SerializeScene() const
{
  return m_Serializer->SaveSceneData(m_Scene);
}

void CSceneDocument::CreateNodeFromSnapshot(const TNodeSnapshot & _Snapshot, const std::shared_ptr<core::CSceneNode> & _Parent)
{
  const auto Node = core::GetObjectFactory().Create(_Snapshot.TypeID);
  if (Node == nullptr)
    return;

  const auto Parent = _Parent != nullptr ? _Parent : GetRootNode();
  
  Node->SetName(core::CNode::FindAvailableNameForNode(Parent, _Snapshot.Name));
  Node->Deserialize(_Snapshot.Data);
  Parent->AddChild(Node);

  for (auto & Child : _Snapshot.Children)
    CreateNodeFromSnapshot(Child, Node);
}

} // namespace editor