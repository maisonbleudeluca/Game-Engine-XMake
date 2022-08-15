#include "Texture.hpp"
#include "common.h"
#include "stbi_include.hpp"
#include <iostream>

Texture::Texture(Device device, std::string path)
{
    path = "models/" + path;

    ENGINE_CORE_INFO("Loading texture: " + path);

    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MaxLOD = 3.402823466e+38;
    samplerDesc.MinLOD = 0;

    samplerDesc.BorderColor[0] = 3;
    samplerDesc.BorderColor[1] = 3;
    samplerDesc.BorderColor[2] = 3;
    samplerDesc.BorderColor[3] = 3;

    ThrowIfFailed(device.device->CreateSamplerState(&samplerDesc, &sampleState));

    int texWidth, texHeight, texNumChannels;
    int texForceNumChannels = 4;

    stbi_set_flip_vertically_on_load(false);

    unsigned char *textureBytes = stbi_load(path.c_str(), &texWidth, &texHeight, &texNumChannels, texForceNumChannels);
    unsigned char *mem = textureBytes;

    if (!textureBytes)
    {
        std::string error = stbi_failure_reason();
        ENGINE_CORE_ERROR("failed to load texture: " + error);
    }
    int texBytesPerRow = 4 * texWidth;

    D3D11_TEXTURE2D_DESC tDesc{};

    tDesc.Width = texWidth;
    tDesc.Height = texHeight;
    tDesc.MipLevels = 0;
    tDesc.ArraySize = 1;
    tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    tDesc.SampleDesc.Count = 1;
    tDesc.Usage = D3D11_USAGE_DEFAULT;
    tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    tDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = textureBytes;
    initData.SysMemPitch = texBytesPerRow;

    ThrowIfFailed(device.device->CreateTexture2D(&tDesc, nullptr, &texture));

    device.context->UpdateSubresource(texture, 0, nullptr, textureBytes, initData.SysMemPitch, 0);

    ThrowIfFailed(device.device->CreateShaderResourceView(texture, nullptr, &resourceView));

    device.context->GenerateMips(resourceView);
    free(textureBytes);
}
