#pragma once
#include "Structure/Component.h"
#include "Structure/ComponentBuilder.h"

namespace luabridge {
class LuaRef;
}

namespace Tapioca {
class TAPIOCA_API LuaComponent : public Component { 
private:
    luabridge::LuaRef* objectTable;
    std::string name;

public:
    // Esto no va a funcionar bien
    COMPONENT_ID("LuaComponent")

    LuaComponent(luabridge::LuaRef* objectTable);
    ~LuaComponent();
    bool initComponent(const CompMap& variables) override;
    void start() override;
};

class TAPIOCA_API LuaComponentBuilder : public ComponentBuilder {
private:
    luabridge::LuaRef* classTable;

public:
    LuaComponentBuilder(std::string name, luabridge::LuaRef* table);
    ~LuaComponentBuilder();

    Component* createComponent() override;
};
}