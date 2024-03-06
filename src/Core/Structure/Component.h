#pragma once
#include <unordered_set>
#include "Utilities/defs.h"
#include "Utilities/componentDefs.h"

namespace Tapioca {
class GameObject;

class TAPIOCA_API Component {
private:
    std::string id;
    // Lista de atributos que tiene el componente
    // TODO: Para que sirve esto?????
    std::unordered_map<std::string, std::variant<int, std::string, float, bool>> attributes;

    friend class GameObject;
protected:
    GameObject* object;
    bool alive;
    bool active;   // Indica si el componente esta activo (si se actualizan update,handleEvents,...)
    static std::unordered_set<Component*> listeners;

public:
    Component();
    virtual ~Component() { }

    // Devuelve el objeto donde esta este componente
    inline GameObject* getObject() const { return object; }

    // Devuelve si el componente esta "vivo" (si es 'false' se va a borrar).
    inline bool isAlive() const { return alive; }
    // Devuelve si el componente esta activo (si es 'false' no se llama a ningun metodo excepto 'receiveEvent').
    inline bool isActive() const { return active; }
    // Activa o desactiva el componente (si es 'false' no se llama a ningun metodo excepto 'receiveEvent').
    inline virtual void setActive(const bool b) { active = b; }

    // Metodo que se usa para recibir los parametros iniciales y guardarlos.
    // No garantiza que todos los componentes iniciales esten creados.
    virtual bool initComponent(const CompMap& variables) = 0;
    // Metodo que se usa para inicializar el componente.
    // Garantiza que todos los componentes iniciales esten creados.
    virtual void start() { }
    // Metodo que se usa para actualizar el estado de un componente. Se llama todas las veces posibles,
    // y el parametro deltaTime contiene cuanto tiempo ha pasado desde la ultima llamada.
    virtual void update(const uint64_t deltaTime) { }
    // Metodo que se usa para actualizar el estado de un componente.
    // Se llama cada cierto tiempo fijo (Game::FIXED_DELTA_TIME),
    // asi que no tiene parametro para indicar cuanto tiempo ha pasado desde la ultima llamada.
    virtual void fixedUpdate() { }
    // Metodo que se usa para recibir eventos. El parametro 'info' se usa para pasar informacion si hace falta.
    // TODO: Incluir cuando se llama.
    virtual void handleEvent(std::string const& id, void* info) { }

    void pushEvent(std::string const& id, void* info, const bool global = true);

    template<typename T>
    inline bool setValueFromMap(T& var, std::string const& varName, const CompMap& map) {
        auto v = map.find(varName);
        if (v == map.end()) {
#ifdef _DEBUG
            std::cerr << "Nombre de variable no encontrado en el mapa: " << varName << '\n';
#endif
            return false;
        }

        // Si la variante no tiene el tipo T
        if (!std::holds_alternative<T>(v->second)) {
#ifdef _DEBUG
            std::cerr << "Incompatibilidad de tipo.\n";
#endif
            return false;
        }

        var = std::get<T>(v->second);
        return true;
    }
};

#define COMPONENT_ID(_id) inline static const std::string id = _id;
}
