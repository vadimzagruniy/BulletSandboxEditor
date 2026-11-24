#include "WorkspaceWindow.h"

#include "editor/ui/WindowIDs.h"
#include "editor/workspace/WorkspaceManager.h"
#include "editor/workspace/EditorWorkspace.h"
#include "editor/asset/AssetDocument.h"

#include "imgui.h"

namespace ui
{

//
// Constructor/Destructor
//

CWorkspaceWindow::CWorkspaceWindow(std::shared_ptr<editor::CWorkspaceManager> _WorkspaceManager):
  CDockedWindow(WORKSPACE, true),
  m_WorkspaceManager(std::move(_WorkspaceManager))
{
  // Empty
}

CWorkspaceWindow::~CWorkspaceWindow() = default;

//
// CDockedWindow
//

std::string CWorkspaceWindow::GetDisplayTitle() const
{
  const auto & Workspace = m_WorkspaceManager->GetActiveWorkspace();
  if (Workspace == nullptr)
    return "Workspace";

  const auto & AssetDocument = Workspace->GetAssetDocument();
  if (AssetDocument == nullptr)
    return "Workspace";

  const auto Title = std::filesystem::path(AssetDocument->GetFilePath()).filename().string();

  return AssetDocument->IsDirty() ? Title + "*": Title;
}

//
// CBaseWindow events
//

void CWorkspaceWindow::OnRenderGui()
{
  if (const auto & Workspace = m_WorkspaceManager->GetActiveWorkspace())
    Workspace->Render();
}

void CWorkspaceWindow::OnUpdate(float _TimeDelta)
{
  if (const auto & Workspace = m_WorkspaceManager->GetActiveWorkspace())
    Workspace->Update(_TimeDelta);
}

} // namespace ui