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
SceneLoader::SceneLoader()
    : luaState(nullptr), mainLoop(nullptr), factManager(nullptr), windowManager(nullptr),
      scenesPath("assets\\scenes\\") { }

SceneLoader::~SceneLoader() {
    luaState = nullptr;
    mainLoop = nullptr;
    factManager = nullptr;
    windowManager = nullptr;
}

bool SceneLoader::init() {
    mainLoop = MainLoop::instance();
    factManager = FactoryManager::instance();
    windowManager = WindowManager::instance();
    if (mainLoop == nullptr) {
        logError("SceneLoader: Instancia de MainLoop invalida.");
        return false;
    }
    if (factManager == nullptr) {
        logError("SceneLoader: Instancia de FactoryManager invalida.");
        return false;
    }
    if (windowManager == nullptr) {
        logError("SceneLoader: Instancia de WindowManager invalida.");
        return false;
    }
    return true;
}

bool SceneLoader::initConfig() {
    logInfo("SceneLoader: Configurando la escena inicial...");
    DynamicLibraryLoader* loader = mainLoop->getLoader();
    if (loader == nullptr) {
        logError("SceneLoader: Instancia de DynamicLibraryLoader invalida.");
        return false;
    }

    EntryPointGetInitScene initScene = (EntryPointGetInitScene)GetProcAddress(loader->getModule(), "getInitScene");
    if (initScene == nullptr) {
        logError("SceneLoader: La DLL del juego no tiene la funcion \"getInitScene\".");
        return false;
    }
    return loadScene(initScene()) != nullptr;
}

Scene* SceneLoader::loadScene(std::string const& sceneName, const bool active) {
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
    scene->setActive(active);
    scene->setFirstWindowSize(windowManager->getWindowW(), windowManager->getWindowH());
    scene->setWindowSize(windowManager->getWindowW(), windowManager->getWindowH());

    mainLoop->loadScene(scene);
    lua_close(luaState);
    logInfo("SceneLoader: Escena creada correctamente.");
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
        gameObject->setScene(scene);
        std::string gameObjectName = "";

        if (!lua_isinteger(luaState, -2)) gameObjectName = lua_tostring(luaState, -2);

        logInfo(("SceneLoader: \tGameObject: " + gameObjectName).c_str());

        int zIndex = 0;
        if (!loadGameObject(gameObject, zIndex) || !scene->addObject(gameObject, gameObjectName, zIndex)) {
            delete gameObject;
            return false;
        }

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
            zIndex = (int)lua_tointeger(luaState, -1);
        }
        lua_pop(luaState, 1);
    }

    // Relaciona los hijos con el padre
    Transform* tr = gameObject->getComponent<Transform>();
    for (GameObject* obj : children)
        obj->getComponent<Transform>()->setParent(tr);
    children.clear();
    return true;
}

bool SceneLoader::loadGameObjects(Scene* const scene, std::vector<GameObject*>& gameObjects) {
    logInfo("SceneLoader: Children: start");

    std::string name = "";
    std::string gameObjectName = "";
    int zIndex = 0;
    while (lua_next(luaState, -2) != 0) {
        GameObject* gameObject = new GameObject();
        gameObject->setScene(scene);

        if (!lua_isinteger(luaState, -2)) name = lua_tostring(luaState, -2);
        if (name == "zIndex") zIndex = (int)lua_tointeger(luaState, -1);
        else {
            gameObjects.push_back(gameObject);
            gameObjectName = name;

            logInfo(("SceneLoader: \tGameObject: " + gameObjectName).c_str());

            if (!loadGameObject(gameObject, zIndex) || !scene->addObject(gameObject, gameObjectName, zIndex))
                return false;
        }

        lua_pop(luaState, 1);
    }
    logInfo("SceneLoader: Children: end");
    return true;
}

bool SceneLoader::loadComponents(GameObject* const gameObject) {
    Component* component = nullptr;
    std::string componentName = "";
    while (lua_next(luaState, -2) != 0) {
        componentName = lua_tostring(luaState, -2);
        logInfo(("SceneLoader: \t\tComponent: " + componentName).c_str());
        if (!loadComponent(componentName, gameObject)) return false;
        lua_pop(luaState, 1);
    }
    // Si no tiene transform, le anado uno por defecto
    if (gameObject->getComponent<Transform>() == nullptr) gameObject->addComponent(new Transform(), Transform::id);
    return true;
}

bool SceneLoader::loadComponent(std::string const& name, GameObject* const gameObject) {
    if (!lua_istable(luaState, -1)) return false;

    lua_pushnil(luaState);
    CompMap map;
    std::string key = "";
    CompValue value;
    bool load = true;
    bool hasTable = false;
    while (lua_next(luaState, -2) != 0 && load) {
        if (lua_istable(luaState, -1)) {
            hasTable = true;
            load = loadComponent(name, gameObject);
            lua_pop(luaState, 1);
        }
        else {
            key = lua_tostring(luaState, -2);

            if (lua_isboolean(luaState, -1)) {
                value = lua_toboolean(luaState, -1) == 1;
                logInfo(("SceneLoader: \t\tvariable: " + key + " valor: " + std::to_string(get<bool>(value))).c_str());
            }
            else if (lua_isinteger(luaState, -1)) {
                value = (int)lua_tointeger(luaState, -1);
                logInfo(("SceneLoader: \t\tvariable: " + key + " valor: " + std::to_string(get<int>(value))).c_str());
            }
            else if (lua_isnumber(luaState, -1)) {
                value = (float)lua_tonumber(luaState, -1);
                logInfo(("SceneLoader: \t\tvariable: " + key + " valor: " + std::to_string(get<float>(value))).c_str());
            }
            else if (lua_isstring(luaState, -1)) {
                value = lua_tostring(luaState, -1);
                logInfo(("SceneLoader: \t\tvariable: " + key + " valor: " + get<std::string>(value)).c_str());
            }
            map[key] = value;
            lua_pop(luaState, 1);
        }
    }
    if (hasTable) return true;

    Component* comp = nullptr;
    if (load) comp = factManager->createComponent(name);
    if (comp == nullptr) {
        logError(("SceneLoader: No existe el componente \"" + name + "\".").c_str());
        return false;
    }
    if (!comp->initComponent(map)) {
        logError(("SceneLoader: Error al inicializar el componente \"" + name + "\".").c_str());
        delete comp;
        return false;
    }
    gameObject->addComponent(comp, name);

    return true;
}

void SceneLoader::exposeUIvalues() {

    luaL_openlibs(luaState);

    lua_pushnumber(luaState, WindowManager::instance()->getWindowW());
    lua_setglobal(luaState, "WINDOW_WIDTH");
    lua_pushnumber(luaState, WindowManager::instance()->getWindowH());
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