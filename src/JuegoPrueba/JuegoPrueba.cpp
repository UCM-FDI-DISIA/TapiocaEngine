#include "JuegoPrueba.h"
#include "EjemploComponentFactory.h"

JUEGO_API void saluda(const char* nombre) { std::cout << "Hola, " << nombre << '\n'; }

JUEGO_API FactoryInfo** getComponentFactories(int& count) {

    count = NUM_FACTORIES;
    FactoryInfo** factories = new FactoryInfo*[NUM_FACTORIES];
    factories[0] = new FactoryInfo();
    factories[0]->name = "EjemploComponent";
    factories[0]->factory = new EjemploComponentFactory();

    return factories;
}
