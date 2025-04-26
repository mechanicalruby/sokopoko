#include "render_list.hpp"

namespace Turbine {
void auto_render_list(const std::vector<Sprite*>& sprites) {
    Sprite* last = nullptr;
    
    for(Sprite* sprite : sprites)
    {
        if(last == nullptr || sprite == nullptr)
            continue;
        
        if(sprite->texture == last->texture) {
            
        }
    }
}
}
