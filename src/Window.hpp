#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "Logger.hpp"
#include <string>


class Window
{
  public:
    Window(uint32_t width, uint32_t height, std::string title);
    bool WindowShouldClose();
    void PollEvent();
    bool resized = true;
    bool minimized;

  private:
  public:
    int width = 600, height = 800;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    GLFWwindow *glfwWindow = nullptr;
    HWND hwnd = nullptr;
};