#include "BaseWidget.h"
#include "UIManager.h"
#include "LuaManager.h"
#include <imgui.h>
#include <random>
#include <string>
#include <sstream>

namespace Tapioca {
BaseWidget::BaseWidget()
    : name(generateRandomName()), transform(nullptr), windowFlags(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                                ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings) {
    uiManager = UIManager::instance();
    luaManager = LuaManager::instance();
}

BaseWidget::~BaseWidget() {
    transform = nullptr;
    uiManager = nullptr;
}

std::string BaseWidget::generateRandomName() {
    static std::mt19937 rng(std::random_device {}());
    static std::uniform_int_distribution<int> dist(0, 999999);
    std::stringstream ss;
    ss << "Widget_" << dist(rng);
    return ss.str();
}
}
