#include "render_list.hpp"

namespace Turbine {
void RenderList::queue(Sprite& sprite) {
    if (!sprite.texture) return;

    uint32_t id = sprite.texture->id;

    auto it = batches.find(id);
    if (it != batches.end()) {
        Batch& batch = it->second;

        if (last_texture == sprite.texture) {
            batch.queue(sprite);
        }
        else {
            if (last_texture) {
                auto last_it = batches.find(last_texture->id);
                if (last_it != batches.end()) {
                    last_it->second.end(); // finish previous
                }
            }

            batch.begin(); // reuse previous batch, but for this frame
            batch.queue(sprite);
        }

        last_texture = sprite.texture;
        return;
    }

    // first time seeing this texture
    Batch& new_batch = batches[id];
    new_batch.texture = sprite.texture;
    new_batch.initialize();
    new_batch.begin();
    new_batch.queue(sprite);

    last_texture = sprite.texture;
}

void RenderList::begin() {
    current_batch = nullptr;
    last_texture = nullptr;
}

void RenderList::end() {
    if (last_texture) {
        auto it = batches.find(last_texture->id);
        if (it != batches.end()) {
            it->second.end(); // finish the final batch
        }
    }

    current_batch = nullptr;
    last_texture = nullptr;
}
}
