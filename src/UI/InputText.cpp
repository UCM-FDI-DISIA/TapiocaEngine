#include "InputText.h"
#include <cstring>
#include <OgreSceneManager.h>

namespace Tapioca {
InputText::InputText(const InputTextOptions& options) {
    InputText(options.position, options.placeHolderText, options.bufferSize, options.onTextEntered, options.constWidth,
              options.textFont, options.textColor, options.bgColor, options.flags, options.callback, options.userData,
              options.canCloseWindow, options.windowFlags);
}

InputText::InputText(const ImVec2& position, const std::string& placeHolderText, const size_t bufferSize,
                     std::function<void()> onTextEntered, const float constWidth, ImFont* const textFont,
                     const ImVec4& textColor, const ImVec4& bgColor, const ImGuiInputTextFlags& flags,
                     const ImGuiInputTextCallback& callback, void* userData, bool* canCloseWindow,
                     ImGuiWindowFlags windowFlags)
    : position(position), placeHolderText(placeHolderText), bufferSize(bufferSize),
      onTextEntered(std::move(onTextEntered)), constWidth(constWidth), textFont(textFont), textColor(textColor),
      bgColor(bgColor), flags(flags), callback(callback), userData(userData), canCloseWindow(canCloseWindow),
      windowFlags(windowFlags) {
    buffer = new char[bufferSize];
    startBuffer();
}

InputText::~InputText() { delete[] buffer; }

void InputText::startBuffer() { strcpy_s(buffer, bufferSize, placeHolderText.c_str()); }
}