#pragma once
#include <string>
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class Entity;
}

namespace Tapioca {
class RenderNode;
class GraphicsEngine;

class TAPIOCA_API Mesh : public RenderObject {
private:
    friend GraphicsEngine;

    Ogre::Entity* mesh;

public:
    Mesh(Ogre::SceneManager* const sceneManager, RenderNode* const node, std::string const& meshName);

    void setMaterial(std::string const& materialName);
};
}