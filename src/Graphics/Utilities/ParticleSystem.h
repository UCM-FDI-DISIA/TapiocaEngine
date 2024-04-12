#pragma once
#include <vector>
#include <string>
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class ParticleSystem;
}

namespace Tapioca {
class RenderNode;

/*
* @brief Wrapper de la clase ParticleSystem de Ogre.
*/
class TAPIOCA_API ParticleSystem : public RenderObject {
private:
    Ogre::ParticleSystem* mParticleSystem;      // ParticleSystem de Ogre

public:

    /*
    * @brief Constructora de la clase ParticleSystem.
    * Se crea a partir de un sistema ya creado en un .particle
    * @param scnMgr SceneManager de ogre
    * @param node Nodo para renderizado
    * @param name Nombre del ParticleSystem
    * @param templateName Nombre del sistema de particulas definido en el .particle 
    * @param emitting True si el sistema comienza emitiendo por defecto, false en caso contrario
    */
    ParticleSystem(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& name,
                   std::string const& templateName, const bool emitting = false);

    /*
    * @brief Destructora de la clase ParticleSystem
    */
    virtual ~ParticleSystem() { }

    /*
    * @brief Comienza o para el sistema de particulas
    * @param emitting True para que emita particulas, false para que no emita
    */
    void setEmitting(const bool emitting);
    /*
    * @brief Devuelve si el sistema de particulas esta emitiendo o no
    * @return True si emite particulas, false en caso contrario
    */
    bool isEmitting();

    /*
    * @brief Cambia el numero maximo de particulas que puede haber en el sistema
    * @param q Quota nueva del sistem a de particulas
    */
    void setQuota(const int q);
    /*
    * @brief Devuelve el numero maximo de particulas que puede haber en el sistema
    * @return Quota actual del sistema de particulas
    */
    int getQuota();

    /* 
    * @brief Acelera el sistema de particulas. Puede usarse para hacer que un sistema de particulas
    * que tarda cierto tiempo en comenzar a funcionar completamente lo haga al momento.
    * @param time Tiempo en segundos que se adelantan
    * @param interval Muestreo para generar las particulas (cuanto menor sea, mas realista, pero tambien mas costoso)
    */
    void fastForward(const float time, const float interval);

};
}