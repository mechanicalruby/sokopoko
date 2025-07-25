#include "texture.hpp"

namespace Turbine {
bool load_texture(Texture& tex, const std::string& file_path, uint32_t filter_mag, uint32_t filter_min, bool keep_data) {
    uint32_t texture;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    tex.id = texture;
    tex.filter_mag = filter_mag;
    tex.filter_min = filter_min;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);

    int width, height, comp;
    stbi_set_flip_vertically_on_load(0);
    unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &comp, STBI_rgb_alpha);

    if(data) {
        tex.width = width;
        tex.height = height;
        tex.data = data;
    } else {
        generate_texture(tex, width, height);
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data);

    if(!keep_data) {
        stbi_image_free(data);
    }
    
    printf("TEXTURE: Loaded texture (%i) from (%s). [%i, %i]\n", tex.id, file_path.c_str(), tex.width, tex.height);
    return true;
}

void generate_texture(Texture& tex, uint32_t width, uint32_t height, const uint8_t* data) {
    uint32_t texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);    
    glBindTexture(GL_TEXTURE_2D, 0);

    tex.id = texture;
    tex.width = width;
    tex.height = height;
}

void bind_texture(Texture& tex) {
    glBindTexture(GL_TEXTURE_2D, tex.id);
}

void unbind_texture(void) {
    glBindTexture(GL_TEXTURE_2D, 0);
}
}
