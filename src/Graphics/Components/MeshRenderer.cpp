#include "MeshRenderer.h"
#include "GraphicsEngine.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "Node.h"
#include "Mesh.h"

namespace Tapioca {
MeshRenderer::MeshRenderer() : mesh(nullptr) {
	
}

MeshRenderer::~MeshRenderer() {
	// Esto???
	/*mesh->detachFromNode();*/
	delete mesh;
}

void MeshRenderer::initComponent(const CompMap& variables) {
	setValueFromMap(meshName, "meshName", variables);
	if(meshName == "") {
		// No hay nombre de mesh
		alive = false;
		return;
	}
	setValueFromMap(materialName, "materialName", variables);
}
void MeshRenderer::start() {
	Transform* trans = parent->getComponent<Transform>();
	mesh = GraphicsEngine::instance()->createMesh(static_cast<Node*>(trans->getNode()), meshName);

	if(materialName != "") {
		mesh->setMaterial(materialName);
	}
}
}
