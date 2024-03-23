#pragma once
#include <imgui.h>
#include <functional>
#include "Utilities/Vector2.h"
#include "Utilities/Singleton.h"
#include "WindowModule.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <OgreRenderTargetListener.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

#include "Button.h"

struct SDL_Window;

namespace Ogre {
class RenderWindow;
class SceneManager;
}

namespace Tapioca {
class RenderNode;

/* 
* @brief Funcion hash para pares para poder usarlos como clave en unordered_map
* (https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key)
*/
struct pair_hash {
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1> {}(pair.first);
        auto hash2 = std::hash<T2> {}(pair.second);
        return hash1 ^ hash2;
    }
};
/*
* @brief Clase que se encarga de la interfaz de usuario
*/
class TAPIOCA_API UIManager : public Singleton<UIManager>, public WindowModule, public Ogre::RenderTargetListener {
private:
    friend Singleton<UIManager>;

    SDL_Window* sdlWindow;                            // Referencia a la ventana de SDL
    void* glContext;                                  // Referencia al contexto de OpenGL
    Ogre::SceneManager* sceneManager;                 // Referencia al SceneManager de Ogre
    std::string fontsPath;                            // Ruta de la carpeta de fuentes
    static constexpr float fontDefaultSize = 16.0f;   // Tamano por defecto de las fuentes

    // TEMPORAL?
    std::unordered_map<std::string, Button*> buttons;                              // Botones de la interfaz de usuario
    std::unordered_map<std::pair<std::string, float>, ImFont*, pair_hash> fonts;   // Fuentes de la interfaz de usuario

    /*
    * @brief Inicializa a nulo los punteros
    */
    UIManager();

public:
    /*
    * @brief Termina ImGui con SDL y OpenGL y destruye el contexto
    */
    ~UIManager();

    UIManager(UIManager&) = delete;
    UIManager(UIManager&&) = delete;
    UIManager& operator=(UIManager&) = delete;
    UIManager& operator=(UIManager&&) = delete;

    /*
    * @brief Se llama cuando Ogre termine de renderizar en el viewport al que hemos asignado el UIManager como listener
    */
    void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) override;

    /*
    * @brief Inicializa ImGui con SDL y OpenGL, y anade el UIManager como listener de la ventana de Ogre
    */
    bool init() override;

    /*
    * @brief Renderiza la interfaz de usuario
    */
    void render() override;

    /*
    * @brief Maneja los eventos de SDL
    */
    bool handleEvents(const SDL_Event& event) override;

    /*
    * @brief Devuelve si existe o no la carpeta de fuentes
    */
    bool fontsFolderExists();

    /*
    * @brief Carga todas las fuentes de letra de la carpeta de fuentes
    * @param pixelSize Tamano de la fuente, por defecto 16.0f
    */
    bool loadFonts(float pixelSize = fontDefaultSize);

    /*
    * @brief Carga una fuente de la carpeta de fuentes
    * @param name Nombre de la fuente con extension
    * @param pixelSize Tamano de la fuente, por defecto 16.0f
    */
    void loadFont(const std::string& name, float pixelSize = fontDefaultSize);

    /*
    * @brief Devuelve la fuente a partir de un nombre, si no existe la intenta cargar
    * @param name Nombre de la fuente con extension
    * @param pixelSize Tamano de la fuente, por defecto 16.0f
    * @return Puntero a la fuente solicitada, nullptr si no se ha podido cargar
    */
    ImFont* getFont(const std::string& name, float pixelSize = fontDefaultSize);

    /*
    * @brief Crea un boton
    * @param name Nombre del boton
    * @param options Opciones del boton
    */
    Button* createButton(const std::string& name, const Button::ButtonOptions& options);

    /*
    * @brief Crea un boton
    * @param node Nodo de renderizado al que se asocia el boton
    * @param position Posicion del boton
    * @param text Texto del boton
    * @param onClick Funcion que se llama cuando se pulsa el boton
    * @param constSize Tamano constante del boton, si no se especifica (-1, -1) se calcula a partir del texto
    * @param padding Padding del boton, solo se aplica si constSize es (-1, -1)
    * @param textFont Fuente del texto del boton
    * @param textColor Color del texto del boton
    * @param normalColor Color del boton para el estado "normal"
    * @param hoverColor Color del boton para el estado "hover"
    * @param activeColor Color del boton para el estado "active"
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param flags Flags de la ventana de ImGui
    * @return Puntero al boton creado
	*/
    Button* createButton(const std::string& name, RenderNode* const node, const ImVec2& position,
                         const std::string& text, std::function<void()> onClick, const ImVec2& constSize,
                         const ImVec2& padding, ImFont* const textFont = ImGui::GetIO().FontDefault,
                         const ImVec4& textColor = ImGui::GetStyle().Colors[ImGuiCol_Text],
                         const ImVec4& normalColor = ImGui::GetStyle().Colors[ImGuiCol_Button],
                         const ImVec4& hoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered],
                         const ImVec4& activeColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive],
                         bool* canCloseWindow = nullptr,
                         ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);

    /*
    * @brief Crea un boton
    * @param node Nodo de renderizado al que se asocia el boton
    * @param position Posicion del boton
    * @param text Texto del boton
    * @param onClick Funcion que se llama cuando se pulsa el boton
    * @param constSize Tamano constante del boton, si es (-1, -1) se calcula a partir del texto con padding predeterminado
    * @param textFont Fuente del texto del boton
    * @param textColor Color del texto del boton
    * @param normalColor Color del boton para el estado "normal"
    * @param hoverColor Color del boton para el estado "hover"
    * @param activeColor Color del boton para el estado "active"
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param flags Flags de la ventana de ImGui
    * @return Puntero al boton creado
	*/
    Button* createButton(const std::string& name, RenderNode* const node, const ImVec2& position,
                         const std::string& text, std::function<void()> onClick, const ImVec2& constSize,
                         ImFont* const textFont = ImGui::GetIO().FontDefault,
                         const ImVec4& textColor = ImGui::GetStyle().Colors[ImGuiCol_Text],
                         const ImVec4& normalColor = ImGui::GetStyle().Colors[ImGuiCol_Button],
                         const ImVec4& hoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered],
                         const ImVec4& activeColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive],
                         bool* canCloseWindow = nullptr,
                         ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);

    /*
    * @brief Devuelve el boton a partir de un nombre
    * @param name Nombre del boton
    * @return Puntero al boton solicitado
    */
    Button* getButton(const std::string& name);

    /*
    * @brief Elimina un boton
    * @param name Nombre del boton
    */
    bool deleteButton(const std::string& name);
};
}