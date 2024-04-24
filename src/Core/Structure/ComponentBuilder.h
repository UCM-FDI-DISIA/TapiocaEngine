#pragma once
#include "defs.h"

namespace Tapioca {
class Component;

class TAPIOCA_API ComponentBuilder {
public:
    ComponentBuilder(const std::string& id) : id(id) { }
    virtual ~ComponentBuilder() { }
    /*
    * @brief Id del componente que crea.
    */
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string id;
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    /*
    * @brief Crea un componente.
    */
    virtual Component* createComponent() = 0;
};
}
