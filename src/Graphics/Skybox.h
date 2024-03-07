#pragma once
#include <string>
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class Skybox;
}

namespace Tapioca {

class TAPIOCA_API Skybox {
protected:
    std::string texture;
    float distC;
    bool orderC;
    Ogre::SceneManager* scnM;

    /*
    * @brief Construye un skybox con la textura texture a una distancia distC de la c�mara
    * e indicando si se pinta antes o despu�s del resto de elementos en funci�n de orderC
    */
    Skybox(Ogre::SceneManager* const scnMgr, std::string const& texture, const float distC = 5000,
           const bool orderC = true);

public:
    void setEnable(const bool enable);
    bool isEnabled();
};
}
