#include "DockedWindow.h"

#include <imgui.h>

namespace ui
{

//
// Constructor/Destructor
//

CDockedWindow::CDockedWindow(std::string _Title, bool _IsVisible):
  CBaseWindow(std::move(_Title), _IsVisible)
{
  // Empty
}

CDockedWindow::~CDockedWindow() = default;

//
// CBaseWindow service
//

bool CDockedWindow::BeginFrame()
{
  const std::string WindowLabel = GetDisplayTitle() + "###" + m_Title;

  m_IsActive = ImGui::Begin(WindowLabel.c_str(), nullptr);

  return m_IsActive;
}

void CDockedWindow::EndFrame()
{
  ImGui::End();
}

ImGuiCond CDockedWindow::GetSizingCondition() const
{
  return ImGuiCond_FirstUseEver;
}

//
// CBaseWindow events
//

void CDockedWindow::OnUpdate(float _TimeDelta)
{
  // Empty
}

//
// Interface
//

std::string CDockedWindow::GetDisplayTitle() const
{
  return m_Title;
}

} // namespace ui