#include "test_base.h"

class HelloTriangleApp : public TestBase {
protected:
    std::string testName() override { return "HelloTriangleApp"; }

    void prerun() override { initVulkan(); }

    int run() override {
        mainLoop();
        return 0;
    }

    void postrun() override { cleanup(); }

private:
    void initVulkan() { createSyncObjects(); }

    void mainLoop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(m_device);
    }

    void cleanup() {
        vkDestroySemaphore(m_device, m_imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(m_device, m_renderFinishedSemaphore, nullptr);
        vkDestroyFence(m_device, m_inFlightFence, nullptr);
    }

    void createSyncObjects() {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VK_CHECK(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore));
        VK_CHECK(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore));
        VK_CHECK(vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFence));
    }

    void drawFrame() {
        vkWaitForFences(m_device, 1, &m_inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(m_device, 1, &m_inFlightFence);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE,
                              &imageIndex);

        vkResetCommandBuffer(m_commandBuffer, 0);
        recordCommandBuffer(m_commandBuffer, imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffer;

        VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VK_CHECK(vkQueueSubmit(m_queue, 1, &submitInfo, m_inFlightFence));

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_swapChain;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        vkQueuePresentKHR(m_queue, &presentInfo);
    }

    VkSemaphore m_imageAvailableSemaphore = VK_NULL_HANDLE;
    VkSemaphore m_renderFinishedSemaphore = VK_NULL_HANDLE;
    VkFence m_inFlightFence = VK_NULL_HANDLE;
};

int main(int argc, char** argv) {
    HelloTriangleApp app;
    return app.runTest(argc, argv);
}
