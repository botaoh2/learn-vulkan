#pragma once

#include <fmt/format.h>

#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <cstdlib>
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

inline void VK_CHECK(VkResult r, const std::source_location location = std::source_location::current()) {
    if (r != VK_SUCCESS) {
        LOG("VK_CHECK failed: {} at {}:{}", static_cast<int>(r), location.file_name(), location.line());
        __debugbreak();
        std::abort();
    }
}

#ifdef NDEBUG
constexpr bool k_enableValidationLayers = false;
#else
constexpr bool k_enableValidationLayers = true;
#endif

const std::vector<const char*> g_validationLayers = {"VK_LAYER_KHRONOS_validation"};

bool checkValidationLayerSupport();
std::vector<const char*> getRequiredExtensions();

static inline VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                           VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                           const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                           void* pUserData) {

    LOG("Vulkan validation layer: {}", pCallbackData->pMessage);

    return VK_FALSE;
}

inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                             const VkAllocationCallbacks* pAllocator,
                                             VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                          const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

template <typename Derived>
class TestBase {
public:
    int execute() {
        int result = 0;

        static_cast<Derived*>(this)->prerun();
        result += static_cast<Derived*>(this)->run();
        static_cast<Derived*>(this)->postrun();

        return result;
    }
};
