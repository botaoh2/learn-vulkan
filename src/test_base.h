#pragma once

#include <fmt/format.h>

#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <cstdint>

#define VK_CHECK(x)                                                                                                    \
    do {                                                                                                               \
        VkResult err = x;                                                                                              \
        if (err != VK_SUCCESS) {                                                                                       \
            fmt::print("Detected Vulkan error: {} at {}:{}\n", static_cast<int>(err), __FILE__, __LINE__);             \
            std::abort();                                                                                              \
        }                                                                                                              \
    } while (0)

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
