#pragma once

#include "editor/asset/AssetDocument.h"
#include "nlohmann/json_fwd.hpp"

#include <string>
#include <memory>
#include <imgui.h>

namespace core
{

class CScene;
class CSceneNode;
class CSceneSerializer;

} // namespace core

namespace editor
{

class CNodeCopyBuffer;
struct TNodeSnapshot;

class CSceneDocument:
  public CAssetDocument
{
public: // Constructor/Destructor

  CSceneDocument(std::string _FilePath);

  virtual ~CSceneDocument();

public: // CAssetDocument

  bool Load() override;

  bool Save() override;

  void Update(float _DeltaTime) override;

  void Render(const core::CCamera & _Camera) override;

public: // Interface

  std::shared_ptr<core::CSceneNode> GetRootNode() const;

  std::shared_ptr<core::CSceneNode> FindNodeByPosition(const ImVec2 & _WorldPos) const;

  std::shared_ptr<core::CSceneNode> CreateNode(const std::string & _TypeID, const std::shared_ptr<core::CSceneNode> & _Parent, const ImVec2 & _WorldPos);

  void PasteNode(const CNodeCopyBuffer & _Buffer, const std::shared_ptr<core::CSceneNode> & _Parent);

  bool DeleteNode(const std::shared_ptr<core::CSceneNode> & _Node);

  void RenameNode(const std::string & _Name, const std::string & _NewName);

  void MarkDirty();

  nlohmann::ordered_json SerializeScene() const;

private: // Service

  void CreateNodeFromSnapshot(const TNodeSnapshot & _Snapshot, const std::shared_ptr<core::CSceneNode> & _Parent);

protected: // Attr

  std::shared_ptr<core::CScene>           m_Scene;
  std::shared_ptr<core::CSceneSerializer> m_Serializer;
};

} // namespace editor