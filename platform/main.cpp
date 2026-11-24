#include "Window.h"
#include "Gfx.h"
#include "Gui.h"
#include "Application.h"

#include <windows.h>
#include <d3d11.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <tchar.h>

//
// Main
//

int WINAPI wWinMain(HINSTANCE _HInstance, HINSTANCE, PWSTR, int nCmdShow)
{
  // Make process DPI aware and obtain main monitor scale
  ImGui_ImplWin32_EnableDpiAwareness();

  float DpiScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

  // Create application window
  HWND HWnd = window::Create(_HInstance, L"ImGui Bullet Editor", int(1280 * DpiScale), int(800 * DpiScale));
  if (!HWnd)
    return 1;

  // Initialize Direct3D
  if (!gfx::Init(HWnd))
  {
    window::Destroy(HWnd, _HInstance);
    return 1;
  }

  // Show the window
  ::ShowWindow(HWnd, SW_MAXIMIZE);
  ::UpdateWindow(HWnd);

  // Setup Dear ImGui context
  gui::Init(HWnd, DpiScale);

  CApplication Application;
  Application.Init();

  while (window::DispatchMessages())
  {
    
    // Handle window being minimized or screen locked
    if (!gfx::StartFrame())
    {
      ::Sleep(10);
      continue;
    }
    
    Application.Update(ImGui::GetIO().DeltaTime);

    // Start the Dear ImGui frame
    gui::NewFrame();

    Application.Render();

    gfx::Clear();
    gui::Render();
    gfx::Present();
  }

  // Cleanup
  Application.Shutdown();

  gui::Shutdown();
  gfx::Shutdown();
  window::Destroy(HWnd, _HInstance);

  return 0;
}