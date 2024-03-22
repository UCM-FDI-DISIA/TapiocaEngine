#pragma once
#include <imgui.h>
#include <functional>
#include "Utilities/Vector2.h"
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <OgreRenderTargetListener.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

struct SDL_Window;

namespace Ogre {
class RenderWindow;
class SceneManager;
}

namespace Tapioca {
class RenderNode;
class Button;

/*
* @brief Clase que se encarga de la interfaz de usuario
*/
class TAPIOCA_API UIManager : public Singleton<UIManager>, public Module, public Ogre::RenderTargetListener {
private:
    friend Singleton<UIManager>;

    SDL_Window* mySDLWindow;              // Referencia a la ventana de SDL
    Ogre::RenderWindow* myOgreWindow;     // Referencia a la ventana de Ogre
    void* myGLContext;                    // Referencia al contexto de OpenGL
    Ogre::SceneManager* mySceneManager;   // Referencia al SceneManager de Ogre

    // TEMPORAL?
    std::unordered_map<std::string, Button*> buttons;   // Botones de la interfaz de usuario

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
    * @brief Carga todas las fuentes de letra de la carpeta de fuentes
    * @param io Estructura de ImGui
    * @return True si se han cargado todas las fuentes, false en caso contrario
    */
    bool loadFonts(ImGuiIO& io, float pixelSize);

    /*
    * @brief Crea un boton
    * @param node Nodo de renderizado al que se asocia el boton
    * @param position Posicion del boton
    * @param text Texto del boton
    * @param onClick Funcion que se llama cuando se pulsa el boton
    * @param constSize Tamano constante del boton, si no se especifica (-1, -1) se calcula a partir del texto
    * @param padding Padding del boton
    * @param normalColor Color del boton para el estado "normal"
    * @param hoverColor Color del boton para el estado "hover"
    * @param activeColor Color del boton para el estado "active"
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param flags Flags de la ventana de ImGui
    * @return Puntero al boton creado
	*/
    Button* createButton(const std::string& name, RenderNode* const node, const ImVec2& position,
                         const std::string& text, std::function<void()> onClick,
                         const ImVec2& constSize = ImVec2(-1, -1), const ImVec2& padding = ImVec2(10, 5),
                         const ImVec4& normalColor = ImGui::GetStyle().Colors[ImGuiCol_Button],
                         const ImVec4& hoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered],
                         const ImVec4& activeColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive],
                         bool* canCloseWindow = nullptr,
                         ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);
    Button* createButton(const std::string& name, RenderNode* const node, const ImVec2& position,
                         const std::string& text, std::function<void()> onClick,
                         const ImVec2& constSize,
                         const ImVec4& normalColor = ImGui::GetStyle().Colors[ImGuiCol_Button],
                         const ImVec4& hoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered],
                         const ImVec4& activeColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive],
                         bool* canCloseWindow = nullptr,
                         ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);
    Button* createButton(const std::string& name, RenderNode* const node, const ImVec2& position,
                         const std::string& text, std::function<void()> onClick,
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