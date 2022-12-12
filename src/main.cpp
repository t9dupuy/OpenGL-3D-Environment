#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <windows.h>
#include <vector>
#include <chrono>
#include <thread>

#include "utils/utils.hpp"
#include "render/camera.hpp"
#include "render/shaders.hpp"
#include "render/text_rendering.h"
#include "render/basis.hpp"

inline static constexpr auto windowed_width  = 800;
inline static constexpr auto windowed_height = 800;
inline static constexpr auto FPS    =  60;

#define WINDOWED

int main()
{
    /* ---------------------------OpenGL--------------------------- */
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width, height;

#ifdef WINDOWED
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(windowed_width, windowed_height, "My Window", NULL, NULL);
    width = windowed_width; height = windowed_height;
#else
    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    width = mode->width; height = mode->height;
    window = glfwCreateWindow(width, height, "My Window", monitor, NULL);
#endif

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }
    /* ------------------------------------------------------------ */
    
    /* ------------------------Load data in VRAM-------------------- */
    
    Shader shader("src/render/shaders/shader.vs", "src/render/shaders/shader.fs");
    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f);
    glm::mat4 view;
    shader.setMat4("projection", projection);
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setFloat("const_div", 1.0f);
    
    /* ------------------------------------------------------------ */

    
    Camera camera;
    camera.position = glm::vec3(10.0, 10.0, 10.0);

    Text_Renderer textRenderer("resources/fonts/erika.ttf", width, height);

    Basis base(5.0f, projection);
    

    while(!glfwWindowShouldClose(window))
    {
        auto t0 = std::chrono::high_resolution_clock::now();

        processInput(window);
        camera_controls(window, &camera);

        glfwPollEvents();
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        view = camera.getViewMat();
        textRenderer.RenderText("Press R to reset camera", 10, 10, 0.3, glm::vec3(255,255,255));

        base.draw(view);

        shader.use();
        shader.setMat4("view", view);

        glfwSwapBuffers(window);

        auto t1 = std::chrono::high_resolution_clock::now();
        auto delta = t1 - t0;
        auto sleep_dur = std::chrono::milliseconds(1000/FPS) - std::chrono::duration_cast<std::chrono::milliseconds>(delta);
        std::this_thread::sleep_for(sleep_dur);
        //std::cout << (std::chrono::microseconds(1000000/FPS) - std::chrono::duration_cast<std::chrono::microseconds>(delta)).count() << "\n";
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
