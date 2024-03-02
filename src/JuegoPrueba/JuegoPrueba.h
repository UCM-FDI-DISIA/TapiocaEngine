#pragma once
#include <Windows.h>
#include <vector>
#include <iostream>
#include "Structure/ComponentBuilder.h"
#include "EjemploComponentFactory.h"
#include "Utilities/defs.h"
#include "defs.h"
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
JUEGO_API FactoryInfo** getComponentFactories(int& count);
}
    