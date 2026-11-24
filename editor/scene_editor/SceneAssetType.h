#pragma once

#include "editor/asset/AssetType.h"

#include <memory>

namespace editor
{

class CEditorWorkspace;

class CSceneAssetType:
  public CAssetType
{
public: // Constructor

  CSceneAssetType();

public: // CAssetType

  std::shared_ptr<CEditorWorkspace> OpenAsset(const std::string & _FilePath) override;

  bool CreateAsset(const std::string & _FilePath);
};

} // namespace editor