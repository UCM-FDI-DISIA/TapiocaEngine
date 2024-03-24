#include "BaseWidget.h"
#include <OgreSceneManager.h>

namespace Tapioca {
BaseWidget::BaseWidget(const WidgetOptions& options) {
    BaseWidget(options.name, options.position, options.canCloseWindow, options.windowFlags);
}

BaseWidget::BaseWidget(const std::string& name, const ImVec2& position, bool* canCloseWindow,
                       ImGuiWindowFlags windowFlags)
    : name(name), position(position), canCloseWindow(canCloseWindow), windowFlags(windowFlags) { }
}