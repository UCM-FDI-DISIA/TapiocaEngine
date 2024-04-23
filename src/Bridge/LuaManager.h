#pragma once
#include "Structure/Module.h"
#include "Utilities/Singleton.h"
#include "componentDefs.h"
#include <functional>

struct lua_State;
namespace std {
namespace filesystem {
    class path;
}
}

namespace Tapioca {
class LuaRegistry;

class TAPIOCA_API LuaManager : public Module, public Singleton<LuaManager> { 
private:
    friend class Singleton<LuaManager>;

    lua_State* L;
    LuaRegistry* reg;

    bool initialized;

    const char* BASE_FILE = "BaseComponent.lua";
    bool loadBase();

    bool loadScript(const std::filesystem::path& path);
    bool loadScripts();

    static int print(lua_State* L);

public:
    LuaManager();
    ~LuaManager();


    bool init() override;

    lua_State* getLuaState() const { return L; }

    bool callLuaFunction(const std::string& name, const std::vector<CompValue>& parameters = {});
    bool addLuaFunction(const std::string& name, std::function<void()> f);
    CompValue getValueFromLua(const std::string& name);
    bool setValueOnLua(const std::string& name, CompValue value);

    LuaRegistry* getRegistry() const { return reg; }

    
};
}