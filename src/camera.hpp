#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "shader.hpp"
#include "sprite.hpp"

namespace Turbine {
struct Camera {
    Vector2 position;
    Vector2 target; // smoothing .. ?
    Vector2 origin;
    float zoom;
    float smooth;
    glm::mat4 matrix;

    Camera(void) {
        position = Vector2(0.0f, 0.0f);
        origin   = Vector2(0.0f, 0.0f);
        zoom     = 1.0f;
        smooth   = 1.0f;
        matrix   = glm::mat4(1.0f);
    }
};

void apply_camera(Camera& cam, Shader& shader);
void reset_camera(Camera& cam, Shader& shader);
}

#endif
