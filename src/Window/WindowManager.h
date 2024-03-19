#pragma once
#include <Utilities/Singleton.h>
#include <Structure/Module.h>

struct SDL_Window;

namespace Tapioca {
class TAPIOCA_API WindowManager : public Singleton<WindowManager>, public Module {
    friend Singleton<WindowManager>;

private:
    // warning C4251 'Tapioca::WindowManager::windowName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::WindowManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string windowName;              // Nombre de la ventana
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    uint32_t windowWidth, windowHeight;   // Anchura y altura de la ventana, respectivamente
    SDL_Window* sdlWindow;                // Ventana de SDL

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

    inline std::string const& getWindowName() { return windowName; } 
    inline SDL_Window* getWindow() { return sdlWindow; }
    inline uint32_t getWindowW() { return windowWidth; }
    inline uint32_t getWindowH() { return windowHeight; }
};

/*
* @brief Para acortar el metodo WindowManager::instance()->method() a windowManager().method()
*/ 
inline WindowManager& windowManager() { return *WindowManager::instance(); }
}