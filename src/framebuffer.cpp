#include "framebuffer.hpp"

namespace Turbine {
Framebuffer::Framebuffer (uint32_t width, uint32_t height) {    
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    this->width = width;
    this->height = height;

    generate_texture(texture, width, height);
    bind_texture(texture);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("FRAMEBUFFER: Couldn't create a buffer\n");
    }
    
    unbind_framebuffer();
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &id);
}

void bind_framebuffer(Framebuffer& fb) {
    glBindFramebuffer(GL_FRAMEBUFFER, fb.id);
}

void unbind_framebuffer(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}
