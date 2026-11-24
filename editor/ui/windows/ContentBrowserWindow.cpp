#include "ContentBrowserWindow.h"

#include "editor/ui/content_browser/IContentBrowserController.h"
#include "editor/ui/content_browser/FolderTreeView.h"
#include "editor/ui/content_browser/ContentItemsView.h"
#include "editor/ui/WindowIDs.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace ui
{

//
// Constructors/Desstructors
//

CContentBrowserWindow::CContentBrowserWindow(std::shared_ptr<IContentBrowserController> _Controller):
  CDockedWindow(CONTENT_BROWSER),
  m_Controller(std::move(_Controller)),
  m_FolderTreeView(std::make_unique<CFolderTreeView>(m_Controller)),
  m_ContentItemsView(std::make_unique<CContentItemsView>(m_Controller)),
  m_SplitRatio(0.2f)
{
  // Empty
}

CContentBrowserWindow::~CContentBrowserWindow() = default;

//
// CBaseWindow events
//

void CContentBrowserWindow::OnRenderGui()
{
  const float AvailableWidth = ImGui::GetContentRegionAvail().x;
  const float LeftPanelWidth = AvailableWidth * m_SplitRatio;

  if (ImGui::BeginChild("##FolderTree", ImVec2(LeftPanelWidth, 0.0f)))
    m_FolderTreeView->Render();

  ImGui::EndChild();

  ImGui::SameLine();
  RenderSplitter(AvailableWidth);
  ImGui::SameLine();

  if (ImGui::BeginChild("##ContentItems"))
    m_ContentItemsView->Render();

  ImGui::EndChild();
}

void CContentBrowserWindow::OnUpdate(float _TimeDelta)
{
  m_Controller->ProcessPendingActions();
}

//
// Service
//

void CContentBrowserWindow::RenderSplitter(float _AvailableWidth)
{
  const float  Thickness = ImGui::GetStyle().ItemSpacing.x * 2.0f;
  const ImVec2 Position  = ImGui::GetCursorScreenPos();
  const float  Height    = ImGui::GetContentRegionAvail().y;

  if (Height <= 0.0f)
    return;

  ImGui::InvisibleButton("##ContentSplitter", ImVec2(Thickness, Height));

  const bool IsHovered = ImGui::IsItemHovered();
  const bool IsActive  = ImGui::IsItemActive();

  if (IsHovered || IsActive)
    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

  if (IsActive && _AvailableWidth > 0.0f)
    m_SplitRatio = std::clamp(m_SplitRatio + ImGui::GetIO().MouseDelta.x / _AvailableWidth, 0.1f, 0.9f);

  const ImGuiCol ColorIndex = IsActive ? ImGuiCol_SeparatorActive : (IsHovered ? ImGuiCol_SeparatorHovered : ImGuiCol_Separator);
  const float    LineX      = Position.x + Thickness * 0.5f;

  ImGui::GetWindowDrawList()->AddLine(ImVec2(LineX, Position.y), ImVec2(LineX, Position.y + Height), ImGui::GetColorU32(ColorIndex), IsHovered || IsActive ? 2.0f : 1.0f);
}

} // namespace ui