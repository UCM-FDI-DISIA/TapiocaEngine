#pragma once
#include <unordered_set>
#include "defs.h"
#include "componentDefs.h"

namespace Tapioca {
class GameObject;

class TAPIOCA_API Component {
private:
    friend class GameObject;

protected:
    GameObject* object;   // Gameobject al que esta adjunto este componente
    bool alive;           // Indica si se deberia borrar el componente
    bool active;          // Indica si el componente esta activo (si se actualizan update y fixedUpdate)

public:
    /*
    * @brief Constructor de la clase Component
    */
    Component();
    /*
    * @brief Destructora de la clase Component
    */
    virtual ~Component() { }

    /*
    * @brief Recibe los parametros iniciales y se inicializan los atributos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return true si se ha inicializado correctamente, false si no
    */
    virtual bool initComponent(const CompMap& variables) { return true; }
    /*
    * @brief Inicializa el componente. Se ejecuta antes que el start
    * Garantiza que todos los componentes iniciales esten creados
    */
    virtual void awake() { }
    /*
    * @brief Inicializa el componente.
    * Garantiza que todos los componentes iniciales esten creados
    */
    virtual void start() { }
    /*
    * @brief Actualiza el estado de un componente. Se llama todas las veces posibles
    * @param deltaTime Tiempo que ha pasado desde la ultima llamada
    */
    virtual void update(const uint64_t deltaTime) { }
    /*
    * @brief Actualiza el estado de un componente
    * Se llama cada cierto tiempo fijo (Game::FIXED_DELTA_TIME),
    */
    virtual void fixedUpdate() { }
    /*
    * @brief Renderizado de un componente
    */
    virtual void render() const { }
    /*
    * @brief Recibe eventos.
    * Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent.
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    */
    virtual void handleEvent(std::string const& id, void* info) { }
    /*
    * @brief Envia un evento
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    * @param global Indica si el evento debe ser enviado glabalmente
    */
    void pushEvent(std::string const& id, void* info, const bool global = true, const bool delay = false);

    /*
    * @brief Da valor a una variable declarada en un CompMap
    * @param var Tipo de variable que se quiere actualizar
    * @param varName Nombre de la variable que se quiere actualizar
    * @param map Unordered_map en el que se que se quiere actualizar
    * @return true si se ha ejecutado correctamente, false si no
    */
    template<typename T>
    inline bool setValueFromMap(T& var, std::string const& varName, const CompMap& map) {
        auto v = map.find(varName);
        if (v == map.end()) {
            logWarn(("SetValueFromMap: Nombre de variable \"" + varName + "\" no encontrado en el mapa.").c_str());
            return false;
        }

        // Si la variante no tiene el tipo T
        if (!std::holds_alternative<T>(v->second)) {
            logError(("SetValueFromMap: Incompatibilidad de tipo en la variable \"" + varName + "\".").c_str());
            return false;
        }

        var = std::get<T>(v->second);
        return true;
    }

    /*
    * @brief Devuelve el objeto donde esta este componente
    * @return Objeto al que esta adjunto este componente
    */
    inline GameObject* getObject() const { return object; }

    /*
    * @brief Devuelve si el componente esta "vivo" (si se actualizan update,handleEvents,...)
    * @return true si esta "vivo", false si no
    */
    inline bool isAlive() const { return alive; }
    /*
    * @brief Devuelve si el componente esta activo (si es 'false' no se llama a ningun metodo excepto 'receiveEvent')
    * @return true si esta activo, false si no
    */
    inline bool isActive() const { return active; }
    /*
    * @brief Activa o desactiva el componente (si es 'false' no se llama a ningun metodo excepto 'receiveEvent')
    * @param b true si se quiere activar, false si no
    */
    inline virtual void setActive(const bool b) { active = b; }
};
/*
* @brief Id del componente
*/
#define COMPONENT_ID(_id) inline static const char* id = _id;
}
