#include "EjemploComponentFactory.h"
using namespace TapiocaGame;

Component* EjemploComponentFactory::createComponent() { return new EjemploComponent(); }