
#include "AssetType.h"

namespace editor
{

//
// Constructor/Destructor
//

CAssetType::CAssetType(std::string _Type, std::string _FileExtention):
  m_Type(std::move(_Type)),
  m_FileExtention(std::move(_FileExtention))
{
  // Empty
}

CAssetType::~CAssetType() = default;

bool CAssetType::HandlesExtension(const std::string & _Extension) const
{
  return m_FileExtention == _Extension;
}

const std::string & CAssetType::GetExtension() const
{
  return m_FileExtention;
}

const std::string & CAssetType::GetTypeName() const
{
  return m_Type;
}

} // namespace editor