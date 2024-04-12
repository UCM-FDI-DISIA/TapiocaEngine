#include "MeshRenderer.h"
#include "GraphicsManager.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "Utilities/RenderNode.h"
#include "Utilities/Mesh.h"

namespace Tapioca {
MeshRenderer::MeshRenderer()
    : mesh(nullptr), node(nullptr), transform(nullptr), initialRotation(Vector3(0.0f)), meshName(), materialName(""),
      castShadows(true) { }

MeshRenderer::~MeshRenderer() { delete node; }

bool MeshRenderer::initComponent(const CompMap& variables) {
    // No se ha podido establecer o No hay nombre de mesh
    if (!setValueFromMap(meshName, "meshName", variables) || meshName == "") {
        logError("MeshRenderer: No se pudo inicializar el nombre del mesh.");
        return false;
    }

    // Da igual si no hay material o si el material tiene un nombre vacio
    if (!setValueFromMap(materialName, "materialName", variables)) {
        logInfo("MeshRenderer: No existe nombre para el material: se coloca el predefinido por la mesh.");
    }

    bool rotationSet = setValueFromMap(initialRotation.x, "initRotationX", variables) &&
        setValueFromMap(initialRotation.y, "initRotationY", variables) &&
        setValueFromMap(initialRotation.z, "initRotationZ", variables);
    if (!rotationSet) {
        logInfo("MeshRenderer: La rotacion inicial es Vector3(0,0,0).");
    }

    if (!setValueFromMap(castShadows, "castShadows", variables)) {
        logInfo("MeshRenderer: El objeto produce sombras por defecto.");
    }

    return true;
}

void MeshRenderer::awake() {
    GameObject* gameobject = getObject();
    transform = gameobject->getComponent<Transform>();
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
    mesh = g->createMesh(node, meshName);

    if (materialName != "") {
        mesh->setMaterial(materialName);
    }
    if (!castShadows) {
        mesh->castShadows(false);
    }
}

void MeshRenderer::handleEvent(std::string const& id, void* info) {
    if (id == "transformChanged") {
        //node->setPosition(transform->getGlobalPosition());
        node->setPosition(transform->getRotationPosition());
        node->setRotation(transform->getGlobalRotation() * Quaternion(initialRotation));
        node->setScale(transform->getGlobalScale());
    }
}

Mesh* MeshRenderer::getMesh() const { return mesh; }

void MeshRenderer::setMeshName(const std::string& n) { meshName = n; }

void MeshRenderer::setMaterialName(const std::string& n) { materialName = n; }

void MeshRenderer::setVisible(const bool enable) { mesh->setVisible(enable); }
bool MeshRenderer::isVisible() const { return mesh->isVisible(); }
}
