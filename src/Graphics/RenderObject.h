#pragma once
#include "Utilities/defs.h"

namespace Ogre {
class MovableObject;
class SceneManager;
}

namespace Tapioca {
class Node;

class TAPIOCA_API RenderObject {
private:
    friend Node;

    Ogre::MovableObject* movObject;

    inline Ogre::MovableObject* getMovObject() const { return movObject; }

protected:
    Ogre::SceneManager* sceneManager;
    Node* node;

    RenderObject(Node* const node, Ogre::SceneManager* const sceneManager);

    void init(Ogre::MovableObject* const movObject);

    void detachFromNode();

public:
    virtual ~RenderObject();

    void setVisible(const bool enable);

    bool getVisible() const;
};
}