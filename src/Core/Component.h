#pragma once

namespace Tapioca {
class GameObject;

class Component {
protected:
    GameObject* parent;
    bool alive;
    bool active;   //indica si el componente esta activo (si se actualizan update,handleEvents,...)
    const uint32_t& deltaTime;
public:
    Component(GameObject* parent, std::string id, bool active = true);
    virtual ~Component() { }

    inline bool isAlive() const { return alive; }     //para comprobar su existencia, en caso contrario se borra
    inline bool isActive() const { return active; }   //para comprobar si esta activo, en caso contrario no se actualiza
    virtual void setActive(bool b);   //activar/desactivar componente

    virtual void update() { }
    virtual void handleEvents() { }
    virtual void initComponent() { }
};
}