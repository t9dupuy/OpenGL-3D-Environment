#ifndef OPENGL_3D_ENVIRONMENT_CAMERA_HPP
#define OPENGL_3D_ENVIRONMENT_CAMERA_HPP

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>

class Camera
{
public:
    glm::vec3 position{glm::vec3(1.0f, 1.0f, 1.0f)};
    glm::vec3 target{glm::vec3(0.0f, 0.0f, 0.0f)};
    glm::vec3 up{glm::vec3(0.0f, 1.0f, 0.0f)};

    inline static std::vector<Camera *> instances;

    Camera()
    {
        instances.emplace_back(this);
    }

    Camera(const Camera &) =delete;
    Camera & operator=(const Camera &) =delete;

    ~Camera()
    {
        auto it = std::find(instances.begin(), instances.end(), this);

        if(it != instances.end()) {
            instances.erase(it);
        }
    }

    [[nodiscard]] inline
    glm::mat4 getViewMat() const
    {
        return glm::lookAt(position, target, up);
    }
};

void camera_controls(GLFWwindow *window, Camera *camera) {
    static bool rot = false;

    double xpos, ypos;
    static glm::vec2 mouse_startPos;

    static glm::vec3 camera_startPos;
    static glm::vec3 camera_startTar;

    static glm::vec3 vec_tar_pos;

    static float dist_p;
    static float angle_phi;
    static float angle_theta;

    static bool left_button_pressed = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !left_button_pressed) {
        left_button_pressed = true;
        rot = true;

        glfwGetCursorPos(window, &xpos, &ypos);
        mouse_startPos = glm::vec2(xpos, ypos);
        camera_startPos = camera->position;
        camera_startTar = camera->target;

        vec_tar_pos = camera_startPos - camera_startTar;

        dist_p = glm::length(vec_tar_pos);
        angle_phi = atan2f(vec_tar_pos.z, vec_tar_pos.x);
        angle_theta = atanf(vec_tar_pos.y / hypotf(vec_tar_pos.x, vec_tar_pos.z));
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS && left_button_pressed) {
        left_button_pressed = false;
        rot = false;
    }

    if (rot) {
        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 mouse_delta = {0.003f * (xpos - mouse_startPos.x),
                                 0.003f * (ypos - mouse_startPos.y)};

        camera->position.x =
                camera_startTar.x + dist_p * cosf(angle_theta + mouse_delta.y) * cosf(angle_phi + mouse_delta.x);
        camera->position.y = camera_startTar.y + dist_p * sinf(angle_theta + mouse_delta.y);
        camera->position.z =
                camera_startTar.z + dist_p * cosf(angle_theta + mouse_delta.y) * sinf(angle_phi + mouse_delta.x);
    }
}

void zoom(Camera *camera, double amount)
{
    glm::vec3 vec_tar_pos = camera->position - camera->target;

    camera->position += glm::vec3(-amount * 0.1) * vec_tar_pos;
}

#endif //OPENGL_3D_ENVIRONMENT_CAMERA_HPP
