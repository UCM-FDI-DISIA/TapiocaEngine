#pragma once
#include "../Structure/Component.h"
#include "../Utilities/Vector3.h"
namespace Tapioca {

class Transform : public Component {
private:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

public:
    COMPONENT_ID("Transform")

    Transform();
    ~Transform() { }
    void initComponent() override;
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
};


}