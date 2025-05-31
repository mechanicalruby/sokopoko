#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>

namespace Turbine {
struct Shader {
    std::string file_path;
    GLuint id;
};

enum ProgramType {
    TB_VERT_SHADER,
    TB_FRAG_SHADER,
    TB_SHADER_PROG,
};

bool check_compile_error(GLuint object, ProgramType type);
void compile_shader(Shader& shader, const char* vert, const char* frag);
void use_shader(Shader& shader);

bool load_shader_from_file(Shader& shader, const std::string& vert_path, const std::string& frag_path);
void compile_default_shader(Shader& shader);

void uniform_mat4(Shader& shader, const char* name, const glm::mat4& value);
void uniform_float(Shader& shader, const char* name, float value);
}
#endif
