#pragma once

namespace Ogre {
class MovableObject;
}

namespace Tapioca {
class Node;
}

namespace Tapioca {
class RenderObject {
private:
    Ogre::MovableObject* movObject;

protected:
    Tapioca::Node* node;

    RenderObject(Node* node);

    void init(Ogre::MovableObject* movObject);

public:
    virtual ~RenderObject() { }

    void setVisible(bool enable);

    void detachFromNode();
};
}