#pragma once
#include <vector>
#include "Utilities/Singleton.h"
#include "WindowModule.h"

struct SDL_Window;

namespace Tapioca {
class MainLoop;

/**
* @brief Clase que se encarga de la ventana de SDL y de los eventos de ventana
*/
class TAPIOCA_API WindowManager : public Singleton<WindowManager>, public Module {
private:
    friend Singleton<WindowManager>;

    // warning C4251 'Tapioca::WindowManager::windowName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita tener una interfaz DLL
    // para que la utilicen los clientes de class 'Tapioca::WindowManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string windowName;   // Nombre de la ventana
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    uint32_t windowWidth, windowHeight;   // Anchura y altura de la ventana, respectivamente
    uint32_t firstWindowWidth,
        firstWindowHeight;   // Anchura y altura de la ventana antes de escalarse, respectivamente
    SDL_Window* sdlWindow;   // Ventana de SDL
    void* glContext;         // Contexto de OpenGL

    bool resized;   // Si se ha redimensionado

    // warning C4251 'Tapioca::WindowManager::modules' :
    // class 'std::vector<Tapioca::WindowModule *,std::allocator<Tapioca::WindowModule *>>' necesita tener una
    // interfaz DLL para que la utilicen los clientes de class 'Tapioca::WindowManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::vector<WindowModule*> modules;   // Modulos suscritos a los eventos de ventana
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif

    MainLoop* mainLoop;   // Puntero a MainLoop

    /**
    * @brief Intenta obtener el tamano de la ventana de SDL
    */
    void tryGetWindowSize();

    /**
    * @brief Constructor de la clase WindowManager
    */
    WindowManager(std::string const& windowName = "TapiocaEngine", const uint32_t w = 680, const uint32_t h = 480);

public:
    /**
    * @brief Destructor de la clase WindowManager
    */
    virtual ~WindowManager();

    /**
    * @brief Inicializa el WindowManager
    * @return Si se ha podido inicializar correctamente o no el WindowManager
    */
    bool init() override;
    /**
    * @brief Inicializa la configuracion del juego
    * @return Si se ha podido inicializar correctamente o no la configuracion
    */
    bool initConfig() override;
    /**
    * @brief Inicializa el puntero al MainLoop
    */
    void start() override;
    /**
    * @brief Maneja los eventos de la ventana
    * @param deltaTime Tiempo transcurrido desde el ultimo frame
    */
    void update(const uint64_t deltaTime) override;

    /**
    * @brief Agrega un modulo al vector de modulos
    * @param mod Modulo a agregar en el vector
    */
    void subscribeModule(WindowModule* mod);

    /**
    * @brief Envia el evento recibido a Game
    * @param id Nombre del evento
    * @param info Informacion sobre el evento
    */
    void sendEvent(std::string const& id, void* info);

    /**
    * @brief Establece el nombre de la ventana de SDL
    * @param name Nombre de la ventana de SDL
    */
    void setWindowName(std::string const& name);
    /**
    * @brief Devuelve el nombre de la ventana de SDL
    * @return String con el nombre de la ventana de SDL
    */
    inline std::string getWindowName() const { return windowName; }

    /**
    * @brief Establece el contexto de OpenGL
    * @param context Puntero al contexto de OpenGL
    */
    inline void setGLContext(void* context) { glContext = context; }
    /**
    * @brief Devuelve un puntero al contexto de OpenGL
    * @return Puntero al contexto de OpenGL
    */
    inline void* getGLContext() const { return glContext; }

    /**
    * @brief Devuelve un puntero a la ventana de SDL
    * @return Puntero a la ventana de SDL
    */
    inline SDL_Window* getWindow() const { return sdlWindow; }

    /**
    * @brief Devuelve el ancho de ventana de SDL
    * @return Ancho de la ventana de SDL
    */
    inline uint32_t getWindowW() const { return windowWidth; }

    /**
    * @brief Devuelve el alto de ventana de SDL
    * @return Alto de la ventana de SDL
    */
    inline uint32_t getWindowH() const { return windowHeight; }

    /**
    * @brief Establece el ancho de ventana antes de escalarse
    * @param w Ancho de la ventana antes de escalarse
    */
    inline void setFirstWindowW(uint32_t w) { firstWindowWidth = w; }

    /**
    * @brief Devuelve el ancho de ventana antes de escalarse
    * @return Ancho de la ventana antes de escalarse
    */
    inline uint32_t getFirstWindowW() const { return firstWindowWidth; }

    /**
    * @brief Devuelve el alto de la primera ventana
    * @return Alto de la primera ventana
    */
    inline uint32_t getFirstWindowH() const { return firstWindowHeight; }

    /**
    * @brief Devuelve si se ha redimensionado
    * @return true si se ha redimensionado, false si no
    */
    inline bool getResized() const { return resized; }
    /**
    * @brief Establece si se ha redimensionado
    * @param resized True si se ha redimensionado, false si no
    */
    inline void setResized(bool resized) { this->resized = resized; }
};

/**
* @brief Para acortar el metodo WindowManager::instance()->method() a windowManager().method()
*/
inline WindowManager& windowManager() { return *WindowManager::instance(); }
}
