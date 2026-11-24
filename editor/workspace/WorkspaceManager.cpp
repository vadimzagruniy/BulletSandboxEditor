#include "WorkspaceManager.h"

#include "editor/asset/AssetDocument.h"
#include "editor/asset/AssetTypeRegistry.h"
#include "editor/asset/AssetType.h"
#include <editor/workspace/EditorWorkspace.h>
#include <editor/ui/IWindowHost.h>
#include "editor/ui/windows/MessageWindow.h"

namespace editor
{

CWorkspaceManager::CWorkspaceManager(const std::weak_ptr<ui::IWindowHost> & _WindowHost, std::shared_ptr<CAssetTypeRegistry> _AssetTypes):
  m_AssetTypes(std::move(_AssetTypes)),
  m_WindowHost(_WindowHost)
{
  // Empty
}


bool CWorkspaceManager::OpenAsset(const std::string & _FilePath)
{
  if (m_ActiveWorkspace && m_ActiveWorkspace->GetAssetDocument()->GetFilePath() == _FilePath)
    return true;

  if (m_ActiveWorkspace && m_ActiveWorkspace->GetAssetDocument()->IsDirty())
  {
    HandleUnsavedChanges([this, _FilePath](){ CreateWorkspace(_FilePath); });
    return true;
  }

  return CreateWorkspace(_FilePath);
}

bool CWorkspaceManager::SeveActiveWorkspace()
{
  if (m_ActiveWorkspace == nullptr)
    return false;

  return m_ActiveWorkspace->GetAssetDocument()->Save();
}

void CWorkspaceManager::CloseActiveWorkspace()
{
  if (m_ActiveWorkspace == nullptr)
    return;

  if (!m_ActiveWorkspace->GetAssetDocument()->IsDirty())
  {
    OnCloseWorkspaceConfirm();
    return;
  }

  HandleUnsavedChanges([this](){ OnCloseWorkspaceConfirm(); });
}

const std::shared_ptr<CEditorWorkspace> & CWorkspaceManager::GetActiveWorkspace() const
{
  return m_ActiveWorkspace;
}

bool CWorkspaceManager::CreateWorkspace(const std::string & _FilePath)
{
  const auto AssetType = m_AssetTypes->GetAssetType(_FilePath);
  if (AssetType == nullptr)
    return false;

  const auto NewWorkspace = AssetType->OpenAsset(_FilePath);
  if (NewWorkspace == nullptr)
    return false;

  if (m_ActiveWorkspace)
    m_ActiveWorkspace->OnDeactivate();

  m_ActiveWorkspace = NewWorkspace;
  m_ActiveWorkspace->OnActivate(m_WindowHost);

  return true;
}

void CWorkspaceManager::HandleUnsavedChanges(std::function<void()> _ConfirmCalback)
{
  if (const auto WindowManager = m_WindowHost.lock())
  {
    const auto MessageWindow = std::make_shared<ui::CMessageWindow>("Unsaved changes", "The workspace has unsaved changes. Changes will be lost if you don't save them");
    MessageWindow->SetConfirmCallback(_ConfirmCalback);
    MessageWindow->SetCancelCallback([this](){});
    WindowManager->AddModalWindow(MessageWindow);
  }
}

//
// Events
//

void CWorkspaceManager::OnCloseWorkspaceConfirm()
{
  if (m_ActiveWorkspace)
    m_ActiveWorkspace->OnDeactivate();

  m_ActiveWorkspace = nullptr;
}

} // namespace editor
