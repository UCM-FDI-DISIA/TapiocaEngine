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
// Propios
#include "GraphicsEngine.h"
#include "Node.h"
#include "Utilities/checkML.h"
using namespace Tapioca;

// BORRAR
Ogre::SceneNode* node;

GraphicsEngine::GraphicsEngine(std::string windowName, uint32_t w, uint32_t h)
    : fsLayer(nullptr)
    , mShaderGenerator(nullptr)
    , cfgPath()
    , mRoot(nullptr)
    , scnMgr(nullptr)
    , renderSys(nullptr)
    , mMaterialMgrListener(nullptr)
    , ogreWindow(nullptr)
    , sdlWindow(nullptr)
    , mwindowName(windowName)
    , windowWidth(w)
    , windowHeight(h) { }

GraphicsEngine::~GraphicsEngine() {
    for (auto& node : nodes) {
        delete node;
    }
    shutDown();
}

void GraphicsEngine::init() {
    // hayamos la ubicacion de plugins.cfg y a partir de la misma obtenenmos la ruta relativa de la carpeta de assets
    // el nombre es para crear un directorio dentro del home del usuario para distinguir entre diferentes aplicaciones de Ogre (da igual el nombre)
    fsLayer = new Ogre::FileSystemLayer("TapiocaDirectory");
    Ogre::String pluginsPath;
    // importante: la ruta donde esta plugins.cfg no puede tener caracteres especiales (solo alfabeto en ingles)
    pluginsPath = fsLayer->getConfigFilePath("plugins.cfg");

    // tratamiento de errores
    if (!Ogre::FileSystemLayer::fileExists(pluginsPath)) {
        //enviar mensaje al main para liberar toda la memoria y cerrar el programa
        //main->handleError(errormsg)
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

    // Iniciar SDL
    SDL_Init(SDL_INIT_EVERYTHING);

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
}

void GraphicsEngine::loadPlugIns() {
#ifdef _DEBUG
    mRoot->loadPlugin("Codec_STBI_d.dll");   // Necesario para tener codec de archivos png jpg ...
#else
    mRoot->loadPlugin("Codec_STBI.dll");   // Necesario para tener codec de archivos png jpg ...
#endif
}

void GraphicsEngine::loadResources() {
    // todos los assets deben estar en la carpeta assets (no pueden estar en subcarpetas)
    // sino, habria que poner mas rutas
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(cfgPath + "/assets", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void GraphicsEngine::loadShaders() {
    if (Ogre::RTShader::ShaderGenerator::initialize()) {
        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
        mShaderGenerator->addSceneManager(scnMgr);
        // CREAR LOS SHADERS PARA MATERIALES QUE VENGAN SIN ELLOS
        mMaterialMgrListener = new SGTechniqueResolverListener(mShaderGenerator);
        Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
    }
}

void GraphicsEngine::handleEvents() {
    //SDL_Event event;
    //while (SDL_PollEvent(&event)) {
    //    //ImGui_ImplSDL2_ProcessEvent(&event);
    //    if (event.type == SDL_QUIT) done = true;
    //    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
    //        event.window.windowID == SDL_GetWindowID(sdlWindow))
    //        done = true;
    //}
}

void GraphicsEngine::fixedUpdate() { node->yaw(Ogre::Degree(2)); }

void GraphicsEngine::render() { mRoot->renderOneFrame(); }

void GraphicsEngine::shutDown() {
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

void GraphicsEngine::testScene() {
    // Luz
    Ogre::Light* light = scnMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // Camara
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 15);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5);
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    // Puerto de vista de Ogre
    Ogre::Viewport* vp = ogreWindow->addViewport(cam);

    // fondo
    vp->setBackgroundColour(Ogre::ColourValue(0.83f, 0.5f, 0.9f));

    // objeto
    Ogre::Entity* ent = scnMgr->createEntity("mapache.mesh");
    //ent->setMaterialName("white");//si el material tiene vertex program y fragment program no da ningun problema
    node = scnMgr->getRootSceneNode()->createChildSceneNode();
    // node->yaw(Ogre::Degree(90));
    node->attachObject(ent);

    //try {
    //    // mroot->startRendering();// FALTAN ARCHIVOS .H Y . HLSL QUE ESTAN EN SRC MEDIA MAIN Y TIENEN QEU ESTAR EN LA CARPETA DE RECURSOS
    //    //la carpeta rtShader lib parece que tambien hara falta
    //} catch (Ogre::Exception& e) {

    //    std::cout << e.getFullDescription() << '\n';
    //}
    //bool done = false;
    //while (!done) {
    //    try {


    //        SDL_Event event;
    //        while (SDL_PollEvent(&event)) {
    //            //ImGui_ImplSDL2_ProcessEvent(&event);
    //            if (event.type == SDL_QUIT) done = true;
    //            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
    //                event.window.windowID == SDL_GetWindowID(sdlWindow))
    //                done = true;
    //        }
    //        node->yaw(Ogre::Degree(2));
    //        render();
    //    } catch (Ogre::Exception& e) {

    //        // std::cout << e.getFullDescription() << '\n';
    //    };
    //}
}

Node* GraphicsEngine::createNode(Vector3 pos, Vector3 scale) {
    Node* node = new Node(scnMgr, pos, scale);
    nodes.insert(node);
    return node;
}

Node* GraphicsEngine::createChildNode(Node* parent, Vector3 pos, Vector3 scale) {
    Node* node = new Node(scnMgr, pos, scale, parent);
    nodes.insert(node);
    return node;
}

void GraphicsEngine::removeNode(Node* node) {
    if (nodes.contains(node)) {
        nodes.erase(node);
        node->removeFromTree(&nodes);
        delete node;
    }
}