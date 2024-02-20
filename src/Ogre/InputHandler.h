//#pragma once
//
//#include "Singleton.h"
//#include <string>
//
//class Sint32;
//class Uint8;
//class SDL_Joystick;
//class Vector2;
//class SDL_Event;
//class SDL_Scancode;
//class SDL_Keycode;
//using namespace std;
//
//class InputHandler : public Singleton<InputHandler> {
//	friend Singleton<InputHandler>;
//
//private:
//	// Tipos de eventos
//	enum EVENTS {
//		e_closeWindow,
//
//		e_keyUp,
//		e_keyDown,
//
//		e_mouseMoving,
//		e_mousebuttonDown,
//
//		e_joyAxisMotion,
//		e_joyButtonUp,
//		e_joyButtonDown,
//		//e_joyDevice,
//
//		e_lastEventType
//	};
//	bool eventTriggered_[e_lastEventType];
//
//	// Estados del ratón y teclado
//	std::pair<Sint32, Sint32> mousePos_;
//	bool mbState_[3];
//	const Uint8* kbState_;
//
//	// Joysticks
//	const int JOYSTICK_DEAD_ZONE = 8000;
//	bool joystickConnected_;
//	std::vector<SDL_Joystick*> joysticks_;								// Todos los joysticks conectados
//	std::vector<std::pair<Vector2, Vector2>> joysticksValues_;			// Valores de los ejes
//	std::vector<std::vector<bool>> jbState_;							// Estados de los botones
//
//
//	// Input de texo
//	std::string text_;
//	const char* composition_;
//	Sint32 cursor_;
//	Sint32 selection_len_;
//
//
//	InputHandler();
//
//	// Comprueba si hay algún joystick conectado y los inicializa todos
//    void initJoystick();
//
//	// Limpia el input
//    void clear();
//
//	// Activa los eventos de pulsar/soltar del teclado
//    void onKeyDown();
//    void onKeyUp();
//
//	// Actival los eventos de movimiento del ratón y establece la posición del ratón
//    void onMouseMotion(const SDL_Event& event);
//
//	// Activa los eventos de botones del ratón y cambia su estado
//    void onMouseButtonChange(const SDL_Event& event, bool isDown);
//
//	// Activa el evento de cerrar ventana
//    void handleWindowEvent(const SDL_Event& event);
//
//	// Cambia los valores de los sticks
//    void joyAxisMotionEvent(const SDL_Event& event);
//
//	// Activa el evento de pulsar botón del joystick y cambia el estado de sus botones
//    void joyButtonDown(const SDL_Event& event);
//
//	// Activa el evento de soltar botón del joystick y cambia el estado de sus botones
//    void joyButtonUp(const SDL_Event& event);
//
//
//public:
//	enum MOUSEBUTTON : uint8_t { LEFT = 0, MIDDLE = 1, RIGHT = 2 };
//	enum STICKS { leftX, leftY, rightX, rightY };
//
//	virtual ~InputHandler();
//
//	void update(const SDL_Event& event);
//	void refresh();
//
//	// Devuelve true si se ha activado el evento de cerrar ventana
//	bool isWindowClosed();
//
//	// Devuelve true si se ha activado el evento de pulsar tecla
//	bool keyDownEvent();
//
//	// Devuelve true si la tecla especificada ha sido presionada
//	bool isKeyDown(SDL_Scancode key);
//	bool isKeyDown(SDL_Keycode key);
//
//	// Devuelve true si se ha activado el evento de soltar tecla
//	bool keyUpEvent();
//
//	// Devuelve true si la tecla especificada ha sido soltada
//	bool isKeyUp(SDL_Scancode key);
//	bool isKeyUp(SDL_Keycode key);
//	
//
//	// Devuelve true si se ha activado el evento de mover el ratón 
//	bool isMouseMoving();
//	// Devuelve true si se ha activado el evento de pulsar un botón del ratón
//	bool isMouseButtonDown();
//	// Devuelve la posición del ratón
//	const std::pair<Sint32, Sint32>& getMousePos();
//	// Devuelve true si el botón especificado del botón ha sido pulsado
//	int isMouseButtonDown(MOUSEBUTTON b);
//
//
//	// Devuelve el texto introducido
//	std::string getInputText();
//	// Limpia el texto introducido
//	void resetText();
//	// Borra el último char del texto introducido
//	void removeLastChar();
//
//	
//	// Devuelve true si hay al menos 1 joystick conectado
//	bool isjoystickConnected();
//
//	// Devuelve el valor X del stick especificado del joystick especificado
//	int joystickXValue(int joystick, int stick);
//	// Devuelve el valor Y del stick especificado del joystick especificado
//	int joystickYValue(int joystick, int stick);
//
//
//	// Devuelve true si se ha activado el evento de pulsar un botón del joystick
//	bool joystickButtonDownEvent();
//	// Devuelve true si se ha activado el evento de soltar un botón del joystick
//	bool joystickButtonUpEvent();
//
//	// Devuelve true si la el botón especificado del joystick especificado ha sido presionado
//	bool isJoystickButtonDown(int joystick, int button);
//
//
//};
//
//// Para acortar el método InputHandler::instance()->method() a inputHandler().method()
//inline InputHandler& inputHandler() { return *InputHandler::instance(); }
