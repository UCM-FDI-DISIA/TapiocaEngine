#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

#ifdef _DEBUG
#include <iostream>
#endif;

namespace Tapioca {
class GameObject;

class Component {
private:
    std::string id;
    //lista de atributos que tiene el componente
    std::unordered_map<std::string, std::variant<int, std::string, float, bool>> attributes;   

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
    inline virtual void setActive(bool b) { active = b; }   //activar/desactivar componente

    template<typename T> 
    inline void setValueFromMap(T& var, std::string varName, const CompMap& map) {
        auto v = map.find(varName);
        if (v != map.end()) {
            // Si la variante tiene el tipo T
            if (std::holds_alternative<T>(v->second)) var = std::get<T>(v->second);
#ifdef _DEBUG 
            else std::cerr << "Incompatibilidad de tipo.\n";
#endif
            /*
            try {
                var = std::get<T>(v->second);
            } 
            catch (const std::bad_variant_access&) {
#ifdef _DEBUG
                std::cerr << "Error al obtener el valor para la variable: " << varName
                          << " - Incompatibilidad de tipo.\n";
#endif
            }
            */
        } 
#ifdef _DEBUG
        else std::cerr << "Nombre de variable no encontrado en el mapa: " << varName << '\n';
#endif
    }

    virtual void update(const uint64_t deltaTime) { }
    virtual void handleEvents() { }
    virtual void initComponent(const CompMap& variables) = 0;
    virtual void fixedUpdate() { }
    virtual void start() { }

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
