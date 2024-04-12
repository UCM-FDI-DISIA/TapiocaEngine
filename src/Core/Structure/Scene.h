#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include "defs.h"

namespace Tapioca {
class GameObject;

class TAPIOCA_API Scene {
private:
    friend class GameObject;
    // warning C4251 'Tapioca::Scene::objects' :
    // class 'std::vector<Tapioca::GameObject *,std::allocator<Tapioca::GameObject *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Scene'
    // warning C4251 'Tapioca::Scene::handlers' :
    // class 'std::unordered_map<std::string,Tapioca::GameObject *,std::hash<std::string>,std::equal_to<std::string>,
    // std::allocator<std::pair<const std::string,Tapioca::GameObject *>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Scene'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::vector<GameObject*> objects;                        // Objetos que tiene la escena
    std::unordered_map<std::string, GameObject*> handlers;   // Relaciones entre handlers y objetos
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    // FALTA WARNING C4251 PARA name
    std::string name;
    // FALTA WARNING C4251 PARA layers
    std::map<int, std::vector<GameObject*>> layers;         // Objetos que tiene la escena por orden de capa

    bool active;

public:
    /*
    * @brief Constructora de la clase Scene
    */
    Scene(std::string const& name);
    /*
    * @brief Destructora de la clase Scene
    */
    ~Scene();
    /*
    * @brief Devuelve los objetos de esta escena
    * @return Vector que contiene punteros a los objetos de esta escena
    */
    std::vector<GameObject*> getObjects() const;
    /*
    * @brief Devuelve el objeto al que esta asociado un handler. Si no encuentra un objeto, devuelve nullptr
    * @param handler Handler del objeto que se quiere obtener
    * @return Objeto asociado al handler o nullptr
    */
    GameObject* getHandler(std::string const& handler) const;

    /*
    * @brief Aniade un objeto a la escena
    * @param object Objeto que se quiere aniadir a la escena
    * @param handler Handler que se quiere asociar al objeto que se quiere aniadir. Los handlers deben ser unicos
    * @return True se se ha aniadido correctamente a la escena, false en caso contrario
    */
    bool addObject(GameObject* const object, std::string const& handler = "", int zIndex = 0);
    /*
    * @brief Inicializa los objetos de la escena. Se ejecuta antes del start
    */
    void awake();
    /*
    * @brief Inicializa los objetos de la escena
    */
    void start();
    /*
    * @brief Actualiza los objetos activos
    * @param deltaTime Tiempo que ha pasado desde el ultimo update
=   */
    void update(const uint64_t deltaTime);
    /*
    * @brief Actualiza las componentes activas del objeto.
    * Se llama cada cierto tiempo fijo (Game::FIXED_DELTA_TIME),
    */
    void fixedUpdate();
    /*
    * @brief Renderiza los objetos de la escena
    */
    void render() const;
    /*
    * @brief Elimina los objetos muertos de la escena ademas de todas sus componentes
=   */
    void refresh();
    /*
    * @brief Procesa un evento recibido
    * @param id String que indica el tipo de evento
    * @param info Parametros del mensaje, cada evento gestiona sus propios parametros
    */
    void handleEvent(std::string const& id, void* info);
    /*
    * @brief Metodo que se usa para enviar un evento
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    * @param global Indica si el evento debe ser enviado glabalmente
    */
    void pushEvent(std::string const& id, void* info);

    /*
    * @brief Devuelve el nombre de la escena
    * @return Nombre de la escena
    */
    std::string const& getName() const;

    /*
    * @brief Establece si la escena esta activa
    * @param a True si se quiere activar la escena, false en caso contrario
    */
    void setActive(const bool a);
    /*
    * @brief Devuelve si la escena esta activa
    * @return True si la escena esta activa, false en caso contrario
    */
    bool isActive() const;

    /*
    * @brief Actualiza el zIndex de un objeto
    * @param obj Objeto al que se le quiere cambiar el zIndex
    * @param zIndex Nuevo zIndex del objeto
    */
    void updateZIndex(GameObject* obj, int zIndex);
};
}
