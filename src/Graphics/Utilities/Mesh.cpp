#include "Mesh.h"
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "RenderNode.h"

namespace Tapioca {
Mesh::Mesh(Ogre::SceneManager* const sceneManager, RenderNode* const node, std::string const& meshName)
    : RenderObject(node, sceneManager), mesh(sceneManager->createEntity(meshName)) 
{
    init(mesh);
    mesh->setCastShadows(true);
}

void Mesh::setMaterial(std::string const& materialName) { mesh->setMaterialName(materialName); }

void Mesh::castShadows(const bool enable) { mesh->setCastShadows(enable); }
}
