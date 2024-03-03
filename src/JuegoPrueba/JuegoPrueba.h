#pragma once
#include "Utilities/defs.h"
#include "defs.h"

#include <Windows.h>
#include <vector>


#define JUEGO_API __declspec(dllexport)

extern "C" {
/*
* @brief Saluda a una persona
* @param nombre Nombre de la persona a saludar
*/
JUEGO_API void saluda(const char* nombre);

//
JUEGO_API FactoryInfo** getComponentFactories(int& count);
}
    