#pragma once
#include "Utilities/defs.h"

namespace Tapioca {
class Component;

class TAPIOCA_API ComponentBuilder {
public:
    /*
    * @brief Crea una componente
    */
    virtual Component* createComponent() = 0;
};
}

