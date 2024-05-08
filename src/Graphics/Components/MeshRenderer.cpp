#include "MeshRenderer.h"
#include "GraphicsManager.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "Utilities/RenderNode.h"
#include "Utilities/Mesh.h"
#include "checkML.h"

namespace Tapioca {
MeshRenderer::MeshRenderer() : graphicsManager(nullptr), node(nullptr), transform(nullptr), mesh(nullptr), 
    initialRotation(Vector3(0.0f)), materialName(""), castShadows(true), meshName("") { }

MeshRenderer::~MeshRenderer() {
    graphicsManager = nullptr;
    if (node != nullptr) delete node;
    transform = nullptr;
    mesh = nullptr;
}

bool MeshRenderer::initComponent(const CompMap& variables) {
    // No se ha podido establecer o No hay nombre de mesh
    if (!setValueFromMap(meshName, "meshName", variables) || meshName == "") {
        logError("MeshRenderer: No se pudo inicializar el nombre del mesh.");
        return false;
    }
    if (!GraphicsManager::instance()->checkResourceExists(meshName)) {
        logError("MeshRenderer: No existe una mesh con ese nombre.");
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
    graphicsManager = GraphicsManager::instance();
    GameObject* gameobject = getObject();
    transform = gameobject->getComponent<Transform>();
    node = graphicsManager->createNode();

    if (meshName != "") {
        mesh = graphicsManager->createMesh(node, meshName);
        if (mesh != nullptr) {
            if (materialName != "") mesh->setMaterial(materialName);
            if (!castShadows) mesh->castShadows(false);
        }
        else alive = false;
    }
}

void MeshRenderer::handleEvent(std::string const& id, void* info) {
    if (id == "posChanged") node->setPosition(transform->getGlobalPosition());
    else if (id == "rotChanged") node->setRotation(transform->getGlobalRotation() * Quaternion(initialRotation));
    else if (id == "scaleChanged") node->setScale(transform->getGlobalScale());
}

void MeshRenderer::setMaterial(const std::string& name) {
    if (mesh != nullptr) {
        materialName = name;
        mesh->setMaterial(name);
    }
}

void MeshRenderer::setMesh(const std::string& name) {
    if (mesh != nullptr) {
        delete mesh;
        mesh = nullptr;
    }
    if (graphicsManager != nullptr) {
        if (graphicsManager->checkResourceExists(name)) {
            meshName = name;
            mesh = graphicsManager->createMesh(node, name);
        }
    }
}

void MeshRenderer::setVisible(const bool enable) {
    if (mesh != nullptr) mesh->setVisible(enable);
}
bool MeshRenderer::isVisible() const {
    if (mesh != nullptr) return mesh->isVisible();
    return false;
}
}
