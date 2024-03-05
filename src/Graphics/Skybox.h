#pragma once
#include "RenderObject.h"
#include <string>

namespace Ogre {
class SceneManager;
class Skybox;
}

namespace Tapioca {
class Node;

class Skybox : public RenderObject {
protected:
    std::string texture;
    float distC;
    bool orderC;
    Ogre::SceneManager* scnM;

    /*
    * @brief Construye un skybox con la textura texture a una distancia distC de la c�mara
    * e indicando si se pinta antes o despu�s del resto de elementos en funci�n de orderC
    */
    Skybox(Ogre::SceneManager* scnMgr, Node* node, std::string const& texture, float distC = 5000, bool orderC = true);
public:
    virtual ~Skybox() {/*if (mSkybox != nullptr) delete mSkybox;*/};
    void setEnable(bool enable);
    bool isEnable();
};
}
