#pragma once
#include "Structure/Module.h"
#include "Utilities/Singleton.h"
#include "componentDefs.h"
#include <functional>

/** 
* ----------- VALORES VALIDOS -----------
* Se usa CompValue para representar variables sacadas de Lua o usadas en Lua.
* Los tipos validos son char, int, float, bool, string y nullptr/nil.
* Realmente CompValue es un std::variant, asi que para coger un valor hay que usar los metodos de la biblioteca "std":
* std::get para coger un valor, std::holds_alternative para saber si el valor es capaz de convertirse a ese tipo.
* (Tambien se puede usar la funcion std::variant::index() para saber el tipo que tiene segun el indice.
*  Los indices siguen el orden especificado arriba.)
*/

struct lua_State;
namespace std {
namespace filesystem {
    class path;
}
}
namespace luabridge {
class LuaRef;
}

namespace Tapioca {
class LuaRegistry;

/**
* @brief Clase que se encarga de gestionar el scripting en Lua
*/
class TAPIOCA_API LuaManager : public Module, public Singleton<LuaManager> {
private:
    friend class Singleton<LuaManager>;

    lua_State* L;       // Estado de Lua
    LuaRegistry* reg;   // Registro de variables de Lua

    bool initialized;   // Si se ha inicializado el modulo

    /**
    * @brief Constructor del manager. No crear nuevos managers
    */
    LuaManager();

    /**
    * @brief Metodo interno para inicializar el manager. No usar
    * @return Si se ha inicializado el modulo
    */
    bool init() override;

    /**
    * @brief Carga la base de los componentes en Lua
    * @return Devuelve si se ha cargado correctamente
    */
    bool loadBase();

    /**
    * @brief Carga un script de Lua
    * @param path Ruta del script
    * @return Si se ha cargado correctamente
    */
    bool loadScript(const std::filesystem::path& path);
    /**
    * @brief Carga todos los scripts de Lua
    */
    bool loadScripts();

    /**
    * @brief Funcion para imprimir en consola desde Lua
    * @param L Estado de Lua
    * @return 0
    */
    static int print(lua_State* L);

public:
    LuaManager(LuaManager&) = delete;
    LuaManager(LuaManager&&) = delete;
    LuaManager& operator=(LuaManager&) = delete;
    LuaManager& operator=(LuaManager&&) = delete;

    /**
    * @brief Destructor del manager. No borrar el manager
    */
    ~LuaManager();

    /**
    * @brief Devuelve un puntero al estado de Lua usado para scripting
    * @return Puntero al estado de Lua usado para scripting
    */
    lua_State* getLuaState() const { return L; }

    /**
    * @brief Llama a una funcion de Lua global. Para comprobar si ha producido un error usar el parametro success
    * @param name Nombre de la funcion a llamar
    * @param parameters Parametros validos (mirar al principio del archivo) a pasar en la funcion
    * @param success Puntero a booleano (ya debe haberse reservado espacio) que escribe en el si todo ha ido bien.
    * Si se pasa nullptr, no ocurre nada
    * @return Parametros devueltos por la funcion.
    * Si un parametro no se puede recoger con CompValue, se convertira en nullptr.
    * Si la funcion produce un error, devuelve un vector vacio
    */
    std::vector<CompValue> callLuaFunction(const std::string& name, const std::vector<CompValue>& parameters = {},
                                           bool* success = nullptr);
    /**
    * @brief Anade una funcion sin parametros y sin devolver nada a la tabla global
    * @param name Nombre de la funcion en Lua
    * @param f Funcion
    * @return Si se ha podido anadir
    */
    bool addLuaFunction(const std::string& name, std::function<void()> f);
    /**
    * @brief Recoge una variable valida (mirar al principio del archivo) global
    * @param name Nombre de la variable
    * @return Valor de la variable recogida
    */
    CompValue getValueFromLua(const std::string& name);
    /**
    * @brief Crea una variable valida (mirar al principio del archivo) global
    * @param name Nombre de la variable
    * @param value Valor de la variable
    * @return Si se ha asignado correctamente
    */
    bool setValueOnLua(const std::string& name, CompValue value);

    /**
    * @brief Devuelve el registro de variables de Lua
    * @return Registro de variables de Lua
    */
    inline LuaRegistry* getRegistry() const { return reg; }
};
}
