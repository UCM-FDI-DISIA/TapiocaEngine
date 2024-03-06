#pragma once
#include "RenderObject.h"
#include "Utilities/Vector4.h"

namespace Ogre {
class SceneManager;
class Light;
}

namespace Tapioca {
class Node;

class TAPIOCA_API Light : public RenderObject {
protected:
    Ogre::Light* mLight;
    Vector4 mColor;

    Light(Ogre::SceneManager* const scnMgr, Node* const node, const Vector4 color);

public:
    virtual ~Light() { }

    void setColor(const Vector4 color);
};
}