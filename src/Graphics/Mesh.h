#pragma once
#include <string>
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class Entity;
}

namespace Tapioca {
class RenderNode;
class GraphicsEngine;

/*
* @brief Wrapper de la clase Mesh de Ogre.
*/
class TAPIOCA_API Mesh : public RenderObject {
private:
    friend GraphicsEngine;

    Ogre::Entity* mesh;         // Mesh de Ogre

public:
    /*
    * @brief Constructora de la clase Mesh.
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param name Nombre del mesh
    */
    Mesh(Ogre::SceneManager* const sceneManager, RenderNode* const node, std::string const& meshName);

    /*
    * @brief Cambia el material de la malla
    * @param materialName Nombre del material que se quiere asignar a la malla
    */
    void setMaterial(std::string const& materialName);
    Ogre::Entity* getMesh() const;
};
}