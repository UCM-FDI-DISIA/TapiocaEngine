#pragma once
#include <string>
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class Entity;
}

namespace Tapioca {
class RenderNode;
class GraphicsManager;
class AnimationHelper;

/*
* @brief Wrapper de la clase Mesh de Ogre.
*/
class TAPIOCA_API Mesh : public RenderObject {
private:
    friend GraphicsManager;
    friend AnimationHelper;

    Ogre::Entity* mesh;   // Mesh de Ogre

    Ogre::Entity* const getMesh() const;

public:
    /*
    * @brief Constructor de la clase Mesh.
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

    void castShadows(const bool enable);
};
}