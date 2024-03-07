#pragma once
#include <unordered_set>
#include "../Structure/Component.h"
#include "../Utilities/Vector3.h"
#include "../Utilities/defs.h"

namespace Tapioca {
class INode;
class TransformBuilder;

/*
* Transform guarda la posicion, rotacion y escala del objeto, ademas de la interfaz del nodo.
* Este componente esta en todos los objetos, y si se intenta borrar, se borrara todo el objeto.
*/
class TAPIOCA_API Transform : public Component {
    friend TransformBuilder;

private:
    Vector3 position;
    void moved() {};
    Vector3 rotation;
    void rotated() {};
    Vector3 scale;
    void scaled() {};

    Transform* parent;
    std::unordered_set<Transform*> children;

    void getAllChildrenAux(std::vector<Transform*>& allChildren) const;
    void addChild(Transform* const child);
    void removeChild(Transform* const child);
    void removeConnections();
    void removeParent();

public:
    COMPONENT_ID("Transform")

    Transform();
    ~Transform();

    bool initComponent(const CompMap& variables) override;

    inline Vector3 getPosition() const { return position; }
    Vector3 getGlobalPosition() const;
    inline Vector3 getRotation() const { return rotation; }
    Vector3 getGlobalRotation() const;
    inline Vector3 getScale() const { return scale; }
    Vector3 getGlobalScale() const;

    void setPosition(const Vector3& p);
    void setRotation(const Vector3& r);
    void setScale(const Vector3& s);

    // Mueve el objeto tanto como el vector dado indica.
    void translate(const Vector3& p);
    // Rota el objeto sobre los ejes tanto como el vector dado indica.
    void rotate(const Vector3& r);

    // Devuelve el eje X de coordenadas locales en coordenadas globales.
    Vector3 right();
    // Devuelve el eje Y de coordenadas locales en coordenadas globales.
    Vector3 up();
    // Devuelve el eje Z de coordenadas locales en coordenadas globales.
    Vector3 forward();

    // Hace que el Transform dado sea padre de este.
    void setParent(Transform* const transform);
    // Devuelve el padre de este Transform.
    Transform* getParent() const;

    // Devuelve los hijos directos de este Transform.
    std::vector<Transform*> getChildren() const;
    // Devuelve tanto los hijos directos como los indirectos de este Transform.
    std::vector<Transform*> getAllChildren() const;
};
}