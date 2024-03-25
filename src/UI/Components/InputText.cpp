//#include "InputText.h"
//#include <cstring>
//#include <OgreSceneManager.h>
//
//namespace Tapioca {
//InputText::InputText(const InputTextOptions& options)
//    : BaseWidget(options.name, options.position, options.canCloseWindow, options.windowFlags),
//      placeHolderText(options.placeHolderText), bufferSize(options.bufferSize),
//      onTextEntered(std::move(options.onTextEntered)), constWidth(options.constWidth), textFont(options.textFont),
//      textColor(options.textColor), bgColor(options.bgColor), flags(options.flags), callback(options.callback),
//      userData(options.userData) {
//    buffer = new char[bufferSize];
//    startBuffer();
//}
//
//InputText::InputText(const std::string& name, const ImVec2& position, const std::string& placeHolderText,
//                     const size_t bufferSize, std::function<void()> onTextEntered, const float constWidth,
//                     ImFont* const textFont, const ImVec4& textColor, const ImVec4& bgColor,
//                     const ImGuiInputTextFlags& flags, const ImGuiInputTextCallback& callback, void* userData,
//                     bool* canCloseWindow, ImGuiWindowFlags windowFlags)
//    : BaseWidget(name, position, canCloseWindow, windowFlags), placeHolderText(placeHolderText), bufferSize(bufferSize),
//      onTextEntered(std::move(onTextEntered)), constWidth(constWidth), textFont(textFont), textColor(textColor),
//      bgColor(bgColor), flags(flags), callback(callback), userData(userData) {
//    buffer = new char[bufferSize];
//    startBuffer();
//}
//
//InputText::~InputText() { delete[] buffer; }
//
//void InputText::startBuffer() { strcpy_s(buffer, bufferSize, placeHolderText.c_str()); }
//}