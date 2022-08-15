#pragma once

#include "Buffer.hpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class Camera
{
  public:
    Camera(Device &device, Window &window, float speed);
    void UpdateInput();
    void UpdatePosition(Device &device);

  private:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    bool firstClick = true;
    float speed;
    float sensitivity = 10;

    Window &window;
    Device &device;

    Buffer CameraConstantsBuffer;

    struct
    {
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec3 position;
    } CameraConstants;
};