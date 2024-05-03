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
* @brief Wrapper de la clase Mesh de Ogre
*/
class TAPIOCA_API Mesh : public RenderObject {
private:
    friend GraphicsManager;
    friend AnimationHelper;

    Ogre::Entity* mesh;   // Mesh de Ogre

protected:
    /*
    * @brief Devuelve el mesh de Ogre
    * @return Puntero al mesh de Ogre
    */
    inline Ogre::Entity* getMesh() const { return mesh; }

public:
    /*
    * @brief Constructor de la clase Mesh
    * @param scnMngr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param meshName Nombre del mesh
    */
    Mesh(Ogre::SceneManager* const scnMngr, RenderNode* const node, std::string const& meshName);

    /*
    * @brief Cambia el material de la malla
    * @param materialName Nombre del material que se quiere asignar a la malla
    */
    void setMaterial(std::string const& materialName);

    /*
    * @brief Activa o desactiva las sombras de la malla
    */
    void castShadows(const bool enable);
};
}
