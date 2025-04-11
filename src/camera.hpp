#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "shader.hpp"

namespace Turbine {
struct Camera {
    float x, y;
    float origin_x, origin_y;
    float zoom;
    float smooth;
    glm::mat4 matrix;

    Camera(void) {
        x        = 0.0f;
        y        = 0.0f;
        origin_x = 0.0f;
        origin_y = 0.0f;
        zoom     = 1.0f;
        smooth   = 1.0f;
        matrix   = glm::mat4(1.0f);
    }
};

void apply_camera(Camera& cam, Shader& shader) {
    cam.matrix = glm::mat4(1.0f);
    cam.matrix = glm::translate(cam.matrix, glm::vec3(-cam.x, -cam.y, 0.0f));
    cam.matrix = glm::translate(cam.matrix, glm::vec3(cam.origin_x, cam.origin_y, 0.0f));
    cam.matrix = glm::scale(cam.matrix, glm::vec3(cam.zoom, cam.zoom, 1.0f));
    
    Turbine::uniform_mat4(shader, "view", cam.matrix);
}

void reset_camera(Camera& cam, Shader& shader) {
    cam.matrix = glm::mat4(1.0f);
    Turbine::uniform_mat4(shader, "view", cam.matrix);
}
}

#endif
