#pragma once

#include <string>
#include <vector>
#include <memory>

namespace core
{

class CNode:
  public std::enable_shared_from_this<CNode>
{
public: // Constructor/Destructor

  CNode();

  virtual ~CNode();

public: // Interface

  bool AddChild(
      std::shared_ptr<CNode> _Child
    );

  bool DeleteChild(
      const std::string & _Name
    );

  std::shared_ptr<CNode> FindChild(
      const std::string & _Name
    ) const;

  std::vector<std::shared_ptr<CNode>> GetChildren(bool _IsRecursive) const;

  bool SetName(const std::string & _Name);

  const std::string & GetName() const;
  
  void SetParent(std::shared_ptr<CNode> _Parent);

  const std::weak_ptr<CNode> & GetParent() const;

  std::shared_ptr<CNode> GetRoot() const;

  bool IsRoot() const;

  bool CanBeIdentifiedAs(const std::string & _Id) const;

public: // Service

  static std::string FindAvailableNameForNode(const std::shared_ptr<CNode> & _Parent, const std::string_view & _BaseName);

protected: // Attr

  std::string                         m_Name;
  std::weak_ptr<CNode>                m_Parent;
  std::vector<std::shared_ptr<CNode>> m_Children;
};

} // namespace core
