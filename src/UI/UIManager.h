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
    * @brief Crea un boton
    * @param node Nodo de renderizado al que se asocia el boton
    * @param position Posicion del boton
    * @param text Texto del boton
    * @param onClick Funcion que se llama cuando se pulsa el boton
    * @param size Tamano del boton
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param flags Flags de la ventana de ImGui
    * @return Puntero al boton creado
	*/
    Button* createButton(const std::string& name, RenderNode* const node, const ImVec2 position,
                         const std::string& text, std::function<void()> onClick, const ImVec2 size = ImVec2(130, 40),
                         bool* canCloseWindow = nullptr,
                         ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

    /*
    * @brief Devuelve el boton a partir de un nombre
    * @param name Nombre del boton
    * @return Puntero al boton solicitado
    */
    Button* getButton(const std::string& name);
};
}