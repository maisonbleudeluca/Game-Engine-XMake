#pragma once
#include "Device.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include <d3d11_1.h>

class Swapchain
{
  public:
    Swapchain(Window *window, Device *device);
    void UpdateResize();
    IDXGISwapChain1 *swapchain;
    ID3D11RenderTargetView *frameBufferView;
    ID3D11DepthStencilView *depthStencilView;

  private:
    Window *engineWindow;
    Device *device;
    ID3D11Texture2D *depthStencilBuffer;
};