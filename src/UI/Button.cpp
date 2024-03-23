#include "Button.h"
#include <OgreSceneManager.h>

namespace Tapioca {
Button::Button(const ButtonOptions& options) {
    Button(options.position, options.text, options.onClick, options.constSize, options.padding, options.textFont,
           options.textColor, options.normalColor, options.hoverColor, options.activeColor, options.canCloseWindow,
           options.windowFlags);
}

Button::Button(const ImVec2& position, const std::string& text, std::function<void()> onClick, const ImVec2& constSize,
               const ImVec2& padding, ImFont* const textFont, const ImVec4& textColor, const ImVec4& normalColor,
               const ImVec4& hoverColor, const ImVec4& activeColor, bool* canCloseWindow, ImGuiWindowFlags windowFlags)
    : position(position), text(text), onClick(std::move(onClick)), constSize(constSize), padding(padding),
      textFont(textFont), textColor(textColor), normalColor(normalColor), hoverColor(hoverColor),
      activeColor(activeColor), canCloseWindow(canCloseWindow), windowFlags(windowFlags) { }
}