#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>

namespace Tapioca {
class GameObject;

class Component {
private:
    std::string id;
    std::unordered_map<std::string, std::variant<int, std::string, float, bool>> attributes; //lista de atributos que tiene el componente

protected:
    GameObject* parent;
    bool alive;
    bool active;   //indica si el componente esta activo (si se actualizan update,handleEvents,...)
    const uint32_t& deltaTime;
    const uint32_t& fixedDeltaTime;
public:
    Component();
    virtual ~Component() { }

    inline bool isAlive() const { return alive; }    //para comprobar su existencia, en caso contrario se borra
    inline bool isActive() const { return active; }  //para comprobar si esta activo, en caso contrario no se actualiza
    virtual void setActive(bool b) { active = b; }   //activar/desactivar componente

    virtual void update() { }
    virtual void handleEvents() { }
    virtual void initComponent() { }
    virtual void fixedUpdate() { }
};
}
