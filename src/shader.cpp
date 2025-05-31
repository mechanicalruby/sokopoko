#include "shader.hpp"

namespace Turbine {
bool check_compile_error(GLuint object, ProgramType type) {
    switch(type) {
    case TB_SHADER_PROG: {
            GLint is_compiled = 0;
            glGetProgramiv(object, GL_LINK_STATUS, &is_compiled);

            if (is_compiled == GL_FALSE) {
                GLint max_length = 0;
                glGetProgramiv(object, GL_INFO_LOG_LENGTH, &max_length);

                GLchar error_log[1024];
                glGetProgramInfoLog(object, max_length, &max_length, &error_log[0]);
                printf("SHADER PROGRAM: failed to compile: %s\n", error_log);
                return false;
            }
        }
        break;
    default: {
            GLint success = 0;
            char info_log[512];
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            
            if (success == GL_FALSE) {
                glGetShaderInfoLog(object, 512, NULL, info_log);
                printf("SHADER TYPE %i: failed to compile: %s", type, info_log);
                glDeleteShader(object);
                return false;
            }
        }
        break;
    }
    return true;
}

void compile_shader(Shader& shader, const char* vert, const char* frag) {
    GLuint vertex_s, fragment_s;
    printf("SHADER: beginning shader compilation.\n");

    vertex_s = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_s, 1, &vert, NULL);
    glCompileShader(vertex_s);
    printf("SHADER: vertex shader #%i attempt compile.\n", vertex_s);
    if(check_compile_error(vertex_s, TB_VERT_SHADER) == false) {
        printf("Failed to initialize vertex shader.\n");
        return;
    }
    
    fragment_s = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_s, 1, &frag, NULL);
    glCompileShader(fragment_s);
    printf("SHADER: fragment shader #%i attempt compile.\n", fragment_s);
    if(check_compile_error(vertex_s, TB_FRAG_SHADER) == false) {
        printf("Failed to initialize fragment shader.\n");
        return;
    }

    shader.id = glCreateProgram();
    glAttachShader(shader.id, vertex_s  );
    glAttachShader(shader.id, fragment_s);

    /* GLES2 hardcode */
    glBindAttribLocation(shader.id, 0, "aPosition");
    glBindAttribLocation(shader.id, 1, "aTexCoord");
    glBindAttribLocation(shader.id, 2, "aVertexColor");
    /* end of GLES2 code */

    glLinkProgram(shader.id);
    if(check_compile_error(shader.id, TB_SHADER_PROG) == false) {
        printf("Failed to initialize shader program.\n");
        return;
    }

    printf("SHADER: linked program successfully\n");

    glDetachShader(shader.id, vertex_s  );
    glDetachShader(shader.id, fragment_s);

    glDeleteShader(vertex_s  );
    glDeleteShader(fragment_s);
}

bool load_shader_from_file(Shader& shader, const std::string& vert_path, const std::string& frag_path) {
    // load vertex shader
    std::ifstream vert_stream;
    vert_stream.open(vert_path);

    if(!vert_stream.is_open()) {
        printf("SHADER: Could not read vertex shader from file (%s).\n", vert_path.c_str());
        return false;
    }

    auto v_size = vert_stream.gcount();
    if(v_size > 0x100000) {
        printf("SHADER: Vertex shader is too large (64KiB >)!\n");
        return false;
    }

    // load fragment shader
    std::ifstream frag_stream;
    frag_stream.open(frag_path);

    if(!frag_stream.is_open()) {
        printf("SHADER: Could not read fragment shader from file (%s).\n", frag_path.c_str());
        return false;
    }

    auto f_size = frag_stream.gcount();
    if(f_size > 0x100000) {
        printf("SHADER: Fragment shader is too large (64KiB >)!\n");
        return false;
    }

    // move to stringstream
    std::stringstream vss_string;
    std::stringstream fss_string;
    vss_string << vert_stream.rdbuf();
    fss_string << frag_stream.rdbuf();
    std::string v_out = vss_string.str();
    std::string f_out = fss_string.str();

    compile_shader(shader, v_out.c_str(), f_out.c_str());
   
    vert_stream.close(); 
    frag_stream.close(); 
    return true;
}

void compile_default_shader(Shader& shader) {
/*
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
*/
    
    // compile_shader(shader, vertex_shader, fragment_shader);
}

void use_shader(Shader& shader) {
    glUseProgram(shader.id);
}

// uniforms

void uniform_mat4(Shader& shader, const char* name, const glm::mat4& value) {
    GLuint uniform = glGetUniformLocation(shader.id, name);
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
}
  
void uniform_float(Shader& shader, const char* name, float value) {
    GLuint uniform = glGetUniformLocation(shader.id, name);
    glUniform1f(uniform, value);
}
}
