#pragma once

#include <windows.h>
#include <d3d11.h>

namespace gfx
{

bool Init(HWND _Hwnd);

void Shutdown();

bool StartFrame();

void Clear();

void Present();

void RequestSize(UINT _Width, UINT _Height);

ID3D11Device * GetDevice();

ID3D11DeviceContext * GetDeviceContext();

} // namespace gfx