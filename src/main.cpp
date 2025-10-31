
#include <fmt/format.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <cstdint>
#include <iostream>

#define VK_CHECK(x)                                                                                                    \
    do {                                                                                                               \
        VkResult err = x;                                                                                              \
        if (err != VK_SUCCESS) {                                                                                       \
            std::cerr << "Detected Vulkan error: " << err << " at " << __FILE__ << ":" << __LINE__ << "\n";            \
            std::abort();                                                                                              \
        }                                                                                                              \
    } while (0)

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

    void initVulkan() { createInstance(); }

    void mainLoop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyInstance(m_instance, nullptr);

        glfwDestroyWindow(m_window);

        glfwTerminate();
    }

    void createInstance() {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfw_extension_count = 0;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

        createInfo.enabledExtensionCount = glfw_extension_count;
        createInfo.ppEnabledExtensionNames = glfw_extensions;
        createInfo.enabledLayerCount = 0;

        VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance));
    }

    uint32_t m_width = 800;
    uint32_t m_height = 600;

    GLFWwindow* m_window = nullptr;

    VkInstance m_instance = VK_NULL_HANDLE;
};

int main() {
    HelloTriangleApp app;
    app.run();
}