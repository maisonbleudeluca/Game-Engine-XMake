#include "Window.hpp"

void Window::framebuffer_size_callback(GLFWwindow *window, int dwidth, int dheight)
{
    auto dwindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    dwindow->resized = true;

    if (dwidth == 0 || dheight == 0)
    {
        dwindow->minimized = true;
    }
    else
    {
        dwindow->minimized = false;
    }
}

Window::Window(uint32_t width, uint32_t height, std::string title)
{
    this->width = width;
    this->height = height;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    hwnd = glfwGetWin32Window(glfwWindow);

    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);

    ENGINE_CORE_INFO("Window successfully created");
}

bool Window::WindowShouldClose()
{
    return glfwWindowShouldClose(glfwWindow);
}

void Window::PollEvent()
{
    glfwPollEvents();
    glfwGetWindowSize(glfwWindow, &width, &height);
}