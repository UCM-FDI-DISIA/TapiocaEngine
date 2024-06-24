#include "PrefabManager.h"
#include "GameObject.h"

namespace Tapioca {
PrefabManager::PrefabManager() { }

PrefabManager::~PrefabManager() { }

bool PrefabManager::addPrefab(std::string name, GameObject* prefab) {
    if (prefabsL.count(name) == 0) {
        prefabsL[name] = prefab;
        return true;
    }
    else
        return false;
}

bool PrefabManager::isPrefab(std::string name) { return prefabsL.count(name) == 1; }

GameObject* PrefabManager::getPrefab(std::string name) { return prefabsL.at(name); }

GameObject* PrefabManager::instantiate(std::string name, Scene* scene, Transform* transform) {
    return prefabsL.at(name)->InstantiatePrefab(scene, transform);
}
}
