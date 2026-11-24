#include "BaseWindow.h"

#include <imgui.h>

namespace ui
{

//
// Constructor/Destructor
//

CBaseWindow::CBaseWindow(std::string _Title, bool _IsVisible):
  m_Title(std::move(_Title)),
  m_IsVisible(_IsVisible),
  m_IsActive(false),
  m_Size(ImVec2(800.0f, 600.0f))
{
  // Empty
}

CBaseWindow::~CBaseWindow() = default;

//
// Interface
//

void CBaseWindow::Render()
{
  if (!IsVisible())
    return;

  ImGui::SetNextWindowSize(m_Size, GetSizingCondition());

  if (BeginFrame())
    OnRenderGui();

  EndFrame();
}

void CBaseWindow::Update(float _TimeDelta)
{
  OnUpdate(_TimeDelta);
}

void CBaseWindow::SetVisible(bool _IsVisible)
{
  m_IsVisible = _IsVisible;
}

bool CBaseWindow::IsVisible() const
{
  return m_IsVisible;
}

} // namespace ui