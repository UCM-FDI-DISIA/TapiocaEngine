#pragma once
#include "ComponentBuilder.h"
#include "Utilities/Concepts.h"

namespace Tapioca {
template<IsComponent TComp>
class BasicBuilder : public ComponentBuilder {
public:
	inline Component* createComponent() override { return new TComp(); }
};
}