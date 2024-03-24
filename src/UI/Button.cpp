#include "Button.h"
#include <OgreSceneManager.h>

namespace Tapioca {
Button::Button(const ButtonOptions& options)
    : BaseWidget(options.name, options.position, options.canCloseWindow, options.windowFlags), text(options.text),
      onClick(options.onClick), constSize(options.constSize), padding(options.padding), textFont(options.textFont),
      textColor(options.textColor), normalColor(options.normalColor), hoverColor(options.hoverColor),
      activeColor(options.activeColor) { }

Button::Button(const std::string& name, const ImVec2& position, const std::string& text, std::function<void()> onClick,
               const ImVec2& constSize, const ImVec2& padding, ImFont* const textFont, const ImVec4& textColor,
               const ImVec4& normalColor, const ImVec4& hoverColor, const ImVec4& activeColor, bool* canCloseWindow,
               ImGuiWindowFlags windowFlags)
    : BaseWidget(name, position, canCloseWindow, windowFlags), text(text), onClick(std::move(onClick)),
      constSize(constSize), padding(padding), textFont(textFont), textColor(textColor), normalColor(normalColor),
      hoverColor(hoverColor), activeColor(activeColor) { }
}