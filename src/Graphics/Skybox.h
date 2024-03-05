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
    int distC;
    bool orderC;
    Ogre::SceneManager* scnM;
    // Construye un skybox con la textura texture
    Skybox(Ogre::SceneManager* scnMgr, Node* node, std::string const& texture);
    // Construye un skybox con la textura texture a una distancia distC de la cámara
    Skybox(Ogre::SceneManager* scnMgr, Node* node, std::string const& texture, int distC);
    // Construye un skybox con la textura texture a una distancia distC de la cámara indicando si 
    // se pinta antes o después del resto de elementos en función de orderC
    Skybox(Ogre::SceneManager* scnMgr, Node* node, std::string const& texture, int distC, bool orderC);

public:
    virtual ~Skybox() {/*if (mSkybox != nullptr) delete mSkybox;*/};
    void setVisible(bool visible);
};
}
