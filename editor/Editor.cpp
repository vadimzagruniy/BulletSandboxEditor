#include "Editor.h"

#include "asset/AssetManager.h"
#include "asset/AssetTypeRegistry.h"
#include "workspace/WorkspaceManager.h"
#include "ui/WindowManager.h"
#include "ui/windows/WorkspaceWindow.h"
#include "ui/windows/ContentBrowserWindow.h"
#include "ui/content_browser/ContentBrowserController.h"
#include "ui/WindowIDs.h"
#include "scene_editor/SceneEditor.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <filesystem>

#include <windows.h>
#include <platform/utils/SystemUtils.h>

namespace editor
{

namespace // unnamed
{

constexpr const char * DOCKSPACE_NAME = "EditorDockspace";

constexpr float LEFT_RATIO   = 0.2f;
constexpr float RIGHT_RATIO  = 0.2f;
constexpr float BOTTOM_RATIO = 0.4f;
constexpr float SPLIT_HALF   = 0.5f;


void DockWindow(const std::string & _WindowID, ImGuiID _DockNodeID)
{
  const std::string Name = "###" + _WindowID;
  ImGui::DockBuilderDockWindow(Name.c_str(), _DockNodeID);
}

} // namespace unnamed

//
// Constructor/Destructor
//

CEditor::CEditor():
  m_WindowManager(std::make_shared<ui::CWindowManager>()),
  m_AssetManager(std::make_shared<CAssetManager>()),
  m_AssetTypeRegistry(std::make_shared<CAssetTypeRegistry>()),
  m_WorkspaceManager(std::make_shared<CWorkspaceManager>(m_WindowManager, m_AssetTypeRegistry)),
  m_WorkspaceWindow(std::make_shared<ui::CWorkspaceWindow>(m_WorkspaceManager)),
  m_ContentBrowserWindow(std::make_shared<ui::CContentBrowserWindow>(std::make_shared<ui::CContentBrowserController>(m_AssetManager, m_WorkspaceManager))),
  m_SceneEditor(std::make_shared<CSceneEditor>(m_WindowManager, m_WorkspaceManager, m_AssetManager)),
  m_RequestResetLoyaut(false)
{
  // Empty
}

CEditor::~CEditor() = default;

//
// Interface
//

bool CEditor::Init()
{
  m_SceneEditor->Init(m_AssetTypeRegistry);
  m_AssetManager->OpetnProject(utils::GetExecDirectory());

  m_WindowManager->AddDockedWindow(m_WorkspaceWindow);
  m_WindowManager->AddDockedWindow(m_ContentBrowserWindow);

  return true;
}

void CEditor::Shutdown()
{
  // Empty
}

void CEditor::Update(float _DeltaTime)
{
  m_WindowManager->Update(_DeltaTime);
}

void CEditor::Render()
{
  RenderMainMenu();

  ImGuiID DockspaceID = ImHashStr(DOCKSPACE_NAME);
  UpdateDockLoyaut(DockspaceID);
  ImGui::DockSpaceOverViewport(DockspaceID, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

  m_WindowManager->Render();

  HandleShortcuts();
}

const std::shared_ptr<CSceneEditor> & CEditor::GetSceneEditor() const
{
  return m_SceneEditor;
}

//
// Service
//

void CEditor::RenderMainMenu()
{
  if (!ImGui::BeginMainMenuBar())
    return;

  if (ImGui::BeginMenu("File"))
  {
    if (ImGui::MenuItem("New Scene"))
      m_SceneEditor->OnCreateNewScene();

    if (ImGui::MenuItem("Save"))
      m_WorkspaceManager->SeveActiveWorkspace();

    if (ImGui::MenuItem("Close"))
      m_WorkspaceManager->CloseActiveWorkspace();

    ImGui::Separator();

    if (ImGui::MenuItem("Exit"))
      PostQuitMessage(0);

    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("View"))
  {
    m_WindowManager->ShowMenuItems();
    
    ImGui::Separator();

    if (ImGui::MenuItem("Reset Loyaut"))
      m_RequestResetLoyaut = true;
    
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}

void CEditor::UpdateDockLoyaut(ImGuiID _DockspaceID)
{
  const auto DockNode = ImGui::DockBuilderGetNode(_DockspaceID);
  if (!m_RequestResetLoyaut && DockNode != nullptr && DockNode->IsSplitNode())
    return;

  m_RequestResetLoyaut = false;

  const ImGuiViewport * Viewport = ImGui::GetMainViewport();

  ImGui::DockBuilderRemoveNode(_DockspaceID);
  ImGui::DockBuilderAddNode(_DockspaceID);
  ImGui::DockBuilderSetNodePos(_DockspaceID, Viewport->Pos);
  ImGui::DockBuilderSetNodeSize(_DockspaceID, Viewport->WorkSize);

  ImGuiID CenterID      = _DockspaceID;
  ImGuiID RightID       = ImGui::DockBuilderSplitNode(CenterID, ImGuiDir_Right, RIGHT_RATIO,  nullptr, &CenterID);
  ImGuiID BottomID      = ImGui::DockBuilderSplitNode(CenterID, ImGuiDir_Down,  BOTTOM_RATIO, nullptr, &CenterID);
  ImGuiID LeftID        = ImGui::DockBuilderSplitNode(CenterID, ImGuiDir_Left,  LEFT_RATIO,   nullptr, &CenterID);
  ImGuiID RightBottomID = ImGui::DockBuilderSplitNode(RightID,  ImGuiDir_Down,  SPLIT_HALF,   nullptr, &RightID);

  DockWindow(ui::WORKSPACE,       CenterID);
  DockWindow(ui::CONTENT_BROWSER, BottomID);
  DockWindow(ui::SCENE_NODE_TREE, RightID);
  DockWindow(ui::SCENE_OBJECTS,   LeftID);
  DockWindow(ui::SCENE_PROPERTY,  RightBottomID);

  ImGui::DockBuilderFinish(_DockspaceID);
}

void CEditor::HandleShortcuts()
{
  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_N, ImGuiInputFlags_RouteGlobal))
    m_SceneEditor->OnCreateNewScene();

  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S, ImGuiInputFlags_RouteGlobal))
    m_WorkspaceManager->SeveActiveWorkspace();
}

} // namespace editor