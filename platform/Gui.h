#pragma once

#include <windows.h>

namespace gui
{

void Init(HWND _HWnd, float _DpiScale);

void Shutdown();

void NewFrame();

void Render();

} // namespace gui
