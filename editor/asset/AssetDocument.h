#pragma once

#include <string>
#include <filesystem>

namespace core
{

class CCamera;

} // namespace core

namespace editor
{

class CAssetDocument
{
public: // Constructor/Destructor

  CAssetDocument(std::string _FilePath);

  virtual ~CAssetDocument();

public: // Interface

  bool IsDirty() const;

  const std::string & GetFilePath() const;

  virtual bool Load() = 0;

  virtual bool Save() = 0;

  virtual void Update(float _DeltaTime);

  virtual void Render(const core::CCamera & _Camera);

protected: // Attr

  std::string m_FilePath;
  bool        m_IsDirty;
};

} // namespace editor