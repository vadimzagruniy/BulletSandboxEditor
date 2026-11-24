#include "AssetTypeRegistry.h"
#include "AssetType.h"

#include <filesystem>

namespace editor
{

//
// Interface
//

bool CAssetTypeRegistry::CanHandleFile(const std::string & _FilePath) const
{
  return GetAssetType(_FilePath) != nullptr;
}

std::shared_ptr<CAssetType> CAssetTypeRegistry::GetAssetType(const std::string & _FilePath) const
{
  const auto AssetTypeIter = std::find_if(m_Types.begin(), m_Types.end(), [&_FilePath](const std::shared_ptr<CAssetType> & _AssetType) {
      std::filesystem::path FilePath = _FilePath;
      return _AssetType->HandlesExtension(FilePath.extension().string());
    });

  return AssetTypeIter != m_Types.end() ? *AssetTypeIter : nullptr;
}

void CAssetTypeRegistry::RegisterType(std::shared_ptr<CAssetType> _AssetType)
{
  m_Types.push_back(std::move(_AssetType));
}

} // namespace editor