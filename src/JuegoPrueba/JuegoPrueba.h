#pragma once
namespace Tapioca {
class FactoryManager;
}

#define JUEGO_API __declspec(dllexport)

extern "C" {
/**
* @brief Inicializa el juego
*/
JUEGO_API void init(Tapioca::FactoryManager* manager);

/**
* @brief Anade las factorias de componentes del juego al motor
*/
JUEGO_API void addComponentFactories(Tapioca::FactoryManager* manager);
}
