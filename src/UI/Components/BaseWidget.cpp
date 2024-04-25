#include "BaseWidget.h"
#include "UIManager.h"
#include "LuaManager.h"
#include <imgui.h>
#include <random>
#include <string>
#include <sstream>

namespace Tapioca {
BaseWidget::BaseWidget()
    : name(""), transform(nullptr), windowFlags(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                                ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings) {
    uiManager = UIManager::instance();
    luaManager = LuaManager::instance();
}

BaseWidget::~BaseWidget() {
    transform = nullptr;
    uiManager = nullptr;
}
}
