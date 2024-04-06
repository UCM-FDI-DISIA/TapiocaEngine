#include "SceneManager.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <fstream>
#include <imgui.h>

#include "Structure/Game.h"
#include "Structure/DynamicLibraryLoader.h"
#include "Structure/FactoryManager.h"
#include "WindowManager.h"
#include "Structure/Scene.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "checkML.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<SceneManager>;
template<>
SceneManager* Singleton<SceneManager>::instance_ = nullptr;

SceneManager::SceneManager() : luaState(nullptr), game(nullptr), factMngr(nullptr), scenesPath("assets\\scenes\\") { }

SceneManager::~SceneManager() {
    luaState = nullptr;
    game = nullptr;
    factMngr = nullptr;
}

bool SceneManager::init() {
    game = Game::instance();
    factMngr = FactoryManager::instance();

    if (game == nullptr) {
#ifdef _DEBUG
        std::cerr << "Instancia de Game invalido\n";
#endif
        return false;
    }
    if (factMngr == nullptr) {
#ifdef _DEBUG
        std::cerr << "Instancia de FactoryManager invalido\n";
#endif
        return false;
    }
    return true;
}

bool SceneManager::initConfig() {
#ifdef _DEBUG
    std::cout << "SceneManager: Configurando la escena inicial...\n";
#endif

    EntryPointGetInitScene initScene = (EntryPointGetInitScene)GetProcAddress(DynamicLibraryLoader::module, "getInitScene");
    if (initScene == nullptr) {
#ifdef _DEBUG
		std::cerr << "La DLL del juego no tiene la funcion \"getInitScene\"\n";
#endif
		return false;
    }
    return loadScene(initScene());
}

bool SceneManager::loadScene(std::string const& sceneName) {
    luaState = luaL_newstate();
    if (luaState == nullptr) {
#ifdef _DEBUG
        std::cerr << "Error al crear el estado de lua\n";
#endif
        return false;
    }

    exposeUIvalues();

    std::string extension = ".lua";
    std::string name = sceneName;
    if (sceneName.size() < extension.size()||sceneName.substr(sceneName.size()-extension.size(),sceneName.size())!=extension){
        name = sceneName + extension;
    }


    std::string path = scenesPath + name;
    if (luaL_dofile(luaState, path.c_str()) != 0) {
#ifdef _DEBUG
        std::cerr << "Error al cargar el archivo LUA: " << lua_tostring(luaState, -1) << '\n';
#endif
        lua_close(luaState);
        return false;
    }

    Scene* scene = new Scene(name.substr(0, name.size() - extension.size()));
    if (!loadScene(scene)) {
#ifdef _DEBUG
        std::cerr << "Error al cargar de escena, gameobjects y componentes\n";
#endif
        lua_close(luaState);
        delete scene;
        return false;
    }

    game->addScene(scene);
    lua_close(luaState);
#ifdef _DEBUG
    std::cout << "Escena cargada correctamente\n";
#endif
    return true;
}

bool SceneManager::loadScene(Scene* const scene) {
    lua_getglobal(luaState, "scene");
    if (!lua_istable(luaState, -1)) return false;
    lua_pushnil(luaState);
    return loadGameObjects(scene);
}

bool SceneManager::loadGameObjects(Scene* const scene) {
    while (lua_next(luaState, -2) != 0) {
        GameObject* gameObject = new GameObject();
        std::string gameObjectName = "";

        if (!lua_isinteger(luaState, -2)) gameObjectName = lua_tostring(luaState, -2);
#ifdef _DEBUG
        std::cout << "\tGameObject: " << gameObjectName << "\n";
#endif
        if (!scene->addObject(gameObject, gameObjectName) || !loadGameObject(gameObject)) return false;

        lua_pop(luaState, 1);
    }
    return true;
}

bool SceneManager::loadGameObject(GameObject* const gameObject) {
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

bool SceneManager::loadGameObjects(Scene* const scene, std::vector<GameObject*>& gameObjects) {
#ifdef _DEBUG
    std::cout << "Children: start\n";
#endif
    while (lua_next(luaState, -2) != 0) {
        GameObject* gameObject = new GameObject();
        std::string gameObjectName = "";
        if (!lua_isinteger(luaState, -2)) gameObjectName = lua_tostring(luaState, -2);
        scene->addObject(gameObject, gameObjectName);
        gameObjects.push_back(gameObject);
#ifdef _DEBUG
        std::cout << "\tGameObject: " << gameObjectName << "\n";
#endif
        if (!loadGameObject(gameObject)) return false;

        lua_pop(luaState, 1);
    }

#ifdef _DEBUG
    std::cout << "Children: end\n";
#endif
    return true;
}

bool SceneManager::loadComponents(GameObject* const gameObject) {
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
        gameObject->addComponent<Transform>();
    }
    return true;
}

Component* SceneManager::loadComponent(std::string const& name) {
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
#ifdef _DEBUG
        std::cerr << "No existe el componente: " << name << "\n";
#endif
        return nullptr;
    }

    if (!comp->initComponent(map)) {
#ifdef _DEBUG
        std::cerr << "Error al inicializar el componente: " << name << "\n";
#endif
        delete comp;
        return nullptr;
    }

    return comp;
}

void SceneManager::exposeUIvalues() {

    luaL_openlibs(luaState);

    lua_pushnumber(luaState, WindowManager::instance()->getWindowW());
    lua_setglobal(luaState, "WINDOW_WIDTH");
    lua_pushnumber(luaState, WindowManager::instance()->getWindowH());
    lua_setglobal(luaState, "WINDOW_HEIGHT");

    // Window flags
    lua_pushnumber(luaState, ImGuiWindowFlags_None);
    lua_setglobal(luaState, "ImGuiWindowFlags_None");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoTitleBar);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoTitleBar");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoResize);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoResize");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoMove);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoMove");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoScrollbar);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoScrollbar");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoScrollWithMouse);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoScrollWithMouse");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoCollapse);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoCollapse");
    lua_pushnumber(luaState, ImGuiWindowFlags_AlwaysAutoResize);
    lua_setglobal(luaState, "ImGuiWindowFlags_AlwaysAutoResize");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoBackground);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoBackground");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoSavedSettings);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoSavedSettings");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoMouseInputs);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoMouseInputs");
    lua_pushnumber(luaState, ImGuiWindowFlags_MenuBar);
    lua_setglobal(luaState, "ImGuiWindowFlags_MenuBar");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoFocusOnAppearing);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoFocusOnAppearing");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoBringToFrontOnFocus);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoBringToFrontOnFocus");
    lua_pushnumber(luaState, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    lua_setglobal(luaState, "ImGuiWindowFlags_AlwaysVerticalScrollbar");
    lua_pushnumber(luaState, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
    lua_setglobal(luaState, "ImGuiWindowFlags_AlwaysHorizontalScrollbar");
    lua_pushnumber(luaState, ImGuiWindowFlags_AlwaysUseWindowPadding);
    lua_setglobal(luaState, "ImGuiWindowFlags_AlwaysUseWindowPadding");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoNavInputs);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoNavInputs");
    lua_pushnumber(luaState, ImGuiWindowFlags_UnsavedDocument);
    lua_setglobal(luaState, "ImGuiWindowFlags_UnsavedDocument");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoNav);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoNav");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoDecoration);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoDecoration");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoScrollbar);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoScrollbar");
    lua_pushnumber(luaState, ImGuiWindowFlags_NoInputs);
    lua_setglobal(luaState, "ImGuiWindowFlags_NoInputs");

    // Input Text flags
    lua_pushnumber(luaState, ImGuiInputTextFlags_None);
    lua_setglobal(luaState, "ImGuiInputTextFlags_None");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CharsDecimal);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CharsDecimal");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CharsHexadecimal);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CharsHexadecimal");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CharsUppercase);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CharsUppercase");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CharsNoBlank);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CharsNoBlank");
    lua_pushnumber(luaState, ImGuiInputTextFlags_AutoSelectAll);
    lua_setglobal(luaState, "ImGuiInputTextFlags_AutoSelectAll");
    lua_pushnumber(luaState, ImGuiInputTextFlags_EnterReturnsTrue);
    lua_setglobal(luaState, "ImGuiInputTextFlags_EnterReturnsTrue");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CallbackCompletion);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackCompletion");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CallbackHistory);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackHistory");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CallbackAlways);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackAlways");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CallbackCharFilter);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackCharFilter");
    lua_pushnumber(luaState, ImGuiInputTextFlags_AllowTabInput);
    lua_setglobal(luaState, "ImGuiInputTextFlags_AllowTabInput");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CtrlEnterForNewLine);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CtrlEnterForNewLine");
    lua_pushnumber(luaState, ImGuiInputTextFlags_NoHorizontalScroll);
    lua_setglobal(luaState, "ImGuiInputTextFlags_NoHorizontalScroll");
    lua_pushnumber(luaState, ImGuiInputTextFlags_AlwaysOverwrite);
    lua_setglobal(luaState, "ImGuiInputTextFlags_AlwaysOverwrite");
    lua_pushnumber(luaState, ImGuiInputTextFlags_ReadOnly);
    lua_setglobal(luaState, "ImGuiInputTextFlags_ReadOnly");
    lua_pushnumber(luaState, ImGuiInputTextFlags_Password);
    lua_setglobal(luaState, "ImGuiInputTextFlags_Password");
    lua_pushnumber(luaState, ImGuiInputTextFlags_NoUndoRedo);
    lua_setglobal(luaState, "ImGuiInputTextFlags_NoUndoRedo");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CharsScientific);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CharsScientific");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CallbackResize);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackResize");
    lua_pushnumber(luaState, ImGuiInputTextFlags_CallbackEdit);
    lua_setglobal(luaState, "ImGuiInputTextFlags_CallbackEdit");
    lua_pushnumber(luaState, ImGuiInputTextFlags_EscapeClearsAll);
    lua_setglobal(luaState, "ImGuiInputTextFlags_EscapeClearsAll");
}
}