#pragma once

#include <string>
#include <memory>

namespace editor
{

class CEditorWorkspace;

class CAssetType
{
public: // Constructor/Destructor

  CAssetType(std::string _Type, std::string _FileExtention);

  virtual ~CAssetType();

public: // Interface

  bool HandlesExtension(const std::string & _Extension) const;

  const std::string & GetExtension() const;

  const std::string & GetTypeName() const;

  virtual std::shared_ptr<CEditorWorkspace> OpenAsset(const std::string & _FilePath) = 0;

protected: // Attr

  std::string m_Type;
  std::string m_FileExtention;
};

} // namespace editor