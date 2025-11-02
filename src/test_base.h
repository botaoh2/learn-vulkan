#pragma once

#include <fmt/format.h>

#include <volk.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <cstdlib>
#include <optional>
#include <source_location>
#include <vector>

#define LOG(...) fmt::println(__VA_ARGS__)

inline void CHECK(bool condition, const std::source_location location = std::source_location::current()) {
    if (!condition) {
        LOG("CHECK failed at {}:{}", location.file_name(), location.line());
        __debugbreak();
        std::abort();
    }
}

inline void VK_CHECK(VkResult vkResult, const std::source_location location = std::source_location::current()) {
    if (vkResult != VK_SUCCESS) {
        LOG("VK_CHECK failed: {} at {}:{}", static_cast<int>(vkResult), location.file_name(), location.line());
        __debugbreak();
        std::abort();
    }
}

#ifdef NDEBUG
constexpr bool k_enableValidationLayers = false;
#else
constexpr bool k_enableValidationLayers = true;
#endif

static inline VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                           VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                           const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                           void* pUserData) {

    LOG("Vulkan validation layer: {}", pCallbackData->pMessage);

    return VK_FALSE;
}

constexpr uint32_t width = 800;
constexpr uint32_t height = 600;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() { return graphicsFamily.has_value(); }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

class TestBase {
public:
    TestBase();
    virtual ~TestBase();

    virtual void prerun() {}
    virtual int run() = 0;
    virtual void postrun() {}

    int execute() {
        int result = 0;

        prerun();
        result += run();
        postrun();

        return result;
    }

protected:
    GLFWwindow* m_window = nullptr;

    VkInstance m_instance = VK_NULL_HANDLE;

    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

private:
    void initWindow();
    void initVulkanInstance();
    void initDebugMessenger();
    void initPhysicalDevice();
};
