#pragma once
#include "Utilities/Singleton.h"
#include <unordered_map>

namespace Tapioca {
class Transform;
class Scene;

class TAPIOCA_API PrefabManager : public Singleton<PrefabManager> {
private:
    // warning C4251 'Tapioca::PrefabManager::prefabsL' :
    // class 'std::unordered_map<std::string,Tapioca::GameObject *,std::hash<std::string>,std::equal_to<std::string>,std::allocator<std::pair<const std::string,Tapioca::GameObject *>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::PrefabManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_map<std::string, GameObject*> prefabsL;   // Prefabs cargados
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
public:
    PrefabManager();

    ~PrefabManager();
    /**
    * @brief Anade el prefab como uno válido para instanciar
    * @param name Nombre del prefab (debe ser único)
    * @param g GameObject que representa al prefab
    * @return true si se ha añadido correctamente (no hay otro prefab con el mismo nombre) y false en caso contrario
    */
    bool addPrefab(std::string name, GameObject* g);
    /**
    * @brief Indica si existe un prefab con dicho nombre
    * @param name Nombre del prefab (debe ser único)
    * @return true si existe un prefab con dicho nombre y false en caso contrario
    */
    bool isPrefab(std::string name);
    /**
    * @brief Elimina un prefab
    * @param name Nombre del prefab (debe ser único)
    */
    void erasePrefab(std::string name);
    /**
    * @brief Indica si existe un prefab con dicho nombre
    * @param name Nombre del prefab (debe ser único)
    * @return true si existe un prefab con dicho nombre y false en caso contrario
    */
    GameObject* getPrefab(std::string name);
    /**
    * @brief Crea una instancia del prefab en una escena
    * @param name Nombre del prefab (debe ser único)
    * @param scene Escena en la que se creará la instancia del prefab
    * @return Puntero al GameObject
    */
    GameObject* instantiate(std::string name, Scene* scene, Transform* transform);
};
}