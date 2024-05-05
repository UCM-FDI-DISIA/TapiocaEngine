#pragma once
#include <string>
#include "defs.h"

namespace Tapioca {
/**
* @brief Clase base para los modulos del motor
*/
class TAPIOCA_API Module {
public:
    /**
    * Constructor vacio
    */
    Module() { }
    /**
    * Destructor vacio
    */
    virtual ~Module() { }

    /**
    * @brief Inicializa el modulo
    */
    virtual bool init() { return true; }
    /**
    * @brief Inicializa la configuracion especificada por el juego
    */
    virtual bool initConfig() { return true; }
    /**
    * @brief Inicializa las variables
    */
    virtual void start() { }
    /**
    * @brief Actualiza el modulo
    * @param deltaTime Tiempo transcurrido desde la ultima actualizacion
    */
    virtual void update(const uint64_t deltaTime) { }
    /**
    * @brief Actualiza el modulo a una velocidad fija
    */
    virtual void fixedUpdate() { }
    /**
    * @brief Renderiza el modulo
    */
    virtual void render() { }
    /**
    * @brief Refresca el modulo
    */
    virtual void refresh() { }
};
}
