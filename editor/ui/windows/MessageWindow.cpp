#include "MessageWindow.h"

#include "imgui.h"

namespace ui
{

//
// Constructor/Destructor
//

CMessageWindow::CMessageWindow(std::string _Title, std::string _Message):
  CModalWindow(std::move(_Title)),
  m_Message(std::move(_Message))
{
  m_Size = ImVec2(800.f, 250.f);
}

//
// CBaseWindow events
//

void CMessageWindow::OnRenderGui()
{
  const ImVec2 ItemSpacing       = ImGui::GetStyle().ItemSpacing;
  const float FrameHeight        = ImGui::GetFrameHeight();
  const float ButtonsPanelHeight = FrameHeight + ItemSpacing.y * 2.f + ImGui::GetStyle().WindowPadding.y;

  ImGui::BeginChild("##Message", ImVec2(0.0f, -ButtonsPanelHeight));
  ImGui::TextWrapped("%s", m_Message.c_str());
  ImGui::EndChild();

  ImGui::Separator();
  ImGui::Spacing();
  
  const ImVec2 ButtonSize    = ImVec2(150.f, FrameHeight);
  const float  ButtonsWidth  = ButtonSize.x * 2.0f + ItemSpacing.x;
  const float  ButtonsOffset = (ImGui::GetContentRegionAvail().x - ButtonsWidth) * 0.5f;

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ButtonsOffset); 

  if (ImGui::Button("OK", ButtonSize))
    OnConfirm();

  if (m_CancelCallback)
  {
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ButtonSize))
      OnCacncel();
  }
}

//
// Interface
//

void CMessageWindow::SetConfirmCallback(TCallback _OnConfirm)
{
  m_ConfirmCallback = std::move(_OnConfirm);
}

void CMessageWindow::SetCancelCallback(TCallback _OnOncancel)
{
  m_CancelCallback = std::move(_OnOncancel);
}

//
// Events
//

void CMessageWindow::OnConfirm()
{
  if (m_ConfirmCallback)
    m_ConfirmCallback();

  Close();
}

void CMessageWindow::OnCacncel()
{
  if (m_CancelCallback)
    m_CancelCallback();

  Close();
}

} // namespace ui