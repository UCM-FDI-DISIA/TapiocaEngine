#pragma once
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

protected:
    enum messageType { MESSAGE_A, MESSAGE_B, MESSAGE_C, MESSAGE_D };
    GameObject* parent;
    bool alive;
    bool active;   //indica si el componente esta activo (si se actualizan update,handleEvents,...)
    const uint32_t& deltaTime;
    const uint32_t& fixedDeltaTime;
    static std::unordered_set<Component*> listeners;

public:
    Component();
    virtual ~Component() { }

    void setParent(GameObject* obj);
    GameObject* getParent();

    inline bool isAlive() const { return alive; }     //para comprobar su existencia, en caso contrario se borra
    inline bool isActive() const { return active; }   //para comprobar si esta activo, en caso contrario no se actualiza
    virtual void setActive(bool b) { active = b; }    //activar/desactivar componente

    virtual void update() { }
    virtual void handleEvents() { }
    virtual void initComponent() { }
    virtual void fixedUpdate() { }

    virtual void onCollisionEnter(GameObject* other) {};
    virtual void onCollisionExit(GameObject* other) {};
    virtual void onCollisionStay(GameObject* other) {};

    static void addListener(Component* component);                       //Añadir componente como listener
    static void removeListener(Component* component);                    //Quitar componente como listener
    void sendEvent(Component* component, messageType m);                 //Funcion para enviar mensajes
    virtual void receiveEvent(Component* component, messageType m) {};   //Funcion para recibir mensajes
};
}
