#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_EXPOSE_NATIVE_WIN32

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")

#include "Camera.hpp"
#include "Device.hpp"
#include "Logger.hpp"
#include "Model.hpp"
#include "Pipeline.hpp"
#include "Swapchain.hpp"
#include "Window.hpp"

int main()
{
    Logger::Init();

    Window window = {800, 600, "Engine - [TEST]"};
    Device device = {};
    Swapchain swapchain = {&window, &device};
    Pipeline basePipeline = {&device, "", ""};
    Camera camera = {device, window, 100};

    Model e{device, "models/Sponza.gltf"};

    swapchain.UpdateResize();

    while (!window.WindowShouldClose())
    {
        window.PollEvent();
        if (!window.minimized)
        {
            swapchain.UpdateResize();
            camera.UpdateInput();
            camera.UpdatePosition(device);

            FLOAT backgroundColor[4] = {0.1f, 0.2f, 0.6f, 1.0f};
            device.context->ClearRenderTargetView(swapchain.frameBufferView, backgroundColor);
            device.context->ClearDepthStencilView(swapchain.depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                                  1.0f, 0);

            RECT winRect;
            GetClientRect(window.hwnd, &winRect);
            D3D11_VIEWPORT viewport = {
                0.0f, 0.0f, (FLOAT)(winRect.right - winRect.left), (FLOAT)(winRect.bottom - winRect.top), 0.0f, 1.0f};
            device.context->RSSetViewports(1, &viewport);

            device.context->OMSetRenderTargets(1, &swapchain.frameBufferView, swapchain.depthStencilView);
            device.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            basePipeline.bind(device.context);

            e.Draw();

            swapchain.swapchain->Present(1, 0);
        }
    }

    return 0;
}