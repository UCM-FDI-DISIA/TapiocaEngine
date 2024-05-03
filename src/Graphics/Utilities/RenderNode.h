#pragma once
#include <unordered_set>
#include <vector>
#include "defs.h"

namespace Ogre {
class SceneManager;
class SceneNode;
class MovableObject;
}

namespace Tapioca {
class Vector3;
class Quaternion;
class RenderObject;
class GraphicsManager;
class Camera;
class LightDirectional;
class LightSpotlight;
class LightRectlight;
class NodeAnimator;

/*
* @brief Clase que representa un nodo de renderizado en la escena
*/
class TAPIOCA_API RenderNode {
private:
    friend GraphicsManager;
    friend RenderObject;
    friend Camera;
    friend LightDirectional;
    friend LightSpotlight;
    friend LightRectlight;
    friend NodeAnimator;

    Ogre::SceneManager* sceneManager;       // Puntero al manager de escenas de ogre
    Ogre::SceneNode* node;                  // Nodo para renderizado
    RenderObject* object;                   // Objecto para renderizar

    /*
    * @brief Anade un objeto al nodo de renderizado
    * @param object Puntero al objeto a anadir
    */
    void attachObject(RenderObject* const object);
    /*
    * @brief Anade un objeto al nodo de renderizado (solo para ManualObject)
    * @param object Puntero al objeto a anadir
    */
    void attachObject(Ogre::MovableObject* const object);
    /*
    * @brief Elimina el objeto del nodo de renderizado
    */
    void detachObject();

    /*
    * @brief Establece el punto de mira del nodo
    * @param target Punto de mira
    */
    void lookAt(const Vector3 target);

    /*
    * @brief Establece la direccion del nodo
    * @param dir Direccion
    */
    void setDirection(const Vector3 dir);

    /*
    * @brief Constructor de la clase RenderNode
    * @param scnMngr Puntero al manager de escenas de ogre
    * @param pos Posicion del nodo
    * @param scale Escala del nodo
    */
    RenderNode(Ogre::SceneManager* const scnMngr, const Vector3 pos, const Vector3 scale);

protected:
    /*
    * @brief Devuelve el nodo
    * @return Puntero al nodo
    */
    inline Ogre::SceneNode* getNode() const { return node; }

public:
    virtual ~RenderNode();

    /*
    * @brief Establece la posicion del nodo
    * @param pos Posicion
    */
    void setPosition(const Vector3 pos);
    /*
    * @brief Traslada t unidades el nodo
    * @param t Vector de traslacion
    */
    void translate(const Vector3 t);
    /*
    * @brief Establece la escala del nodo
    * @param sc Escala
    */
    void setScale(const Vector3 sc);
    
    /*
    * @brief Escala el nodo
    * @param s Escala
    */
    void scale(const Vector3 s);
    /*
    * @brief Establece la rotacion del nodo
    * @param q Cuaternion de rotacion
    */
    void setRotation(const Quaternion q);
    /*
    * @brief Establece la rotacion del nodo
    * @param rot Rotacion
    */
    void setRotation(const Vector3 rot);
    /*
    * @brief Rota el nodo
    * @param r Rotacion
    */
    void rotate(const Vector3 r);

    /*
    * @brief Rota el nodo en el eje X
    * @param angle Angulo de rotacion
    */
    void roll(const float angle);
    /*
    * @brief Rota el nodo en el eje Y
    * @param angle Angulo de rotacion
    */
    void yaw(const float angle);
    /*
    * @brief Rota el nodo en el eje Z
    * @param angle Angulo de rotacion
    */
    void pitch(const float angle);

    /*
    * @brief Devuelve la posicion del nodo
    * @return Posicion del nodo
    */
    Vector3 getPosition() const;
};
}
