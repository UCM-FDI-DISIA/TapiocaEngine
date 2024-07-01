#include "PrefabManager.h"
#include "GameObject.h"
#include "componentDefs.h"

namespace Tapioca {
PrefabManager::PrefabManager() { }

PrefabManager::~PrefabManager() { }

bool PrefabManager::addPrefab(std::string name, GameObject* prefab) {
    if (load) {
        if (prefabsL.count(name) == 0) {
            prefabsL[name] = prefab;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool PrefabManager::isPrefab(std::string name) {
    if (load) return prefabsL.count(name) == 1;
    else
        return false;
}

void PrefabManager::erasePrefab(std::string name) {
    if (load) prefabsL.erase(name);
}

GameObject* PrefabManager::getPrefab(std::string name) {
    if (load) return prefabsL.at(name);
    else
        return nullptr;
}

GameObject* PrefabManager::instantiate(std::string name, Scene* scene, Transform* transform) {
    if (load) {
        if (isPrefab(name)) return prefabsL.at(name)->InstantiatePrefab(scene, transform);
        else
            return nullptr;
    }
    else
        return nullptr;
}

GameObject* PrefabManager::instantiate(std::string name, Scene* scene, Transform* transform,
                                       std::vector<std::pair<std::string, CompMap>>& idAndVars) {
    if (load) {
        if (isPrefab(name)) {
            GameObject* gameObject = prefabsL.at(name)->InstantiatePrefab(scene, transform);
            gameObject->addComponents(idAndVars);
            return gameObject;
        }
        else
            return nullptr;
    }
    else
        return nullptr;
}

}
