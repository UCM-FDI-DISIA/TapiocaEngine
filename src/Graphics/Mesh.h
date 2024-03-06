#pragma once
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class Entity;
}

namespace Tapioca {
class Node;
class GraphicsEngine;

class TAPIOCA_API Mesh : public RenderObject {
private:
    friend GraphicsEngine;

    Ogre::Entity* mesh;

public:
    Mesh(Ogre::SceneManager* const sceneManager, Node* const node, std::string const& meshName);

    void setMaterial(std::string const& materialName);
};
}