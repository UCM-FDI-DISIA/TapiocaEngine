#include "Text.h"
#include <OgreSceneManager.h>

namespace Tapioca {
Text::Text(const TextOptions& options) {
    Text(options.name, options.position, options.text, options.constSize, options.textFont, options.textColor, options.canCloseWindow,
         options.windowFlags);
}

Text::Text(const std::string& name, const ImVec2& position, const std::string& text, const ImVec2& constSize, ImFont* const textFont,
           const ImVec4& textColor, bool* canCloseWindow, ImGuiWindowFlags windowFlags)
    : name(name), position(position), text(text), textFont(textFont), textColor(textColor), canCloseWindow(canCloseWindow),
      windowFlags(windowFlags) { }
}