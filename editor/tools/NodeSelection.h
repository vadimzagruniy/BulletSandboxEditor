#pragma once

#include <memory>

namespace core
{

class CNode;

} // namespace core

namespace editor
{

class CNodeSelection
{
public: // Interface

  void Select(const std::shared_ptr<core::CNode> & _Node);

  void ClearSelection();

  std::shared_ptr<core::CNode> GetSelected() const;

  bool IsSelected(const std::shared_ptr<core::CNode> & _Node) const;

private: // Attr

  std::weak_ptr<core::CNode> m_Selected;
};

} // namespace editor