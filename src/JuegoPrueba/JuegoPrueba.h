#pragma once
#include <iostream>
#include "Structure/ComponentFactory.h"
#include "EjemploComponentFactory.h"
using namespace std;
using namespace Tapioca;
#define JUEGO_API __declspec(dllexport)



extern "C" {
/*
* @brief Saluda a una persona
* @param nombre Nombre de la persona a saludar
*/
JUEGO_API void saluda(const char* nombre);

//
}
    