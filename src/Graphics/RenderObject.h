#pragma once
#include "defs.h"

namespace Ogre {
class MovableObject;
class SceneManager;
}

namespace Tapioca {
class RenderNode;

class TAPIOCA_API RenderObject {
private:
    friend RenderNode;

    Ogre::MovableObject* movObject;

    /*
    * @brief Devuelve el movable object
    * @return Movable object
    */
    inline Ogre::MovableObject* getMovObject() const { return movObject; }

protected:
    Ogre::SceneManager* sceneManager;       // Puntero al manager de escenas de ogre
    RenderNode* node;                       // Nodo para renderizado

    /*
    * @brief Constructora de la clase RenderObject.
    * @param node Nodo para renderizado
    * @param scnMgr Puntero al manager de escenas de ogre
    */
    RenderObject(RenderNode* const node, Ogre::SceneManager* const sceneManager);

    /*
    * @brief Inicializa el movableObject
    * @param movObject MovableObject que se quiere inicializar
    */
    void init(Ogre::MovableObject* const movObject);

    /*
    * @brief Quita este objeto del nodo
    */
    void detachFromNode();

public:
    /*
    * @brief Destructora de la clase RenderObject.
    */
    virtual ~RenderObject();

    /*
    * @brief Cambia la visibilidad del objeto
    * @param enable True si se quiere que se vea el objeto, false en caso contrario
    */
    void setVisible(const bool enable);

    /*
    * @brief Devuelve la visibilidad del objeto
    * @return Visibilidad del objeto
    */
    bool isVisible() const;
};
}