#include "GraphicsManager.h"

// PROPIOS
#include "LightPoint.h"
#include "LightDirectional.h"
#include "RenderNode.h"
#include "Camera.h"
#include "Mesh.h"
#include "Viewport.h"
#include "GraphicsManager.h"
#include "BillboardSet.h"
#include "Billboard.h"
#include "ParticleSystem.h"
#include "Plane.h"
#include "AnimationHelper.h"
#include "Skybox.h"
#include "Skyplane.h"

// OGRE
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#include "SGTechniqueResolverListener.h"
#include <OgreOverlaySystem.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

// SDL
#include <SDL.h>
#include <SDL_syswm.h>
#undef main

#include "WindowManager.h"
#include "GraphicsManager.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<GraphicsManager>;
template<>
GraphicsManager* Singleton<GraphicsManager>::instance_ = nullptr;

GraphicsManager::GraphicsManager(std::string const& windowName, const uint32_t w, const uint32_t h)
    : fsLayer(nullptr), mShaderGenerator(nullptr), cfgPath(), mRoot(nullptr), scnMgr(nullptr), mshMgr(nullptr),
      renderSys(nullptr), mMaterialMgrListener(nullptr), windowManager(nullptr), ogreWindow(nullptr), sdlWindow(nullptr),
      mwindowName(windowName), glContext(), overSys(nullptr) { }

GraphicsManager::~GraphicsManager() {
    for (auto& node : selfManagedNodes) delete node;
    selfManagedNodes.clear();

    shutDown();
}

bool GraphicsManager::init() {
    windowManager = WindowManager::instance();
    sdlWindow = windowManager->getWindow();
    uint32_t windowWidth = windowManager->getWindowW();
    uint32_t windowHeight = windowManager->getWindowH();

    // Obtenemos la ubicacion de plugins.cfg y a partir de la misma obtenemos la ruta relativa
    // de la carpeta de assets. El nombre es para crear un directorio dentro del home del usuario
    // para distinguir entre diferentes aplicaciones de Ogre (da igual el nombre)
    // ESTO ESTA FATAL, NO DEBERIA CREAR UN DIRECTORIO EN EL HOME DEL USUARIO
    fsLayer = new Ogre::FileSystemLayer("TapiocaDirectory");

    Ogre::String homePath = fsLayer->getWritablePath(".");
    if (fsLayer->fileExists(homePath)) {
        fsLayer->removeDirectory(homePath);
    }

    // importante: la ruta donde esta plugins.cfg no puede tener caracteres especiales (solo alfabeto en ingles)
    Ogre::String pluginsPath;
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
    overSys = new Ogre::OverlaySystem();

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


    
    // crear ventana de Ogre (solo para render)
    // ya antes se le ha indicado en los parametros que existe la ventana de SDL
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    if (!SDL_GetWindowWMInfo(sdlWindow, &wmInfo)) {
#ifdef _DEBUG
        std::cerr << "Error al obtener informacion de la ventana de SDL: " << SDL_GetError() << '\n';
#endif
        return false;
    }
    // vincular ventana de SDL con Ogre
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));
    ogreWindow = mRoot->createRenderWindow(mwindowName, windowWidth, windowHeight, false, &miscParams);

    scnMgr = mRoot->createSceneManager();
    scnMgr->addRenderQueueListener(overSys);
    loadShaders();

    // guardarse el mesh manager
    mshMgr = mRoot->getMeshManager();

    // si da problemas usar el renderSys cogerlo directamente desde root
    renderSys->_initRenderTargets();
    loadResources();


    ogreWindow->getCustomAttribute("GLCONTEXT", &glContext);
    if (glContext == nullptr) {
#ifdef _DEBUG
    	std::cerr << "Error al obtener el contexto de OpenGL\n";
#endif
        return false;
    }
    windowManager->setGLContext(glContext);

    SDL_GL_SetSwapInterval(1);

    return true;
}

void GraphicsManager::loadPlugIns() {
#ifdef _DEBUG
    mRoot->loadPlugin("Codec_STBI_d.dll");   // Necesario para tener codec de archivos png jpg ...
#else
    mRoot->loadPlugin("Codec_STBI.dll");   // Necesario para tener codec de archivos png jpg ...
#endif
}

void GraphicsManager::loadResources() {
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "/assetsConfig", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

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

void GraphicsManager::loadShaders() {
    if (Ogre::RTShader::ShaderGenerator::initialize()) {
        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
        mShaderGenerator->addSceneManager(scnMgr);

        // CREAR LOS SHADERS PARA MATERIALES QUE VENGAN SIN ELLOS
        mMaterialMgrListener = new SGTechniqueResolverListener(mShaderGenerator);
        Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
    }
}

void GraphicsManager::render() { mRoot->renderOneFrame(); }

bool GraphicsManager::handleEvents(const SDL_Event& event) { 
    if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
#ifdef _DEBUG
            std::cout << "Resized\n";
#endif
            ogreWindow->resize(windowManager->getWindowW(), windowManager->getWindowH());
        }
    }    
    return true; 
}



void GraphicsManager::shutDown() {
    if (mRoot == nullptr) return;

    // ELIMINAR EL SCENE MANAGER
    scnMgr->removeRenderQueueListener(overSys);
    mShaderGenerator->removeSceneManager(scnMgr);
    mRoot->destroySceneManager(scnMgr);

    if (overSys != nullptr) {
        delete overSys;
        overSys = nullptr;
    }

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

    if (glContext != nullptr) {
        SDL_GL_DeleteContext((SDL_GLContext)glContext);
        glContext = nullptr;
    }


    // ELIMINAR EL ROOT
    delete mRoot;
    mRoot = nullptr;

    // ELIMINAR EL SISTEMA DE BUSQUEDA DE FICHEROS DE CONFIGURACION
    delete fsLayer;
    fsLayer = nullptr;
}

Ogre::SceneManager* GraphicsManager::getSceneManager() { return nullptr; }

RenderNode* GraphicsManager::createNode(const Vector3 pos, const Vector3 scale) {
    return new RenderNode(scnMgr, pos, scale);
}

RenderNode* GraphicsManager::createSelfManagedNode(const Vector3 pos, const Vector3 scale) {
    RenderNode* node = new RenderNode(scnMgr, pos, scale);
    selfManagedNodes.insert(node);
    return node;
}

Camera* GraphicsManager::createCamera(RenderNode* const node, std::string const& name) {
    return new Camera(scnMgr, node, name);
}

Viewport* GraphicsManager::createViewport(Camera* const camera, const int zOrder) {
    return new Viewport(ogreWindow, camera, zOrder);
}

LightDirectional* GraphicsManager::createLightDirectional(RenderNode* const node, const Vector3 direction,
                                                         const Vector4 color) {
    return new LightDirectional(scnMgr, node, color, direction);
}

Mesh* GraphicsManager::createMesh(RenderNode* const node, std::string const& meshName) {
    return new Mesh(scnMgr, node, meshName);
}

Billboard* GraphicsManager::createBillboard(RenderNode* const node, std::string const& name, const Vector3 position,
                                           const Vector4 colour) {
    Tapioca::BillboardSet* set = new BillboardSet(scnMgr, node, name, 1);
    return set->addBillboard(position, colour);
}

BillboardSet* GraphicsManager::createBillboardSet(RenderNode* const node, std::string const& name,
                                                 const unsigned int poolSize) {
    return new BillboardSet(scnMgr, node, name, poolSize);
}

ParticleSystem* GraphicsManager::createParticleSystem(Ogre::SceneManager* const scnMgr, RenderNode* const node,
                                                     std::string const& name, std::string const& templateName,
                                                     const bool emitting) {
    return new ParticleSystem(scnMgr, node, name, templateName, emitting);
}

Plane* GraphicsManager::createPlane(RenderNode* const node, const Vector3 rkNormal, const float fConstant,
                                   std::string const& name, const float width, const float height, const int xSegments,
                                    const int ySegments, std::string const& material) {
    return new Plane(scnMgr, node, mshMgr, rkNormal, fConstant, name, width, height, xSegments, ySegments);
}

Plane* GraphicsManager::createPlane(RenderNode* const node, const float a, const float b, const float c, const float _d,
                                   std::string const& name, const float width, const float height, const int xSegments,
                                   const int ySegments, std::string const& material) {
    return new Plane(scnMgr, node, mshMgr, a, b, c, _d, name, width, height, xSegments, ySegments);
}

AnimationHelper* GraphicsManager::createAnimationHelper(Mesh* const object, const bool autoPlay, const bool loop) {
    return new AnimationHelper(object, autoPlay, loop);
}

Skybox* GraphicsManager::createSkybox(RenderNode* const node, std::string const& texture, const float distC,
                                     const bool orderC) {
    return new Skybox(scnMgr, node, texture, distC, orderC);
}

Skyplane* GraphicsManager::createSkyplane(RenderNode* const node, std::string const& materialName, const bool enable,
                                         const Vector3 rkNormal, const float fConstant, const float scale,
                                         const float tiling, const bool drawFirst, const float bow, const int xsegments,
                                         const int ysegments) {
    return new Skyplane(scnMgr, node, materialName, enable, rkNormal, fConstant, scale, tiling, drawFirst, bow,
                        xsegments, ysegments);
}

Ogre::ManualObject* GraphicsManager::createManualObject(RenderNode* const node) {
    Ogre::ManualObject* manualObject = scnMgr->createManualObject();
    node->attachObject(manualObject);
    return manualObject;
}

void GraphicsManager::destroyManualObject(Ogre::ManualObject* const object) { scnMgr->destroyManualObject(object); }
}