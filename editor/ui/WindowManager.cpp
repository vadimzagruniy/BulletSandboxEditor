#include "WindowManager.h"

#include "imgui.h"

namespace ui
{

CWindowManager::CWindowManager()
{
  // Empty
}

CWindowManager::~CWindowManager() = default;

//
// Interface
//

void CWindowManager::AddModalWindow(
    const std::shared_ptr<CModalWindow> & _Window
  )
{
  _Window->SetExitCallback([this](const std::shared_ptr<CModalWindow> & _Window){ RemoveModalWindow(_Window); });

  m_ModalWindows.emplace_back(_Window);
}

void CWindowManager::RemoveModalWindow(const std::shared_ptr<CModalWindow> & _Window)
{
  m_ModalWindowsToHide.emplace_back(_Window);
}

void CWindowManager::AddDockedWindow(
    const std::shared_ptr<CDockedWindow> & _Window
  )
{
  m_DockedWindows.emplace_back(_Window);
}

void CWindowManager::RemoveDockedWindow(const std::shared_ptr<CDockedWindow> & _Window)
{
  m_DockedWindowsToHide.emplace_back(_Window);
}

void CWindowManager::Render()
{
  const auto ModalWindows = m_ModalWindows;
  for (const auto & ModalWindow : ModalWindows)
    ModalWindow->Render();

  const auto DockedWindows = m_DockedWindows;
  for (const auto & DockedWindow : DockedWindows)
    DockedWindow->Render();
}

void CWindowManager::Update(const float _TimeDelta)
{
  if (!m_ModalWindowsToHide.empty())
  {
    for (const auto & Window : m_ModalWindowsToHide)
    {
      const auto Iterator = std::find(m_ModalWindows.cbegin(), m_ModalWindows.cend(), Window);
      if (Iterator != m_ModalWindows.end())
        m_ModalWindows.erase(Iterator);
    }

    m_ModalWindowsToHide.clear();
  }

  if (!m_DockedWindowsToHide.empty())
  {
    for (const auto & Window : m_DockedWindowsToHide)
    {
      const auto Iterator = std::find(m_DockedWindows.cbegin(), m_DockedWindows.cend(), Window);
      if (Iterator != m_DockedWindows.end())
        m_DockedWindows.erase(Iterator);
    }

    m_DockedWindowsToHide.clear();
  }

  const auto ModalWindows = m_ModalWindows;
  for (const auto & ModalWindow : ModalWindows)
    ModalWindow->Update(_TimeDelta);

  const auto DockedWindows = m_DockedWindows;
  for (const auto & DockedWindow : DockedWindows)
    DockedWindow->Update(_TimeDelta);
}

void CWindowManager::ShowMenuItems()
{
  for (const auto & DockedWindow : m_DockedWindows)
  {
    bool IsVisible = DockedWindow->IsVisible();
    if (ImGui::MenuItem(DockedWindow->GetDisplayTitle().c_str(), nullptr, &IsVisible))
      DockedWindow->SetVisible(IsVisible);
  }
}

} // namespace ui