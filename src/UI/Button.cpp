#include "Button.h"
#include <OgreSceneManager.h>

namespace Tapioca {
Button::Button(Ogre::SceneManager* const scnMgr, const ButtonOptions& options) {
    Button(scnMgr, options.node, options.position, options.text, options.onClick, options.constSize, options.padding,
           options.textFont, options.textColor, options.normalColor, options.hoverColor, options.activeColor,
           options.canCloseWindow, options.flags);
}

Button::Button(Ogre::SceneManager* const scnMgr, RenderNode* const node, const ImVec2& position,
               const std::string& text, std::function<void()> onClick, const ImVec2& constSize, const ImVec2& padding,
               ImFont* const textFont, const ImVec4& textColor, const ImVec4& normalColor, const ImVec4& hoverColor,
               const ImVec4& activeColor, bool* canCloseWindow, ImGuiWindowFlags flags)
    : position(position), text(text), onClick(std::move(onClick)), constSize(constSize), padding(padding),
      textFont(textFont), textColor(textColor), normalColor(normalColor), hoverColor(hoverColor),
      activeColor(activeColor), canCloseWindow(canCloseWindow), flags(flags) { }
}