#include "Light.hpp"

Light::Light(Device &device, glm::vec3 position, glm::vec3 direction)
    : device{device},
      LightConstantBuffer{
          device, D3D11_BIND_CONSTANT_BUFFER, (D3D11_CPU_ACCESS_FLAG)0, D3D11_USAGE_DEFAULT, sizeof(data), nullptr} {};

void Light::use()
{
}