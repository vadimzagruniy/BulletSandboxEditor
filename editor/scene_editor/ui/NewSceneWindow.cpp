#include "NewSceneWindow.h"

#include "platform/utils/SystemUtils.h"
#include "imgui.h"
#include <filesystem>

namespace ui
{

namespace // unnamed
{

constexpr const char * DEFAULT_SCENE_NAME = "NewScene";

const ImVec4 ERROR_COLOR = ImVec4(255, 0, 0, 255);

} // namespace unnamed 

//
// Constructor/Destructor
//

CNewSceneWindow::CNewSceneWindow(std::string _CurrentPath, std::string _SceneFileExt, TCreateSceneCallback _CreateSceneCallback):
  CModalWindow("NewSceneWindow"),
  m_CurrentPath(std::move(_CurrentPath)),
  m_SceneFileExt(std::move(_SceneFileExt)),
  m_CreateSceneCallback(std::move(_CreateSceneCallback))
{
  m_Size = ImVec2(800.f, 250.f);
  std::snprintf(m_SceneName, sizeof(m_SceneName), "%s", DEFAULT_SCENE_NAME);
}

//
// CBaseWindow events
//

void CNewSceneWindow::OnRenderGui()
{
  const ImVec2 ItemSpacing        = ImGui::GetStyle().ItemSpacing;
  const float  FrameHeight        = ImGui::GetFrameHeight();
  const float  ButtonsPanelHeight = FrameHeight + ItemSpacing.y * 2.f + ImGui::GetStyle().WindowPadding.y;
  
  ImGui::BeginChild("##Message", ImVec2(0.0f, -ButtonsPanelHeight));
  
  ImGui::InputText("Scene name", m_SceneName, sizeof(m_SceneName), ImGuiInputTextFlags_EnterReturnsTrue);
  ImGui::Spacing();
  
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Folder: %s", m_CurrentPath.c_str());
  ImGui::SameLine();
  
  if (ImGui::Button("..."))
    OnSelectFolder();
  
  ImGui::EndChild();
  
  ImGui::Separator();
  ImGui::Spacing();

  const std::string FilePath    = m_CurrentPath + "/" + m_SceneName + m_SceneFileExt;
  const bool        IsNameExist = std::filesystem::exists(FilePath);

  if (IsNameExist)
  {
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ERROR_COLOR);
    ImGui::SetTooltip("Name '%s' is already exist", m_SceneName);
    ImGui::PopStyleColor();
  }

  const ImVec2 ButtonSize    = ImVec2(150.f, FrameHeight);
  const float  ButtonsWidth  = ButtonSize.x * 2.0f + ItemSpacing.x;
  const float  ButtonsOffset = (ImGui::GetContentRegionAvail().x - ButtonsWidth) * 0.5f;

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ButtonsOffset);

  const bool IsNameValid = m_SceneName[0] != '\0' && !IsNameExist;
  ImGui::BeginDisabled(!IsNameValid && m_CreateSceneCallback != nullptr);
  if (ImGui::Button("Create", ButtonSize))
  {
    m_CreateSceneCallback(FilePath);
    Close();
  }
  ImGui::EndDisabled();

  ImGui::SameLine();
  if (ImGui::Button("Cancel", ButtonSize))
    Close();  
}

//
// Events
//

void CNewSceneWindow::OnSelectFolder()
{
  const auto FolderPathOpt = utils::SelectFolder(m_CurrentPath);
  if (FolderPathOpt.has_value())
    m_CurrentPath = FolderPathOpt.value();
}

} // namespace ui