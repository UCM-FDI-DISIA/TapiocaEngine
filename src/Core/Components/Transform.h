#pragma once
#include "../Structure/Component.h"
#include "../Utilities/Vector3.h"

namespace Tapioca {
class INode;
class TransformBuilder;

/*
* Transform guarda la posicion, rotacion y escala del objeto, ademas de la interfaz del nodo.
* Este componente esta en todos los objetos, y si se intenta borrar, se borrara todo el objeto.
*/
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

    bool initComponent(const CompMap& variables) override;

    inline Vector3 getPosition() const { return position; }
    inline Vector3 getRotation() const { return rotation; }
    inline Vector3 getScale() const { return scale; }

    void setPosition(Vector3 p);
    void setRotation(Vector3);
    void setScale(Vector3 s);

    // Mueve el objeto tanto como el vector dado indica.
    void translate(Vector3 p);
    // Rota el objeto sobre los ejes tanto como el vector dado indica.
    void rotate(Vector3 r);

    // Devuelve el eje X de coordenadas locales en coordenadas globales.
    Vector3 right();
    // Devuelve el eje Y de coordenadas locales en coordenadas globales.
    Vector3 up();
    // Devuelve el eje Z de coordenadas locales en coordenadas globales.
    Vector3 forward();

    // Hace que el Transform dado sea padre de este.
    void setParent(Transform* tranform);
    // Devuelve el padre de este Transform.
    Transform* getParent() const;
    // Devuelve la interfaz de nodo de este Transform.
    inline INode* getNode() const { return node; }

    // Devuelve los hijos directos de este Transform.
    std::vector<Transform*> getChildren() const;
    // Devuelve tanto los hijos directos como los indirectos de este Transform.
    std::vector<Transform*> getAllChildren() const;
    virtual void fixedUpdate() override;
};
}