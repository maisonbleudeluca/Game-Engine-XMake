#pragma once
#include "Device.hpp"
#include "common.h"
#include <d3d11_1.h>

class Buffer
{
  public:
    Buffer(Device &device, D3D11_BIND_FLAG bindFlag, D3D11_CPU_ACCESS_FLAG, D3D11_USAGE usage, UINT size,
           void *initData);
    ID3D11Buffer *buffer = nullptr;
    void UpdateBuffer(void *data);

  private:
    const Device &device;
    UINT size = 0;
};