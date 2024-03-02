#pragma once

namespace Ogre {
class MovableObject;
}

namespace Tapioca {
class Node;
class GraphicsEngine;
}

namespace Tapioca {
class RenderObject {
public:
    friend Node;
    friend GraphicsEngine;

private:
    Ogre::MovableObject* movObject;

    Ogre::MovableObject* getMovObject() const { return movObject; }

protected:
    Tapioca::Node* node;

    RenderObject(Node* node);

    void init(Ogre::MovableObject* movObject);

    void detachFromNode();

public:
    virtual ~RenderObject() { }

    void setVisible(bool enable);
};
}