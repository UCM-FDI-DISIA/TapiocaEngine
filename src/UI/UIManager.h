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

//#include "Components/Text.h"
#include "Components/Button.h"
//#include "Components/InputText.h"

struct SDL_Window;

namespace Ogre {
class RenderWindow;
class SceneManager;
}

namespace Tapioca {
class Game;
class RenderNode;
class WindowManager;

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

    Game* game;                                       // Referencia al Game
    WindowManager* windowManager;                     // Referencia al WindowManager
    SDL_Window* sdlWindow;                            // Referencia a la ventana de SDL
    void* glContext;                                  // Referencia al contexto de OpenGL
    Ogre::SceneManager* sceneManager;                 // Referencia al SceneManager de Ogre
    std::string fontsPath;                            // Ruta de la carpeta de fuentes
    static constexpr float fontDefaultSize = 16.0f;   // Tamano por defecto de las fuentes

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
    * @return true si se ha inicializado correctamente, false si no
    */
    bool init() override;
    /*
    * @brief Renderiza la interfaz de usuario
    */
    void render() override;
    /*
    * @brief Maneja los eventos de SDL
    * @param event Evento de SDL
    * @return true si se ha manejado el evento, false si no
    */
    bool handleEvents(const SDL_Event& event) override;

    /*
    * @brief Devuelve si existe o no la carpeta de fuentes
    * @return true si existe, false si no
    */
    bool fontsFolderExists();

    /*
    * @brief Carga todas las fuentes de letra de la carpeta de fuentes
    * @param pixelSize Tamano de la fuente, por defecto 16.0f
    */
    void loadFonts(float pixelSize = fontDefaultSize);

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
    * @return Puntero a la fuente solicitada, defaultFont si no se ha podido cargar
    */
    ImFont* getFont(const std::string& name, float pixelSize = fontDefaultSize);
};
}