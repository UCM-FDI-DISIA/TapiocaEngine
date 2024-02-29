#pragma once
#include <string>
#include "RenderObject.h"

namespace Tapioca {
class Node;
}

namespace Ogre {
class SceneManager;
class Entity;
}

namespace Tapioca {
class Mesh : public RenderObject {
private:
    Ogre::Entity* mesh;

public:
    Mesh(Ogre::SceneManager* sceneManager, Node* node, std::string meshName);

    void setMaterial(std::string materialName);
};
}