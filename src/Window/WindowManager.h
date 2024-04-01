#pragma once
#include <Utilities/Singleton.h>
#include "WindowModule.h"

#include <vector>
struct SDL_Window;

namespace Tapioca {
class Game;

class TAPIOCA_API WindowManager : public Singleton<WindowManager>, public Module {
    friend Singleton<WindowManager>;

private:
    
    // C4251 'Tapioca::WindowManager::windowName'
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita tener una interfaz DLL
    // para que la utilicen los clientes de class 'Tapioca::WindowManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string windowName;                 // Nombre de la ventana
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    uint32_t windowWidth, windowHeight;     // Anchura y altura de la ventana, respectivamente
    SDL_Window* sdlWindow;                  // Ventana de SDL
    void* glContext;                        // Contexto de OpenGL


    // warning C4251 'Tapioca::WindowManager::modules' :
    // class 'std::vector<Tapioca::WindowModule *,std::allocator<Tapioca::WindowModule *>>' necesita tener una
    // interfaz DLL para que la utilicen los clientes de class 'Tapioca::WindowManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::vector<WindowModule*> modules;     // Modulos suscritos a los eventos de ventana
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif

    Game* game;
    
    /*
    * @brief Constructora de la clase WindowManager.
    */
    WindowManager(std::string const& windowName = "TapiocaEngine", const uint32_t w = 680, const uint32_t h = 480);
    
public:
    /*
    * @brief Inicializa el WindowManager
    */
    bool init() override;
    /*
    * @brief Destructora de la clase WindowManager
    */
    virtual ~WindowManager();

    void setWindowName(std::string const& name);

    inline void setGLContext(void* context) { glContext = context; }
    /*
    * @brief Devuelve un puntero al contexto de OpenGL
    */
    inline void* getGLContext() { return glContext; }


    /*
    * @brief Devuelve el nombre de la ventana de SDL
    * @return String con el nombre de la ventana de SDL
    */
    inline std::string const& getWindowName() { return windowName; } 

    /*
    * @brief Devuelve un puntero a la ventana de SDL
    * @return Puntero a la ventana de SDL
    */
    inline SDL_Window* getWindow() { return sdlWindow; }

    /*
    * @brief Devuelve el ancho de ventana de SDL
    * @return ancho de la ventana de SDL
    */
    inline uint32_t getWindowW() { return windowWidth; }

    /*
    * @brief Devuelve el alto de ventana de SDL
    * @return alto de la ventana de SDL
    */
    inline uint32_t getWindowH() { return windowHeight; }

    /*
    * @brief Agrega un modulo al vector de modulos
    * @param mod Modulo a agregar en el vector
    */
    void subscribeModule(WindowModule* mod);

    /*
    * @brief Envia el evento recibido a Game
    * @param id Nombre del evento
    * @param info Informacion sobre el evento
    */
    void sendEvent(std::string const& id, void* info);

    void update(const uint64_t deltaTime) override;
};

/*
* @brief Para acortar el metodo WindowManager::instance()->method() a windowManager().method()
*/ 
inline WindowManager& windowManager() { return *WindowManager::instance(); }
}