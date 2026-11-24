#pragma once

#include <windows.h>

namespace window
{

HWND Create(HINSTANCE _HInstance, const wchar_t * _Title, int _Width, int _Height);

void Destroy(HWND _HWnd, HINSTANCE _HInstance);

bool DispatchMessages();

} // namespace window