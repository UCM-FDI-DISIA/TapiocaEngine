#include "MeshRenderer.h"
#include "GraphicsEngine.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "Node.h"
#include "Mesh.h"

namespace Tapioca {
MeshRenderer::MeshRenderer() : mesh(nullptr) { }

MeshRenderer::~MeshRenderer() {
	delete mesh;
}

bool MeshRenderer::initComponent(const CompMap& variables) {
    // No se ha podido establecer o No hay nombre de mesh
    if (!setValueFromMap(meshName, "meshName", variables) || meshName == "") {
#ifdef _DEBUG
        std::cerr << "Error: MeshRenderer: no se pudo inicializar el nombre del mesh.\n";
#endif
        return false;
    }

    // Da igual si no hay material o si el material tiene un nombre vac�o
    setValueFromMap(materialName, "materialName", variables);

    return true;
}

void MeshRenderer::start() {
    Transform* trans = parent->getComponent<Transform>();
    mesh = GraphicsEngine::instance()->createMesh(static_cast<Node*>(trans->getNode()), meshName);

    if (materialName != "") 
        mesh->setMaterial(materialName);
}

void MeshRenderer::update(const uint64_t deltaTime) { }
}
