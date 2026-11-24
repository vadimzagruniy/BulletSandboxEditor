
#include "AssetDocument.h"


namespace editor
{

//
// Constructor/Destructor
//

CAssetDocument::CAssetDocument(std::string _FilePath):
  m_FilePath(std::move(_FilePath)),
  m_IsDirty(false)
{
  // Empty
}

CAssetDocument::~CAssetDocument() = default;

//
// Interface
//

bool CAssetDocument::IsDirty() const
{
  return m_IsDirty;
}

const std::string & CAssetDocument::GetFilePath() const
{
  return m_FilePath;
}

void CAssetDocument::Update(float _DeltaTime)
{
  // Empty
}

void CAssetDocument::Render(const core::CCamera & _Camera)
{
  // Empty
}

} // namespace editor