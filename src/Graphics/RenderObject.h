#pragma once

namespace Ogre {
class MovableObject;
class SceneManager;
}

namespace Tapioca {
class Node;

class RenderObject {
private:
    friend Tapioca::Node;

    Ogre::MovableObject* movObject;
    Ogre::SceneManager* sceneManager;

    inline Ogre::MovableObject* getMovObject() const { return movObject; }

protected:
    Tapioca::Node* node;

    RenderObject(Node* node, Ogre::SceneManager* sceneManager);

    void init(Ogre::MovableObject* movObject);

    void detachFromNode();

public:
    virtual ~RenderObject();

    void setVisible(bool enable);
};
}