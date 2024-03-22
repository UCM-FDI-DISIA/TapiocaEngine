#include "Button.h"
#include <OgreSceneManager.h>

namespace Tapioca {
Button::Button(Ogre::SceneManager* const scnMgr, RenderNode* const node, const ImVec2& position,
               const std::string& text, std::function<void()> onClick, const ImVec2& constSize, const ImVec2& padding,
               const ImVec4& normalColor, const ImVec4& hoverColor, const ImVec4& activeColor, bool* canCloseWindow,
               ImGuiWindowFlags flags)
    : position(position), text(text), onClick(std::move(onClick)), constSize(constSize), padding(padding), normalColor(normalColor),
      hoverColor(hoverColor), activeColor(activeColor), canCloseWindow(canCloseWindow), flags(flags) { }
}