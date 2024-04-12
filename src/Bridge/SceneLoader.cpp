#include "SceneLoader.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <fstream>
#include <imgui.h>

#include "Structure/MainLoop.h"
#include "Structure/DynamicLibraryLoader.h"
#include "Structure/FactoryManager.h"
#include "WindowManager.h"
#include "Structure/Scene.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "checkML.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<SceneLoader>;
template<>
SceneLoader* Singleton<SceneLoader>::instance_ = nullptr;

SceneLoader::SceneLoader() : luaState(nullptr), mainLoop(nullptr), factMngr(nullptr), scenesPath("assets\\scenes\\") { }

SceneLoader::~SceneLoader() {
    luaState = nullptr;
    mainLoop = nullptr;
    factMngr = nullptr;
}

bool SceneLoader::init() {
    mainLoop = MainLoop::instance();
    factMngr = FactoryManager::instance();

    if (mainLoop == nullptr) {
        logError("SceneLoader: Instancia de MainLoop invalida.");
        return false;
    }
    if (factMngr == nullptr) {
        logError("SceneLoader: Instancia de FactoryManager invalida.");
        return false;
    }
    return true;
}

bool SceneLoader::initConfig() {
    logInfo("SceneLoader: Configurando la escena inicial...");

    EntryPointGetInitScene initScene =
        (EntryPointGetInitScene)GetProcAddress(DynamicLibraryLoader::module, "getInitScene");
    if (initScene == nullptr) {
        logError("SceneLoader: La DLL del juego no tiene la funcion \"getInitScene\".");
        return false;
    }
    return loadScene(initScene()) != nullptr;
}

Scene* SceneLoader::loadScene(std::string const& sceneName) {
    logInfo(("SceneLoader: Cargando escena \"" + sceneName + "\"...").c_str());
    luaState = luaL_newstate();
    if (luaState == nullptr) {
        logError("SceneLoader: Error al crear el estado de Lua.");
        return nullptr;
    }

    exposeUIvalues();

    std::string extension = ".lua";
    std::string name = sceneName;
    if (sceneName.size() < extension.size() ||
        sceneName.substr(sceneName.size() - extension.size(), sceneName.size()) != extension) {
        name = sceneName + extension;
    }

    std::string path = scenesPath + name;
    if (luaL_dofile(luaState, path.c_str()) != 0) {
        logError(("SceneLoader: Error al cargar el archivo Lua: " + std::string(lua_tostring(luaState, -1))).c_str());
        lua_close(luaState);
        return nullptr;
    }

    Scene* scene = new Scene(name.substr(0, name.size() - extension.size()));
    if (!loadScene(scene)) {
        logError("SceneLoader: Error al cargar la escena.");
        lua_close(luaState);
        delete scene;
        return nullptr;
    }

    mainLoop->loadScene(scene);
    lua_close(luaState);
    logInfo("SceneLoader: Escena cargada correctamente.");
    return scene;
}

bool SceneLoader::loadScene(Scene* const scene) {
    lua_getglobal(luaState, "scene");
    if (!lua_istable(luaState, -1)) return false;
    lua_pushnil(luaState);
    return loadGameObjects(scene);
}

bool SceneLoader::loadGameObjects(Scene* const scene) {
    while (lua_next(luaState, -2) != 0) {
        GameObject* gameObject = new GameObject();
        std::string gameObjectName = "";

        if (!lua_isinteger(luaState, -2)) gameObjectName = lua_tostring(luaState, -2);
#ifdef _DEBUG
        std::cout << "\tGameObject: " << gameObjectName << "\n";
#endif
        int zIndex = 0;
        if (!loadGameObject(gameObject, zIndex) || !scene->addObject(gameObject, gameObjectName, zIndex)) return false;

        lua_pop(luaState, 1);
    }
    return true;
}

bool SceneLoader::loadGameObject(GameObject* const gameObject, int& zIndex) {
    lua_pushnil(luaState);
    std::string name = "";
    std::vector<GameObject*> children;

    while (lua_next(luaState, -2) != 0) {
        if (!lua_isinteger(luaState, -2)) name = lua_tostring(luaState, -2);
        if (name == "components") {
            lua_pushnil(luaState);
            if (!loadComponents(gameObject)) return false;
        }
        else if (name == "children") {
            lua_pushnil(luaState);
            if (!loadGameObjects(gameObject->getScene(), children)) return false;
        }
        else if (name == "zIndex") {
            zIndex = lua_tointeger(luaState, -1);
        }
        lua_pop(luaState, 1);
    }

    // Relaciona los hijos con el padre
    Transform* tr = gameObject->getComponent<Transform>();
    for (GameObject* obj : children) {
        obj->getComponent<Transform>()->setParent(tr);
    }
    children.clear();
    return true;
}

bool SceneLoader::loadGameObjects(Scene* const scene, std::vector<GameObject*>& gameObjects) {
#ifdef _DEBUG
    std::cout << "Children: start\n";
#endif
    std::string name = "";
    std::string gameObjectName = "";
    int zIndex = 0;
    while (lua_next(luaState, -2) != 0) {
        GameObject* gameObject = new GameObject();
        if (!lua_isinteger(luaState, -2)) name = lua_tostring(luaState, -2);
        if (name == "zIndex") {
            zIndex = lua_tointeger(luaState, -1);
        }
        else {
            gameObjects.push_back(gameObject);
            gameObjectName = name;
#ifdef _DEBUG
            std::cout << "\tGameObject: " << gameObjectName << "\n";
#endif
            if (!loadGameObject(gameObject, zIndex) || !scene->addObject(gameObject, gameObjectName, zIndex))
                return false;
        }

        lua_pop(luaState, 1);
    }

#ifdef _DEBUG
    std::cout << "Children: end\n";
#endif
    return true;
}

bool SceneLoader::loadComponents(GameObject* const gameObject) {
    Component* component = nullptr;
    std::string componentName = "";
    while (lua_next(luaState, -2) != 0) {
        componentName = lua_tostring(luaState, -2);

#ifdef _DEBUG
        std::cout << "\t\tComponent: " << componentName << "\n";
#endif
        component = loadComponent(componentName);
        if (component == nullptr) return false;
        // Si no tengo creado el componente
        gameObject->addComponent(component, componentName);
        lua_pop(luaState, 1);
    }
    if (gameObject->getComponent<Transform>() == nullptr) {
        gameObject->addComponent(new Transform(), Transform::id);
    }
    return true;
}

Component* SceneLoader::loadComponent(std::string const& name) {
    if (!lua_istable(luaState, -1)) return nullptr;

    lua_pushnil(luaState);
    CompMap map;
    std::string key = "";
    CompValue value;
    bool load = true;

    while (lua_next(luaState, -2) != 0 && load) {
        if (lua_istable(luaState, -1)) {
            lua_pop(luaState, 1);
            load = false;
        }
        else {
            key = lua_tostring(luaState, -2);

            if (lua_isboolean(luaState, -1)) {
                value = lua_toboolean(luaState, -1) == 1;
#ifdef _DEBUG
                std::cout << "\t\tvariable: " << key << " valor: " << (get<bool>(value) ? "true" : "false") << "\n";
#endif
            }
            else if (lua_isinteger(luaState, -1)) {
                value = (int)lua_tointeger(luaState, -1);
#ifdef _DEBUG
                std::cout << "\t\tvariable: " << key << " valor: " << get<int>(value) << "\n";
#endif
            }
            else if (lua_isnumber(luaState, -1)) {
                value = (float)lua_tonumber(luaState, -1);
#ifdef _DEBUG
                std::cout << "\t\tvariable: " << key << " valor: " << get<float>(value) << "\n";
#endif
            }
            else if (lua_isstring(luaState, -1)) {
                value = lua_tostring(luaState, -1);
#ifdef _DEBUG
                std::cout << "\t\tvariable: " << key << " valor: " << get<std::string>(value) << "\n";
#endif
            }

            map[key] = value;
            lua_pop(luaState, 1);
        }
    }

    Component* comp = nullptr;
    if (load) {
        comp = factMngr->createComponent(name);
    }
    if (comp == nullptr) {
        logError(("SceneLoader: No existe el componente \"" + name + "\".").c_str());
        return nullptr;
    }

    if (!comp->initComponent(map)) {
        logError(("SceneLoader: Error al inicializar el componente \"" + name + "\".").c_str());
        delete comp;
        return nullptr;
    }

    return comp;
}

void SceneLoader::exposeUIvalues() {

    luaL_openlibs(luaState);

    lua_pushinteger(luaState, WindowManager::instance()->getWindowW());
    lua_setglobal(luaState, "WINDOW_WIDTH");
    lua_pushinteger(luaState, WindowManager::instance()->getWindowH());
    lua_setglobal(luaState, "WINDOW_HEIGHT");

    // Window flags
    lua_pushinteger(luaState, ImGuiWindowFlags_None);
    lua_setglobal(luaState, "ImGuiWindowFlags_None");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoTitleBar);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoTitleBar");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoResize);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoResize");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoMove);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoMove");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoScrollbar);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoScrollbar");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoScrollWithMouse);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoScrollWithMouse");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoCollapse);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoCollapse");
    lua_pushinteger(luaState, ImGuiWindowFlags_AlwaysAutoResize);
    lua_setglobal(luaState, "ImGuiWindowFlags_AlwaysAutoResize");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoBackground);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoBackground");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoSavedSettings);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoSavedSettings");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoMouseInputs);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoMouseInputs");
    lua_pushinteger(luaState, ImGuiWindowFlags_MenuBar);
    lua_setglobal(luaState, "ImGuiWindowFlags_MenuBar");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoFocusOnAppearing);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoFocusOnAppearing");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoBringToFrontOnFocus);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoBringToFrontOnFocus");
    lua_pushinteger(luaState, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    lua_setglobal(luaState, "ImGuiWindowFlags_AlwaysVerticalScrollbar");
    lua_pushinteger(luaState, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
    lua_setglobal(luaState, "ImGuiWindowFlags_AlwaysHorizontalScrollbar");
    lua_pushinteger(luaState, ImGuiWindowFlags_AlwaysUseWindowPadding);
    lua_setglobal(luaState, "ImGuiWindowFlags_AlwaysUseWindowPadding");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoNavInputs);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoNavInputs");
    lua_pushinteger(luaState, ImGuiWindowFlags_UnsavedDocument);
    lua_setglobal(luaState, "ImGuiWindowFlags_UnsavedDocument");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoNav);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoNav");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoDecoration);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoDecoration");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoScrollbar);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoScrollbar");
    lua_pushinteger(luaState, ImGuiWindowFlags_NoInputs);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoInputs");

    // Input Text flags
    lua_pushinteger(luaState, ImGuiInputTextFlags_None);
    lua_setglobal(luaState, "ImGuiInputTextFlags_None");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CharsDecimal);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CharsDecimal");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CharsHexadecimal);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CharsHexadecimal");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CharsUppercase);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CharsUppercase");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CharsNoBlank);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CharsNoBlank");
    lua_pushinteger(luaState, ImGuiInputTextFlags_AutoSelectAll);
    lua_setglobal(luaState, "ImGuiInputTextFlags_AutoSelectAll");
    lua_pushinteger(luaState, ImGuiInputTextFlags_EnterReturnsTrue);
    lua_setglobal(luaState, "ImGuiInputTextFlags_EnterReturnsTrue");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CallbackCompletion);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackCompletion");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CallbackHistory);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackHistory");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CallbackAlways);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackAlways");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CallbackCharFilter);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackCharFilter");
    lua_pushinteger(luaState, ImGuiInputTextFlags_AllowTabInput);
    lua_setglobal(luaState, "ImGuiInputTextFlags_AllowTabInput");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CtrlEnterForNewLine);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CtrlEnterForNewLine");
    lua_pushinteger(luaState, ImGuiInputTextFlags_NoHorizontalScroll);
    lua_setglobal(luaState, "ImGuiInputTextFlags_NoHorizontalScroll");
    lua_pushinteger(luaState, ImGuiInputTextFlags_AlwaysOverwrite);
    lua_setglobal(luaState, "ImGuiInputTextFlags_AlwaysOverwrite");
    lua_pushinteger(luaState, ImGuiInputTextFlags_ReadOnly);
    lua_setglobal(luaState, "ImGuiInputTextFlags_ReadOnly");
    lua_pushinteger(luaState, ImGuiInputTextFlags_Password);
    lua_setglobal(luaState, "ImGuiInputTextFlags_Password");
    lua_pushinteger(luaState, ImGuiInputTextFlags_NoUndoRedo);
    lua_setglobal(luaState, "ImGuiInputTextFlags_NoUndoRedo");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CharsScientific);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CharsScientific");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CallbackResize);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackResize");
    lua_pushinteger(luaState, ImGuiInputTextFlags_CallbackEdit);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackEdit");
    lua_pushinteger(luaState, ImGuiInputTextFlags_EscapeClearsAll);
    lua_setglobal(luaState, "ImGuiInputTextFlags_EscapeClearsAll");
}
}