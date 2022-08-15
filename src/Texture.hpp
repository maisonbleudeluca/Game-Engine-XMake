#pragma once
#include "Device.hpp"
#include "Logger.hpp"
#include <d3d11.h>
#include <string>

class Texture
{
  public:
    Texture(Device device, std::string path);
    Texture(){};
    ID3D11SamplerState *sampleState = nullptr;
    ID3D11ShaderResourceView *resourceView = nullptr;
    ID3D11Texture2D *texture = nullptr;
};