#pragma once

#include <Utilities/Singleton.h>

#include <vector>
#include <string>
#include <map>
#include <unordered_map>

struct _SDL_Joystick;

typedef _SDL_Joystick SDL_Joystick;
union SDL_Event;
//typedef enum SDL_scancode;
//typedef enum SDL_GameControllerButton;
#include <SDL_scancode.h>
#include <SDL_gamecontroller.h>

namespace Tapioca { 
	class Vector2;

	static enum EVENTS : uint8_t { ev_ACCEPT = 0 };

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
                ie_mousebuttonDown,

                ie_joyAxisMotion,
                ie_joyButtonUp,
                ie_joyButtonDown,
                //e_joyDevice,

                ie_lastEventType
            };
            bool inputEventTriggered[ie_lastEventType];

            // Estados del ratón y teclado
            std::pair<int32_t, int32_t> mousePos;
            bool mbState[3];
            const uint8_t* kbState;

            // Joysticks
            bool joystickConnected;
            const int DEFAULT_DEADZONE = 8000;
            std::map<int, int> joystickDeadZones;
            std::map<int, SDL_Joystick*> joysticks;   // Todos los joysticks conectados
            std::map<int, std::pair<Tapioca::Vector2, Tapioca::Vector2>> joysticksValues;   // Valores de los ejes
            std::map<int, std::vector<bool>> jbState;                                       // Estados de los botones


            // Input de texo
            std::string inputText;
            const char* compositionText;
            int32_t cursor;
            int32_t selectionLen;

            InputManager();

            void initJoysticks();
            void addJoystick(int i);
            void removeJoystick(int i);
            void clearInput();

            /*
	        * Guarda si se ha activado algún evento de teclado, ratón o mando y su estado
	        * (el estado del teclado se guarda en kbState, que es un puntero al que se le
	        * asigna SDL_GetKeyboardState(0) en la constructora. SDL se encarga de actualizar
	        * el estado de teclado, y como kbState es un puntero, se actualiza al mismo tiempo).
	        */
            inline void onKeyDown() { inputEventTriggered[ie_keyDown] = true; }
            inline void onKeyUp() { inputEventTriggered[ie_keyUp] = true; }
            void onMouseMotion(const SDL_Event& event);
            void onMouseButtonChange(const SDL_Event& event, bool isDown);
            void joyAxisMotionEvent(const SDL_Event& event);
            void joyButtonDown(const SDL_Event& event);
            void joyButtonUp(const SDL_Event& event);


            std::unordered_multimap<EVENTS, SDL_Scancode> kbMapping;
            std::unordered_multimap<EVENTS, SDL_GameControllerButton> jMapping;

            // std::unordered_map<EVENTS, std::vector<SDL_Scancode>> kbMapping;
            // std::unordered_multimap<EVENTS, std::vector<SDL_GameControllerButton>> jMapping;
            // faltan eventos de mouse y sticks de mando
            void mapInput();


        public:
            virtual ~InputManager();

            void update(const SDL_Event& event);
            void refresh();


            bool eventHappened(EVENTS ev);

            inline bool windowClosed() { return inputEventTriggered[ie_closeWindow]; }

            // Devuelve la posición del ratón
            inline const std::pair<int32_t, int32_t>& getMousePos() { return mousePos; }

            // Devuelve el texto introducido
            inline std::string getInputText() { return inputText; }
            // Limpia el texto introducido
            inline void resetText() { inputText.clear(); }
            // Borra el último char del texto introducido
            inline void removeLastChar() {
                if (!inputText.empty()) inputText.pop_back();
            }


            // Devuelve true si hay al menos 1 joystick conectado
            inline bool isjoystickConnected() { return joystickConnected; }


            /*
	        // Returns true if the key press event has been triggered
	        inline bool keyDownEvent() { return inputEventTriggered[ie_keyDown]; }

	        // Returns true if the specified key has been pressed
	        inline bool isKeyDown(SDL_Scancode key) { return keyDownEvent() && kbState[key] == 1; }
	        inline bool isKeyDown(SDL_Keycode key) { return isKeyDown(SDL_GetScancodeFromKey(key)); }


	        // Returns true if the key release event has been triggered
	        inline bool keyUpEvent() { return inputEventTriggered[ie_keyUp]; }

	        // Returns true if the specified key has been released
	        inline bool isKeyUp(SDL_Scancode key) { return keyUpEvent() && kbState[key] == 0; }
	        inline bool isKeyUp(SDL_Keycode key) { return isKeyUp(SDL_GetScancodeFromKey(key)); }
	

	        // Reeturns true if the mouse moving event has been triggered
	        inline bool isMouseMoving() { return inputEventTriggered[ie_mouseMoving]; }
	        // Returns true if the mouse button press has been triggered
	        inline bool isMouseButtonDown() { return inputEventTriggered[ie_mousebuttonDown]; }
	        // Returns the mouse position
	        inline const std::pair<Sint32, Sint32>& getMousePos() { return mousePos; }
	        // Returns true if the specified mouse button has been pressed
                inline int isMouseButtonDown(MOUSE_BUTTONS b) { return mbState[b]; }

	        // Returns X value from the specified stick of the specified joystick  
	        inline int joystickXValue(int joystick, int stick) {
		        if (!joysticksValues.empty()) {
			        if (stick == 1) return joysticksValues[joystick].first.x;
			        else if (stick == 2) return joysticksValues[joystick].second.x;
			        else return 0;
		        }
	        }
	        // Returns Y value from the specified stick of the specified joystick  
	        inline int joystickYValue(int joystick, int stick) {
		        if (!joysticksValues.empty()) {
			        if (stick == 1) return joysticksValues[joystick].first.y;
			        else if (stick == 2) return joysticksValues[joystick].second.y;
			        else return 0;
		        }
	        }


	        // Returns true if the joystick button press event has been triggered
	        inline bool joystickButtonDownEvent() { return inputEventTriggered[ie_joyButtonDown]; }
	        // Returns true if the joystick button release event has been triggered
	        inline bool joystickButtonUpEvent() { return inputEventTriggered[ie_joyButtonUp]; }

	        // Returns true if the specified joystick specified button has been pressed
	        bool isJoystickButtonDown(int joystick, int button) { return jbState[joystick][button]; }
	        */
        };
        
    // Para acortar el método InputManager::instance()->method() a inputManager().method()
    inline InputManager& inputManager() { return *InputManager::instance(); }


}

