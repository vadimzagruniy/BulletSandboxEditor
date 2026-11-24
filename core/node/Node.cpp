#include "Node.h"

namespace core
{

//
// Constructor/Destructor
//

CNode::CNode() = default;

CNode::~CNode() = default;

//
// Interface
//

bool CNode::AddChild(std::shared_ptr<CNode> _Child)
{
  if (_Child == nullptr || _Child->GetName().empty() || !_Child->IsRoot())
    return false;

  if (GetRoot()->FindChild(_Child->GetName()))
    return false;

  _Child->SetParent(shared_from_this());
  m_Children.push_back(std::move(_Child));

  return true;
}

bool CNode::DeleteChild(const std::string & _Name)
{
  const auto ChildIter = std::find_if(m_Children.begin(), m_Children.end(), [&_Name](const auto & _Child){ return _Name == _Child->GetName(); });
  if (ChildIter != m_Children.end())
  {
    (*ChildIter)->m_Parent.reset();
    m_Children.erase(ChildIter);
    return true;
  }

  for (const auto & Child : m_Children)
    if (Child->DeleteChild(_Name))
      return true;

  return false;
}

std::shared_ptr<CNode> CNode::FindChild(const std::string & _Name) const
{
  if (_Name == GetName())
    return std::const_pointer_cast<CNode>(shared_from_this());

  for(const auto & Child : m_Children)
    if (const auto Node = Child->FindChild(_Name))
      return Node;

  return nullptr;
}

std::vector<std::shared_ptr<CNode>> CNode::GetChildren(bool _IsRecursive) const
{
  std::vector<std::shared_ptr<CNode>> Children;
  for (const auto & Child : m_Children)
  {
    Children.emplace_back(Child);
    if (!_IsRecursive)
      continue;

    std::vector<std::shared_ptr<CNode>> GrandChildren = Child->GetChildren(_IsRecursive);
    Children.insert(Children.end(), GrandChildren.begin(), GrandChildren.end());
  }

  return Children;
}

bool CNode::SetName(const std::string & _Name)
{
  if (!CanBeIdentifiedAs(_Name))
    return false;

  m_Name = _Name;

  return true;
}

const std::string & CNode::GetName() const
{
  return m_Name;
}

void CNode::SetParent(std::shared_ptr<CNode> _Parent)
{
  m_Parent = std::move(_Parent);
}

const std::weak_ptr<CNode> & CNode::GetParent() const
{
  return m_Parent;
}

std::shared_ptr<CNode> CNode::GetRoot() const
{
  if (IsRoot())
    return std::const_pointer_cast<CNode>(shared_from_this());

  std::shared_ptr<CNode> Root = GetParent().lock();
  while (!Root->IsRoot())
    Root = Root->GetParent().lock();

  return Root;
}

bool CNode::IsRoot() const
{
  return m_Parent.expired();
}

bool CNode::CanBeIdentifiedAs(const std::string & _Id) const
{
  if (_Id == GetName())
    return true;

  return GetRoot()->FindChild(_Id) == nullptr;
}

//
// Service
//

std::string CNode::FindAvailableNameForNode(const std::shared_ptr<CNode> & _Parent, const std::string_view & _BaseName)
{
  const auto RootNode = _Parent->GetRoot();

  std::string SuitableName(_BaseName);
  for (int Index = 0; RootNode->FindChild(SuitableName) != nullptr; ++Index)
    SuitableName = std::string(_BaseName) + std::to_string(Index);

  return SuitableName;
}

} // namespace core
