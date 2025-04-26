#include "state_menu.hpp"

namespace Sokoban {
MenuState::~MenuState() {
    for(int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
}

void MenuState::init() {
    // Loading
    Turbine::load_texture(texture,        "./res/ui.png", TB_NEAREST, TB_NEAREST, false);
    Turbine::load_texture(object_texture, "./res/rpg_atlas.png", TB_NEAREST, TB_NEAREST, false);
    b1.texture = &texture;
    b2.texture = &object_texture;

    sprite.color = 0x50FFFFFF;
    sprite.offset.y = 16;
    sprite.centered = true;
    sprite.region = Turbine::Rect{84, 0, 24, 12};

    cam.origin.x   = 320.0f / 2.0f;
    cam.origin.y   = 240.0f / 2.0f;
    cam.position.x = 320.0f / 2.0f;
    cam.position.y = 240.0f / 2.0f;

    objects.push_back(new Ross({0, 0}));
    objects.push_back(new Crate({4, 4}));
    objects.push_back(new Crate({6, 4}));
}

void MenuState::update(double delta_time, Turbine::InputState& input) {
    Sokoban::Ross* ross = static_cast<Sokoban::Ross*>(objects[0]);

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

    for(auto& object : objects) {
        object->sprite.position.x = object->position.x;
        object->sprite.position.y = object->position.y;
    }
}

void MenuState::draw(Turbine::Window& window, Turbine::Shader& base_shader) {
    Sokoban::Ross* ross = static_cast<Sokoban::Ross*>(objects[0]);
    Turbine::clear(window, 0.0f, 0.0f, 1.0f);
    Turbine::set_blend_mode(window);

    Turbine::apply_camera(cam, base_shader);

    b2.begin();
    for(auto& object : objects) {
        sprite.position = object->sprite.position;
        
        b2.queue(sprite);
        b2.queue(object->sprite);
    }
    b2.draw();
    
    Turbine::reset_camera(cam, base_shader);

    ImGui::Begin("Objects");
    ImGui::Text("Camera:");
    ImGui::SameLine();
    ImGui::SliderFloat2("##camx", &cam.position.x, -240.0f, 240.0f);
    ImGui::Separator();
    ImGui::Text("Ross step: %f", ross->step.get_current_time());
    ImGui::Separator();
    for(Sokoban::SokoObject* object : objects) {
        if (ImGui::TreeNode(object->name.c_str())) {
            ImGui::Text("Position: %i, %i", object->position.x, object->position.y);
            ImGui::TreePop();
        }
    }
    ImGui::End();

    ImGui::Begin("Animations");
    if (ImGui::TreeNode("Yep")) {

        ImGui::TreePop();
    }
    ImGui::End();
}
}
