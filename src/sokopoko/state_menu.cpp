#include "state_menu.hpp"

namespace Sokoban {
MenuState::~MenuState() {
    for(int i = 0; i < map.objects.size(); i++) {
        delete map.objects[i];
    }
}

void MenuState::init() {
    // Loading
    Turbine::load_texture(texture,        "./res/ui.png", TB_NEAREST, TB_NEAREST, false);
    Turbine::load_texture(object_texture, "./res/objects.png", TB_NEAREST, TB_NEAREST, false);
    Turbine::load_texture(map_texture,    "./res/map.png", TB_NEAREST, TB_NEAREST, false);
    b1.texture = &map_texture;
    b2.texture = &object_texture;

    sprite.color = 0x7CFFFFFF;
    sprite.offset.y = 16;
    sprite.centered = true;
    sprite.region = Turbine::Rect{84, 0, 24, 12};

    load_map(map, "C:/Users/robbi/Desktop/mother.json");
    
    cam.origin.x   = 320.0f / 2.0f;
    cam.origin.y   = 240.0f / 2.0f;
    
    map.objects.push_back(new Crate({2, 2}));
    map.objects.push_back(new Crate({4, 1}));
    map.objects.push_back(new Barrier({-1, -1}));
    map.objects.push_back(new Barrier({0, -1}));
    map.objects.push_back(new Barrier({1, -1}));
    map.objects.push_back(new Ross({0, 0}));
}

void MenuState::update(double delta_time, Turbine::InputState& input) {
    Sokoban::Ross* ross = static_cast<Sokoban::Ross*>(map.objects[5]);
    mouse_x = input.mouse_x / 3; // scale
    mouse_y = input.mouse_y / 3;

    cam.position.x = ((map.width  * 24.0f) - 12) / 2.0f;
    cam.position.y = ((map.height * 24.0f) - 12) / 2.0f;

    ross->step.tick(1.0f / 60.0f);
    
    if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_UP)) {
        ross->position.y--;
        ross->step.play();
    }
    if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_DOWN)) {
        ross->position.y++;
        ross->step.play();
    }
    if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_LEFT)) {
        ross->position.x--;
        ross->step.play();
    }
    if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_RIGHT)) {
        ross->position.x++;
        ross->step.play();
    }

    if(Turbine::just_pressed(input, Turbine::InputAction::EDITOR_PLACE_TILE)) {
        change_tile(map, SokoPosition{static_cast<int>((mouse_x+12) / 24), static_cast<int>((mouse_y+12) / 24)}, 2);
    }

    for(auto& object : map.objects) {
        object->sprite.position.x = static_cast<float>(std::lerp(object->sprite.position.x, (object->position.x * 24), 20 * delta_time));
        object->sprite.position.y = static_cast<float>(std::lerp(object->sprite.position.y, (object->position.y * 24) - 12, 20 * delta_time));
    }
}

void MenuState::draw(Turbine::Window& window, Turbine::Shader& base_shader) {
    Sokoban::Ross* ross = static_cast<Sokoban::Ross*>(map.objects[5]);
    Turbine::clear(window, 0.0f, 0.0f, 1.0f);
    Turbine::set_blend_mode(window);

    Turbine::apply_camera(cam, base_shader);
    draw_map(map, b1);

    b2.begin();
    for(auto& object : map.objects) {
        if(object->hidden) {
            if(map.show_hidden_objects) {
                object->sprite.color = 0x50FFFFFF;
                object->sprite.visible = true;
                sprite.visible = true;
            } else {
                object->sprite.visible = false;
                sprite.visible = false;
            }
        } else {
            sprite.visible = true;
            object->sprite.color = 0xFFFFFFFF;
        }
        
        sprite.position = object->sprite.position;
        
        b2.queue(sprite);
        b2.queue(object->sprite);
    }
    b2.end();
    
    Turbine::reset_camera(cam, base_shader);
    Sokoban::imgui_map_inspect(map);
}
}
