

#include "core/scene/SceneNode.h"
#include "SceneAssetType.h"
#include "SceneDocument.h"
#include "SceneEditorWorkspace.h"

namespace editor
{

//
// Constructor
//

CSceneAssetType::CSceneAssetType():
  CAssetType("Scene", ".scene")
{
  // Empty
}

//
// CAssetType
//

std::shared_ptr<CEditorWorkspace> CSceneAssetType::OpenAsset(const std::string & _FilePath)
{
  const auto SceneDocument = std::make_shared<CSceneDocument>(_FilePath);
  if (!SceneDocument->Load())
    return nullptr;

  return std::make_shared<CSceneEditorWorkspace>(SceneDocument);
}

bool CSceneAssetType::CreateAsset(const std::string & _FilePath)
{
  const auto SceneDocument = std::make_shared<CSceneDocument>(_FilePath);
  SceneDocument->GetRootNode()->SetName("RootNode");

  return SceneDocument->Save();
}

} // namespace editor