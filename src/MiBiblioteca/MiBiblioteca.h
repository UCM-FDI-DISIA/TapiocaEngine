#pragma once
#include <iostream>

#define JUEGO_API __declspec(dllexport)

extern "C" {
/*
* @brief Saluda a una persona
* @param nombre Nombre de la persona a saludar
*/
JUEGO_API void saluda(const char* nombre);
}
    