#pragma once

#include <fmt/format.h>

#include <volk.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <cstdlib>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#define LOG(...) fmt::println(__VA_ARGS__)

#define CHECK(expr)                                                                                                    \
    do {                                                                                                               \
        if (!(expr)) {                                                                                                 \
            LOG("CHECK failed at {}:{}", __FILE__, __LINE__);                                                          \
            __debugbreak();                                                                                            \
            std::abort();                                                                                              \
        }                                                                                                              \
    } while (0)

#define VK_CHECK(expr)                                                                                                 \
    do {                                                                                                               \
        VkResult vkResult = (expr);                                                                                    \
        if (vkResult != VK_SUCCESS) {                                                                                  \
            LOG("VK_CHECK failed: {} at {}:{}", static_cast<int>(vkResult), __FILE__, __LINE__);                       \
            __debugbreak();                                                                                            \
            std::abort();                                                                                              \
        }                                                                                                              \
    } while (0)

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

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

class TestBase {
public:
    virtual ~TestBase() = default;

    int runTest(std::unordered_map<std::string, std::string> parameters);
    int runTest(int argc, char** argv);
    int runTest();

protected:
    virtual std::string testName() { return "TestName"; }
    virtual void prerun() {}
    virtual int run() = 0;
    virtual void postrun() {}

    std::unordered_map<std::string, std::string> m_parameters;

    GLFWwindow* m_window = nullptr;

    VkInstance m_instance = VK_NULL_HANDLE;

    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

    VkSurfaceKHR m_surface = VK_NULL_HANDLE;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

    VkDevice m_device = VK_NULL_HANDLE;
    VkQueue m_queue = VK_NULL_HANDLE;

    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat{};
    VkExtent2D m_swapChainExtent{};

    std::vector<VkImageView> m_swapChainImageViews;

    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;

private:
    void parseArgs(int argc, char** argv);

    void init();
    void cleanup();

    void initWindow();
    void initVulkanInstance();
    void initDebugMessenger();
    void initSurface();
    void initPhysicalDevice();
    void initLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();

    bool isDeviceSuitable(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkShaderModule createShaderModule(const std::vector<uint32_t>& code);
};
