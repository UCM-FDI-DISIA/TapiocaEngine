#include "BaseWidget.h"
#include <imgui.h>

namespace Tapioca {
BaseWidget::BaseWidget()
    : name(""), transform(nullptr), windowFlags(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                                ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings) { }

BaseWidget::~BaseWidget() {
    transform = nullptr;
}
}
