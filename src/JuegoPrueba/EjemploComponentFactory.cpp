#include "EjemploComponentFactory.h"
#include "Structure/Component.h"
#include "EjemploComponent.h"
#include "defs.h"


namespace TapiocaGame {

	Tapioca::Component* EjemploComponentFactory::createComponent() { return new EjemploComponent(); }

}
