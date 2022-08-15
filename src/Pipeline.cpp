#include "Pipeline.hpp"
#include <cassert>
#include <d3dcompiler.h>

Pipeline::Pipeline(Device *d3d11Device, std::string vertFilePath, std::string fragFilePath)
{
#pragma region Compile Vertex Shader
    ID3DBlob *vsBlob;
    {
        ID3DBlob *shaderCompileErrorsBlob;
        HRESULT hResult = D3DCompileFromFile(L"shaders/shaders.hlsl", nullptr, nullptr, "vs_main", "vs_5_0", 0, 0,
                                             &vsBlob, &shaderCompileErrorsBlob);
        if (FAILED(hResult))
        {
            const char *errorString = NULL;
            if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                errorString = "Could not compile shader; file not found";
            else if (shaderCompileErrorsBlob)
            {
                errorString = (const char *)shaderCompileErrorsBlob->GetBufferPointer();
                shaderCompileErrorsBlob->Release();
            }
            MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
        }

        hResult = d3d11Device->device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr,
                                                          &vertShader);
        assert(SUCCEEDED(hResult));
    }
#pragma endregion

#pragma region Compile Pixel Shader
    {
        ID3DBlob *psBlob;
        ID3DBlob *shaderCompileErrorsBlob;
        HRESULT hResult = D3DCompileFromFile(L"shaders/shaders.hlsl", nullptr, nullptr, "ps_main", "ps_5_0", 0, 0,
                                             &psBlob, &shaderCompileErrorsBlob);
        if (FAILED(hResult))
        {
            const char *errorString = NULL;
            if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                errorString = "Could not compile shader; file not found";
            else if (shaderCompileErrorsBlob)
            {
                errorString = (const char *)shaderCompileErrorsBlob->GetBufferPointer();
                shaderCompileErrorsBlob->Release();
            }
            MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
        }

        hResult = d3d11Device->device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr,
                                                         &pixelShader);
        assert(SUCCEEDED(hResult));
        psBlob->Release();
    }
#pragma endregion

#pragma region Create Input Element
    {
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
            {"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        HRESULT hResult =
            d3d11Device->device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc),
                                                   vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
        assert(SUCCEEDED(hResult));
        vsBlob->Release();
    }
#pragma endregion

#pragma region Rasterizer Description
    D3D11_RASTERIZER_DESC rasterizerDesc;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = true;
    rasterizerDesc.DepthBias = false;
    rasterizerDesc.DepthBiasClamp = 0;
    rasterizerDesc.SlopeScaledDepthBias = 0;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.AntialiasedLineEnable = false;
    d3d11Device->device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
#pragma endregion
}

void Pipeline::bind(ID3D11DeviceContext1 *d3d11DeviceContext)
{
    d3d11DeviceContext->VSSetShader(vertShader, nullptr, 0);
    d3d11DeviceContext->PSSetShader(pixelShader, nullptr, 0);
    d3d11DeviceContext->IASetInputLayout(inputLayout);
    d3d11DeviceContext->RSSetState(rasterizerState);
}