#pragma once
#include "Structure/Component.h"
#include "Structure/ComponentBuilder.h"

// ----------- VALORES VALIDOS -----------
// Se usa CompValue para representar variables sacadas de Lua o usadas en Lua.
// Los tipos validos son char, int, float, bool, string y nullptr/nil.
// Realmente CompValue es un std::variant, asi que para coger un valor hay que usar los metodos de la biblioteca "std":
// std::get para coger un valor, std::holds_alternative para saber si el valor es capaz de convertirse a ese tipo.
// (Tambien se puede usar la funcion std::variant::index() para saber el tipo que tiene segun el indice.
//  Los indices siguen el orden especificado arriba.)

namespace luabridge {
class LuaRef;
}

namespace Tapioca {
class TAPIOCA_API LuaComponent : public Component { 
private:
    luabridge::LuaRef* objectTable;
    std::string name;

    void callSimpleFunction(const std::string& name);
    void registerFunctions();

public:
    // Esto no va a funcionar bien
    COMPONENT_ID("LuaComponent")
    /*
    * @brief Constructora del componente. Para crear un componente, usar addComponent en los GameObject con la id.
    */
    LuaComponent(luabridge::LuaRef* objectTable);
    /*
    * @brief Destructora del componente. Para borrar un componente, usar Component::die().
    */
    ~LuaComponent();
    /*
    * @brief Funcion para inicializar un componente. Se llama solo, no llamar manualmente.
    * Para saber como se implementa, revisar la clase Component.
    */
    bool initComponent(const CompMap& variables) override;
    /*
    * @brief Funcion para inicializar un componente. Se llama solo, no llamar manualmente.
    * Para saber como se implementa, revisar la clase Component.
    */
    void awake() override;
    /*
    * @brief Funcion para inicializar un componente. Se llama solo, no llamar manualmente.
    * Para saber como se implementa, revisar la clase Component.
    */
    void start() override;
    /*
    * @brief Funcion para actualizar el estado de un componente. Se llama solo, no llamar manualmente.
    * Para saber como se implementa, revisar la clase Component.
    */
    void update(const uint64_t deltaTime) override;
    /*
    * @brief Funcion para actualizar el estado de un componente. Se llama solo, no llamar manualmente.
    * Para saber como se implementa, revisar la clase Component.
    */
    void fixedUpdate() override;
    /*
    * @brief Funcion para recibir eventos. Se llama solo, no llamar manualmente.
    * Para saber como se implementa, revisar la clase Component.
    */
    void handleEvent(std::string const& id, void* info) override;

    /*
    * @brief Devuelve la tabla del objeto de Lua. Para usarla hay que incluir lua.hpp y LuaBridge.h.
    * @return Tabla del objeto de Lua.
    */
    luabridge::LuaRef* getTable() const { return objectTable; }
    /*
    * @brief Llama a una funcion del objeto de Lua. Para comprobar si ha producido un error usar el parametro success.
    * @param name Nombre de la funcion a llamar.
    * @param parameters Parametros validos (mirar al principio del archivo) a pasar en la funcion.
    * @param success Puntero a booleano (ya debe haberse reservado espacio) que escribe en el si todo ha ido bien.
    * Si se pasa nullptr, no ocurre nada.
    * @return Parametros devueltos por la funcion.
    * Si un parametro no se puede recoger con CompValue, se convertira en nullptr.
    * Si la funcion produce un error, devuelve un vector vacio.
    */
    std::vector<CompValue> callFunction(const std::string& name, const std::vector<CompValue>& parameters = {}, bool* success = nullptr);
};

class TAPIOCA_API LuaComponentBuilder : public ComponentBuilder {
private:
    luabridge::LuaRef* classTable;

public:
    LuaComponentBuilder(const std::string& name, luabridge::LuaRef* table);
    ~LuaComponentBuilder();

    Component* createComponent() override;
};
}