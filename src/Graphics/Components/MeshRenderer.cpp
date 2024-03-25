#include "MeshRenderer.h"
#include "../GraphicsManager.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "../RenderNode.h"
#include "../Mesh.h"

namespace Tapioca {
MeshRenderer::MeshRenderer() : mesh(nullptr), node(nullptr), transform(nullptr), initialRotation(Vector3(0.0f)) { }

MeshRenderer::~MeshRenderer() { delete node; }

bool MeshRenderer::initComponent(const CompMap& variables) {
    // No se ha podido establecer o No hay nombre de mesh
    if (!setValueFromMap(meshName, "meshName", variables) || meshName == "") {
#ifdef _DEBUG
        std::cerr << "Error: MeshRenderer: no se pudo inicializar el nombre del mesh.\n";
#endif
        return false;
    }

    // Da igual si no hay material o si el material tiene un nombre vacio
    if (!setValueFromMap(materialName, "materialName", variables)) {
#ifdef _DEBUG
        std::cout << "MeshRenderer: no existe nombre para el material: se coloca el predefinido por la mesh.\n";
#endif
    }

    bool rotationSet = setValueFromMap(initialRotation.x, "initRotationX", variables) &&
        setValueFromMap(initialRotation.y, "initRotationY", variables) &&
        setValueFromMap(initialRotation.z, "initRotationZ", variables);
    if (!rotationSet) {
#ifdef _DEBUG
        std::cerr << "MeshRenderer: la rotacion inicial es Vector3(0,0,0).\n";
#endif
    }

    return true;
}

void MeshRenderer::awake() {
    GameObject* gameobject = getObject();
    transform = gameobject->getComponent<Transform>();
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
    mesh = g->createMesh(node, meshName);

}

void MeshRenderer::handleEvent(std::string const& id, void* info) {
    if (id == "transformChanged") {
        node->setPosition(transform->getGlobalPosition());
        // TODO: hacer que gire respecto de su padre y no de si mismo
        node->setRotation(initialRotation + transform->getGlobalRotation());
        // es tan grande que no se ve
        node->setScale(transform->getGlobalScale());
    }
}

Mesh* MeshRenderer::getMesh() const { return mesh; }

void MeshRenderer::setMeshName(const std::string& n) { meshName = n; }

void MeshRenderer::setMaterialName(const std::string& n) { materialName = n; }

}

