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
    void initVulkan() {}

    void mainLoop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {}
};

int main(int argc, char** argv) {
    HelloTriangleApp app;
    return app.runTest(argc, argv);
}