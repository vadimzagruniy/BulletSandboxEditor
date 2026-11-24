#pragma once

#include <string>
#include <memory>
#include <vector>

namespace editor
{

class CAssetType;

class CAssetTypeRegistry
{
public: // Interface

  bool CanHandleFile(const std::string & _FilePath) const;

  std::shared_ptr<CAssetType> GetAssetType(const std::string & _FilePath) const;

  void RegisterType(std::shared_ptr<CAssetType> _AssetType);

private: // Attr

  std::vector<std::shared_ptr<CAssetType>> m_Types;
};

} // namespace editor