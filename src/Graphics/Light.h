#pragma once
#include "RenderObject.h"
#include "Utilities/Vector4.h"

namespace Ogre {
class SceneManager;
class Light;
}
namespace Tapioca {
class Node;
}

namespace Tapioca {
class Light : public RenderObject {
protected:
    Ogre::Light* mLight;
    Tapioca::Vector4 mColor;

    Light(Ogre::SceneManager* scnMgr, Tapioca::Node* node, Vector4 color);

public:
    virtual ~Light() { }

    void setColor(Vector4 color);
};
}