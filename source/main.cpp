#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "date/date.h"

#include "common/drawable.h"
#include "modules.h"

static const unsigned int TARGET_FPS = 144;

template <class clock, class duration>
void sleep_until(std::chrono::time_point<clock, duration> tp)
{
    using namespace std::chrono;
    std::this_thread::sleep_until(tp - 10us);
    while (tp >= clock::now())
        ;
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

std::vector<drawable> drawables;

int main()
{
    using namespace std;
    using namespace std::chrono;

    using framerate = duration<int, ratio<1, TARGET_FPS>>;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int primary_monitor_width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    int primary_monitor_height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

    GLFWwindow* window = glfwCreateWindow(
        primary_monitor_width,
        primary_monitor_height, 
        "Planet Generator", 
        glfwGetPrimaryMonitor(), 
        nullptr);
        
    if (!window)
    {
        std::cout << "Failed to create the GLFW window\n";
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, primary_monitor_width, primary_monitor_height);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        modules::getInstance().main_camera->update_window_size(width, height);
    });
    
    glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y) {
        for (auto inputable : modules::getInstance().inputables) 
        {  
            inputable->on_scroll(x, y);
        }
    });

    glfwSwapInterval(0);

    modules::getInstance().resolve(window);

    for (auto drawable : modules::getInstance().drawables) 
    {  
        drawable->on_create();
    }

    for (auto inputable : modules::getInstance().inputables) 
    {  
        inputable->on_input_init(window);
    }

    modules::getInstance().main_camera->update_window_size(800,600);
    modules::getInstance().main_camera->look_at(glm::vec3(0.0f,0.0f,10.0), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));

    auto prev_frame_time = system_clock::now();
    auto next_frame_time = prev_frame_time + framerate{1};

    while (!glfwWindowShouldClose(window))
    {
        std::this_thread::sleep_until(next_frame_time);
        next_frame_time += framerate{1};

        glClearColor(1.00f, 0.19f, 0.04f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        process_input(window);

        std::chrono::duration<double> frame_time = system_clock::now() - prev_frame_time;

        for (auto drawable : modules::getInstance().drawables) 
        {  
            drawable->on_update(frame_time.count());
        }

        for (auto drawable : modules::getInstance().drawables)
        {  
            drawable->on_draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        prev_frame_time = system_clock::now();
    }

    for (auto drawable : modules::getInstance().drawables)
    {  
        drawable->on_destroy();
    }

    glfwTerminate();
    return 0;
}