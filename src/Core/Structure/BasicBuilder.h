#pragma once
#include "ComponentBuilder.h"
#include "Utilities/Concepts.h"

namespace Tapioca {
template<IsComponent TComp>
class TAPIOCA_API BasicBuilder : public ComponentBuilder {
public:
    BasicBuilder() : ComponentBuilder(TComp::id) { }

    /*
    * @brief Crea una componente
    * @return Puntero al componente creado
    */
	inline Component* createComponent() override { return new TComp(); }
};
}