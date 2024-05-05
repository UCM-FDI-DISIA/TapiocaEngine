#pragma once
#include "defs.h"

namespace Ogre {
class MovableObject;
class SceneManager;
}

namespace Tapioca {
class RenderNode;

/**
* @brief Clase base para los objetos que se renderizan en la escena
*/
class TAPIOCA_API RenderObject {
private:
    friend RenderNode;

    Ogre::MovableObject* movObject;   // Puntero al objeto movible

protected:
    Ogre::SceneManager* sceneManager;   // Puntero al manager de escenas de ogre
    RenderNode* node;                   // Nodo para renderizado

    /**
    * @brief Devuelve el movable object
    * @return Movable object
    */
    inline Ogre::MovableObject* getMovObject() const { return movObject; }

    /**
    * @brief Constructor de la clase RenderObject
    * @param node Nodo para renderizado
    * @param scnMngr Puntero al manager de escenas de ogre
    */
    RenderObject(RenderNode* const node, Ogre::SceneManager* const scnMngr);

    /**
    * @brief Inicializa el movableObject
    * @param movObject MovableObject que se quiere inicializar
    */
    void init(Ogre::MovableObject* const movObject);

    /**
    * @brief Quita este objeto del nodo
    */
    void detachFromNode();

public:
    /**
    * @brief Se libera del nodo al que pertenecia y se destruye el objeto
    */
    virtual ~RenderObject();

    /**
    * @brief Cambia la visibilidad del objeto
    * @param enable True si se quiere que se vea el objeto, false en caso contrario
    */
    void setVisible(const bool enable);

    /**
    * @brief Devuelve la visibilidad del objeto
    * @return Visibilidad del objeto
    */
    bool isVisible() const;
};
}
