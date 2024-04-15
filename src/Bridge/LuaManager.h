#pragma once
#include "Structure/Module.h"
#include "Utilities/Singleton.h"

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

public:
    LuaManager();
    ~LuaManager();

    bool init() override;

    lua_State* getLuaState() const { return L; }

    LuaRegistry* getRegistry() const { return reg; }

    void loadBase();

    bool loadScript(const std::filesystem::path& path);
    void loadScripts();
};
}