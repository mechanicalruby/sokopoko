#include "camera.hpp"

namespace Turbine {
void apply_camera(Camera& cam, Shader& shader) {
    cam.matrix = glm::mat4(1.0f);
    cam.matrix = glm::translate(cam.matrix, glm::vec3(-cam.position.x, -cam.position.y, 0.0f));
    cam.matrix = glm::translate(cam.matrix, glm::vec3(cam.origin.x, cam.origin.y, 0.0f));
    cam.matrix = glm::scale(cam.matrix, glm::vec3(cam.zoom, cam.zoom, 1.0f));
    
    Turbine::uniform_mat4(shader, "view", cam.matrix);
}

void reset_camera(Camera& cam, Shader& shader) {
    cam.matrix = glm::mat4(1.0f);
    Turbine::uniform_mat4(shader, "view", cam.matrix);
}
}
