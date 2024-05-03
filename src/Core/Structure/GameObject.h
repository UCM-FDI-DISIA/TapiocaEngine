#pragma once
#include <vector>
#include "Utilities/Concepts.h"
#include "defs.h"
#include "componentDefs.h"

namespace Tapioca {
class Component;
class Scene;

/**
* @brief Clase que representa un objeto en el juego
*/
class TAPIOCA_API GameObject {
private:
    friend class Scene;

    Scene* scene;   // Escena a la que pertenece el objeto
    bool alive;     // Indica si se deberia borrar el objeto
                    // warning C4251 'Tapioca::GameObject::handler' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::GameObject'
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
    std::string handler;                                           // Handler del objeto
    std::unordered_multimap<std::string, Component*> components;   // Componentes del objeto, cada uno asociado a una id
    std::vector<Component*> cmpOrder;   // Vector que contiene los componentes en el orden en el que se han anadido
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    /**
    * @brief Actualiza los componentes activos del objeto
    * @param deltaTime Tiempo que ha pasado desde el ultimo update
    */
    void update(const uint64_t deltaTime);
    /**
    * @brief Actualiza los componentes activos del objeto
    * Se llama cada cierto tiempo fijo (Game::FIXED_DELTA_TIME)
    */
    void fixedUpdate();
    /**
    * @brief Renderiza los componentes del objeto
    */
    void render() const;
    /**
    * @brief Elimina los componentes muertos del objeto
    */
    void refresh();

    /**
    * @brief Elimina una componente del objeto
    * @param comp Componente que se quiere eliminar
    */
    void deleteCompVector(Component* const comp);

public:
    /**
    * @brief Inicializa en true el atributo alive
    */
    GameObject();
    /**
    * @brief Elimina los componentes del objeto
    */
    ~GameObject();

    /**
    * @brief Inicializa los componentes del objeto. Se ejecuta antes del start.
    * Garantiza que todos los componentes iniciales esten creados
    */
    void awake();
    /**
    * @brief Inicializa los componentes del objeto.
    * Garantiza que todos los componentes iniciales esten creados
    */
    void start();
    /**
    * @brief Procesa un evento recibido
    * @param id Indica el tipo de evento
    * @param info Parametros del mensaje, cada evento gestiona sus propios parametros
    */
    void handleEvent(std::string const& id, void* info);

    /**
    * @brief Devuelve el handler del objeto
    * @return Handler del objeto
    */
    inline std::string getHandler() const { return handler; }

    /**
    * @brief Devuelve si el objeto esta "vivo" (si se actualizan update, handleEvents,... y si se va a borrar).
    * @return true si esta "vivo", false en caso contrario
    */
    inline bool isAlive() const { return alive; }
    /**
    * @brief Marca al objeto como "muerto" (se dejan de actualizar update, handleEvents,...) y se borra
    */
    inline void die() { alive = false; }

    /**
    * @brief Cambia la escena a la que pertence el gameObject
    * @param sc Escena a la que se quiere mover el objeto
    */
    inline void setScene(Scene* const sc) { scene = sc; }
    /**
    * @brief Devuelve puntero de la escena a la que pertenece el objeto
    * @return Escena a la que pertenece el objeto
    */
    inline virtual Scene* getScene() const { return scene; }

    /**
    * @brief Anade un componente al objeto, no se deberia de usar en ejecucion
    * @param comp Componente que se quiere anadir al objeto
    * @param id Id del componente que se quiere anadir
    */
    void addComponent(Component* const comp, std::string const& id);
    /**
    * @brief Anade un componente al objeto
    * @param id Id del componente que se quiere anadir
    * @param variables Variables con las que se inicializa el componente
    * @return Puntero al componente anadido, nullptr si no se ha podido inicializar
    */
    Component* addComponent(const std::string& id, const CompMap& variables = CompMap());
    /**
    * @brief Anade un componente al objeto
    * @param variables Variables con las que se inicializa el componente
    * @return Puntero al componente anadido, nullptr si no se ha podido inicializar
    */
    template<IsComponent TComp>
    inline TComp* addComponent(const CompMap& variables = CompMap()) {
        TComp* comp = new TComp();
        if (!comp->initComponent(variables)) {
            delete comp;
            return nullptr;
        }
        addComponent(comp, TComp::id);
        comp->awake();
        comp->start();
        return comp;
    }
    /**
    * @brief Anade varios componentes al objeto
    * @param idsAndVariables Pareja de ids y variables para los componentes
    * @return Vector de punteros. Si no se ha podido inicializar un componente, se cancela la operacion completamente,
    * borrando los componentes ya inicializados y devolviendo un vector vacio
    */
    std::vector<Component*> addComponents(const std::vector<std::pair<std::string, CompMap>>& idsAndVariables);
    /**
    * @brief Devuelve un puntero a un componente del objeto
    * @param id Id del componente que se quiere conseguir del objeto
    * @return Puntero al componente. nullptr si el objeto no contiene el componente
    */
    Component* getComponent(std::string const& id);
    /**
    * @brief Devuelve un puntero a un componente del objeto
    * @return Puntero al componente. nullptr si el objeto no contiene el componente
    */
    template<IsComponent TComp>
    inline TComp* getComponent() {
        auto it = components.find(TComp::id);
        if (it == components.end()) return nullptr;
        return static_cast<TComp*>(it->second);
    }
    /**
    * @brief Devuelve todos los componentes de este objeto
    * @return Vector que contiene los punteros de todos los componentes de este objeto
    */
    std::vector<Component*> getAllComponents();
    /**
    * @brief Devuelve todos los componentes del objeto con la id marcada
    * @param id Id de los componentes que se quieren obtener
    * @return Vector que contiene los punteros de todos los componentes con la id marcada
    */
    std::vector<Component*> getComponents(std::string const& id);
    /**
    * @brief Devuelve todos los componentes del objeto de la misma clase
    * @return Vector que contiene los punteros de todos los componentes de la clase indicada
    */
    template<IsComponent TComp>
    inline std::vector<TComp*> getComponents() {
        std::vector<TComp*> out;

        for (auto& comp : components) {
            if (comp.first == TComp::id) out.push_back(static_cast<TComp*>(comp.second));
        }

        return out;
    }
    /**
    * @brief Metodo que se usa para enviar un evento
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    * @param global Indica si el evento debe ser enviado glabalmente
    * @param delay Indica si el evento se recibira un ciclo mas tarde. Si es false, se recibe immediatamente
    */
    void pushEvent(std::string const& id, void* info, const bool global = true, const bool delay = false);
};
}
