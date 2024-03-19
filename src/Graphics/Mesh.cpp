#include "Mesh.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "RenderNode.h"

namespace Tapioca {
Ogre::Entity* const Mesh::getMesh() const { return mesh; }

Mesh::Mesh(Ogre::SceneManager* const sceneManager, RenderNode* const node, std::string const& meshName)
    : RenderObject(node, sceneManager) , mesh(sceneManager->createEntity(meshName)) 
{
    init(mesh);
}

void Mesh::setMaterial(std::string const& materialName) { mesh->setMaterialName(materialName); }

}