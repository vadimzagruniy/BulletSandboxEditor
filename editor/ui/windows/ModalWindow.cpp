#include "ModalWindow.h"

#include <imgui.h>

namespace ui
{

//
// Constructor/Destructor
//

CModalWindow::CModalWindow(std::string _Title):
  CBaseWindow(std::move(_Title), true),
  m_IsExitRequired(false)
{
  // Empty
}

CModalWindow::~CModalWindow() = default;

//
// CBaseWindow service
//

bool CModalWindow::BeginFrame()
{
  ImGui::SetNextWindowBgAlpha(1.0f);

  if (!ImGui::IsPopupOpen(m_Title.c_str()))
    ImGui::OpenPopup(m_Title.c_str());

  bool IsOpen = true;
  m_IsActive  = ImGui::BeginPopupModal(m_Title.c_str(), &IsOpen, ImGuiWindowFlags_NoCollapse);

  if (!IsOpen)
    m_IsExitRequired = true;

  return m_IsActive;
}

void CModalWindow::EndFrame()
{
  if (m_IsActive)
  {
    if (m_IsExitRequired)
      ImGui::CloseCurrentPopup();
    
    ImGui::EndPopup();
  }

  if (m_IsExitRequired && m_OnExitCallback)
    m_OnExitCallback(shared_from_this());
}

ImGuiCond CModalWindow::GetSizingCondition() const
{
  return ImGuiCond_Appearing;
}

//
// CBaseWindow events
//

void CModalWindow::OnUpdate(float _TimeDelta)
{
  // Empty
}

//
// Interface
//

void CModalWindow::SetExitCallback(const TExitCallback & _ExitCallback)
{
  m_OnExitCallback = _ExitCallback;
}

void CModalWindow::Close()
{
  m_IsExitRequired = true;
}

} // namespace ui