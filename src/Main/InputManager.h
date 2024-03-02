#pragma once
#include <Utilities/Singleton.h>
#include <Structure/Module.h>

#include <vector>
#include <string>
#include <unordered_map>

union SDL_Event;
struct _SDL_GameController;
typedef _SDL_GameController SDL_GameController;
struct lua_State;

namespace Tapioca {
class InputManager : public Singleton<InputManager>, public Module {
    friend Singleton<InputManager>;

private:
    // Tipos de eventos (de input, no los mapeados)
    std::unordered_map<std::string, std::vector<SDL_Event>> inputEventTriggered;

    // Posición del ratón
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

    // Mapeo de los controles
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> inputMap;
    const std::string MAP_FILE_PATH = "\\assets\\controlsMapping.lua";
    lua_State* luaState;


    InputManager();
    
    // @brief Mapear el input a diferentes eventos
    void mapInput();

    // @brief Comprueba si hay algún joystick conectado y si hay al menos uno, empieza a escuchar eventos de joystick
    void initControllers();
    // @brief Añade el mando con su deadzone por defecto al mapa de mandos
    void addController(int i);
    // @brief Elimina el mando y su deadzone del mapa de mandos
    void removeController(int i);

    // @brief Limpia el input
    void clearInput();
    

public:
    virtual ~InputManager();

    void updateState(const SDL_Event& event);
    void handleEvents();

    /*
    * @brief Comprueba si se ha producido el evento indicado
    * @param ev Identificador del evento
    */
    bool eventHappened(std::string ev);


    // @brief Devuelve la posición del ratón
    inline const std::pair<int32_t, int32_t>& getMousePos() { return mousePos; }

    // @brief Devuelve true si hay al menos 1 mando conectado
    inline bool isControllerConnected() { return !controllers.empty(); }

    // @brief Devuelve el texto introducido
    inline std::string getInputText() { return inputText; }
  
    // @brief Limpia el texto introducido
    inline void resetText() { inputText.clear(); }

    // @brief Borra el último char del texto introducido
    inline void removeLastChar() { if (!inputText.empty()) inputText.pop_back(); }

};

// @brief Para acortar el método InputManager::instance()->method() a inputManager().method()
inline InputManager& inputManager() { return *InputManager::instance(); }
}