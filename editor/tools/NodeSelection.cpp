#include "NodeSelection.h"

#include <core/node/Node.h>

namespace editor
{

//
// Interface
//

void CNodeSelection::Select(const std::shared_ptr<core::CNode> & _Node)
{
  if (_Node == GetSelected())
    return;

  m_Selected = _Node;

  // TODO: Notify
}
void CNodeSelection::ClearSelection()
{
  Select(nullptr);
}

std::shared_ptr<core::CNode> CNodeSelection::GetSelected() const
{
  return m_Selected.lock();
}
bool CNodeSelection::IsSelected(const std::shared_ptr<core::CNode> & _Node) const
{
  return GetSelected() == _Node;
}

} // namespace editor