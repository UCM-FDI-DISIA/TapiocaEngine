#pragma once
#include "ComponentBuilder.h"
#include "Utilities/Concepts.h"

namespace Tapioca {
/**
* @brief Plantilla de clase para construir componentes especificos
*/
template<IsComponent TComp>
class BasicBuilder : public ComponentBuilder {
public:
    /**
    * @brief Constructor de la clase
    */
    BasicBuilder() : ComponentBuilder(TComp::id) { }

    /**
    * @brief Crea un componente
    * @return Puntero al componente creado
    */
	inline Component* createComponent() override { return new TComp(); }
};
}
