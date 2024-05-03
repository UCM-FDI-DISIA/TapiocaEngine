#pragma once
#include "Utilities/Singleton.h"
#include "WindowModule.h"
#include <string>
#include <unordered_map>
#include <set>

struct SDL_Window;

struct ImFont;
typedef void* ImTextureID;

namespace Ogre {
class RenderWindow;
}

namespace Tapioca {
class MainLoop;
class WindowManager;
class GraphicsManager;
class RenderListener;

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
class TAPIOCA_API UIManager : public Singleton<UIManager>, public WindowModule {
private:
    friend Singleton<UIManager>;

    MainLoop* mainLoop;                 // Referencia al MainLoop
    WindowManager* windowManager;       // Referencia al WindowManager
    GraphicsManager* graphicsManager;   // Referencia al GraphicsManager
    SDL_Window* sdlWindow;              // Referencia a la ventana de SDL
    void* glContext;                    // Referencia al contexto de OpenGL
    Ogre::RenderWindow* ogreWindow;     // Referencia a la superficie de renderizado de Ogre
    RenderListener* renderListener;     // Listener de renderizado de Ogre

    // 'Tapioca::UIManager::fontsPath' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::UIManager'
    // 'Tapioca::UIManager::texturesPath' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::UIManager'
    // 'Tapioca::UIManager::fonts' :
    // class 'std::unordered_map<std::pair<std::string,float>,ImFont *,Tapioca::pair_hash,std::equal_to<std::pair<std::string,float>>,std::allocator<std::pair<const std::pair<std::string,float>,ImFont *>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::UIManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string fontsPath;                                                         // Ruta de la carpeta de fuentes
    std::string texturesPath;                                                      // Ruta de la carpeta de imagenes
    std::unordered_map<std::pair<std::string, float>, ImFont*, pair_hash> fonts;   // Fuentes de la interfaz de usuario
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    static constexpr float fontDefaultSize = 16.0f;   // Tamano por defecto de las fuentes

    std::set<std::string> widgetNames;   // Nombres de los widgets
    long long int widgetCounter;         // Contador de widgets

    /*
    * @brief Genera un nombre para un widget
    * @param widget Nombre del widget
    * @return Nombre generado
    */
    std::string generateName(const std::string& widget);

    /*
    * @brief Comprueba si existe la carpeta de fuentes y la carpeta de imagenes.
    * Si no existen, las crea
    */
    void check();

    /*
    * @brief Inicializa a nulo los punteros
    */
    UIManager();

public:
    UIManager(UIManager&) = delete;
    UIManager(UIManager&&) = delete;
    UIManager& operator=(UIManager&) = delete;
    UIManager& operator=(UIManager&&) = delete;

    /*
    * @brief Termina ImGui con SDL y OpenGL y destruye el contexto
    */
    ~UIManager();

    /*
    * @brief Inicializa el puntero al MainLoop
    */
    bool init() override;
    /*
    * @brief Inicializa ImGui con SDL y OpenGL, y anade el UIManager como listener de la ventana de Ogre
    * @return true si se ha inicializado correctamente, false si no
    */
    bool initConfig() override;
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
    * @brief Devuelve si existe o no la carpeta de imagenes
    * @return true si existe, false si no
    */
    bool texturesFolderExists();

    /*
    * @brief Devuelve si esta vacia o no la carpeta de fuentes
    * @return true si esta vacia, false si no
    */
    bool fontsFolderEmpty();

    /*
    * @brief Carga una fuente de la carpeta de fuentes
    * @param name Nombre de la fuente con extension
    * @param pixelSize Tamano de la fuente, por defecto 16.0f
    * @return Puntero a la fuente cargada, nullptr si no se ha podido cargar
    */
    ImFont* loadFont(const std::string& name, float pixelSize = fontDefaultSize);

    /*
    * @brief Devuelve la fuente a partir de un nombre, si no existe la intenta cargar
    * @param name Nombre de la fuente con extension
    * @param pixelSize Tamano de la fuente, por defecto 16.0f
    * @return Puntero a la fuente solicitada, defaultFont si no se ha podido cargar
    */
    ImFont* getFont(const std::string& name, float pixelSize = fontDefaultSize);

    /*
    * @brief Devuelve el identificador de la textura de una imagen
    */
    ImTextureID getTextureId(const std::string& name);

    /*
    * @brief Anade un nombre de widget a la lista
    * @param name Nombre del widget
    * @return Nombre del widget anadido
    */
    void addWidgetName(std::string& name);

    /*
    * @brief Elimina un nombre de widget de la lista
    * @param name Nombre del widget
    */
    void removeWidgetName(const std::string& name);

    /*
    * @brief Devuelve si un nombre de widget esta en la lista
    * @param name Nombre del widget
    * @return true si esta, false si no
    */
    bool widgetNameExists(const std::string& name);
};
}