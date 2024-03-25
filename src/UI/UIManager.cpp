#include "UIManager.h"

#include <filesystem>
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <OgreRenderWindow.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "SDL_opengl.h"

#include "Structure/Game.h"
#include "Structure/Scene.h"
#include "WindowManager.h"

#include "Components/Transform.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<UIManager>;
template<>
UIManager* Singleton<UIManager>::instance_ = nullptr;

UIManager::UIManager()
    : game(nullptr), windowManager(nullptr), sdlWindow(nullptr), glContext(nullptr), sceneManager(nullptr),
      fontsPath("assets/fonts/") { }

UIManager::~UIManager() {
    sdlWindow = nullptr;
    glContext = nullptr;
    sceneManager = nullptr;

    fonts.clear();
    /*for (auto text : texts) {
        delete text.second;
    }
    texts.clear();*/
    for (auto button : buttons) {
        delete button.second;
    }
    buttons.clear();
    /*for (auto inputText : inputTexts) {
        delete inputText.second;
    }
    inputTexts.clear();*/

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) { render(); }

bool UIManager::init() {
    game = Game::instance();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    io.WantCaptureKeyboard = true;
    io.WantCaptureMouse = true;
    windowManager = WindowManager::instance();
    sdlWindow = windowManager->getWindow();
    glContext = windowManager->getGLContext();
    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, glContext);
    ImGui_ImplOpenGL3_Init("#version 130");

    loadFonts();

    return true;
}

void UIManager::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    game->getTopScene()->render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//void UIManager::renderTexts() {
    //for (auto myText : texts) {
    //    Text* text = myText.second;

    //    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del texto
    //    Vector3 position = text->getTransform()->getPosition();
    //    ImVec2 ImPosition = ImVec2(position.x, position.y);
    //    ImGui::SetNextWindowPos(ImPosition);
    //    ImGui::SetNextWindowSize(text->getConstSize());

    //    // Establece los estilos de la ventana de fondo, sin borde, sin padding y transparente
    //    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    //    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
    //    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());

    //    ImGui::Begin(text->getName().c_str(), text->getCanCloseWindow(), text->getWindowFlags());

    //    ImGui::PopStyleVar(2);   // Pop para WindowBorderSize y WindowPadding

    //    // Establece la fuente del texto
    //    ImGui::PushFont(text->getFont());
    //    // Establece el color del texto
    //    ImGui::PushStyleColor(ImGuiCol_Text, text->getTextColor());
    //    // Imprime el texto
    //    ImGui::Text(text->getText().c_str());

    //    // Pop para WindowBg, el color del texto
    //    ImGui::PopStyleColor(2);
    //    // Pop para la fuente del texto
    //    ImGui::PopFont();

    //    ImGui::End();
    //}
//}

//void UIManager::renderInputTexts() {
    //for (auto myInputText : inputTexts) {
    //    InputText* inputText = myInputText.second;
    //    ImVec2 inputTextSize(inputText->getConstWidth(), 0);

    //    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del inputText
    //    ImGui::SetNextWindowPos(inputText->getPosition());
    //    ImGui::SetNextWindowSize(inputTextSize);

    //    // Establece los estilos de la ventana de fondo, sin borde, sin padding y transparente
    //    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    //    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
    //    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());

    //    ImGui::Begin(inputText->getName().c_str(), inputText->getCanCloseWindow(), inputText->getWindowFlags());

    //    ImGui::PopStyleVar(2);   // Pop para WindowBorderSize y WindowPadding

    //    // Establece la fuente del texto
    //    ImGui::PushFont(inputText->getFont());
    //    // Establece el color del texto
    //    ImGui::PushStyleColor(ImGuiCol_Text, inputText->getTextColor());
    //    // Establece el color de la caja de texto
    //    ImGui::PushStyleColor(ImGuiCol_FrameBg, inputText->getBgColor());
    //    // Establece el ancho de envoltura para el texto del inputText
    //    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + inputTextSize.x);

    //    if (ImGui::InputText("##", inputText->getBuffer(), inputText->getBufferSize(), inputText->getFlags(),
    //                         inputText->getCallback(), inputText->getUserData())) {
    //        if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
    //            inputText->getOnTextEntered()();
    //        }
    //    }

    //    // Pop para el ancho de envoltura
    //    ImGui::PopTextWrapPos();
    //    // Pop para WindowBg, el color del texto y el color de fondo
    //    ImGui::PopStyleColor(3);
    //    // Pop para la fuente del texto
    //    ImGui::PopFont();

    //    ImGui::End();
    //}
//}

bool UIManager::handleEvents(const SDL_Event& event) {
    /*    if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            
        }
    }  */
    if (ImGui_ImplSDL2_ProcessEvent(&event) &&
        (ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive() || ImGui::IsAnyItemFocused())) {
        //#ifdef _DEBUG
        //        std::cout << "Interactuando con UI\n";
        //#endif
        return true;
    }
    return false;
}

bool UIManager::fontsFolderExists() {
    if (!std::filesystem::exists(fontsPath)) {
#ifdef _DEBUG
        std::cout << "La carpeta " << fontsPath << " no existe\n";
#endif
        return false;
    }
    return true;
}

void UIManager::loadFonts(float pixelSize) {

    if (!fontsFolderExists()) {
#ifdef _DEBUG
        std::cout << "No se han cargado las fuentes\n";
#endif
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(fontsPath)) {
        if (entry.is_regular_file()) {
            auto path = entry.path();
            std::string extension = path.extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            if (extension == ".ttf" || extension == ".otf") {
                loadFont(path.stem().string() + extension, pixelSize);
            }
        }
    }
}

void UIManager::loadFont(const std::string& name, float pixelSize) {
    std::string path = fontsPath + name;
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(path.c_str(), pixelSize);
    fonts.insert({{name, pixelSize}, font});
#ifdef _DEBUG
    std::cout << "Fuente " << name << " cargada correctamente\n";
#endif
}

ImFont* UIManager::getFont(const std::string& name, float pixelSize) {
    // Si el tamano de la fuente es la que hay por defecto y ya se ha cargado, se devuelve
    if (pixelSize == fontDefaultSize && fonts.contains({name, pixelSize})) return fonts[{name, pixelSize}];
    // Si el tamano de la fuente es la que hay por defecto y no se ha cargado, se devuelve nullptr
    else if (pixelSize == fontDefaultSize && !fonts.contains({name, pixelSize})) {
#ifdef _DEBUG
        std::cout << "No existe la fuente con el nombre " << name << " con el tamano predeterminado de "
                  << fontDefaultSize << '\n';
        std::cout << "Se usara la fuente por defecto\n";
#endif
        return ImGui::GetIO().FontDefault;
    }
    // Si no es el tamano de fuente por defecto, se intenta cargar
    else {
        // Se comprueba si existe la carpeta de fuentes
        if (!fontsFolderExists()) {
#ifdef _DEBUG
            std::cout << "Se usara la fuente por defecto\n";
#endif
            return ImGui::GetIO().FontDefault;
        }

        // Se intenta cargar la fuente
        loadFont(name, pixelSize);
        return fonts[{name, pixelSize}];
    }
}

Button* UIManager::getButton(const std::string& name) {
    if (buttons.contains(name)) return buttons[name];
    else
        return nullptr;
}

bool UIManager::deleteButton(const std::string& name) {
    if (buttons.contains(name)) {
        delete buttons[name];
        buttons.erase(name);
        return true;
    }
    else {
#ifdef _DEBUG
        std::cout << "No existe el boton con el nombre " << name << '\n';
#endif
        return false;
    }
}

//InputText* UIManager::createInputText(const InputText::InputTextOptions& options) {
//    return createInputText(options.name, options.position, options.placeHolderText, options.bufferSize,
//                           options.onTextEntered, options.constWidth, options.textFont, options.textColor,
//                           options.bgColor, options.flags, options.callback, options.userData, options.canCloseWindow,
//                           options.windowFlags);
}

//InputText* UIManager::createInputText(const std::string& name, const ImVec2& position,
//                                      const std::string& placeHolderText, const size_t bufferSize,
//                                      std::function<void()> onTextEntered, const float constWidth,
//                                      ImFont* const textFont, const ImVec4& textColor, const ImVec4& bgColor,
//                                      const ImGuiInputTextFlags& flags, const ImGuiInputTextCallback& callback,
//                                      void* userData, bool* canCloseWindow, ImGuiWindowFlags windowFlags) {
//
//    if (!inputTexts.contains(name)) {
//        InputText* inputText =
//            new InputText(name, position, placeHolderText, bufferSize, onTextEntered, constWidth, textFont, textColor,
//                          bgColor, flags, callback, userData, canCloseWindow, windowFlags);
//        inputTexts.insert({name, inputText});
//        return inputText;
//    }
//    else {
//
//#ifdef _DEBUG
//        std::cout << "Ya existe la caja de texto con el nombre " << name << '\n';
//#endif
//        return nullptr;
//    }
//}
//
//InputText* UIManager::getInputText(const std::string& name) {
//    if (inputTexts.contains(name)) return inputTexts[name];
//    else
//        return nullptr;
//}
//
//bool UIManager::deleteInputText(const std::string& name) {
//    if (inputTexts.contains(name)) {
//        delete inputTexts[name];
//        inputTexts.erase(name);
//        return true;
//    }
//    else {
//#ifdef _DEBUG
//        std::cout << "No existe la caja de texto con el nombre " << name << '\n';
//#endif
//        return false;
//    }
//}
//
//Text* UIManager::createText(const Text::TextOptions& options) {
//    return createText(options.name, options.position, options.text, options.constSize, options.textFont,
//                      options.textColor, options.canCloseWindow, options.windowFlags);
//}
//
//Text* UIManager::createText(const std::string& name, const ImVec2& position, const std::string& text,
//                            const ImVec2& constSize, ImFont* textFont, const ImVec4& textColor, bool* canCloseWindow,
//                            ImGuiWindowFlags windowFlags) {
//    if (!texts.contains(name)) {
//        Text* textObj = new Text(name, position, text, constSize, textFont, textColor, canCloseWindow, windowFlags);
//        texts.insert({name, textObj});
//        return textObj;
//    }
//    else {
//#ifdef _DEBUG
//        std::cout << "Ya existe el texto con el nombre " << name << '\n';
//#endif
//        return nullptr;
//    }
//}
//
//Text* UIManager::getText(const std::string& name) {
//    if (texts.contains(name)) return texts[name];
//    else
//        return nullptr;
//}
//
//bool UIManager::deleteText(const std::string& name) {
//    if (texts.contains(name)) {
//        delete texts[name];
//        texts.erase(name);
//        return true;
//    }
//    else {
//#ifdef _DEBUG
//        std::cout << "No existe el texto con el nombre " << name << '\n';
//#endif
//        return false;
//    }
//}
