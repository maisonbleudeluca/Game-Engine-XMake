#include "Swapchain.hpp"
#include "common.h"
#include <cassert>
#include <iostream>

Swapchain::Swapchain(Window *window, Device *device)
{
    // TODO: actually use client rect size, can fix imgui
    // https://discord.com/channels/318590007881236480/318783187163545603/1006662968156635166

    engineWindow = window;
    this->device = device;

    {
        // Get DXGI Factory (needed to create Swap Chain)
        IDXGIFactory2 *dxgiFactory;
        {

            IDXGIDevice1 *dxgiDevice;
            HRESULT hResult = device->device->QueryInterface(__uuidof(IDXGIDevice1), (void **)&dxgiDevice);
            assert(SUCCEEDED(hResult));

            IDXGIAdapter *dxgiAdapter;
            hResult = dxgiDevice->GetAdapter(&dxgiAdapter);
            assert(SUCCEEDED(hResult));
            dxgiDevice->Release();

            DXGI_ADAPTER_DESC adapterDesc;
            dxgiAdapter->GetDesc(&adapterDesc);

            OutputDebugStringA("Graphics Device: ");
            OutputDebugStringW(adapterDesc.Description);

            hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void **)&dxgiFactory);
            assert(SUCCEEDED(hResult));
            dxgiAdapter->Release();
        }

        DXGI_SWAP_CHAIN_DESC1 d3d11SwapChainDesc = {};
        d3d11SwapChainDesc.Width = window->width;   // use window width
        d3d11SwapChainDesc.Height = window->height; // use window height
        d3d11SwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        d3d11SwapChainDesc.SampleDesc.Count = 1;
        d3d11SwapChainDesc.SampleDesc.Quality = 0;
        d3d11SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        d3d11SwapChainDesc.BufferCount = 2;
        d3d11SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        d3d11SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        d3d11SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        d3d11SwapChainDesc.Flags = 0;

        HRESULT hResult = dxgiFactory->CreateSwapChainForHwnd(device->device, engineWindow->hwnd, &d3d11SwapChainDesc,
                                                              0, 0, &swapchain);
        assert(SUCCEEDED(hResult));

        dxgiFactory->Release();
    }

    ID3D11Texture2D *d3d11FrameBuffer;
    HRESULT hResult = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&d3d11FrameBuffer);
    assert(SUCCEEDED(hResult));

    hResult = device->device->CreateRenderTargetView(d3d11FrameBuffer, 0, &frameBufferView);
    assert(SUCCEEDED(hResult));
    d3d11FrameBuffer->Release();

    D3D11_TEXTURE2D_DESC depthStencilDesc{};

    depthStencilDesc.Width = window->width;
    depthStencilDesc.Height = window->height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    ThrowIfFailed(device->device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer));
    ThrowIfFailed(device->device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView));

    ENGINE_CORE_INFO("Swapchain successfully created");
}

void Swapchain::UpdateResize()
{
    if (engineWindow->resized == false)
    {
        return;
    }
    ENGINE_CORE_INFO("Window was resized");

    device->context->OMSetRenderTargets(0, 0, 0);
    frameBufferView->Release();
    depthStencilView->Release();

    HRESULT res = swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
    assert(SUCCEEDED(res));
    /////////////////////////////////////////////////////////////////////////////////

    ID3D11Texture2D *d3d11FrameBuffer;
    HRESULT hResult = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&d3d11FrameBuffer);
    assert(SUCCEEDED(hResult));

    hResult = device->device->CreateRenderTargetView(d3d11FrameBuffer, 0, &frameBufferView);
    assert(SUCCEEDED(hResult));

    D3D11_TEXTURE2D_DESC depthBufferDesc;
    d3d11FrameBuffer->GetDesc(&depthBufferDesc);

    d3d11FrameBuffer->Release();

    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D *depthBuffer;
    device->device->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);

    device->device->CreateDepthStencilView(depthBuffer, nullptr, &depthStencilView);

    depthBuffer->Release();

    engineWindow->resized = false;
}