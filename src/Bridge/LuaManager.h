#pragma once
#include "Structure/Module.h"
#include "Utilities/Singleton.h"
#include "componentDefs.h"

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

    bool callLuaFunction(std::string name, const std::vector<CompValue>& parameters);

    LuaRegistry* getRegistry() const { return reg; }
};
}