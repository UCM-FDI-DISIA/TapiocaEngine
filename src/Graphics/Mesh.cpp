#include "Mesh.h"
#include <Ogre.h>
#include "Node.h"

namespace Tapioca {

Mesh::Mesh(Ogre::SceneManager* sceneManager, Node* node, std::string const& meshName)
    : RenderObject(node, sceneManager) , mesh(sceneManager->createEntity(meshName)) 
{
    init(mesh);
}

void Mesh::setMaterial(std::string const& materialName) { mesh->setMaterialName(materialName); }

}