#pragma once
#include <unordered_set>
#include "Utilities/defs.h"
#include "Utilities/componentDefs.h"

namespace Tapioca {
class GameObject;

class TAPIOCA_API Component {
private:
    std::string id;

    friend class GameObject;
protected:
    GameObject* object;                                 // Gameobject al que esta adjunto esta componente
    bool alive;                                         // Indica si se deberia borrar la componente
    bool active;                                        // Indica si la componente esta activa (si se actualizan update,handleEvents,...)
    //static std::unordered_set<Component*> listeners;    // Contiene las componentes que reciben mensajes y eventos 

public:
    /*
    * @brief Constructora de la clase Component
    */
    Component();
    /*
    * @brief Destructora de la clase Component
    */
    virtual ~Component() { }

    /*
    * @brief Devuelve el objeto donde esta este componente
    * @return Objeto al que esta adjunto esta componente
    */
    inline GameObject* getObject() const { return object; }

    /*
    * @brief Devuelve si el componente esta "vivo" (si se actualizan update,handleEvents,...)
    * @return True si esta "vivo", false en caso contrario
    */
    inline bool isAlive() const { return alive; }
    /*
    * @brief Devuelve si el componente esta activo (si es 'false' no se llama a ningun metodo excepto 'receiveEvent')
    * @return True si esta activo, false en caso contrario
    */
    inline bool isActive() const { return active; }
    /*
    * @brief Activa o desactiva el componente (si es 'false' no se llama a ningun metodo excepto 'receiveEvent')
    * @param b true si se quiere activar, false en caso contrario
    */
    inline virtual void setActive(const bool b) { active = b; }

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    virtual bool initComponent(const CompMap& variables) = 0;
    /*
    * @brief Metodo que se usa para inicializar el componente.
    * Garantiza que todos los componentes iniciales esten creados
    */
    virtual void start() { }
    /*
    * @brief Metodo que se usa para actualizar el estado de un componente. Se llama todas las veces posibles
    * @param deltaTime Tiempo que ha pasado desde la ultima llamada
    */
    virtual void update(const uint64_t deltaTime) { }
    /*
    * @brief Metodo que se usa para actualizar el estado de un componente
    * Se llama cada cierto tiempo fijo (Game::FIXED_DELTA_TIME),
    */
    virtual void fixedUpdate() { }
    /*
    * @brief Metodo que se usa para recibir eventos.
    * Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent.
    * @param Id indica el tipo de mensaje
    * @param info puntero a void para pasar parametros
    */
    virtual void handleEvent(std::string const& id, void* info) { }
    /*
    * @brief Metodo que se usa para enviar un evento
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    * @param global Indica si el evento debe ser enviado glabalmente
    */
    void pushEvent(std::string const& id, void* info, const bool global = true);

    template<typename T>
    /*
    * @brief Metodo que se usa para actualizar el valor de una variable en un CompMap
    * @param var Tipo de variable que se quiere actualizar
    * @param varName Nombre de la variable que se quiere actualizar
    * @param map Unordered_map en el que se que se quiere actualizar
    * @return Devuelve true si se ha actualizado correctamente, false en caso contrario
    */
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
    /*
    * @brief Id de la componente
    */
#define COMPONENT_ID(_id) inline static const std::string id = _id;
}
