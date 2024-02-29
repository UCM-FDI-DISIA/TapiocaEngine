#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

namespace Tapioca {
class GameObject;

class Component {
private:
    std::string id;
    std::unordered_map<std::string, std::variant<int, std::string, float, bool>>
        attributes;   //lista de atributos que tiene el componente

    friend class GameObject;
    void setParent(GameObject* obj);

protected:
    enum messageType { MESSAGE_A, MESSAGE_B, MESSAGE_C, MESSAGE_D };
    GameObject* parent;
    bool alive;
    bool active;   //indica si el componente esta activo (si se actualizan update,handleEvents,...)
    static std::unordered_set<Component*> listeners;

public:
    using CompMap = std::unordered_map<std::string, std::variant<char, int, float, bool, std::string>>;
    // Para convertir el nombre de la variable en un string
    #define VAR_NAME_TO_STRING(var) #var

    Component();
    virtual ~Component() { }

    GameObject* getParent();

    inline bool isAlive() const { return alive; }     //para comprobar su existencia, en caso contrario se borra
    inline bool isActive() const { return active; }   //para comprobar si esta activo, en caso contrario no se actualiza
    virtual void setActive(bool b) { active = b; }    //activar/desactivar componente

    template<typename T> 
    inline void setValueFromMap(T& var, const CompMap& map) {
        auto v = map.find(VAR_NAME_TO_STRING(var));
        if (v != map.end()) {
            try {
                var = std::get<T>(v->second);
            } catch (const std::bad_variant_access&) {
                std::cerr << "Error al obtener el valor para la variable: " << VAR_NAME_TO_STRING(var)
                          << " - Incompatibilidad de tipo.\n";
            }
        } else {
            std::cerr << "Nombre de variable no encontrado en el mapa: " << VAR_NAME_TO_STRING(var) << '\n';
        }
    }

    virtual void update(const uint64_t deltaTime) { }
    virtual void handleEvents() { }
    virtual void initComponent(const CompMap& variables) = 0;
    virtual void fixedUpdate() { }

    virtual void onCollisionEnter(GameObject* other) {};
    virtual void onCollisionExit(GameObject* other) {};
    virtual void onCollisionStay(GameObject* other) {};

    static void addListener(Component* component);                       //Añadir componente como listener
    static void removeListener(Component* component);                    //Quitar componente como listener
    void sendEvent(Component* component, messageType m);                 //Funcion para enviar mensajes
    virtual void receiveEvent(Component* component, messageType m) {};   //Funcion para recibir mensajes
};

#define COMPONENT_ID(_id) inline static const std::string id = _id;
}
