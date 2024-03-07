#pragma once
#include <Utilities/Singleton.h>
#include <Structure/Module.h>
#include <vector>
#include <string>
#include <unordered_map>

class SDL_Window;
union SDL_Event;
struct _SDL_GameController;
typedef _SDL_GameController SDL_GameController;

namespace Tapioca {
class TAPIOCA_API InputManager : public Singleton<InputManager>, public Module {
private:
    friend Singleton<InputManager>;

    // Posicion del raton
    std::pair<int32_t, int32_t> mousePos;

    // Mandos
    const int DEFAULT_DEADZONE = 1000;
    std::unordered_map<int, int> deadZones;
    std::unordered_map<int, SDL_GameController*> controllers;

    // Input de texo
    std::string inputText;
    const char* compositionText;
    int32_t cursor;
    int32_t selectionLen;
    bool removeChar;
    bool toggleTextInput;

    // Mapeo de los controles
    // (se agrupan por tipo de evento de SDL que los produce. Luego, segun el evento, se agrupan
    // por valor de la tecla/boton/etc., y ya cada uno guarda un vector con todos los eventos que envia)
    std::unordered_map<std::string, std::unordered_map<int, std::vector<std::string>>> inputMap;
    const int MOUSE_MOTION_VALUE = -2;
    const int MOUSE_WHEEL_VALUE = -3;


    InputManager();
    
    /*
    * @brief Comprueba si hay algun joystick conectado y si hay al menos uno, empieza a escuchar eventos de joystick
    */
    void initControllers();

    /*
    * @brief Añade el mando con su deadzone por defecto al mapa de mandos
    */
    void addController(const int i);

    /*
    * @brief Elimina el mando y su deadzone del mapa de mandos
    */
    void removeController(const int i);
  
    /*
    * @brief Envia el evento propio si alguno de los eventos mapeados coincide con el producido
    * @param evtent El nombre del evento de input
    * @param eventInfo Informacion del evento producido por SDL
    */
    void sendEvent(std::string const& eventName, SDL_Event const& event, int const& value);

public:
    bool init();
    virtual ~InputManager();

    void updateState(const SDL_Event& event);
    void sendEvents() override;

    /*
    * @brief Asociar eventos propios a eventos de input
    * @param evt El nombre del evento propio
    * @param src El nombre del evento de input
    * @param ctrl El valor del botón/tecla/etc. en los enums de SDL
    */
    void mapInput(std::string const& evt, std::string const& src, int const& ctrl);

    /*
    * @brief Devuelve la posicion del raton
    */
    inline const std::pair<int32_t, int32_t>& getMousePos() { return mousePos; }

    /*
    * @brief Devuelve true si hay al menos 1 mando conectado
    */
    inline bool isControllerConnected() { return !controllers.empty(); }

    /*
    * @brief Devuelve el texto introducido
    */
    inline std::string getInputText() { return inputText; }
    
    /*
    * @brief Limpia el texto introducido
    */
    inline void resetText() { inputText.clear(); }

    
    /*
    * @brief Borra el ultimo char del texto introducido
    */
    inline void removeLastChar() { if (!inputText.empty()) inputText.pop_back(); }

};

/*
* @brief Para acortar el metodo InputManager::instance()->method() a inputManager().method()
*/ 
inline InputManager& inputManager() { return *InputManager::instance(); }
}