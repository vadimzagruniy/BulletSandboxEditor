#pragma once

#include <nlohmann/json.hpp>
#include <memory>
#include <vector>
#include <optional>

namespace core
{

class CSceneNode;

} // namespace core

namespace editor
{

struct TNodeSnapshot
{
  std::string                TypeID;
  std::string                Name;
  std::string                ParentName;
  nlohmann::ordered_json     Data;
  std::vector<TNodeSnapshot> Children;
};

class CNodeCopyBuffer
{
public: // Interface

  void MakeSnapshot(const std::shared_ptr<core::CSceneNode> & _Node);

  TNodeSnapshot GetSnapshot() const;

  bool IsEmpty() const;

  void Clear();

private: // Service

  void FillSnapshot(const std::shared_ptr<core::CSceneNode> & _Node, TNodeSnapshot & _Snapshot);

private: // Attr

  std::optional<TNodeSnapshot> m_NodeSnapshot;
};

} // namespace editor