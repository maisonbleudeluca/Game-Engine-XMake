#pragma once
#include "Device.hpp"
#include <d3d11.h>
#include <d3d11_1.h>
#include <string>

class Pipeline {
public:
  Pipeline(Device *d3d11Device, std::string vertFilePath,
           std::string fragFilePath);
  void bind(ID3D11DeviceContext1 *d3d11DeviceContex);

private:
  ID3D11VertexShader *vertShader = nullptr;
  ID3D11PixelShader *pixelShader = nullptr;
  ID3D11InputLayout *inputLayout = nullptr;
  ID3D11RasterizerState *rasterizerState = nullptr;
};