#include "Text.h"
#include <OgreSceneManager.h>

namespace Tapioca {
Text::Text(const TextOptions& options)
    : BaseWidget(options.name, options.position, options.canCloseWindow, options.windowFlags), text(options.text),
      textFont(options.textFont), textColor(options.textColor) { }

Text::Text(const std::string& name, const ImVec2& position, const std::string& text, const ImVec2& constSize,
           ImFont* const textFont, const ImVec4& textColor, bool* canCloseWindow, ImGuiWindowFlags windowFlags)
    : BaseWidget(name, position, canCloseWindow, windowFlags), text(text), textFont(textFont), textColor(textColor) { }
}