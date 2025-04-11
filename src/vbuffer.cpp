#include "vbuffer.hpp"

namespace Turbine {
void generate_vertex_buffer(GLuint* id) {
    glGenBuffers(1, id);
}

void bind_vertex_buffer(GLuint id) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void unbind_vertex_buffer(void) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void allocate_vertex_buffer(GLuint id, const void* vertices, size_t size) {
    Turbine::bind_vertex_buffer(id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

void update_vertex_buffer(GLuint id, const void* vertices, size_t size, size_t offset) {
    Turbine::bind_vertex_buffer(id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
}

void generate_vertex_array(GLuint* id) {
    glGenVertexArrays(1, id);
}

void bind_vertex_array(GLuint id) {
    glBindVertexArray(id);
}

void unbind_vertex_array(void) {
    glBindVertexArray(0);
}

void set_vertex_attributes(void) {
    // float x, y, z
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);
    // float u, v
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(1);
    // uint32_t color
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);
}
}
