#pragma once
#include "../Structure/Component.h"
#include "../Utilities/Vector3.h"

namespace Tapioca {
class INode;
class TransformBuilder;

class Transform : public Component {
    friend TransformBuilder;

private:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    INode* node;

public:
    COMPONENT_ID("Transform")

    Transform();
    ~Transform();
    void initComponent(const CompMap& variables) override;
    // void update() override;
    // void fixedUpdate() override;

    Vector3 getPosition();
    Vector3 getRotation();
    Vector3 getScale();

    void setPosition(Vector3 p);
    void setRotation(Vector3);
    void setScale(Vector3 s);

    void translate(Vector3 p);
    void rotate(Vector3 r);

    //Eje X
    Vector3 right();
    //Eje Y
    Vector3 up();
    //Eje Z
    Vector3 forward();

    inline INode* getNode() { return node; }
};


}