#ifndef SOKOBAN_CUTSCENE_HPP
#define SOKOBAN_CUTSCENE_HPP

namespace Sokoban {
enum CutsceneEvent {
    TIME_DELAY,
    CAMERA_TARGET_POS,
    CAMERA_SMOOTH_VALUE
};

struct Cutscene {
    std::vector<CutsceneEvent> events;
};

class CutsceneManager {
public:
    CutsceneManager() = default;
    void play(const std::string& name);
private:
    std::unordered_map<std::string, Cutscene> cutscenes;
    
    Sokoban::ObjectList* object_list;
    Turbine::Camera* camera;
};
}

#endif
