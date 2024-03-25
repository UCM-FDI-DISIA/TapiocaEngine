#pragma once
#include <vector>
#include "Utilities/Concepts.h"
#include "defs.h"
#include "componentDefs.h"

namespace Tapioca {
class Component;
class Scene;

class TAPIOCA_API GameObject {
private:
    friend class Scene;
    /*
    * @brief Cambia la escena a la que pertence el gameObject
    * @param sc Escena a la que se quiere mover el objeto
    */
    void setScene(Scene* const sc);

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    void initComponents(const CompMap& variables);
    /*
    * @brief Inicializa las componentes del objeto. Se ejecuta antes del start
    * Garantiza que todas las componentes iniciales esten creados
    */
    void awake();
    /*
    * @brief Inicializa las componentes del objeto
    * Garantiza que todas las componentes iniciales esten creados
    */
    void start();
    /*
    * @brief Actualiza las componentes activas del objeto
    * @param deltaTime Tiempo que ha pasado desde el ultimo update
=   */
    void update(const uint64_t deltaTime);
    /*
    * @brief Actualiza las componentes activas del objeto.
    * Se llama cada cierto tiempo fijo (Game::FIXED_DELTA_TIME),
    */
    void fixedUpdate();
    /*
    * @brief Renderiza las componentes del objeto
    */
    void render() const;
    /*
    * @brief Procesa un evento recibido
    * @param id String que indica el tipo de evento
    * @param info Parametros del mensaje, cada evento gestiona sus propios parametros
    */
    void handleEvent(std::string const& id, void* info);
    /*
    * @brief Elimina las componentes muertas del objeto
=   */
    void refresh();
    

    Scene* scene;           // Escena a la que pertenece el objeto
    bool alive;             // Indica si se deberia borrar la componente
    // warning C4251 'Tapioca::GameObject::handler' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::GameObject'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string handler;    // Handler del objeto
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    /*
    * @brief Elimina una componente del objeto
    * @param comp Componente que se quiere eliminar
    */
    void deleteCompVector(Component* const comp);
    // warning C4251 'Tapioca::GameObject::components' :
    // class 'std::unordered_multimap<std::string,Tapioca::Component *,std::hash<std::string>,
    // std::equal_to<std::string>,std::allocator<std::pair<const std::string,Tapioca::Component *>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::GameObject'
    // warning C4251 'Tapioca::GameObject::cmpOrder' :
    // class 'std::vector<Tapioca::Component *,std::allocator<Tapioca::Component *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::GameObject'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_multimap<std::string, Component*> components;    // Componentes que tiene el objeto, cada una asociada a una id
    std::vector<Component*> cmpOrder;   // TODO: Hace falta??????
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
public:
    /*
    * @brief Constructora de la clase GameObject
    */
    GameObject();
    /*
    * @brief Destructora de la clase GameObject
    */
    ~GameObject();

    /*
    * @brief Devuelve el handler del objeto
    * @return Handler del objeto
    */
    inline std::string getHandler() const { return handler; }

    /*
    * @brief Devuelve si el objeto esta "vivo" (si se actualizan update,handleEvents,...)
    * @return True si esta "vivo", false en caso contrario
    */
    inline bool isAlive() const { return alive; }
    /*
    * @brief Marca al objeto como "muerto" (se dejan de actualizar update,handleEvents,...)
    */
    inline void die() { alive = false; }
    /*
    * @brief Devuelve puntero de la escena a la que pertenece el objeto
    * @return Escena a la que pertenece el objeto
    */
    inline virtual Scene* getScene() const { return scene; }

    /*
    * @brief Aniade una componente al objeto
    * @param comp Componente que se quiere aniadir al objeto
    * @param id Id de la componenete que se quiere aniadir
    */
    void addComponent(Component* const comp, std::string const& id);
    /*
    * @brief Aniade una componente al objeto
    */
    template<IsComponent TComp> inline TComp* addComponent() {
        TComp* comp = new TComp();
        addComponent(comp, TComp::id); 
        return comp;
    }
    /*
    * @brief Devuelve un puntero a una componente del objeto
    * @param id Id de la componente que se quiere conseguir del objeto
    * @return Puntero al componente. nullptr si el objeto no contiene la componente
    */
    Component* getComponent(std::string const& id);
    /*
    * @brief Devuelve un puntero a una componente del objeto
    * @return Puntero al componente. nullptr si el objeto no contiene la componente
    */
    template<IsComponent TComp> inline TComp* getComponent() {
        auto it = components.find(TComp::id);
        if (it == components.end()) return nullptr;
        return static_cast<TComp*>(it->second);
    }
    /*
    * @brief Devuelve los hijos directos de este objeto
    * @return Vector que contiene los punteros los hijos directos de este objeto
    */
    std::vector<Component*> getAllComponents();
    /*
    * @brief Devuelve los hijos directos de este transform
    * @return Vector que contiene los punteros de tanto los hijos directos como los indirectos de este objeto
    */
    std::vector<Component*> getComponents(std::string const& id);
    template<IsComponent TComp> inline std::vector<TComp*> getComponents() {
        std::vector<TComp*> out;

        for (auto& comp : components) {
            if (comp.first == TComp::id) out.push_back(static_cast<TComp*>(comp.second));
        }

        return out;
    }
    /*
    * @brief Metodo que se usa para enviar un evento
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    * @param global Indica si el evento debe ser enviado glabalmente
    */
    void pushEvent(std::string const& id, void* info, const bool global = true);
};
}
