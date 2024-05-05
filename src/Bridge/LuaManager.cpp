#include "LuaManager.h"
#include <lua.hpp>
#include "LuaRegistry.h"
#include <LuaBridge.h>
#include <Vector.h>
#include "VariantStack.h"
#include <filesystem>
#include "Components/LuaComponent.h"
#include "Structure/FactoryManager.h"
#include <sstream>
#include "checkML.h"
#include <functional>
#include "Structure/GameObject.h"
#include "Structure/Scene.h"
#include "Utilities/Vector2.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"
#include "Utilities/Quaternion.h"

namespace Tapioca {
LuaManager* LuaManager::instance_ = nullptr;

LuaManager::LuaManager() : L(nullptr), initialized(true) {
    L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_dofile(L, "TapiocaFiles/Lua/internal.lua") != 0) {
        logError(("LuaManager: Error al cargar internal.lua: " + std::string(lua_tostring(L, -1))).c_str());
        initialized = false;
    }

    luabridge::getGlobalNamespace(L)
        .beginNamespace("_internal")
        .beginNamespace("showGlobal")
        .addFunction("print", &print)
        .endNamespace()
        .endNamespace();

    luabridge::getGlobalNamespace(L)
        .beginNamespace("Tapioca")
        .beginClass<Scene>("Scene")
        .addFunction("getObjects", &Scene::getObjects)
        .addFunction("getHandler", &Scene::getHandler)
        .addProperty("name", &Scene::getName)
        .addProperty("active", &Scene::isActive, &Scene::setActive)
        .endClass()
        .beginClass<GameObject>("GameObject")
        .addFunction("getHandler", &GameObject::getHandler)
        .addProperty("alive", &GameObject::isAlive)
        .addFunction("die", &GameObject::die)
        .addProperty("scene", &GameObject::getScene)
        .addFunction(
            "addComponent",
            +[](GameObject* obj, const std::string& id, const CompMap& variables = {}) -> Component* {
                return obj->addComponent(id, variables);
            })
        .addFunction("addComponents", &GameObject::addComponents)
        .addFunction(
            "getComponent", +[](GameObject* obj, const std::string& id) -> Component* { return obj->getComponent(id); })
        .addFunction("getAllComponents", &GameObject::getAllComponents)
        .addFunction(
            "getComponents",
            +[](GameObject* obj, const std::string& id) -> std::vector<Component*> { return obj->getComponents(id); })
        // Funciones anadidas para ayudar con scripting de Lua
        .addFunction(
            "addLuaComponent",
            +[](GameObject* obj, const std::string& id, const CompMap& variables = {}) -> LuaComponent* {
                return static_cast<LuaComponent*>(obj->addComponent(id, variables));
            })
        .addFunction(
            "addLuaComponents",
            +[](GameObject* obj,
                const std::vector<std::pair<std::string, CompMap>>& idsAndVariables) -> std::vector<LuaComponent*> {
                std::vector<LuaComponent*> out;
                std::vector<Component*> aux = obj->addComponents(idsAndVariables);
                for (auto& comp : aux) {
                    out.push_back(static_cast<LuaComponent*>(comp));
                }
                return out;
            })
        .addFunction(
            "getLuaComponent",
            +[](GameObject* obj, const std::string& id) -> LuaComponent* {
                return static_cast<LuaComponent*>(obj->getComponent(id));
            })
        .addFunction(
            "getLuaComponents",
            +[](GameObject* obj, const std::string& id) -> std::vector<LuaComponent*> {
                std::vector<LuaComponent*> out;
                std::vector<Component*> aux = obj->getComponents(id);
                for (auto& comp : aux) {
                    out.push_back(static_cast<LuaComponent*>(comp));
                }
                return out;
            })
        .endClass()
        .beginClass<Component>("Component")
        .addFunction(
            "pushEvent",
            +[](Component* comp, const std::string& id, bool global = true, bool delay = false) {
                comp->pushEvent(id, nullptr, global, delay);
            })
        .addProperty("object", &Component::getObject)
        .addProperty("alive", &Component::isAlive)
        .addFunction("die", &Component::die)
        .addProperty("active", &Component::isActive, &Component::setActive)
        .endClass()
        .deriveClass<LuaComponent, Component>("LuaComponent")
        .addProperty("table", [](LuaComponent* comp) -> luabridge::LuaRef* { return comp->getTable(); })
        // Funcion anadida para ayudar con scripting de Lua
        .addFunction(
            "toLuaComponent",
            +[](LuaComponent* self, Component* comp) -> LuaComponent* { return static_cast<LuaComponent*>(comp); })
        .endClass()
        .beginClass<Vector2>("Vector2")
        .addConstructor<void (*)(const float)>()
        .addConstructor<void (*)(const float, const float)>()
        .addConstructor<void (*)(const Vector2&)>()
        .addProperty("x", &Vector2::x)
        .addProperty("y", &Vector2::y)
        .addFunction("magnitudeSquared", &Vector2::magnitudeSquared)
        .addFunction("magnitude", &Vector2::magnitude)
        .addFunction("getNormalized", &Vector2::getNormalized)
        .addFunction("normalize", &Vector2::normalize)
        .addFunction("__add", &Vector2::operator+)
        .addFunction("__unm", luabridge::overload<>(&Vector2::operator-))
        .addFunction("__sub", luabridge::overload<const Vector2&>(&Vector2::operator-))
        .addFunction("__mul", &Vector2::operator*)
        .addFunction("__div", &Vector2::operator/)
        .addFunction("__eq", &Vector2::operator==)
        .addFunction("lerp", &Vector2::lerp)
        .addFunction("distance", &Vector2::distance)
        .endClass()
        .addFunction("clamp", &Vector2::clamp)
        .beginClass<Vector3>("Vector3")
        .addConstructor<void (*)(const float)>()
        .addConstructor<void (*)(const float, const float)>()
        .addConstructor<void (*)(const float, const float, const float)>()
        .addConstructor<void (*)(const Vector3&)>()
        .addConstructor<void (*)(const Vector2&)>()
        .addConstructor<void (*)(const Vector2&, const float)>()
        .addProperty("x", &Vector3::x)
        .addProperty("y", &Vector3::y)
        .addProperty("z", &Vector3::z)
        .addFunction("magnitudeSquared", &Vector3::magnitudeSquared)
        .addFunction("magnitude", &Vector3::magnitude)
        .addFunction("getNormalized", &Vector3::getNormalized)
        .addFunction("normalize", &Vector3::normalize)
        .addFunction("rotateX", &Vector3::rotateX)
        .addFunction("rotateY", &Vector3::rotateY)
        .addFunction("rotateZ", &Vector3::rotateZ)
        .addFunction("cross", &Vector3::cross)
        .addFunction("dot", &Vector3::dot)
        .addFunction("__add", &Vector3::operator+)
        .addFunction("__unm", luabridge::overload<>(&Vector3::operator-))
        .addFunction("__sub", luabridge::overload<const Vector3&>(&Vector3::operator-))
        .addFunction("__mul", luabridge::overload<const float&>(&Vector3::operator*),
                     luabridge::overload<const Vector3&>(&Vector3::operator*))
        .addFunction("__div", &Vector3::operator/)
        .addFunction("__eq", &Vector3::operator==)
        .addFunction("lerp", &Vector3::lerp)
        .addFunction("distance", &Vector3::distance)
        .endClass()
        .beginClass<Vector4>("Vector4")
        .addConstructor<void (*)(const float)>()
        .addConstructor<void (*)(const float, const float)>()
        .addConstructor<void (*)(const float, const float, const float)>()
        .addConstructor<void (*)(const float, const float, const float, const float)>()
        .addConstructor<void (*)(const Vector4&)>()
        .addConstructor<void (*)(const Vector3&)>()
        .addConstructor<void (*)(const Vector3&, const float)>()
        .addConstructor<void (*)(const Vector2&)>()
        .addConstructor<void (*)(const Vector2&, const float)>()
        .addConstructor<void (*)(const Vector2&, const float, const float)>()
        .addProperty("x", &Vector4::x)
        .addProperty("y", &Vector4::y)
        .addProperty("z", &Vector4::z)
        .addProperty("w", &Vector4::w)
        .addFunction("magnitudeSquared", &Vector4::magnitudeSquared)
        .addFunction("magnitude", &Vector4::magnitude)
        .addFunction("getNormalized", &Vector4::getNormalized)
        .addFunction("normalize", &Vector4::normalize)
        .addFunction("__add", &Vector4::operator+)
        .addFunction("__unm", luabridge::overload<>(&Vector4::operator-))
        .addFunction("__sub", luabridge::overload<const Vector4&>(&Vector4::operator-))
        .addFunction("__mul", &Vector4::operator*)
        .addFunction("__div", &Vector4::operator/)
        .addFunction("__eq", &Vector4::operator==)
        .addFunction("lerp", &Vector4::lerp)
        .addFunction("distance", &Vector4::distance)
        .endClass()
        .beginClass<Quaternion>("Quaternion")
        .addConstructor<void (*)(const float, const float, const float, const float)>()
        .addConstructor<void (*)(const float, const Vector3&)>()
        .addConstructor<void (*)(const Vector3&)>()
        .addProperty("scalar", &Quaternion::scalar)
        .addProperty("vector", &Quaternion::vector)
        .addProperty("angle", &Quaternion::angle)
        .addFunction("inverse", &Quaternion::inverse)
        .addFunction("conjugate", &Quaternion::conjugate)
        .addFunction("magnitude", &Quaternion::magnitude)
        .addFunction("toEuler", &Quaternion::toEuler)
        //.addFunction("eulerAxis", &Quaternion::eulerAxis)
        .addFunction("__mul", luabridge::overload<const Quaternion&>(&Quaternion::operator*),
                     luabridge::overload<const float>(&Quaternion::operator*))
        .addFunction("__div", &Quaternion::operator/)
        .addFunction("rotatePoint", &Quaternion::rotatePoint)
        .addFunction("normalized", &Quaternion::normalized)
        .addFunction("normalize", &Quaternion::normalize)
        .endClass()
        .endNamespace();

    reg = new LuaRegistry(L);
}

LuaManager::~LuaManager() {
    delete reg;
    lua_close(L);
}

bool LuaManager::init() { return initialized && loadBase() && loadScripts(); }

std::vector<CompValue> LuaManager::callLuaFunction(const std::string& name, const std::vector<CompValue>& parameters,
                                                   bool* success) {
    std::vector<CompValue> out;
    luabridge::LuaRef function = luabridge::getGlobal(L, "_internal")["callGlobal"];
    if (!function.isCallable()) {
        if (success != nullptr) *success = false;
        return out;
    }
    luabridge::LuaResult result = function(name, parameters);
    if (result.hasFailed()) {
        logError(("LuaManager: Error al ejecutar la funcion de Lua \"" + name + "\" [" +
                  std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
                     .c_str());
        if (success != nullptr) *success = false;
        return out;
    }
    for (int i = 0; i < result.size(); i++) {
        luabridge::LuaRef param = result[0];
        luabridge::TypeResult<CompValue> safeValue = param.cast<CompValue>();
        if (!safeValue) {
            out.push_back(nullptr);
        }
        else {
            out.push_back(safeValue.value());
        }
    }
    if (success != nullptr) *success = true;
    return out;
}

bool LuaManager::addLuaFunction(const std::string& name, std::function<void()> f) {
    luabridge::getGlobalNamespace(L).addFunction(name.c_str(), f);
    return true;
}

CompValue LuaManager::getValueFromLua(const std::string& name) {
    luabridge::LuaRef value = luabridge::getGlobal(L, name.c_str());
    luabridge::TypeResult<CompValue> result = value.cast<CompValue>();
    if (!result) {
        logError("LuaManager: Se ha intentado sacar un valor invalido.");
        return nullptr;
    }
    return result.value();
}

bool LuaManager::setValueOnLua(const std::string& name, CompValue value) {
    bool done = luabridge::setGlobal(L, value, name.c_str());
    if (!done) logError("LuaManager: Se ha intentado poner un nombre invalido.");
    return done;
}

bool LuaManager::loadBase() {
    if (luaL_dofile(L, "TapiocaFiles/Lua/BaseComponent.lua") != 0) {
        logError(("LuaManager: Error al cargar base de LuaComponent: " + std::string(lua_tostring(L, -1))).c_str());
        return false;
    }
    return true;
}

bool LuaManager::loadScript(const std::filesystem::path& path) {
    if (!loadBase()) return false;

    if (luaL_dofile(L, path.string().c_str()) != 0) {
        logError(("LuaManager: Error al cargar el archivo Lua: " + std::string(lua_tostring(L, -1))).c_str());
        return false;
    }
    luabridge::LuaRef* table = new luabridge::LuaRef(luabridge::getGlobal(L, "comp"));
    std::string name = path.filename().string();
    // Se le quita la extension (.lua)
    name = name.substr(0, name.length() - 4);
    FactoryManager::instance()->addBuilder(new LuaComponentBuilder(name, table));
    return true;
}

bool LuaManager::loadScripts() {
    logInfo("LuaManager: Cargando scripts...");
    std::string path = "assets\\scripts\\";
    std::string extension = ".lua";
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.path().extension().string() == extension) {
                if (!loadScript(entry.path().string())) return false;
            }
        }
    } catch (std::filesystem::filesystem_error&) {
        logWarn("LuaManager: No existe ruta de scripts.");
        try {
            if (std::filesystem::create_directory(path))
                logInfo("LuaManager: Carpeta de scripts creada correctamente.");
        } catch (const std::filesystem::filesystem_error& e) {
            logError(("LuaManager: No se pudo crear la carpeta de scripts. " + std::string(e.what())).c_str());
        }
    }
    return true;
}

int LuaManager::print(lua_State* L) {
#ifdef _DEBUG
    std::stringstream aux;
    int i = 1;
    while (lua_gettop(L) >= i) {
        if (lua_isboolean(L, i)) aux << (lua_toboolean(L, i) ? "true " : "false ");
        else if (lua_iscfunction(L, i))
            aux << "C function ";
        else if (lua_isfunction(L, i))
            aux << "Lua function ";
        else if (lua_isnil(L, i))
            aux << "nil ";
        else if (lua_isinteger(L, i))
            aux << lua_tointeger(L, i) << ' ';
        else if (lua_isnumber(L, i))
            aux << lua_tonumber(L, i) << ' ';
        else if (lua_isstring(L, i))
            aux << lua_tostring(L, i) << ' ';
        else if (lua_istable(L, i))
            aux << "table ";
        else if (lua_isthread(L, i))
            aux << "thread ";
        else if (lua_isuserdata(L, i))
            aux << "userdata ";
        else
            aux << "something ";
        i++;
    }
    std::cout << "[INFO|JUEGO] " << aux.str() << '\n';
#endif
    return 0;
}
}
