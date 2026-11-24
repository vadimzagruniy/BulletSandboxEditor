#include "SceneEditor.h"

#include <editor/ui/WindowManager.h>
#include "editor/asset/AssetTypeRegistry.h"
#include "editor/asset/AssetManager.h"
#include "editor/workspace/WorkspaceManager.h"
#include "ui/NewSceneWindow.h"
#include "SceneAssetType.h"

namespace editor
{

//
// Constructor
//

CSceneEditor::CSceneEditor(std::shared_ptr<ui::CWindowManager> _WindowManager, std::shared_ptr<CWorkspaceManager> _WorkspaceManager, std::shared_ptr<CAssetManager> _AssetManager):
  m_AssetType(std::make_shared<CSceneAssetType>()),
  m_AssetManager(std::move(_AssetManager)),
  m_WindowManager(std::move(_WindowManager)),
  m_WorkspaceManager(std::move(_WorkspaceManager))
{
  // Empty
}

//
// Interface
//

bool CSceneEditor::Init(const std::shared_ptr<CAssetTypeRegistry> & _AssetTypeRegistry)
{
  _AssetTypeRegistry->RegisterType(m_AssetType);

  return false;
}

void CSceneEditor::CreateNewScene(const std::string & _FilePath) const
{
  if (std::filesystem::exists(_FilePath))
    return; // TODO: Notify error

  if (!m_AssetType->CreateAsset(_FilePath))
    return;

  m_AssetManager->Refresh();
  m_WorkspaceManager->OpenAsset(_FilePath);
}

//
// Event
//

void CSceneEditor::OnCreateNewScene()
{
  const auto NewSceneWindow = std::make_shared<ui::CNewSceneWindow>(m_AssetManager->GetAssetRootDir().string(), m_AssetType->GetExtension(), [this](const std::string & _Name){ CreateNewScene(_Name); });
  m_WindowManager->AddModalWindow(NewSceneWindow);
}

} // namespace editor