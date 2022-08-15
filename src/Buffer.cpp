#include "Buffer.hpp"
#include <cassert>
#include <iostream>


Buffer::Buffer(Device &device, D3D11_BIND_FLAG bindFlag,
               D3D11_CPU_ACCESS_FLAG accessFlags, D3D11_USAGE usage, UINT size,
               void *initData)
    : device{device} {

  D3D11_BUFFER_DESC rDesc{};

  this->size = size;

  rDesc.ByteWidth = size;
  rDesc.Usage = usage;
  rDesc.BindFlags = bindFlag;
  rDesc.CPUAccessFlags = accessFlags;

  if (initData == nullptr) {
    ThrowIfFailed(device.device->CreateBuffer(&rDesc, nullptr, &buffer));
    return;
  }

  D3D11_SUBRESOURCE_DATA sData{};

  sData.pSysMem = initData;
  sData.SysMemPitch = 0;
  sData.SysMemSlicePitch = 0;

  ThrowIfFailed(device.device->CreateBuffer(&rDesc, &sData, &buffer));
}

void Buffer::UpdateBuffer(void *data) {
  D3D11_MAPPED_SUBRESOURCE mappedSubresource;

  std::cout << "\n"
            << size << "\n"
            << "EEEEEEEEEEEEEE";

  ThrowIfFailed(device.context->Map(this->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
                                    &mappedSubresource));

  memcpy(mappedSubresource.pData, data, size);

  device.context->Unmap(this->buffer, 0);
}