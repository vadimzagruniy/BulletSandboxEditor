#include "Gui.h"
#include "Gfx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "editor/EditorStyle.h"

namespace gui
{

void Init(HWND _HWnd, float _DpiScale)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO & GuiIO = ImGui::GetIO();
  GuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  GuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  GuiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ui::ApplyEditorStyle(_DpiScale);

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(_HWnd);
  ImGui_ImplDX11_Init(gfx::GetDevice(), gfx::GetDeviceContext());
}

void Shutdown()
{
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
}

void NewFrame()
{
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void Render()
{
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

} // namespace gui