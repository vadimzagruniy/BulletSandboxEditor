#include "Gfx.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace // unnamed
{

//
// Globals
//

ID3D11Device *           g_D3dDevice            = nullptr;
ID3D11DeviceContext *    g_D3dDeviceContext     = nullptr;
IDXGISwapChain *         g_SwapChain            = nullptr;
ID3D11RenderTargetView * g_MainRenderTargetView = nullptr;

bool                     g_SwapChainOccluded    = false;
UINT                     g_ResizeWidth          = 0;
UINT                     g_ResizeHeight         = 0;

//
// Utils
//

void CreateRenderTarget()
{
  ID3D11Texture2D * pBackBuffer;
  g_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
  g_D3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_MainRenderTargetView);
  pBackBuffer->Release();
}

void CleanupRenderTarget()
{
  if (g_MainRenderTargetView)
  {
    g_MainRenderTargetView->Release();
    g_MainRenderTargetView = nullptr;
  }
}

} // namespace unnamed

namespace gfx
{

bool Init(HWND _Hwnd)
{
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;
  ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

  SwapChainDesc.BufferCount                        = 2;
  SwapChainDesc.BufferDesc.Width                   = 0;
  SwapChainDesc.BufferDesc.Height                  = 0;
  SwapChainDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
  SwapChainDesc.BufferDesc.RefreshRate.Numerator   = 60;
  SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  SwapChainDesc.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  SwapChainDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  SwapChainDesc.OutputWindow                       = _Hwnd;
  SwapChainDesc.SampleDesc.Count                   = 1;
  SwapChainDesc.SampleDesc.Quality                 = 0;
  SwapChainDesc.Windowed                           = TRUE;
  SwapChainDesc.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;

  UINT CreateDeviceFlags = 0;

        D3D_FEATURE_LEVEL FeatureLevel;
  const D3D_FEATURE_LEVEL FeatureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
  
  HRESULT Result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, CreateDeviceFlags, FeatureLevelArray, 2, D3D11_SDK_VERSION, &SwapChainDesc, &g_SwapChain, &g_D3dDevice, &FeatureLevel, &g_D3dDeviceContext);
  if (Result == DXGI_ERROR_UNSUPPORTED) 
    Result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, CreateDeviceFlags, FeatureLevelArray, 2, D3D11_SDK_VERSION, &SwapChainDesc, &g_SwapChain, &g_D3dDevice, &FeatureLevel, &g_D3dDeviceContext);
  
  if (Result != S_OK)
  {
    Shutdown();
    return false;
  }

  CreateRenderTarget();

  return true;
}

void Shutdown()
{
  CleanupRenderTarget();

  if (g_SwapChain)
  {
    g_SwapChain->Release();
    g_SwapChain = nullptr;
  }
  
  if (g_D3dDeviceContext)
  { 
    g_D3dDeviceContext->Release();
    g_D3dDeviceContext = nullptr;
  }
  
  if (g_D3dDevice)
  { 
    g_D3dDevice->Release();
    g_D3dDevice = nullptr;
  }
}

bool StartFrame()
{
  if (g_SwapChainOccluded && g_SwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
    return false;

  g_SwapChainOccluded = false;

  if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
  {
    CleanupRenderTarget();
    g_SwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
    g_ResizeWidth = g_ResizeHeight = 0;
    CreateRenderTarget();
  }

  return true;
}

void Clear()
{
  const float clear_color_with_alpha[4] = {0.45f, 0.55f, 0.60f, 1.00f };
  g_D3dDeviceContext->OMSetRenderTargets(1, &g_MainRenderTargetView, nullptr);
  g_D3dDeviceContext->ClearRenderTargetView(g_MainRenderTargetView, clear_color_with_alpha);
}

void Present()
{
  HRESULT Result = g_SwapChain->Present(0, 0); 
  g_SwapChainOccluded = (Result == DXGI_STATUS_OCCLUDED);
}

void RequestSize(UINT _Width, UINT _Height)
{
  g_ResizeWidth  = _Width;
  g_ResizeHeight = _Height;
}

ID3D11Device * GetDevice()
{
  return g_D3dDevice;
}

ID3D11DeviceContext * GetDeviceContext()
{
  return g_D3dDeviceContext;
}

} // namespace gfx