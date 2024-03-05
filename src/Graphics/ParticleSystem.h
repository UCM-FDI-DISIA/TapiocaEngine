#pragma once
#include "RenderObject.h"
#include <string>

namespace Ogre {
class SceneManager;
class ParticleSystem;
}

namespace Tapioca {
class Node;

class ParticleSystem : public RenderObject {
private:
    Ogre::ParticleSystem* mParticleSystem;

public:

    /*
    * @brief Constructora. Solo se podra crear un ParticleSystem a partir de un sistema ya creado en un .particle
    * @param scnMgr SceneManager de ogre
    * @param node Nodo que contiene el ParticleSystem
    * @param name Nombre del ParticleSystem
    * @param templateName Nombre del sistema de particulas definido en el .particle 
    * @param emitting Si el sistema comienza emitiendo por defecto o no
    */
    ParticleSystem(Ogre::SceneManager* scnMgr, Tapioca::Node* node, std::string const& name,
                   std::string const& templateName, bool emitting = false);

    virtual ~ParticleSystem() { }

    void setEmitting(bool emitting);
    bool isEmitting();

    /* 
    * @brief Acelera el sistema de particulas. Puede usarse para hacer que un sistema de particulas
    * que tarda cierto tiempo en comenzar a funcionar completamente lo haga al momento.
    * @param time Tiempo en segundos que se adelantan
    * @param interval Muestreo para generar las particulas (cuanto menor sea, mas realista, pero tambien mas costoso)
    */
    void fastForward(float time, float interval);
};
}