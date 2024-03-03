#pragma once
#include <type_traits>

namespace Tapioca {
class Component;
// Para que una clase sea un componente valido, necesita heredar de Component y tener una id valida.
template<typename TComp>
concept IsComponent = std::is_base_of<Component, TComp>::value
	&& requires { { TComp::id }->std::convertible_to<std::string>; };
}