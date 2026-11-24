#include "Window.h"

#include "Gfx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"

namespace // unnamed
{

constexpr const wchar_t * CLASS_NAME = L"EditorWindow";

}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND _HWnd, UINT _Msg, WPARAM _WParam, LPARAM _LParam)
{
  if (ImGui_ImplWin32_WndProcHandler(_HWnd, _Msg, _WParam, _LParam))
    return true;

  switch (_Msg)
  {
    case WM_SIZE:
    {
      if (_WParam == SIZE_MINIMIZED)
        return 0;

      gfx::RequestSize((UINT)LOWORD(_LParam), (UINT)HIWORD(_LParam));
      
      return 0;
    }
    case WM_SYSCOMMAND:
    {
      if ((_WParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
        return 0;
      break;
    }
    case WM_DESTROY:
    {
      ::PostQuitMessage(0);
      return 0;   
    }
  }

  return ::DefWindowProcW(_HWnd, _Msg, _WParam, _LParam);
}

namespace window
{

HWND Create(HINSTANCE _HInstance, const wchar_t * _Title, int _Width, int _Height)
{
  WNDCLASSEXW WndClass = {};

  WndClass.cbSize         = sizeof(WndClass);
  WndClass.style          = CS_CLASSDC;
  WndClass.lpfnWndProc    = WndProc;
  WndClass.hInstance      = _HInstance;
  WndClass.hIcon;
  WndClass.hCursor        = LoadCursor(nullptr, IDC_ARROW);
  WndClass.lpszClassName  = CLASS_NAME;

  ::RegisterClassExW(&WndClass);

  return ::CreateWindowW(WndClass.lpszClassName, _Title, WS_OVERLAPPEDWINDOW, 100, 100, _Width, _Height, nullptr, nullptr, WndClass.hInstance, nullptr);
}

void Destroy(HWND _HWnd, HINSTANCE _HInstance)
{
  if (_HWnd)
    DestroyWindow(_HWnd);

  UnregisterClassW(CLASS_NAME, _HInstance);
}

bool DispatchMessages()
{
  MSG Msg;

  while (::PeekMessage(&Msg, nullptr, 0U, 0U, PM_REMOVE))
  {
    ::TranslateMessage(&Msg);
    ::DispatchMessage(&Msg);

    if (Msg.message == WM_QUIT)
      return false;
  }

  return true;
}

} // namespace window