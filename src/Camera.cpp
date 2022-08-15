#include "Camera.hpp"
#include <assert.h>
#include <iostream>

Camera::Camera(Device &device, Window &window, float speed)
    : device{device}, window{window}, CameraConstantsBuffer{device,
                                                            D3D11_BIND_CONSTANT_BUFFER,
                                                            (D3D11_CPU_ACCESS_FLAG)0,
                                                            D3D11_USAGE_DEFAULT,
                                                            2 * sizeof(glm::mat4),
                                                            nullptr},
      speed{10}
{
    Position = glm::vec3(0, 0, 0);
}

void Camera::UpdatePosition(Device &device)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    CameraConstants.view = glm::lookAt(Position, Position + Orientation, Up);
    // Adds perspective to the scene
    // std::cout << Position.x << "  ||  " << Position.y << "  ||  " << Position.z << "\n";
    CameraConstants.projection =
        glm::perspective(glm::radians(80.0f), (float)window.width / window.height, 0.01f, 300.0f);
    CameraConstants.position = glm::normalize(Position);

    assert(CameraConstantsBuffer.buffer);

    device.context->UpdateSubresource(CameraConstantsBuffer.buffer, 0, 0, &CameraConstants, 0, 0);
    device.context->VSSetConstantBuffers(1, 1, &CameraConstantsBuffer.buffer);
}

void Camera::UpdateInput()
{
    if (glfwGetKey(window.glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += speed * Orientation;
    }
    if (glfwGetKey(window.glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window.glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position += speed * -Orientation;
    }
    if (glfwGetKey(window.glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window.glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position += speed * Up;
    }
    if (glfwGetKey(window.glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Position += speed * -Up;
    }
    if (glfwGetKey(window.glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 0.4f;
    }
    else if (glfwGetKey(window.glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = 0.1f;
    }

    // Handles mouse inputs
    if (glfwGetMouseButton(window.glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Hides mouse cursor
        glfwSetInputMode(window.glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first click
        if (firstClick)
        {
            glfwSetCursorPos(window.glfwWindow, (window.width / 2), (window.height / 2));
            firstClick = false;
        }

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window.glfwWindow, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they
        // begin in the middle of the screen and then "transforms" them into
        // degrees
        float rotX = sensitivity * (float)(mouseY - (window.height / 2)) / window.height;
        float rotY = sensitivity * (float)(mouseX - (window.width / 2)) / window.width;

        // Calculates upcoming vertical change in the Orientation
        glm::vec3 newOrientation =
            glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        // Decides whether or not the next vertical Orientation is legal or not
        if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            Orientation = newOrientation;
        }

        // Rotates the Orientation left and right
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Sets mouse cursor to the middle of the screen so that it doesn't end
        // up roaming around
        glfwSetCursorPos(window.glfwWindow, (window.width / 2), (window.height / 2));
    }
    else if (glfwGetMouseButton(window.glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window.glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstClick = true;
    }
}
