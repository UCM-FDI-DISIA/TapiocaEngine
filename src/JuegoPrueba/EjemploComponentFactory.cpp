#include "EjemploComponentFactory.h"
using namespace Tapioca;

Tapioca::Component* Tapioca::EjemploComponentFactory::createComponent() { return new Tapioca::EjemploComponent(); }