#include "JuegoPrueba.h"
#include "EjemploComponentFactory.h"
using namespace std;
using namespace TapiocaGame;

JUEGO_API void saluda(const char* nombre) { cout << "Hola, " << nombre << '\n'; }

JUEGO_API FactoryInfo** getComponentFactories(int& count) {

    count = NUM_FACTORIES;
    FactoryInfo** factories = new FactoryInfo*[NUM_FACTORIES];
    factories[0] = new FactoryInfo();
    factories[0]->name = "ejemploComponent";
    factories[0]->builder = new EjemploComponentFactory();

    return factories;
}
