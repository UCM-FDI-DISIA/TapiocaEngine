#pragma once

#include <Utilities/Singleton.h>

#include <vector>
#include <string>
#include <unordered_map>

union SDL_Event;
struct _SDL_GameController;
typedef _SDL_GameController SDL_GameController;

namespace Tapioca { 
	enum EVENTS : uint8_t { 
        ev_ACCEPT = 0, 
        ev_TOGGLE_TEXT_INPUT,
        ev_REMOVE_CHAR_INPUT,
    };

    class InputManager : public Singleton<InputManager> {
            friend Singleton<InputManager>;

        private:
            enum MOUSE_BUTTONS : uint8_t { m_left = 0, m_middle, m_right };
            enum STICKS : uint8_t { s_leftX = 0, s_leftY, s_rightX, s_rightY };

            // Tipos de eventos (de input, no los mapeados)
            enum INPUT_EVENTS : uint8_t {
                ie_closeWindow = 0,

                ie_keyUp,
                ie_keyDown,

                ie_mouseMoving,
                ie_mouseButtonUp,
                ie_mouseButtonDown,

                ie_ctrlAxisMotion,
                ie_ctrlButtonUp,
                ie_ctrlButtonDown,

                ie_lastEventType
            };
            std::vector<SDL_Event> inputEventTriggered[ie_lastEventType];

            // Estados del ratón y teclado
            std::pair<int32_t, int32_t> mousePos;

            // Mandos
            const int DEFAULT_DEADZONE = 8000;
            std::unordered_map<int, int> deadZones;
            std::unordered_map<int, SDL_GameController*> controllers;

            // Input de texo
            std::string inputText;
            const char* compositionText;
            int32_t cursor;
            int32_t selectionLen;

            // Mapeo de los controles
            std::unordered_map<EVENTS, std::vector<std::pair<INPUT_EVENTS, int>>> inputMap;
            // faltan eventos de mouse y sticks de mando

            
            InputManager();

            void initControllers();
            void addController(int i);
            void removeController(int i);
            void clearInput();

            //void joyAxisMotionEvent(const SDL_Event& event);
            
            void mapInput();


        public:
            virtual ~InputManager();

            void updateState(const SDL_Event& event);
            void update();

            bool windowClosed();
            bool eventHappened(EVENTS ev);

            // Devuelve la posición del ratón
            inline const std::pair<int32_t, int32_t>& getMousePos() { return mousePos; }

            // Devuelve true si hay al menos 1 mando conectado
            inline bool isControllerConnected() { return !controllers.empty(); }

            // Devuelve el texto introducido
            inline std::string getInputText() { return inputText; }
            // Limpia el texto introducido
            inline void resetText() { inputText.clear(); }
            // Borra el último char del texto introducido
            inline void removeLastChar() { if (!inputText.empty()) inputText.pop_back(); }

            

    };
        
    // Para acortar el método InputManager::instance()->method() a inputManager().method()
    inline InputManager& inputManager() { return *InputManager::instance(); }


}

