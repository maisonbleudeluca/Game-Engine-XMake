#pragma once
#include "Buffer.hpp"
#include "Device.hpp"
#include <d3d11.h>
#include <glm/glm.hpp>

class Light
{
  public:
    Light(Device &device, glm::vec3 position, glm::vec3 direction);

    void use();

    struct
    {
        glm::vec3 position;
        glm::vec3 direction;

        float cutOff;
        float outerCutOff;

        float constant;
        float linearV;
        float quadratic;
    } data;

  private:
    const Device &device;
    Buffer LightConstantBuffer;
};