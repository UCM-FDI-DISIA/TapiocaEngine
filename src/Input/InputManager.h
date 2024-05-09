#pragma once
#include <Utilities/Singleton.h>
#include <WindowModule.h>
#include <vector>
#include <string>
#include <unordered_map>

union SDL_Event;
struct _SDL_GameController;
typedef _SDL_GameController SDL_GameController;

namespace Tapioca {
/**
* @brief Clase que se encarga de gestionar los eventos de input de SDL y de mapearlos a eventos propios
*/
class TAPIOCA_API InputManager : public Singleton<InputManager>, public WindowModule {
private:
    friend Singleton<InputManager>;

    const int DEFAULT_DEADZONE = 1000;   // Deadzone por defecto de los mandos
    const int MOUSE_MOTION_VALUE = -2;   // Valor de los eventos de movimiento del raton
    const int MOUSE_WHEEL_VALUE = -3;    // Valor de los eventos de rueda del raton

    // warning C4251 'Tapioca::InputManager::mousePos' :
    // struct 'std::pair<int32_t,int32_t>' necesita tener una interfaz DLL para que la utilicen
    // los clientes de class 'Tapioca::InputManager'
    // warning C4251 'Tapioca::InputManager::deadZones' :
    // class 'std::unordered_map<int,int,std::hash<int>,std::equal_to<int>,std::allocator<std::pair<const int,int>>>'
    // necesita tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::InputManager'
    // warning C4251 'Tapioca::InputManager::controllers' :
    // class 'std::unordered_map<int,SDL_GameController *,std::hash<int>,std::equal_to<int>,
    // std::allocator<std::pair<const int,SDL_GameController *>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::InputManager'
    // warning C4251 'Tapioca::InputManager::inputMap' :
    // class 'std::unordered_map<std::string,std::unordered_map<int,std::vector<std::string,
    // std::allocator<std::string>>,std::hash<int>,std::equal_to<int>,std::allocator<std::pair<const int,
    // std::vector<std::string,std::allocator<std::string>>>>>,std::hash<std::string>,std::equal_to<std::string>,
    // std::allocator<std::pair<const std::string,std::unordered_map<int,std::vector<std::string,
    // std::allocator<std::string>>,std::hash<int>,std::equal_to<int>,std::allocator<std::pair<const int,
    // std::vector<std::string,std::allocator<std::string>>>>>>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::InputManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::pair<int32_t, int32_t> mousePos;   // Posicion del raton

    std::unordered_map<int, int> deadZones;                     // Deadzones de los mandos
    std::unordered_map<int, SDL_GameController*> controllers;   // Mandos

    // Mapeo de los controles
    // (se agrupan por tipo de evento de SDL que los produce. Luego, segun el evento, se agrupan
    // por valor de la tecla/boton/etc., y ya cada uno guarda un vector con todos los eventos que envia)
    std::unordered_map<std::string, std::unordered_map<int, std::vector<std::string>>> inputMap;
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif

    /**
    * @brief Constructor por defecto
    */
    InputManager();

    /**
    * @brief Comprueba si hay algun joystick conectado y si hay al menos uno, empieza a escuchar eventos de joystick
    */
    void initControllers();

    /**
    * @brief Anade el mando con su deadzone por defecto al mapa de mandos
    * @param i Indice del controller
    */
    void addController(const int i);

    /**
    * @brief Elimina el mando y su deadzone del mapa de mandos
    * @param i Indice del controller
    */
    void removeController(const int i);

    /**
    * @brief Envia el evento propio si alguno de los eventos mapeados coincide con el producido
    * @param eventName El nombre del evento de input
    * @param event Evento del input
    * @param eventInfo Informacion del evento producido por SDL
    */
    void sendEvent(std::string const& eventName, SDL_Event const& event, int const& value);

public:
    /**
    * @brief Inicializa el inputManager
    */
    bool init() override;
    /**
    * @brief Destructor por defecto
    */
    virtual ~InputManager();

    /**
    * @brief Maneja los eventos de SDL
    * @param event Evento de SDL
    * @return true si se ha manejado el evento, false si no
    */
    bool handleEvents(const SDL_Event& event) override;

    /**
    * @brief Asociar eventos propios a eventos de input
    * @param evt El nombre del evento propio
    * @param src El nombre del evento de input
    * @param ctrl El valor del boton/tecla/etc. en los enums de SDL
    */
    void mapInput(std::string const& evt, std::string const& src, int const& ctrl);

    /**
    * @brief Devuelve la posicion del raton
    */
    inline const std::pair<int32_t, int32_t>& getMousePos() { return mousePos; };

    /**
    * @brief Devuelve true si hay al menos 1 mando conectado
    */
    inline bool isControllerConnected() { return !controllers.empty(); }
};
/**
* @brief Para acortar el metodo InputManager::instance()->method() a inputManager().method()
*/
inline InputManager& inputManager() { return *InputManager::instance(); }
}
