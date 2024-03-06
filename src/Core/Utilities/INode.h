#pragma once
#include <vector>
#include "Utilities/defs.h"

namespace Ogre {
class SceneNode;
}

namespace Tapioca {
class Vector3;
class Transform;
class TransformBuilder;

class TAPIOCA_API INode {
private:
    friend TransformBuilder;
    Transform* transform;

protected:
    Ogre::SceneNode* node;

    INode() : transform(nullptr), node(nullptr) { }

public:
    virtual void removeChild(INode* node) = 0;
    virtual void removeAttachedParent() = 0;
    virtual void addChild(INode* node) = 0;

    virtual void setPosition(Vector3 position) = 0;
    virtual void translate(Vector3 movement) = 0;
    virtual void setRotation(Vector3 rotation) = 0;
    virtual void rotate(Vector3 r)=0;

   
    virtual void setScale(Vector3 scale) = 0;
    virtual void scale(Vector3 scale) = 0;
    virtual void roll(float angle)=0;
    virtual void yaw(float angle)=0;
    virtual void pitch(float angle) = 0;
    virtual std::vector<INode*> getChildren() = 0;
    virtual void getAllChildrenAux(std::vector<INode*>& allChildren) = 0;
    virtual std::vector<INode*> getAllChildren() = 0;
    virtual void setParent(INode* parent) = 0;

    virtual inline INode* getParent() const = 0;

    inline Transform* getTransform() const { return transform; }
    inline Ogre::SceneNode* getSceneNode() const { return node; }

    virtual ~INode() { }
};
}
