#pragma once
#include <Structure/Module.h>
union SDL_Event;

namespace Tapioca {
/**
* @brief Maneja los eventos de la ventana. Todo aquello que herede de esta clase podra manejar eventos de la ventana
*/
class TAPIOCA_API WindowModule : public Module {
public:
    /**
    * @brief Maneja los eventos de la ventana
    * @param event Evento de SDL
    * @return true si el evento fue manejado, false en caso contrario
    */
    virtual bool handleEvents(const SDL_Event& event) { return true; }
};
}
