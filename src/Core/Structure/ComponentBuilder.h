#pragma once
#include "defs.h"

namespace Tapioca {
class Component;

/**
* @brief Clase base abstracta para crear componentes
*/
class TAPIOCA_API ComponentBuilder {
public:
    /**
    * @brief Constructor por defecto
    * @param id Id del componente que crea
    */
    ComponentBuilder(const std::string& id) : id(id) { }
    /**
    * @brief Destructor virtual por defecto
    */
    virtual ~ComponentBuilder() { }
    
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string id; // Id del componente que crea
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    /**
    * @brief Crea un componente
    */
    virtual Component* createComponent() = 0;
};
}
