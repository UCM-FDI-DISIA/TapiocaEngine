#pragma once
#include "defs.h"

namespace Tapioca {
class Component;

class TAPIOCA_API ComponentBuilder {
public:
    ComponentBuilder(const char* id) : id(nullptr) {
        size_t len = strlen(id) + 1;
        this->id = new char[len];
        strcpy_s(this->id, len, id);
    }
    virtual ~ComponentBuilder() {
        delete[] id;
    }
    /*
    * @brief Id del componente que crea
    */
    char* id;
    /*
    * @brief Crea un componente
    */
    virtual Component* createComponent() = 0;
};
}
