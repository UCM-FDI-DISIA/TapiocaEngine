#pragma once
#include <string>
#include "defs.h"

namespace Tapioca {
/**
* Clase base para los modulos del motor

*/
class TAPIOCA_API Module {
public:
    /**
    * Constructor vacia
    */
    Module() { }
    /**
    * Destructor vacio
    */
    virtual ~Module() { }

    virtual bool init() { return true; }
    virtual bool initConfig() { return true; }
    virtual void start() { }
    virtual void update(const uint64_t deltaTime) { }
    virtual void fixedUpdate() { }
    virtual void render() { }
    virtual void refresh() { }
};
}