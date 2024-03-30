#pragma once
#include "defs.h"

namespace Tapioca {
class Component;

class TAPIOCA_API ComponentBuilder {
public:
    ComponentBuilder(const char* id) : id(id) { }
    /*
    * @brief Id del componente que crea
    */
    const char* id;
    /*
    * @brief Crea un componente
    */
    virtual Component* createComponent() = 0;
};
}

