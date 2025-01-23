#pragma once
//#include <vector>
#include <unordered_set>
#include <string>
#include <unordered_map>
#include <map>
#include "defs.h"

namespace Tapioca {
class GameObject;

/**
* @brief Clase que representa una escena del juego
*/
class TAPIOCA_API Scene {
private:
    friend class GameObject;

    bool active;                  // Indica si la escena esta activa

    // warning C4251 'Tapioca::Scene::objects' :
    // class 'std::vector<Tapioca::GameObject *,std::allocator<Tapioca::GameObject *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Scene'
    // warning C4251 'Tapioca::Scene::handlers' :
    // class 'std::unordered_map<std::string,Tapioca::GameObject *,std::hash<std::string>,std::equal_to<std::string>,
    // std::allocator<std::pair<const std::string,Tapioca::GameObject *>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Scene'
    // warning C4251 'Tapioca::Scene::name' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Scene'
    // warning C4251 'Tapioca::Scene::layers' :
    // class 'std::map<int,std::vector<Tapioca::GameObject *,std::allocator<Tapioca::GameObject *>>,std::less<int>,
    // std::allocator<std::pair<const int,std::vector<Tapioca::GameObject *,std::allocator<Tapioca::GameObject *>>>>>'
    // necesita tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Scene'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_set<GameObject*> objects;                         // Objetos que tiene la escena
    std::unordered_map<std::string, GameObject*> handlers;           // Relaciones entre handlers y objetos
    std::string name;                                                // Nombre de la escena
    std::map<int, std::unordered_set<GameObject*>> layers;           // Objetos que tiene la escena por orden de capa
    std::vector<std::pair<std::string, GameObject*>> lInstantiate;   // Prefabs a instanciar en la escena
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

public:
    /**
    * @brief Constructor de la clase Scene
    */
    Scene(std::string const& name);
    /**
    * @brief Destructor de la clase Scene
    */
    ~Scene();
    /**
    * @brief Devuelve los objetos de esta escena
    * @return Vector que contiene punteros a los objetos de esta escena
    */
    inline std::unordered_set<GameObject*> getObjects() const { return objects; }
    /**
    * @brief Devuelve el objeto al que esta asociado un handler. Si no encuentra un objeto, devuelve nullptr
    * @param handler Handler del objeto que se quiere obtener
    * @return Objeto asociado al handler o nullptr
    */
    GameObject* getHandler(std::string const& handler) const;

    /**
    * @brief Anade un objeto a la escena
    * @param object Objeto que se quiere anadir a la escena
    * @param handler Handler que se quiere asociar al objeto que se quiere anadir. Los handlers deben ser unicos
    * @param zIndex Capa en la que se quiere anadir el objeto
    * @return true se se ha anadido correctamente a la escena, false en caso contrario
    */
    bool addObject(GameObject* const object, std::string const& handler = "", int zIndex = 0);
    /**
    * @brief Inicializa los objetos de la escena. Se ejecuta antes del start
    */
    void awake();
    /**
    * @brief Inicializa los objetos de la escena
    */
    void start();
    /**
    * @brief Actualiza los objetos activos
    * @param deltaTime Tiempo que ha pasado desde el ultimo update
=   */
    void update(const uint64_t deltaTime);
    /**
    * @brief Actualiza las componentes activas del objeto.
    * Se llama cada cierto tiempo fijo (Game::FIXED_DELTA_TIME),
    */
    void fixedUpdate();
    /**
    * @brief Renderiza los objetos de la escena
    */
    void render() const;
    /**
    * @brief Elimina los objetos muertos de la escena ademas de todas sus componentes
    */
    void refresh();
    /**
    * @brief Procesa un evento recibido
    * @param id String que indica el tipo de evento
    * @param info Parametros del mensaje, cada evento gestiona sus propios parametros
    */
    void handleEvent(std::string const& id, void* info);
    /**
    * @brief Metodo que se usa para enviar un evento
    * @param e Evento que se quiere enviar
    * @param delay Indica si se quiere enviar el evento en el siguiente frame
    */
    void pushEvent(Event const& e, bool const delay);

    /**
    * @brief Devuelve el nombre de la escena
    * @return Nombre de la escena
    */
    inline std::string const& getName() const { return name; }

    /**
    * @brief Establece si la escena esta activa
    * @param a True si se quiere activar la escena, false en caso contrario
    */
    inline void setActive(const bool a) { active = a; }
    /**
    * @brief Devuelve si la escena esta activa
    * @return True si la escena esta activa, false en caso contrario
    */
    inline bool isActive() const { return active; }

    /**
    * @brief Actualiza el zIndex de un objeto
    * @param obj Objeto al que se le quiere cambiar el zIndex
    * @param zIndex Nuevo zIndex del objeto
    */
    void updateZIndex(GameObject* obj, int zIndex);

    /**
    * @brief Anade un objeto para instanciar en el primer frame
    * @param name Nombre del prefab del que se instanciará el objeto
    * @param gameObject GameObject en el que se guarda el nuevo transform del prefab
    */
    void addInstance(std::string name, GameObject* gameObject);
};
}
