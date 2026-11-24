#include "NodeCopyBuffer.h"

#include <core/scene/SceneNode.h>

namespace editor
{

//
// Interface
//

void CNodeCopyBuffer::MakeSnapshot(const std::shared_ptr<core::CSceneNode> & _Node)
{
  Clear();

  if (!_Node->IsRoot())
    FillSnapshot(_Node, m_NodeSnapshot.emplace());
}

TNodeSnapshot CNodeCopyBuffer::GetSnapshot() const
{
  return m_NodeSnapshot.value_or(TNodeSnapshot());
}

bool CNodeCopyBuffer::IsEmpty() const
{
  return !m_NodeSnapshot.has_value();
}

void CNodeCopyBuffer::Clear()
{
  m_NodeSnapshot.reset();
}

//
// Service
//

void CNodeCopyBuffer::FillSnapshot(const std::shared_ptr<core::CSceneNode> & _Node, TNodeSnapshot & _Snapshot)
{
  _Snapshot.TypeID = _Node->GetTypeID();
  _Snapshot.Name   = _Node->GetName();
  _Snapshot.Data   = _Node->Serialize();

  for (const auto & Child : _Node->GetChildren(false))
  {
    const auto SceneChild = std::static_pointer_cast<core::CSceneNode>(Child);
    FillSnapshot(SceneChild, _Snapshot.Children.emplace_back());
  }
}

} // namespace editor