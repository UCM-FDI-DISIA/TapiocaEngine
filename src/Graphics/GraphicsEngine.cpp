#include "GraphicsEngine.h"
//#include "checkML.h"
//Includes de Ogre
#include <Ogre.h>
#include <OgreFileSystem.h>
#include <OgreFileSystemLayer.h>
#include <OgreRTShaderSystem.h>
#include "SGTechniqueResolverListener.h"
#include <OgreGL3PlusRenderSystem.h>
//Includes de SDL
#include <SDL.h>
#include <SDL_syswm.h>
#undef main   //sdl lo define aparentemente
#include <iostream>
#include <Windows.h>
// usings
using namespace Tapioca;

// Si se quiere probar hay que llamar main a este metodo y el "main" de Ogre.cpp ponerle otro nombre
int main2() {

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);   // esto genera el informe al acabar el proceso
                                                                    //  Ogre::Root* raiz = new Ogre::Root();
    GraphicsEngine* g = new GraphicsEngine();
    g->init();
    g->testScene();
    g->shutDown();
    delete g;
    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    //_CrtDumpMemoryLeaks(); //no reportara esto memoria qeu usan las dlls de normal por que escribe ntes de que se desvinculen
    //si hacemos wel informe aqui el main todavia  no ha terminado y sale la memoria ocupada por la DLL que todavia no se ha desenlazado
    return 0;
}

GraphicsEngine::GraphicsEngine()
    : fsLayer(nullptr)
    , mShaderGenerator(nullptr)
    , cfgPath()
    , mRoot(nullptr)
    , scnMgr(nullptr)
    , renderSys(nullptr)
    , mMaterialMgrListener(nullptr)
    , ogreWindow(nullptr)
    , windowWidth()
    , windowHeight()
    , sdlWindow(nullptr) { }

void GraphicsEngine::init(std::string windowName, uint32_t w, uint32_t h) {
    mwindowName = windowName;
    windowWidth = w;
    windowHeight = h;

    //Iniciar Ogre
    mRoot = new Ogre::Root();
    //loadPlugIns();

    //El sistema de render debe cargarse no podemos crearlo
   //mRoot->loadPlugin("RenderSystem_GL3PLUS");Se especifica qeu se cargue este sistema de render en el archivo "plugins.cfg" que esta actualmente en TapiocaEngine/bin
    const Ogre::RenderSystemList renderSystems = mRoot->getAvailableRenderers();

    Ogre::NameValuePairList miscParams;
    Ogre::RenderSystemList::iterator r_it;
    renderSys = renderSystems.front();
    mRoot->setRenderSystem(renderSys);
    // inicializa ogre sin crea la ventana, siempre se hace despues de asignar el rendersystem
    mRoot->initialise(false);  
    //informacion de la ventana que vamos a construir
    Ogre::ConfigOptionMap ropts = mRoot->getRenderSystem()->getConfigOptions();
    std::istringstream mode(ropts["Video Mode"].currentValue);
    Ogre::String token;
    mode >> w;       // width
    mode >> token;   // 'x' as seperator between width and height
    mode >> h;       // height
    miscParams["FSAA"] = ropts["FSAA"].currentValue;
    miscParams["vsync"] = ropts["VSync"].currentValue;
    miscParams["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;

    ////Iniciar SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    ////iniciar ventana SDL2
    Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    if (ropts["Full Screen"].currentValue == "Yes") flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALLOW_HIGHDPI;
    //else  flags = SDL_WINDOW_RESIZABLE;
    sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
    miscParams["externalWindowHandle"] =
        Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));   //esta instruccion vincula Ogre y SDL

    //le pasamos la info de la ventana a ogre y lo asignamos como renderer
    ogreWindow = mRoot->createRenderWindow(windowName, w, h, false, &miscParams);
    scnMgr = mRoot->createSceneManager();
    loadShaders();
    mRoot->getRenderSystem()->_initRenderTargets();
    loadResources();
}
void GraphicsEngine::loadPlugIns() {
#ifdef _DEBUG
   // mRoot->loadPlugin("Codec_STBI_d.dll");   //Necesario para tener codec de archivos png jpg ...
#else
   // mRoot->loadPlugin("Codec_STBI.dll");   //Necesario para tener codec de archivos png jpg ...
#endif
}

void GraphicsEngine::loadResources() {
    //hayamos la ubicacion de plugins.cfg y a partir de la misma obtenenmos la ruta relativa de la carpeta de assets
    fsLayer = new Ogre::FileSystemLayer("Directorio");
    cfgPath = fsLayer->getConfigFilePath("plugins.cfg");
    // tratamiento de errores
    if (!Ogre::FileSystemLayer::fileExists(cfgPath)) {
        //enviar mensaje al main para liberar toda la memoria y cerrar el programa
        //main->handelError(errormsg)
    }
    cfgPath.erase(cfgPath.find_last_of("\\") + 1, cfgPath.size() - 1);   // "\\" equivale a "\"
    //Todos los assets estaran en la carpeta de assets verdad?
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(cfgPath + "/assets", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void GraphicsEngine::loadShaders() {
    if (Ogre::RTShader::ShaderGenerator::initialize()) {
        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
        mShaderGenerator->addSceneManager(scnMgr);
        //CREAR LOS SHADERS PARA MATERIALES QUE VENGAN SIN ELLOS
        mMaterialMgrListener = new SGTechniqueResolverListener(mShaderGenerator);
        Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
    }
}

void GraphicsEngine::renderFrame() { mRoot->renderOneFrame(); }

void GraphicsEngine::shutDown() {


    // eliminar el scene manager
    mShaderGenerator->removeSceneManager(scnMgr);
    mRoot->destroySceneManager(scnMgr);

    // eliminar el sistema de shaders
    Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);

    // se desregistra el listener que crea shaders para objetos que vengan sin ellos ya definidos
    // sgtechniqueresolverlistener * mmaterialmgrlistener;
    if (mMaterialMgrListener != nullptr) {
        Ogre::MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
        delete mMaterialMgrListener;
        mMaterialMgrListener = nullptr;   // recomendable siempre hacer que apunte a nullptr cualquier cosa borrada
    }

    // destruir el shader generator (para los objetos que vienen sin shaders)
    if (mShaderGenerator != nullptr) {
        Ogre::RTShader::ShaderGenerator::destroy();
        mShaderGenerator = nullptr;
    }


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
    // eliminar el root
    try {

        delete mRoot;
    } catch (std::exception& e) { 
    std::cout << e.what(); 
    }

    mRoot = nullptr;

    // eliminar el sistema de busqueda de ficheros de configuracion
    delete fsLayer;
    fsLayer = nullptr;
}
void GraphicsEngine::testScene() {
    Ogre::Light* light = scnMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // Ogre Camera
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 15);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5);
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    //puerto de vista de Ogre
    Ogre::Viewport* vp = ogreWindow->addViewport(cam);
    vp->setBackgroundColour(Ogre::ColourValue(0.83, 0.5, 0.9));
    Ogre::Entity* ent = scnMgr->createEntity("mapache.mesh");
    // ent->setMaterialName("Material");//si el material tiene vertex program y fragment program no da ningun problema
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    // node->yaw(Ogre::Degree(90));
    node->attachObject(ent);
    try {
        // mroot->startRendering();// FALTAN ARCHIVOS .H Y . HLSL QUE ESTAN EN SRC MEDIA MAIN Y TIENEN QEU ESTAR EN LA CARPETA DE RECURSOS
        //la carpeta rtShader lib parece que tambien hara falta
    } catch (Ogre::Exception& e) {

        std::cout << e.getFullDescription() << '\n';
    }
    bool done = false;
    while (!done) {
        try {


            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                //ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT) done = true;
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                    event.window.windowID == SDL_GetWindowID(sdlWindow))
                    done = true;
            }
            node->yaw(Ogre::Degree(2));
            renderFrame();
        } catch (Ogre::Exception& e) {

            // std::cout << e.getFullDescription() << '\n';
        };
    }
}
