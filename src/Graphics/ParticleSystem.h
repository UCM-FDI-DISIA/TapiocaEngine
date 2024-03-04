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
    * @brief Constructora.Solo se podra crear un ParticleSystem a partir de un sistema ya creado en un.particle
    */
    ParticleSystem(Ogre::SceneManager* scnMgr, Tapioca::Node* node, std::string name, std::string materialName,
                   bool emitting = false);

    virtual ~ParticleSystem() { }

    void setEmitting(bool emitting);
    bool isEmitting();

    /*
    Se utiliza para acelerar el sistema un numero determinado de segundos
    Puede ser util para que un sistema de particula que tarda un poco en comenzar a funcionar completamente,
    lo haga al momento
    El parametro interval hace referencia al muestreo para generar estas particulas (cuanto mas peque�o sea,
    mas realista es. Sin embargo, tambien es mas costoso)
    */
    void fastForward(float time, float interval);
};
}