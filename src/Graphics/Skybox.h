#pragma once
#include <string>
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class Skybox;
}

namespace Tapioca {

/*
* @brief Wrapper de la clase Skybox de Ogre.
*/
class TAPIOCA_API Skybox {
protected:
    std::string texture;            // Nombre de la textura
    float distC;                    // 
    bool orderC;
    Ogre::SceneManager* scnM;

    /*
    * @brief Construye un skybox con la textura texture a una distancia distC de la cámara
    * e indicando si se pinta antes o después del resto de elementos en función de orderC
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param texture Nombre de la textura
    * @param distC Distancia del plano a la camara
    * @param orderC True si se quiere que se dibuje antes que todas las geometrias de la escena
    */
    Skybox(Ogre::SceneManager* const scnMgr, std::string const& texture, const float distC = 5000,
           const bool orderC = true);

public:
    /*
    * @brief Activa o desactiva el Skybox
    * @param enable True si se quiere activar, false en caso contrario
    */
    void setEnable(const bool enable);
    /*
    * @brief Devuelve si esta activo
    * @return True si esta activo, false en caso contrario
    */
    bool isEnabled();
};
}
