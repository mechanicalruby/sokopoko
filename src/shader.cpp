#include "shader.hpp"

namespace Turbine {
bool check_compile_error(GLuint object, ProgramType type) {
    switch(type) {
    case TB_SHADER_PROG:
        GLint is_compiled;
        glGetShaderiv(object, GL_COMPILE_STATUS, &is_compiled);

        if (!is_compiled) {
            GLint max_length = 0;
            glGetShaderiv(object, GL_INFO_LOG_LENGTH, &max_length);

            GLchar error_log[1024];
            glGetShaderInfoLog(object, max_length, &max_length, &error_log[0]);
            printf("SHADER: failed to compile: %s\n", error_log);
            return false;
        }
        break;
    default:
        int success;
        char info_log[512];
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        
        if (!success) {
            glGetProgramInfoLog(object, 512, NULL, info_log);
            printf("SHADER: type %i failed to compile: %s", type, info_log);
            return false;
        }
        break;
    }
    return true;
}

void compile_shader(Shader& shader, const char* vert_path, const char* frag_path) {
    GLuint vertex_s, fragment_s;
    printf("SHADER: beginning shader compilation.\n");

    vertex_s = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_s, 1, &vert_path, NULL);
    glCompileShader(vertex_s);
    printf("SHADER: vertex shader #%i attempt compile.\n", vertex_s);
    if(check_compile_error(vertex_s, TB_VERT_SHADER) == false) {
        printf("Failed to initialize vertex shader.\n");
        return;
    }
    
    fragment_s = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_s, 1, &frag_path, NULL);
    glCompileShader(fragment_s);
    printf("SHADER: fragment shader #%i attempt compile.\n", fragment_s);
    if(check_compile_error(vertex_s, TB_FRAG_SHADER) == false) {
        printf("Failed to initialize fragment shader.\n");
        return;
    }

    shader.id = glCreateProgram();
    glAttachShader(shader.id, vertex_s  );
    glAttachShader(shader.id, fragment_s);

    glLinkProgram(shader.id);
    if(check_compile_error(vertex_s, TB_SHADER_PROG) == false) {
        printf("Failed to initialize shader program.\n");
        return;
    }

    glDeleteShader(vertex_s  );
    glDeleteShader(fragment_s);
}

void compile_default_shader(Shader& shader) {
    const char* vertex_shader =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "layout (location = 2) in vec4 aColor;\n"
        "\n"
        "out vec2 TexCoord;\n"
        "out vec4 VertexColor;\n"
        "\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "    TexCoord = aTexCoord;\n"
        "    VertexColor = aColor;\n"
        "}\n";
 
    const char* fragment_shader = 
        "#version 330 core\n"
        "in vec2 TexCoord;\n"
        "in vec4 VertexColor;\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "uniform sampler2D tex;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragColor = texture(tex, TexCoord) * VertexColor;\n"
        "}\n";

    compile_shader(shader, vertex_shader, fragment_shader);
}

void use_shader(Shader& shader) {
    glUseProgram(shader.id);
}

// uniforms

void uniform_mat4(Shader& shader, const char* name, const glm::mat4& value) {
    GLuint uniform = glGetUniformLocation(shader.id, name);
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
}
}
