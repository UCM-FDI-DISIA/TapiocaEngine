#pragma once
#include "Utilities/Singleton.h"
#include <unordered_map>
#include "componentDefs.h"

namespace Tapioca {
class Transform;
class Scene;

/**
* @brief Gestor encargado de los prefabs disponibles y crear instancias de los mismos.
*/
class TAPIOCA_API PrefabManager : public Singleton<PrefabManager> {
private:
    bool load = true;   // booleano que indica que la escena Prefabs ha cargado correctamente

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
    /**
    * @brief Crea una instancia del prefab en una escena
    * @param name Nombre del prefab (debe ser único)
    * @param scene Escena en la que se creará la instancia del prefab
    * @param idAndVars CompMap de nuevos componentes para esta instancia del prefab
    * @return Puntero al GameObject
    */
    GameObject* instantiate(std::string name, Scene* scene, Transform* transform,
                            std::vector<std::pair<std::string, CompMap>>& idAndVars);
    /**
    * @brief Indica a PrefabManager si la escena Prefab a cargado correctamente
    * @param loaded valor que indica el estado de la carga de la escena 
    */
    inline void setLoad(bool loaded) { load = loaded; };
};
}