#pragma once
#include "RenderObject.h"
#include <string>

namespace Ogre {
class SceneManager;
class Entity;
}

namespace Tapioca {
class Node;

class Mesh : public RenderObject {
private:
    Ogre::Entity* mesh;

public:
    Mesh(Ogre::SceneManager* sceneManager, Node* node, std::string meshName);

    void setMaterial(std::string materialName);
};
}