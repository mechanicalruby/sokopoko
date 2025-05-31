#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <iostream>
#include <glad/glad.h>
#include "sprite.hpp"

namespace Turbine {
void generate_vertex_buffer(GLuint* id);
void bind_vertex_buffer(GLuint id);
void unbind_vertex_buffer(void);

void allocate_vertex_buffer(GLuint id, const void* vertices, size_t size);
void update_vertex_buffer(GLuint id, const void* vertices, size_t size, size_t offset);

void generate_vertex_array(GLuint* id);
void bind_vertex_array(GLuint id);
void unbind_vertex_array(void);

void set_vertex_attributes(void);
}

#endif
