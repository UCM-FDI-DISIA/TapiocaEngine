#include "GraphicsManager.h"
#include "Utilities/LightPoint.h"
#include "Utilities/LightDirectional.h"
#include "Utilities/LightRectlight.h"
#include "Utilities/LightSpotlight.h"
#include "Utilities/RenderNode.h"
#include "Utilities/Camera.h"
#include "Utilities/Mesh.h"
#include "Utilities/Viewport.h"
#include "GraphicsManager.h"
#include "Utilities/BillboardSet.h"
#include "Utilities/Billboard.h"
#include "Utilities/ParticleSystem.h"
#include "Utilities/Plane.h"
#include "Utilities/AnimationHelper.h"
#include "Utilities/Skybox.h"
#include "Utilities/Skyplane.h"
#include "Components/CameraComponent.h"
#include "Utilities/NodeAnimator.h"
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#include "SGTechniqueResolverListener.h"
#include <OgreOverlaySystem.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include <SDL.h>
#include <SDL_syswm.h>
#undef main
#include "Structure/MainLoop.h"
#include "WindowManager.h"
#include "GraphicsManager.h"
#include "checkML.h"

namespace Tapioca {
GraphicsManager::GraphicsManager(std::string const& windowName, const uint32_t w, const uint32_t h)
    : fsLayer(nullptr), mShaderGenerator(nullptr), mRoot(nullptr), scnMgr(nullptr), mshMgr(nullptr), renderSys(nullptr),
      mMaterialMgrListener(nullptr), mainLoop(nullptr), windowManager(nullptr), ogreWindow(nullptr), sdlWindow(nullptr),
      glContext(), planeNumber(0), billboardNumber(0), nodeAnimatorNumber(0), particleSystemNumber(0),
      skyplaneNumber(0), skyboxNumber(0), mainLight(nullptr), cfgPath(""), windowName(windowName), zOrders(),
      selfManagedNodes() { }


GraphicsManager::~GraphicsManager() {
    for (auto& node : selfManagedNodes)
        delete node;
    selfManagedNodes.clear();

    shutDown();

    mainLoop = nullptr;
    windowManager = nullptr;
    mainLight = nullptr;
}

bool GraphicsManager::init() {
    windowManager = WindowManager::instance();
    mainLoop = MainLoop::instance();
    return windowManager != nullptr && mainLoop != nullptr;
}

bool GraphicsManager::initConfig() {
    sdlWindow = windowManager->getWindow();
    uint32_t windowWidth = windowManager->getWindowW();
    uint32_t windowHeight = windowManager->getWindowH();

    // Obtenemos la ubicacion de plugins.cfg y a partir de la misma obtenemos la ruta relativa
    // de la carpeta de assets. El nombre es para crear un directorio dentro del home del usuario
    // para distinguir entre diferentes aplicaciones de Ogre (da igual el nombre)
    fsLayer = new Ogre::FileSystemLayer("TapiocaDirectory");

    Ogre::String homePath = fsLayer->getWritablePath(".");
    if (fsLayer->fileExists(homePath)) fsLayer->removeDirectory(homePath);

    // importante: la ruta donde esta plugins.cfg no puede tener caracteres especiales (solo alfabeto en ingles)
    Ogre::String pluginsPath;
    pluginsPath = fsLayer->getConfigFilePath("plugins.cfg");

    // tratamiento de errores
    if (!Ogre::FileSystemLayer::fileExists("./plugins.cfg")) {
        delete fsLayer;
        logError("GraphicsManager: No existe la ruta de plugins.cfg.");
        return false;
    }

    cfgPath = pluginsPath;
    cfgPath.erase(cfgPath.find_last_of("\\") + 1, cfgPath.size() - 1);   // "\\" equivale a "\"

    fsLayer->setHomePath("./");

    // (ruta plugins.cfg, ruta ogre.cfg, ruta ogre.log)
    // ogre.cfg sirve para guardar y restaurar la configuracion de render
    // ogre.log guarda un mensaje de depuracion
    // getWritablePath parte del homePath (asignado arriba)
    try {
        mRoot = new Ogre::Root("./plugins.cfg", "", fsLayer->getWritablePath("ogre.log"));
    } catch (Ogre::Exception& e) {
        logError(("GraphicsEngine: Error al cargar root: " + e.getFullDescription()).c_str());
        return false;
    }

    // Otra forma: cargar los plugins desde codigo
    // loadPlugIns();   // cargar codec, que sirve para poder usar png, jpg... (para las texturas)
    // mRoot->loadPlugin("RenderSystem_GL3PLUS");   // cargar sistema de render por nombre

    // El sistema de render debe cargarse no podemos crearlo
    // Se especifica que se cargue este sistema de render en el archivo "plugins.cfg" que esta actualmente en TapiocaEngine/bin
    try {
        const Ogre::RenderSystemList renderSystems = mRoot->getAvailableRenderers();
        renderSys = renderSystems.front();
        mRoot->setRenderSystem(renderSys);
    } catch (Ogre::Exception& e) {
        logError(("GraphicsEngine: Error al cargar render system: " + e.getFullDescription()).c_str());
        return false;
    }

    // Inicializa ogre sin crear la ventana, siempre se hace despues de asignar el sistema de render
    try {
        mRoot->initialise(false);
    } catch (Ogre::Exception& e) {
        logError(("GraphicsEngine: Error al inicializar root: " + e.getFullDescription()).c_str());
        return false;
    }

    // Informacion de la ventana
    Ogre::NameValuePairList miscParams;

    // Se coge la informacion del sistema de render
    Ogre::ConfigOptionMap ropts = renderSys->getConfigOptions();
    std::istringstream mode(ropts["Video Mode"].currentValue);
    Ogre::String token;
    mode >> windowWidth;
    mode >> token;
    mode >> windowHeight;

    // Se agrega informacion del sistema de render y demas
    miscParams["FSAA"] = ropts["FSAA"].currentValue;
    miscParams["vsync"] = ropts["VSync"].currentValue;
    miscParams["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;


    // Crear ventana de Ogre (solo para render)
    SDL_SysWMinfo wmInfo = {0};
    SDL_VERSION(&wmInfo.version);
    if (!SDL_GetWindowWMInfo(sdlWindow, &wmInfo)) {
        logError(("GraphicsManager: Error al obtener informacion de la ventana de SDL: " + std::string(SDL_GetError()))
                     .c_str());
        return false;
    }
    // Vincular ventana de SDL con Ogre
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));
    ogreWindow = mRoot->createRenderWindow(windowName, windowWidth, windowHeight, false, &miscParams);

    scnMgr = mRoot->createSceneManager();

    try {
        loadShaders();
    } catch (Ogre::Exception& e) {
        logError(("GraphicsEngine: Error al cargar shaders: " + e.getFullDescription()).c_str());
        return false;
    }

    // Guardarse el mesh manager
    mshMgr = mRoot->getMeshManager();

    // Si da problemas usar el renderSys cogerlo directamente desde root
    renderSys->_initRenderTargets();

    try {
        loadResources();
    } catch (Ogre::Exception& e) {
        logError(("GraphicsEngine: Error al cargar recursos: " + e.getFullDescription()).c_str());
        return false;
    }

    try {
        setUpShadows();
    } catch (Ogre::Exception& e) {
        logError(("GraphicsEngine: Error al inicializar sombras: " + e.getFullDescription()).c_str());
        return false;
    }

    ogreWindow->getCustomAttribute("GLCONTEXT", &glContext);
    if (glContext == nullptr) {
        logError("GraphicsManager: Error al obtener el contexto de OpenGL.");
        return false;
    }
    windowManager->setGLContext(glContext);

    SDL_GL_SetSwapInterval(1);

    logInfo("GraphicsManager: Inicializado");
    return true;
}

void GraphicsManager::loadPlugIns() {
    // Necesario para tener codec de archivos png jpg ...
#ifdef _DEBUG
    mRoot->loadPlugin("Codec_STBI_d.dll");
#else
    mRoot->loadPlugin("Codec_STBI.dll");
#endif
}

void GraphicsManager::setUpShadows() {
    scnMgr->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

#define _PSSM

    Ogre::MaterialPtr casterMat = Ogre::MaterialManager::getSingletonPtr()->getByName("ShadowCaster");
    Ogre::MaterialPtr receiverMat = Ogre::MaterialManager::getSingletonPtr()->getByName("ShadowReceiverText");

    if (casterMat && receiverMat) {
#ifndef _PSSM
        scnMgr->setShadowCameraSetup(Ogre::FocusedShadowCameraSetup::create());
        scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);

        scnMgr->setShadowTexturePixelFormat(Ogre::PF_DEPTH32);
        scnMgr->setShadowTextureSize(2048);
        scnMgr->setShadowTextureCount(1);
        scnMgr->setShadowFarDistance(300.0f);
        scnMgr->setShadowTextureReceiverMaterial(receiverMat);
#endif
        scnMgr->setShadowTextureCasterMaterial(casterMat);
        scnMgr->setShadowTextureSelfShadow(true);
        scnMgr->setShadowTextureFadeStart(0.3f);

#ifdef _PSSM
        scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);

        scnMgr->setShadowFarDistance(3000.0f);

        scnMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
        scnMgr->setShadowTextureCount(3);
        scnMgr->setShadowTextureConfig(0, 2048, 2048, Ogre::PF_DEPTH32);
        scnMgr->setShadowTextureConfig(1, 1024, 1024, Ogre::PF_DEPTH32);
        scnMgr->setShadowTextureConfig(2, 512, 512, Ogre::PF_DEPTH32);

        Ogre::PSSMShadowCameraSetup* pssmSetup = new Ogre::PSSMShadowCameraSetup();
        pssmSetup->setSplitPadding(1.0);
        pssmSetup->calculateSplitPoints(NUM_TEXTURES, 1.0, scnMgr->getShadowFarDistance());
        Ogre::Vector4 splitPoints(0.0);
        Ogre::PSSMShadowCameraSetup::SplitPointList splitPointList = pssmSetup->getSplitPoints();
        for (int i = 0; i <= NUM_TEXTURES; ++i)
            splitPoints[i] = splitPointList[i];

        pssmSetup->setOptimalAdjustFactor(0, 2);
        pssmSetup->setOptimalAdjustFactor(1, 1);
        pssmSetup->setOptimalAdjustFactor(2, 0.5);
        scnMgr->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(pssmSetup));

        Ogre::ResourceManager::ResourceMapIterator it = Ogre::MaterialManager::getSingleton().getResourceIterator();
        std::vector<Ogre::MaterialPtr> v;
        while (it.hasMoreElements()) {
            auto mat = Ogre::static_pointer_cast<Ogre::Material>(it.getNext());
            if (mat) {
                auto pass = mat->getTechnique(0)->getPass("Lighting");
                if (pass) pass->getFragmentProgramParameters()->setNamedConstant("pssmSplitPoints", splitPoints);
            }
        }
#endif
    }
}

void GraphicsManager::loadResources() const {
#ifdef _CREATE_EXE
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "./Ogre/Main", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "./Ogre/RTShaderLib", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#else
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "./TapiocaFiles/Graphics/OgreFiles/Main", "FileSystem",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "./TapiocaFiles/Graphics/OgreFiles/RTShaderLib", "FileSystem",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#endif

    // Shaders para las sombras y material del physics debug
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "./TapiocaFiles/Graphics/Shadows", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "./TapiocaFiles/Graphics/ColliderDrawer", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        true);

    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "./assets", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);

    Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

void GraphicsManager::loadShaders() {
    if (Ogre::RTShader::ShaderGenerator::initialize()) {
        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
        mShaderGenerator->addSceneManager(scnMgr);

        // Crea los shaders para materiales que vengan sin ellos
        mMaterialMgrListener = new SGTechniqueResolverListener(mShaderGenerator);
        Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
    }
}

void GraphicsManager::render() {
    try {
        mRoot->renderOneFrame();
    } catch (Ogre::Exception e) {
        logError(("GraphicsManager: Error al renderizar frame. Detalles:\n" + e.getDescription()).c_str());
        MainLoop::instance()->exit();
    }
}

bool GraphicsManager::handleEvents(const SDL_Event& event) {
    if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            logInfo("GraphicsManager: Resized");
            ogreWindow->resize(windowManager->getWindowW(), windowManager->getWindowH());
            // Para que UIManager tenga en cuenta el cambio de tamano de la ventana
            return false;
        }
    }
    return false;
}

void GraphicsManager::shutDown() {
    if (mRoot == nullptr) return;

    // Eliminar el scene manager de ogre
    if (scnMgr != nullptr) {
        mShaderGenerator->removeSceneManager(scnMgr);
        mRoot->destroySceneManager(scnMgr);
    }
    scnMgr = nullptr;

    // Elimina el sistema de shaders
    Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);
    if (mMaterialMgrListener != nullptr) {
        Ogre::MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
        delete mMaterialMgrListener;
        mMaterialMgrListener = nullptr;   // recomendable siempre hacer que apunte a nullptr cualquier cosa borrada
    }

    // Elimina el shader generator
    if (mShaderGenerator != nullptr) {
        Ogre::RTShader::ShaderGenerator::destroy();
        mShaderGenerator = nullptr;
    }

    // Elimina ventanas
    if (ogreWindow != nullptr) {
        mRoot->destroyRenderTarget(ogreWindow);
        ogreWindow = nullptr;
    }

    // Elimina el contexto de OpenGL
    if (glContext != nullptr) {
        SDL_GL_DeleteContext((SDL_GLContext)glContext);
        glContext = nullptr;
    }

    // Elimina el root de ogre
    delete mRoot;
    mRoot = nullptr;

    // Elimina el sistema de busqueda de ficheros de configuracion
    delete fsLayer;
    fsLayer = nullptr;
}

bool GraphicsManager::checkResourceExists(std::string name) {
    return Ogre::ResourceGroupManager::getSingletonPtr()->resourceExists(
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, name);
}

bool GraphicsManager::checkParticleSystemExists(std::string name) {
    return Ogre::ParticleSystemManager::getSingletonPtr()->getTemplate(name) != nullptr;
}

int GraphicsManager::askForZOrder(int requiredZOrder) {
    int zOrder = requiredZOrder;
    if (zOrders.contains(requiredZOrder)) {
        bool found = false;
        for (int cont = requiredZOrder + 1; cont <= MAXIMUM_Z_ORDER && !found; ++cont) {
            if (!zOrders.contains(cont)) {
                zOrder = cont;
                zOrders.insert(zOrder);
                found = true;
            }
        }
        if (!found) zOrder = -1;
    }
    else
        zOrders.insert(requiredZOrder);
    return zOrder;
}

void GraphicsManager::removeZOrder(int zOrder) {
    if (zOrders.contains(zOrder)) zOrders.erase(zOrder);
}

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

LightPoint* GraphicsManager::createLightPoint(RenderNode* const node, const Vector4 color) {
    return new LightPoint(scnMgr, node, color);
}

LightRectlight* GraphicsManager::createLightRectlight(RenderNode* const node, const Vector3 direction,
                                                      const float width, const float height, const Vector4 color) {
    return new LightRectlight(scnMgr, node, color, width, height, direction);
}

LightSpotlight* GraphicsManager::createLightSpotlight(RenderNode* const node, const Vector3 direction,
                                                      const Vector4 color) {
    return new LightSpotlight(scnMgr, node, color, direction);
}

void GraphicsManager::setMainLight(LightDirectional* lightDir) {
    if (mainLight != nullptr) {
        logWarn("GraphicsManager: Se ha cambiado la luz principal (la que produce sombras)");
        mainLight->produceShadows(false);
    }
    mainLight = lightDir;
}

void GraphicsManager::removeMainLight(LightDirectional* lightDir) {
    if (mainLight == lightDir) {
        logWarn("GraphicsManager: No hay ninguna luz principal (la que produce sombras)");
        lightDir->produceShadows(false);
        mainLight = nullptr;
    }
}

Mesh* GraphicsManager::createMesh(RenderNode* const node, std::string const& meshName) {
    try {
        return new Mesh(scnMgr, node, meshName);
    } catch (const Ogre::Exception& exception) {
        logError(
            ("GraphicsManager: No se ha podido crear la mesh. Detalles:\n" + exception.getFullDescription()).c_str());
    }
    return nullptr;
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

BillboardSet* GraphicsManager::createBillboardSetWithName(RenderNode* const node, const unsigned int poolSize) {
    std::string name = "Billboard" + std::to_string(billboardNumber++);
    return new BillboardSet(scnMgr, node, name, poolSize);
}

ParticleSystem* GraphicsManager::createParticleSystem(RenderNode* const node, std::string const& name,
                                                      std::string const& templateName, const bool emitting) {
    return new ParticleSystem(scnMgr, node, name, templateName, emitting);
}

ParticleSystem* GraphicsManager ::createParticleSystemWithName(RenderNode* const node, std::string const& templateName,
                                                               const bool emitting) {
    std::string name = "ParticleSystem" + std::to_string(particleSystemNumber++);
    return new ParticleSystem(scnMgr, node, name, templateName, emitting);
}


Plane* GraphicsManager::createPlane(RenderNode* const node, const Vector3 rkNormal, const float fConstant,
                                    const Vector3& up, std::string const& name, const float width, const float height,
                                    const int xSegments, const int ySegments, std::string const& material) {
    return new Plane(scnMgr, node, mshMgr, rkNormal, fConstant, up, name, width, height, xSegments, ySegments);
}

Plane* GraphicsManager::createPlaneWithName(RenderNode* const node, const Vector3 rkNormal, const float fConstant,
                                            const Vector3& up, const float width, const float height,
                                            const int xSegments, const int ySegments, std::string const& material) {
    std::string name = "Plane" + std::to_string(planeNumber++);
    return new Plane(scnMgr, node, mshMgr, rkNormal, fConstant, up, name, width, height, xSegments, ySegments);
}

Plane* GraphicsManager::createPlane(RenderNode* const node, const float a, const float b, const float c, const float d,
                                    const Vector3& up, std::string const& name, const float width, const float height,
                                    const int xSegments, const int ySegments, std::string const& material) {
    return new Plane(scnMgr, node, mshMgr, a, b, c, d, up, name, width, height, xSegments, ySegments);
}

AnimationHelper* GraphicsManager::createAnimationHelper(Mesh* const object, const bool autoPlay, const bool loop) {
    return new AnimationHelper(object, autoPlay, loop);
}

NodeAnimator* GraphicsManager::createNodeAnimator(RenderNode* const node, const float duration,
                                                  const std::string& name) {
    return new NodeAnimator(scnMgr, node, duration, name);
}

NodeAnimator* GraphicsManager::createNodeAnimatorWithName(RenderNode* const node, const float duration) {
    std::string name = "Anim" + std::to_string(nodeAnimatorNumber++);
    return new NodeAnimator(scnMgr, node, duration, name);
}

Skybox* GraphicsManager::createSkybox(RenderNode* const node, std::string const& material, std::string const& name,
                                      const float distC, const bool orderC) {
    return new Skybox(scnMgr, node, material, name, distC, orderC);
}

Skybox* GraphicsManager::createSkyboxWithName(RenderNode* const node, std::string const& material, const float distC,
                                              const bool orderC) {
    std::string name = "Skybox" + std::to_string(skyboxNumber++);
    return new Skybox(scnMgr, node, material, name, distC, orderC);
}

Skyplane* GraphicsManager::createSkyplane(RenderNode* const node, std::string const& materialName,
                                          std::string const& name, const bool enable, const Vector3 rkNormal,
                                          const float fConstant, const float scale, const float tiling,
                                          const bool drawFirst, const float bow, const int xsegments,
                                          const int ysegments) {
    return new Skyplane(scnMgr, node, mshMgr, materialName, name, enable, rkNormal, fConstant, scale, tiling, drawFirst,
                        bow, xsegments, ysegments);
}

Skyplane* GraphicsManager::createSkyplaneWithName(RenderNode* const node, std::string const& materialName,
                                                  const bool enable, const Vector3 rkNormal, const float fConstant,
                                                  const float scale, const float tiling, const bool drawFirst,
                                                  const float bow, const int xsegments, const int ysegments) {
    std::string name = "Skyplane" + std::to_string(skyplaneNumber++);
    return new Skyplane(scnMgr, node, mshMgr, materialName, name, enable, rkNormal, fConstant, scale, tiling, drawFirst,
                        bow, xsegments, ysegments);
}

Ogre::ManualObject* GraphicsManager::createManualObject(RenderNode* const node) {
    Ogre::ManualObject* manualObject = scnMgr->createManualObject();
    node->attachObject(manualObject);
    return manualObject;
}

void GraphicsManager::destroyManualObject(Ogre::ManualObject* const object) { scnMgr->destroyManualObject(object); }
}
