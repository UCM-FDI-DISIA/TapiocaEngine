#include "GraphicsManager.h"

// PROPIOS
#include "LightPoint.h"
#include "LightDirectional.h"
#include "LightRectlight.h"
#include "LightSpotlight.h"
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
      renderSys(nullptr), mMaterialMgrListener(nullptr), windowManager(nullptr), ogreWindow(nullptr),
      sdlWindow(nullptr), mwindowName(windowName), glContext(), overSys(nullptr) { }


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

    loadConfigFiles();
    loadResources();

    setUpShadows();


    Ogre::MaterialPtr casterMat = Ogre::MaterialManager::getSingletonPtr()->getByName("Sinbad/Body");

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

void GraphicsManager::loadConfigFiles() {
    // archivos de Ogre
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "../Dependencies/Ogre/src/Media/Main", "FileSystem",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "../Dependencies/Ogre/src/Media/RTShaderLib", "FileSystem",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        cfgPath + "./shadowsConfig", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    // shaders para las sombras
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
    Son imprecisas porque oscurecen el area uniformemente. Además, el problema de
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
    Ogre::MaterialPtr casterMat = Ogre::MaterialManager::getSingletonPtr()->getByName("ShadowCaster");
    Ogre::MaterialPtr receiverMat = Ogre::MaterialManager::getSingletonPtr()->getByName("ShadowReceiver");
    if (casterMat && false) {

        scnMgr->setShadowCameraSetup(Ogre::LiSPSMShadowCameraSetup::create());
        //scnMgr->setShadowCameraSetup(Ogre::FocusedShadowCameraSetup::create());

        scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);

        scnMgr->setShadowTexturePixelFormat(Ogre::PF_DEPTH32);   // antes depth

        //// tener mas precision a la hora de calcular la profundiad de la sombras (float)
        scnMgr->setShadowTextureCasterMaterial(casterMat);
        //scnMgr->setShadowTextureReceiverMaterial(receiverMat);

        //// los objetos tanto proyectan como reciben sombras. Esto permite crear autosombras,
        //// pero es responsabilidad del programador crear un shader con esa funcionalidad.
        scnMgr->setShadowTextureSelfShadow(true);

        //// el tam de las texturas de sombras
        //// cuanto mas alto es mayor es la calidad. Ademas, tiene que ser multiplo de 2
        scnMgr->setShadowTextureSize(1024);
        //// distancia maxima de las sombras
        //// Aumentando el tam de textura o reduciendo la distancia se pueden conseguir mejores resultados
        //// para las sombras que producen las luces direccionales
        scnMgr->setShadowFarDistance(1000.0f);
        //// cada luz tiene asociada su propia textura de sombra para evitar el estancamiento del pipeline
        //// grafico que supondria utilizar (y cambiar) la misma textura de sombra para cada luz
        //// Para evitar sobrecargar la memoria para las texturas, se establece cuantas texturas de sombra
        //// puede haber, lo que se traduce en cuantas luces pueden proyectar sombras a la vez
        ////scnMgr->setShadowTextureCount(1);
        //scnMgr->setShadowTextureFadeStart(0.3f);
    }
}

void GraphicsManager::loadResources() {
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
#ifdef _DEBUG
            std::cout << "Resized\n";
#endif
            ogreWindow->resize(windowManager->getWindowW(), windowManager->getWindowH());
            return false;
        }
    }

    return false;
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
    auto plane = new Plane(scnMgr, node, mshMgr, rkNormal, fConstant, up_, name, width, height, xSegments, ySegments);
    /*plane->castShadows(shadows);
    plane->setMaterial("ShadowReceiver");*/
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

Skybox* GraphicsManager::createSkybox(RenderNode* const node, std::string const& texture, const float distC,
                                      const bool orderC) {
    return new Skybox(scnMgr, node, texture, distC, orderC);
}

Skyplane* GraphicsManager::createSkyplane(RenderNode* const node, std::string const& materialName, const bool enable,
                                          const Vector3 rkNormal, const float fConstant, const float scale,
                                          const float tiling, const bool drawFirst, const float bow,
                                          const int xsegments, const int ysegments) {
    return new Skyplane(scnMgr, node, mshMgr, materialName, enable, rkNormal, fConstant, scale, tiling, drawFirst, bow,
                        xsegments, ysegments);
}

Ogre::ManualObject* GraphicsManager::createManualObject(RenderNode* const node) {
    Ogre::ManualObject* manualObject = scnMgr->createManualObject();
    node->attachObject(manualObject);
    return manualObject;
}

void GraphicsManager::destroyManualObject(Ogre::ManualObject* const object) { scnMgr->destroyManualObject(object); }
}
