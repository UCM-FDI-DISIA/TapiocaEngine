#include "EjemploComponentFactory.h"
#include "EjemploComponent.h"

namespace TapiocaGame {

Tapioca::Component* EjemploComponentFactory::createComponent() { return new EjemploComponent(); }

}
