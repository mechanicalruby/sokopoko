#include "atlas.hpp"

namespace Turbine {
bool load_atlas(Atlas& at, const std::string& file_path) {
    yyjson_read_err err;
    yyjson_doc* doc = yyjson_read_file(file_path.c_str(), 0, NULL, &err);
    if(doc == NULL || err.pos != 0) {
        printf("JSON: %s at position %zu\n", err.msg, err.pos);
        return false;
    }

    yyjson_val* root = yyjson_doc_get_root(doc);

    if(!at.regions.empty()) {
        at.regions.clear();
    }

    yyjson_val* textures_arr = yyjson_obj_get(root, "textures");
    if(yyjson_is_arr(textures_arr)) {
        size_t idx, max;
        yyjson_val* texture;
        yyjson_arr_foreach(textures_arr, idx, max, texture) {
            // get texture name
            yyjson_val* texture_name = yyjson_obj_get(texture, "name");
            printf("texture atlas %s:\n", yyjson_get_str(texture_name));

            // get regions/images
            yyjson_val* images_arr = yyjson_obj_get(texture, "images");
            if(yyjson_is_arr(images_arr)) {
                size_t idx, max;
                yyjson_val* image;
                yyjson_arr_foreach(images_arr, idx, max, image) {
                    yyjson_val* image_name   = yyjson_obj_get(image, "n");
                    yyjson_val* image_x      = yyjson_obj_get(image, "x");
                    yyjson_val* image_y      = yyjson_obj_get(image, "y");
                    yyjson_val* image_width  = yyjson_obj_get(image, "w");
                    yyjson_val* image_height = yyjson_obj_get(image, "h");

                    yyjson_val* image_offset_x = yyjson_obj_get(image, "fx");
                    yyjson_val* image_offset_y = yyjson_obj_get(image, "fy");
                    
                    Atlas::Region region = {
                        .name = yyjson_get_str(image_name),
                        .rect = Turbine::Rect{
                            static_cast<float>(yyjson_get_int(image_x)),
                            static_cast<float>(yyjson_get_int(image_y)),
                            static_cast<float>(yyjson_get_int(image_width)),
                            static_cast<float>(yyjson_get_int(image_height))
                        },
                        .offset = Turbine::Vector2{
                            static_cast<float>(yyjson_get_int(image_offset_x)),
                            static_cast<float>(yyjson_get_int(image_offset_y))
                        },
                        .tex_id = 0
                    };

                    printf("atlas region %s loaded\n", region.name.c_str());
                    at.regions[region.name] = std::move(region);
                }
            }
        }
    }
    return true;
}
}