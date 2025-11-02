#include "test_base.h"

class HelloTriangleApp : public TestBase {
public:
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

int main() {
    HelloTriangleApp app;
    return app.execute();
}