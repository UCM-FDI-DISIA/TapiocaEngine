#pragma once
#include "RenderObject.h"
#include <string>

namespace Ogre {
class SceneManager;
class Skyplanes;
}

namespace Tapioca {
class Node;

class Skyplane : public RenderObject {
protected:
    std::string material;
    Ogre::SceneManager* scnM;
    // Construye un skyplane
    Skyplane(Ogre::SceneManager* scnMgr, Node* node, std::string const& texture);

public:
    virtual ~Skyplane() {/*if (mSkyplane != nullptr) delete mSkyplane;*/};
    void setVisible(bool visible);
};
}
