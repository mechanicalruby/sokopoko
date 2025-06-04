#include "sprite.hpp"

namespace Turbine {

Sprite::Sprite() {
    position.x = 0.0f;
    position.y = 0.0f;
    origin.x = 0.0f;
    origin.y = 0.0f;
    offset.x = 0.0f;
    offset.y = 0.0f;
    scale.x = 1.0f;
    scale.y = 1.0f;
    rotation = 0.0f;
    layer = 0.0f;
    centered = true;
    flip_h = false;
    flip_v = false;
    visible = true;
    color = 0xFFFFFFFF;
    region = Rect{0.0f, 0.0f, 64.0f, 64.0f};
    texture = nullptr;
}

void Sprite::set_region(const Rect& value) {
    // if(region == value) { return; }
    region = value;
};

Rect Sprite::get_region() const {
    return region;
}

void y_compare_sprites(const Sprite& a, const Sprite& b) {

}

void layer_compare_sprites(const Sprite& a, const Sprite& b) {

}

void Batch::initialize(void) {
    texture = nullptr;
    index_offset = 0;

#if !TB_GRAPHICS_LEGACY
#if !TB_GRAPHICS_GLES2
    Turbine::generate_vertex_array(&vao);
    Turbine::bind_vertex_array(vao);
#endif
    Turbine::generate_vertex_buffer(&vbo);
    Turbine::bind_vertex_buffer(vbo);
    Turbine::allocate_vertex_buffer(vbo, vertices.data(), sizeof(Vertex) * BATCH_SIZE);
    Turbine::set_vertex_attributes();
    Turbine::unbind_vertex_buffer();
#if !TB_GRAPHICS_GLES2
    Turbine::unbind_vertex_array();
#endif
#endif

    is_initialized = true;
}

void Batch::calculate_vertices(Sprite& sprite, size_t offset) {
    if (offset > vertices.size() - 6) {
        std::cerr << "SPRITEBATCH: Failed to find enough space for sprite" << std::endl;
        return;
    }

    if (texture == nullptr && sprite.texture != nullptr) {
        texture = sprite.texture;
    }

    if(texture != nullptr && sprite.texture == nullptr) {
        sprite.texture = texture;
    }

    if (texture == nullptr) {
        return;
    }

    if (sprite.centered) {
        sprite.origin.x = (sprite.region.width * sprite.scale.x) / 2;
        sprite.origin.y = (sprite.region.height * sprite.scale.y) / 2;
    }

    Rect& source = sprite.region;
    float x, y, src_x, src_y, src_width, src_height;

    x          = (sprite.position.x - sprite.origin.x) + sprite.offset.x;
    y          = (sprite.position.y - sprite.origin.y) + sprite.offset.y;
    src_x      = source.x;
    src_y      = source.y;
    src_width  = source.width;
    src_height = source.height;

    float width = source.width * sprite.scale.x;
    float height = source.height * sprite.scale.y;

    glm::vec2 top_left     = {x        , y         };
    glm::vec2 top_right    = {x + width, y         };
    glm::vec2 bottom_left  = {x        , y + height};
    glm::vec2 bottom_right = {x + width, y + height};

    if (sprite.flip_h) {
        std::swap(top_left.x,    top_right.x);
        std::swap(top_left.y,    top_right.y);
        std::swap(bottom_left.x, bottom_right.x);
        std::swap(bottom_left.y, bottom_right.y);
    }

    if (sprite.flip_v) {
        std::swap(top_left.x, bottom_left.x);
        std::swap(top_left.y, bottom_left.y);
        std::swap(top_right.x, bottom_right.x);
        std::swap(top_right.y, bottom_right.y);
    }

    vertices[index_offset + 0] = Vertex{top_left.x    , top_left.y,     -1.0f, src_x / texture->width,               src_y / texture->height,                sprite.color};
    vertices[index_offset + 1] = Vertex{bottom_left.x , bottom_left.y,  -1.0f, src_x / texture->width,               (src_y + src_height) / texture->height, sprite.color};
    vertices[index_offset + 2] = Vertex{bottom_right.x, bottom_right.y, -1.0f, (src_x + src_width) / texture->width, (src_y + src_height) / texture->height, sprite.color};
    vertices[index_offset + 3] = Vertex{top_right.x   , top_right.y,    -1.0f, (src_x + src_width) / texture->width, src_y / texture->height,                sprite.color};
    vertices[index_offset + 4] = vertices[index_offset + 2];
    vertices[index_offset + 5] = vertices[index_offset + 0];
}

void Batch::queue(Sprite& sprite) {
    if(!sprite.visible)
        return;
    
    calculate_vertices(sprite, index_offset);
    index_offset += 6;
}

void Batch::begin(void) {
    index_offset = 0;
}

void Batch::end(void) {
    if(!is_initialized) {
        printf("SPRITEBATCH: Was not initialized!!\n");
        return;
    }
  
    if(texture != nullptr) {
        Turbine::bind_texture(*texture);
    }

    // only draw what we need
    size_t upper_bound = index_offset;

#if !TB_GRAPHICS_LEGACY
#if !TB_GRAPHICS_GLES2
    Turbine::bind_vertex_array(vao); // comment for gles2
#endif
    Turbine::bind_vertex_buffer(vbo);
#if TB_GRAPHICS_GLES2
    Turbine::set_vertex_attributes(); // GLES2 ONLY!! SINCE WE HAVE NO VAO, WE MUST REDEFINE OUR VERTEX ATTRIBS
#endif
    Turbine::update_vertex_buffer(vbo, vertices.data(), upper_bound * sizeof(Vertex), 0);
#endif
    glDrawArrays(GL_TRIANGLES, 0, (int)upper_bound);

    // Turbine::unbind_texture();
}
}
