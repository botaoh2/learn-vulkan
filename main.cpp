
#include <fmt/format.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <cstdint>
#include <iostream>

class HelloTriangleApp {
  public:
    HelloTriangleApp() {
        initWindow();
        initVulkan();
    }

    ~HelloTriangleApp() { cleanup(); }

    void run() { mainLoop(); }

  private:
    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_window = glfwCreateWindow(m_width, m_height, "Vulkan", nullptr, nullptr);
    }

    void initVulkan() {}

    void mainLoop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        glfwDestroyWindow(m_window);

        glfwTerminate();
    }

    uint32_t m_width = 800;
    uint32_t m_height = 600;
    GLFWwindow* m_window = nullptr;
};

int main() {
    HelloTriangleApp app;
    app.run();
}