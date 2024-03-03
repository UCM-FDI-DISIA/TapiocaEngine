// Propios
#include "LightPoint.h"
#include "LightDirectional.h"
#include "Camera.h"
#include "Node.h"
#include "Mesh.h"
#include "Viewport.h"
#include "GraphicsEngine.h"

// OGRE
#include <Ogre.h>
#include <OgreFileSystem.h>
#include <OgreFileSystemLayer.h>
#include <OgreRTShaderSystem.h>
#include "SGTechniqueResolverListener.h"
#include <OgreGL3PlusRenderSystem.h>

// SDL
#include <SDL.h>
#include <SDL_syswm.h>
#undef main

// C++
#include <iostream>
#include <Windows.h>

// Utilidades
#include "Utilities/checkML.h"

Tapioca::GraphicsEngine::GraphicsEngine(std::string windowName, uint32_t w, uint32_t h)
    : fsLayer(nullptr), mShaderGenerator(nullptr), cfgPath(), mRoot(nullptr), scnMgr(nullptr), renderSys(nullptr),
      mMaterialMgrListener(nullptr), ogreWindow(nullptr), sdlWindow(nullptr), mwindowName(windowName), windowWidth(w),
      windowHeight(h) { }

Tapioca::GraphicsEngine::~GraphicsEngine() {
    /*for (auto& object : objects) {
        delete object.first;
    }
    objects.clear();
    for (auto& node : nodes) {
        delete node;
    }
    nodes.clear();*/

    for (auto& node : selfManagedNodes) {
        delete node;
    }
    selfManagedNodes.clear();

    shutDown();
}

bool Tapioca::GraphicsEngine::init() {
    // CONTROLAR LOS POSIBLES ERRORES PARA DEVOLVER FALSE

    // Pbtenemos la ubicacion de plugins.cfg y a partir de la misma obtenenmos la ruta relativa
    // de la carpeta de assets. El nombre es para crear un directorio dentro del home del usuario
    // para distinguir entre diferentes aplicaciones de Ogre (da igual el nombre)
    fsLayer = new Ogre::FileSystemLayer("TapiocaDirectory");
    Ogre::String pluginsPath;

    // importante: la ruta donde esta plugins.cfg no puede tener caracteres especiales (solo alfabeto en ingles)
    pluginsPath = fsLayer->getConfigFilePath("plugins.cfg");

    // tratamiento de errores
    if (!Ogre::FileSystemLayer::fileExists(pluginsPath)) {
        delete fsLayer;
        return false;
    }

    cfgPath = pluginsPath;
    cfgPath.erase(cfgPath.find_last_of("\\") + 1, cfgPath.size() - 1);   // "\\" equivale a "\"

    fsLayer->setHomePath(cfgPath);

    // (ruta plugins.cfg, ruta ogre.cfg, ruta ogre.log)
    // ogre.cfg sirve para guardar y restaurar la configuracion de render
    // ogre.log guarda un mensaje de depuracion
    // getWritablePath parte del homePath (asignado arriba)
    mRoot = new Ogre::Root(pluginsPath, "", fsLayer->getWritablePath("ogre.log"));

    // Otra forma: cargar los plugins desde codigo
    // loadPlugIns();   // cargar codec, que sirve para poder usar png, jpg... (para las texturas)
    // mRoot->loadPlugin("RenderSystem_GL3PLUS");   // cargar sistema de render por nombre

    // El sistema de render debe cargarse no podemos crearlo
    // Se especifica que se cargue este sistema de render en el archivo "plugins.cfg" que esta actualmente en TapiocaEngine/bin
    const Ogre::RenderSystemList renderSystems = mRoot->getAvailableRenderers();
    renderSys = renderSystems.front();
    mRoot->setRenderSystem(renderSys);

    // Inicializa ogre sin crear la ventana, siempre se hace despues de asignar el sistema de render
    mRoot->initialise(false);

    // Iniciar SDL si no se ha inicializado antes
    if (!SDL_WasInit(SDL_INIT_EVERYTHING)) SDL_Init(SDL_INIT_EVERYTHING);

    // informacion de la ventana
    Ogre::NameValuePairList miscParams;

    // se coge la informacion del sistema de render
    Ogre::ConfigOptionMap ropts = renderSys->getConfigOptions();
    std::istringstream mode(ropts["Video Mode"].currentValue);
    Ogre::String token;
    mode >> windowWidth;    // width
    mode >> token;          // 'x' as seperator between width and height
    mode >> windowHeight;   // height

    // se agrega informacion del sistema de render y demas
    miscParams["FSAA"] = ropts["FSAA"].currentValue;
    miscParams["vsync"] = ropts["VSync"].currentValue;
    miscParams["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;

    // Iniciar ventana SDL2
    Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    if (ropts["Full Screen"].currentValue == "Yes") flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALLOW_HIGHDPI;
    // else flags = SDL_WINDOW_RESIZABLE;

    // Crear ventana SDL2
    sdlWindow = SDL_CreateWindow(
        mwindowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, flags);

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
    // vincular ventana de SDL con Ogre
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));

    // crear ventana de Ogre (solo para render)
    // ya antes se le ha indicado en los parametros que existe la ventana de SDL
    ogreWindow = mRoot->createRenderWindow(mwindowName, windowWidth, windowHeight, false, &miscParams);

    scnMgr = mRoot->createSceneManager();
    loadShaders();

    // si da problemas usar el renderSys cogerlo directamente desde root
    renderSys->_initRenderTargets();
    loadResources();

    return true;
}

void Tapioca::GraphicsEngine::loadPlugIns() {
#ifdef _DEBUG
    mRoot->loadPlugin("Codec_STBI_d.dll");   // Necesario para tener codec de archivos png jpg ...
#else
    mRoot->loadPlugin("Codec_STBI.dll");   // Necesario para tener codec de archivos png jpg ...
#endif
}

void Tapioca::GraphicsEngine::loadResources() {
    // todos los assets deben estar en la carpeta assets
#ifdef _RESOURCES_DIR
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "./assets", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
#else
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "/assets", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
#endif
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Tapioca::GraphicsEngine::loadShaders() {
    if (Ogre::RTShader::ShaderGenerator::initialize()) {
        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
        mShaderGenerator->addSceneManager(scnMgr);

        // CREAR LOS SHADERS PARA MATERIALES QUE VENGAN SIN ELLOS
        mMaterialMgrListener = new SGTechniqueResolverListener(mShaderGenerator);
        Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
    }
}

void Tapioca::GraphicsEngine::render() { mRoot->renderOneFrame(); }

void Tapioca::GraphicsEngine::shutDown() {
    if (mRoot == nullptr) return;

    // ELIMINAR EL SCENE MANAGER
    mShaderGenerator->removeSceneManager(scnMgr);
    mRoot->destroySceneManager(scnMgr);

    // ELIMINAR EL SISTEMA DE SHADERS

    // (Un material puede tener varias Techniques
    // Se puede crear varia schemes, que selecciones una technique concreta de un material
    // El usuario podria elegir que scheme usar y ajustar el juego al rendimiento de su maquina
    // Se vuelve a la scheme por defecto por precaucion)
    Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);

    // se desregistra el listener que crea shaders para objetos que vengan sin ellos ya definidos
    // sgtechniqueresolverlistener * mmaterialmgrlistener;
    if (mMaterialMgrListener != nullptr) {
        Ogre::MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
        delete mMaterialMgrListener;
        mMaterialMgrListener = nullptr;   // recomendable siempre hacer que apunte a nullptr cualquier cosa borrada
    }

    // destruir el shader generator
    if (mShaderGenerator != nullptr) {
        Ogre::RTShader::ShaderGenerator::destroy();
        mShaderGenerator = nullptr;
    }

    // ELIMINAR VENTANAS
    // eliminar la ventana de "renderizado de ogre" (que se linkeo con la de sdl)
    if (ogreWindow != nullptr) {
        mRoot->destroyRenderTarget(ogreWindow);
        ogreWindow = nullptr;
    }

    // eliminar la ventana de sdl
    if (sdlWindow != nullptr) {
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        sdlWindow = nullptr;
    }

    // ELIMINAR EL ROOT
    delete mRoot;
    mRoot = nullptr;

    // ELIMINAR EL SISTEMA DE BUSQUEDA DE FICHEROS DE CONFIGURACION
    delete fsLayer;
    fsLayer = nullptr;
}

Tapioca::Node* Tapioca::GraphicsEngine::createNode(Vector3 pos, Vector3 scale) {
    return new Node(scnMgr, pos, scale);
    /*nodes.insert(node);
    return node;*/
}

Tapioca::Node* Tapioca::GraphicsEngine::createSelfManagedNode(Tapioca::Vector3 pos, Tapioca::Vector3 scale) {
    Tapioca::Node* node = new Node(scnMgr, pos, scale);
    selfManagedNodes.insert(node);
    return node;
}

Tapioca::Node* Tapioca::GraphicsEngine::createChildNode(Node* parent, Vector3 relativePos, Vector3 scale) {
    return new Node(scnMgr, relativePos, scale, parent);
    /*nodes.insert(node);
    return node;*/
}

//void GraphicsEngine::removeNode(Node* node) {
//    if (nodes.contains(node)) {
//        nodes.erase(node);
//        node->removeFromTree(&nodes);
//        delete node;
//    }
//}

Tapioca::Camera* Tapioca::GraphicsEngine::createCamera(Node* node, std::string name) {
    return new Camera(scnMgr, node, name);
}

Tapioca::Viewport* Tapioca::GraphicsEngine::createViewport(Camera* camera, int zOrder) {
    return new Viewport(ogreWindow, camera, zOrder);
}

Tapioca::LightDirectional* Tapioca::GraphicsEngine::createLightDirectional(
    Node* node, Vector3 direction, Vector4 color) {
    return new LightDirectional(scnMgr, node, color, direction);
}

Tapioca::Mesh* Tapioca::GraphicsEngine::createMesh(Node* node, std::string meshName) {
    return new Mesh(scnMgr, node, meshName);
}

Ogre::ManualObject* Tapioca::GraphicsEngine::createManualObject(Node* node) {
    Ogre::ManualObject* manualObject = scnMgr->createManualObject();
    node->attachObject(manualObject);
    return manualObject;
}

void Tapioca::GraphicsEngine::destroyManualObject(Ogre::ManualObject* object) { scnMgr->destroyManualObject(object); }

//void GraphicsEngine::removeObject(Tapioca::RenderObject* object) {
//    if (objects.contains(object)) {
//        objects.erase(object);
//        object->detachFromNode();
//    }
//}
