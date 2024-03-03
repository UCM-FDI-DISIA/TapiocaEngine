#include "JuegoPrueba.h"
#include "EjemploComponentFactory.h"
#include "Structure/ComponentBuilder.h"
#include "EjemploComponentFactory.h"

#include <iostream>

// IMPORTANTE: METERLO EN UN NAMESPACE CAUSA ERRORES DE EJECUCION

JUEGO_API void saluda(const char* nombre) { std::cout << "Hola, " << nombre << '\n'; }

JUEGO_API FactoryInfo** getComponentFactories(int& count) {
    count = NUM_FACTORIES;
    FactoryInfo** factories = new FactoryInfo*[NUM_FACTORIES];
    factories[0] = new FactoryInfo();
    factories[0]->name = "ejemploComponent";
    factories[0]->builder = new TapiocaGame::EjemploComponentFactory();

    return factories;
}
