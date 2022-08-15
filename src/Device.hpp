#pragma once
#include "Logger.hpp"
#include <d3d11_1.h>

class Device
{
  public:
    Device();

  private:
  public:
    ID3D11Device1 *device = nullptr;
    ID3D11DeviceContext1 *context = nullptr;
};