#include "GraphicsManager.h"

// PROPIOS
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

// OGRE
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#include "Utilities/SGTechniqueResolverListener.h"
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
      renderSys(nullptr), mMaterialMgrListener(nullptr), windowManager(nullptr), ogreWindow(nullptr),
      sdlWindow(nullptr), mwindowName(windowName), glContext(), planeNumber(0), mainLight(nullptr) { }


GraphicsManager::~GraphicsManager() {
    for (auto& node : selfManagedNodes)
        delete node;
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
        logError("GraphicsManager: No existe la ruta de plugins.cfg.");
        return false;
    }

    cfgPath = pluginsPath;
    cfgPath.erase(cfgPath.find_last_of("\\") + 1, cfgPath.size() - 1);   // "\\" equivale a "\"

    fsLayer->setHomePath(cfgPath);

    // (ruta plugins.cfg, ruta ogre.cfg, ruta ogre.log)
    // ogre.cfg sirve para guardar y restaurar la configuracion de render
    // ogre.log guarda un mensaje de depuracion
    // getWritablePath parte del homePath (asignado arriba)
    try {
        mRoot = new Ogre::Root(pluginsPath, "", fsLayer->getWritablePath("ogre.log"));
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
        logError(("GraphicsManager: Error al obtener informacion de la ventana de SDL: " + std::string(SDL_GetError()))
                     .c_str());
        return false;
    }
    // vincular ventana de SDL con Ogre
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));
    ogreWindow = mRoot->createRenderWindow(mwindowName, windowWidth, windowHeight, false, &miscParams);

    scnMgr = mRoot->createSceneManager();

    try {
        loadShaders();
    } catch (Ogre::Exception& e) {
        logError(("GraphicsEngine: Error al cargar shaders: " + e.getFullDescription()).c_str());
        return false;
    }

    // guardarse el mesh manager
    mshMgr = mRoot->getMeshManager();

    // si da problemas usar el renderSys cogerlo directamente desde root
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
#ifdef _DEBUG
    mRoot->loadPlugin("Codec_STBI_d.dll");   // Necesario para tener codec de archivos png jpg ...
#else
    mRoot->loadPlugin("Codec_STBI.dll");   // Necesario para tener codec de archivos png jpg ...
#endif
}

void GraphicsManager::setUpShadows() {
    // el color por de la luz ambiental es negro, lo que quiere decir que
    // por defecto no hay luz
    // Por lo tanto, para que un objeto se ilumine hay que poner una luz en escena
    scnMgr->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

    /*
    - MODULATIVE -> primer renderiza todos los objetos que se van a sombrear,
    luego hace un "paso" por cada luz oscureciendo las areas de cada objeto
    que necesitan sombras y por ultimo, se renderizan los objetos que no tienen
    sombras.
    Son imprecisas porque oscurecen el area uniformemente. Adem�s, el problema de
    The Silhouette Edge se ve empeorado justo por esto mismo. Es por eso que no es recomendable
    usar mas de dos luces en STENCIL MODULATIVE porque las transiciones no son suaves
    y las aristas son duras, por lo tanto, quedaria un corte si se superpusieran dos sombras.
    Sin embargo, su coste tambien es menor y dejan un acabado mucho mejor en luces estaticas precalculadas.
    - ADDITIVE -> se renderiza la escena por cada luz que haya con la contribucion de luz (y sombras)
    que produciria esa luz. Luego, se combinan/suman todos los renderizados. Esto produce sombras
    y mas realistas. Sin embargo, el coste tambien es mayor.
    */
    /*
    STENCIL -> se utiliza un buffer que incluye las zonas sombreadas. Luego, lo que incluya
    este buffer no se lleva a los posteriores pasos del renderizados (por lo tanto, solo se ve "negro",
    es decir, con sombras, pero no con color). Como el buffer solo tiene la opcion de incluir o no incluir zonas
    se van a ver aristas "duras", es decir, lineas de division entre la luz y la sombra (The Silhouette Edge).
    Para generar la zona de sombreado lo que se hace es extruir la silueta del objeto que ha proyectado la luz
    (por lo tanto, se necesita una lista con sus aristas) e interseccionarla con el resto de objetos.
    Las ventajs son que permite el autosombreado facilmente y funciona en maquinas de pocos recursos.
    Sin embargo, las desventajas son que: el aspecto visual es mas primitivo (por ejemplo, el problema de los arista "duras")
    y como se basa en geometria, esta tecnica comienza a tener mas coste conforme la geometria del mesh es mayor.
    TEXTURE-BASED -> proyectan sombras desde el punto de vista de cada luz que se convierten en texturas. Luego,
    cada una de las texturas de sombra se aplica en el objeto correspondiente. Los motores modernos funcionan de esta
    manera. El coste no escala de forma exponencial con geometrias mayores. La desventaja mayor es que se trata de una textura, por
    lo tanto, esta sujeto a su resolucion y se puede ver pixelado, pero se puede cambiar la resolucion/tam facilmente.
    Directional light -> por definicion son luces infinitas que proyectan sombras en la escena entera. Sin embargo, las texturas
    son finitas, por lo tanto, se veria de muy mala calidad porque habria que estirarla para que cabiera en toda la escena.
    Lo que se hace es poner uan textura en el area enfrente de la camara (lo que ve el usuario) y la va moviendo conforme
    la camara se mueve.
    Spotlight -> se utiliza una textura para la sombra en general y otra para darle la forma redondeada (puesto que las texturas
    son cuadradas y la primera esta rellena "completamente")
    Point light -> como se trata de una luz que apunta en todas las direcciones se requeriria aprox. 6 texturas. Por lo tanto,
    para reducir el coste se aproxima a un spotlight. Sin embargo el resultado no es ideal y solo queda bien si esta fuera del rango
    de vision. Por lo que se recomienda no proyectar sombras con este tipo de luz.
    Como no se soporta el autosombreado, se diferencia entre objetos que proyectan sombras (castShadows a true) y que reciben (castShadows a false)
    */

#define _PSSM

    Ogre::MaterialPtr casterMat = Ogre::MaterialManager::getSingletonPtr()->getByName("ShadowCaster");
    Ogre::MaterialPtr receiverMat = Ogre::MaterialManager::getSingletonPtr()->getByName("ShadowReceiverText");

    if (casterMat && receiverMat) {
        // CUANDO SE SETEA UNA CAMARA DE SOMBRAS, TODOS LOS OBJETOS QUE TIENEN QUE CASTEAR SOMBRAS
        // SINO, SE VE MAL
#ifndef _PSSM
        //scnMgr->setShadowCameraSetup(Ogre::LiSPSMShadowCameraSetup::create());
        scnMgr->setShadowCameraSetup(Ogre::FocusedShadowCameraSetup::create());

        scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);

        // tener mas precision a la hora de calcular la profundidad de la sombras (float)
        scnMgr->setShadowTexturePixelFormat(Ogre::PF_DEPTH32);
        // el tam de las texturas de sombras
        // cuanto mas alto es mayor es la calidad. Ademas, tiene que ser multiplo de 2
        scnMgr->setShadowTextureSize(2048);
        // cada luz tiene asociada su propia textura de sombra para evitar el estancamiento del pipeline
        // grafico que supondria utilizar (y cambiar) la misma textura de sombra para cada luz
        // Para evitar sobrecargar la memoria para las texturas, se establece cuantas texturas de sombra
        // puede haber, lo que se traduce en cuantas luces pueden proyectar sombras a la vez
        scnMgr->setShadowTextureCount(1);

        // distancia maxima de las sombras
        // Aumentando el tam de textura o reduciendo la distancia se pueden conseguir mejores resultados
        // para las sombras que producen las luces direccionales
        scnMgr->setShadowFarDistance(300.0f);

        // (no se si hace falta del todo si se ha utilizado un numero con mucha precision para
        // la profundidad de las sombras)
        scnMgr->setShadowTextureReceiverMaterial(receiverMat);
#endif
        scnMgr->setShadowTextureCasterMaterial(casterMat);

        // los objetos tanto proyectan como reciben sombras. Esto permite crear autosombras,
        // pero es responsabilidad del programador crear un shader con esa funcionalidad.
        scnMgr->setShadowTextureSelfShadow(true);

        scnMgr->setShadowTextureFadeStart(0.3f);

#ifdef _PSSM
        scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);

        scnMgr->setShadowFarDistance(3000.0f);

        // 3 textures per directional light (PSSM)
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
        for (int i = 0; i <= NUM_TEXTURES; ++i) {
            splitPoints[i] = splitPointList[i];
        }
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
                if (pass) {
                    pass->getFragmentProgramParameters()->setNamedConstant("pssmSplitPoints", splitPoints);
                }
            }
        }
#endif
    }
    /*
    else {
        scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    }
    */
}

void GraphicsManager::loadResources() {
    // archivos de Ogre
#ifdef _CREATE_EXE
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "./Ogre/Main", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "./Ogre/RTShaderLib", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#else
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "../Dependencies/Ogre/src/Media/Main", "FileSystem",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "../Dependencies/Ogre/src/Media/RTShaderLib", "FileSystem",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#endif

    // shaders para las sombras y material del physics debug
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "./assetsConfig", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

#ifdef _RESOURCES_DIR
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "./assets", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
#else
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "./assets", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
#endif
    Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
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

Ogre::RenderWindow* GraphicsManager::getOgreRenderTarget() { return ogreWindow; }

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

ParticleSystem* GraphicsManager::createParticleSystem(RenderNode* const node, std::string const& name,
                                                      std::string const& templateName, const bool emitting) {
    return new ParticleSystem(scnMgr, node, name, templateName, emitting);
}

Plane* GraphicsManager::createPlane(RenderNode* const node, const Vector3 rkNormal, const float fConstant,
                                    const Vector3& up_, std::string const& name, const float width, const float height,
                                    const int xSegments, const int ySegments, std::string const& material) {
    return new Plane(scnMgr, node, mshMgr, rkNormal, fConstant, up_, name, width, height, xSegments, ySegments);
}

Plane* GraphicsManager::createPlaneWithName(RenderNode* const node, const Vector3 rkNormal, const float fConstant,
                                            const Vector3& up_, const float width, const float height,
                                            const int xSegments, const int ySegments, std::string const& material) {

    string name = "Plane" + std::to_string(planeNumber++);
    Plane* plane = new Plane(scnMgr, node, mshMgr, rkNormal, fConstant, up_, name, width, height, xSegments, ySegments);
    return plane;
}

Plane* GraphicsManager::createPlane(RenderNode* const node, const float a, const float b, const float c, const float _d,
                                    const Vector3& up_, std::string const& name, const float width, const float height,
                                    const int xSegments, const int ySegments, std::string const& material) {
    return new Plane(scnMgr, node, mshMgr, a, b, c, _d, up_, name, width, height, xSegments, ySegments);
}

AnimationHelper* GraphicsManager::createAnimationHelper(Mesh* const object, const bool autoPlay, const bool loop) {
    return new AnimationHelper(object, autoPlay, loop);
}

Skybox* GraphicsManager::createSkybox(RenderNode* const node, std::string const& material, std::string const& name,
                                      const float distC, const bool orderC) {
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

Ogre::ManualObject* GraphicsManager::createManualObject(RenderNode* const node) {
    Ogre::ManualObject* manualObject = scnMgr->createManualObject();
    node->attachObject(manualObject);
    return manualObject;
}

void GraphicsManager::destroyManualObject(Ogre::ManualObject* const object) { scnMgr->destroyManualObject(object); }
}
