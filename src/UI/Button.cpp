#include "Button.h"
#include <OgreSceneManager.h>

namespace Tapioca {
Button::Button(Ogre::SceneManager* const scnMgr, RenderNode* const node, const ImVec2 position, const std::string& text,
               std::function<void()> onClick, const ImVec2 size, bool* canCloseWindow, ImGuiWindowFlags flags)
    : RenderObject(node, scnMgr), position(position), text(text), onClick(std::move(onClick)), size(size), canCloseWindow(canCloseWindow), flags(flags) { }
}