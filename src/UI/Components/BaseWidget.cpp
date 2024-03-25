#include "BaseWidget.h"
#include <OgreSceneManager.h>

namespace Tapioca {
BaseWidget::BaseWidget()
    : name(""), transform(nullptr), canCloseWindow(nullptr),
      windowFlags(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground |
                  ImGuiWindowFlags_NoSavedSettings) { }
}
