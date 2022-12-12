#ifndef OPENGL_3D_ENVIRONMENT_UTILS_HPP
#define OPENGL_3D_ENVIRONMENT_UTILS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../render/camera.hpp"

#include <fstream>
#include <string>


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        for(const auto &camera : Camera::instances) {
            camera->position = {10.0, 10.0, 10.0};
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    for(const auto &camera : Camera::instances) {
        zoom(camera, yoffset);
    }
}

#endif //OPENGL_3D_ENVIRONMENT_UTILS_HPP
