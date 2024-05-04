#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Utilities/Singleton.h"
#include "WindowModule.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

class SGTechniqueResolverListener;
struct SDL_Window;

namespace Ogre {
class FileSystemLayer;
class Root;
class SceneManager;
class MeshManager;
class GL3PlusRenderSystem;
class RenderSystem;
class RenderWindow;
class ManualObject;
class Viewport;
class OverlaySystem;

namespace RTShader {
    class ShaderGenerator;
}
}

namespace Tapioca {
class RenderNode;
class Camera;
class LightDirectional;
class LightPoint;
class LightRectlight;
class LightSpotlight;
class Mesh;
class Viewport;
class BillboardSet;
class Billboard;
class ParticleSystem;
class Plane;
class AnimationHelper;
class Skybox;
class Skyplane;
class CameraComponent;
class NodeAnimator;
class WindowManager;

/**
* @brief Clase que se encarga de la gestion de la grafica del motor
*/
class TAPIOCA_API GraphicsManager : public Singleton<GraphicsManager>, public WindowModule {
private:
    friend Singleton<GraphicsManager>;

    const int NUM_TEXTURES = 3;       // Numero de texturas por cada directional light
    const int MAXIMUM_Z_ORDER = 20;   // Numero maximo de zOrder que se pueden tener

    Ogre::FileSystemLayer* fsLayer;                      // Sistema de busqueda de archivos de configuracion
    Ogre::RTShader::ShaderGenerator* mShaderGenerator;   // Generador de shaders
    Ogre::Root* mRoot;                                   // Root de Ogre
    Ogre::SceneManager* scnMgr;                          // Gestor de escenas
    Ogre::MeshManager* mshMgr;                           // Gestor de mallas
    Ogre::RenderSystem* renderSys;                       // Sistema de render usado
    SGTechniqueResolverListener*
        mMaterialMgrListener;   // Listener para crear shaders para los materiales que vienen sin ellos

    // warning C4251 'Tapioca::GraphicsManager::cfgPath' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::GraphicsManager'
    // warning C4251 'Tapioca::GraphicsManager::mwindowName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::GraphicsManager'
    // warning C4251 'Tapioca::GraphicsManager::selfManagedNodes' :
    // class 'std::unordered_set<Tapioca::RenderNode *,std::hash<Tapioca::RenderNode *>,
    // std::equal_to<Tapioca::RenderNode *>,std::allocator<Tapioca::RenderNode *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::GraphicsManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string cfgPath;      // Ruta donde se guardar los archivos de config (bin)
    std::string windowName;   // Nombre de la ventana

    std::unordered_set<RenderNode*> selfManagedNodes;   // Nodos gestionados por GraphicsManager
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif


    int nodeAnimatorNumber;
    int planeNumber;
    int billboardNumber;

    // warning C4251 'Tapioca::GraphicsManager::zOrders' :
    // class 'std::vector<std::string,std::allocator<std::string>>' necesita tener una interfaz DLL
    // para que la utilicen los clientes de class 'Tapioca::GraphicsManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_set<int> zOrders;
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    
    // la luz principal es aquella luz direccional que produce sombras
    LightDirectional* mainLight;

    // warning C4251 'Tapioca::GraphicsManager::cameraComps' :
    // class 'std::unordered_map<int,Tapioca::CameraComponent *,std::hash<int>,std::equal_to<int>,std::allocator<std::pair<const int,Tapioca::CameraComponent *>>>'
    //  necesita tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::GraphicsManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_map<int, CameraComponent*> cameraComps;
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif

    Ogre::RenderWindow* ogreWindow;   // Ventana de ogre (solo para render)
    WindowManager* windowManager;   // Puntero al WindowManager
    SDL_Window* sdlWindow;          // Ventana de SDL
    void* glContext;                // Contexto de OpenGL

    
    /**
    * @brief Carga plugIns especificados desde codigo
    */
    void loadPlugIns();

    /**
    * @brief Configura las sombras 
    */
    void setUpShadows();

    /**
    * @brief cargar las rutas donde se ubican los assets para que ogre pueda encontrarlos y usarlos
    */
    void loadResources();

    /**
    * @brief crea el constructor de shaders y anade el listener al gestor de materiales para que a aquellos assets que vengan sin shaders 
    * se les asignen shaders pass through generados automaticamente. Debe invocarse tras crear el RenderSys
    */
    void loadShaders();

    /**
    * @brief Constructor de la clase GraphicsManager
    * @param windowName Nombre de la ventana
    * @param w Anchura de la ventana
    * @param h Altura de la ventana
    */
    GraphicsManager(std::string const& windowName = "TapiocaEngine", const uint32_t w = 680, const uint32_t h = 480);

public:
    GraphicsManager(GraphicsManager&) = delete;
    GraphicsManager(GraphicsManager&&) = delete;
    GraphicsManager& operator=(GraphicsManager&) = delete;
    GraphicsManager& operator=(GraphicsManager&&) = delete;

    /**
    * @brief Destructor de la clase GraphicsManager.
    */
    virtual ~GraphicsManager();

    /**
    * @brief Inicializa el puntero al WindowManager
    */
    bool init() override;
    /**
    * @brief Crea el root de Ogre y prepara los recursos para empezar a renderizar
    */
    bool initConfig() override;
    /**
    * @brief Renderiza 1 frame
    */
    void render() override;
    /**
    * @brief Maneja los eventos de SDL
    * @param event Evento de SDL
    * @return true si se ha manejado el evento, false si no
    */
    bool handleEvents(const SDL_Event& event) override;

    /**
    * @brief Libera la memoria que usa GraphicsManager
    */
    void shutDown();

    /**
    * @brief Devuelve la ventana de ogre
    * @return Puntero a la ventana de ogre
    */
    inline Ogre::RenderWindow* getOgreRenderTarget() const { return ogreWindow; }

    /**
    * @brief Comprueba si existe un recurso con el nombre especificado
    * @param name Nombre del recurso
    * @return true si existe, false si no
    */
    bool checkResourceExists(std::string name);

    /**
    * @brief Elimina una camara en un zOrder especifico
    * @param zOrder ZOrder de la camara
    * @param deleteCamera Indica si se debe eliminar la camara
    */
    void removeCameraCompByZOrder(int zOrder, bool deleteCamera);

    /**
    * @brief Guarda el componente de camara
    * @param cameraComp Componente de camara a guardar
    */
    void saveCameraComp(CameraComponent* cameraComp);

    /**
    * @brief 
    */
    int askForZOrder(int requiredZOrder);

    /**
    * @brief Elimina un zOrder
    * @param zOrder ZOrder a eliminar
    */
    void removeZOrder(int zOrder);

    /**
    * @brief Crea un nodo
    * @param pos Posicion inicial del nodo. Se crea en el origen (0, 0, 0) si no se especifica
    * @param scale Escala inicial del nodo. Se inicia con escala "normal" (1, 1, 1) si no se especifica
    * @return Puntero a la ventana de SDL
    */
    RenderNode* createNode(const Vector3 pos = Vector3(0.0f, 0.0f, 0.0f),
                           const Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    /**
    * @brief Crea un nodo solo para manual object
    * @param pos Posicion inicial del nodo. Se crea en el origen (0, 0, 0) si no se especifica
    * @param scale Escala inicial del nodo. Se inicia con escala "normal" (1, 1, 1) si no se especifica
    * @return Puntero a la ventana de SDL
    */
    RenderNode* createSelfManagedNode(const Vector3 pos = Vector3(0.0f, 0.0f, 0.0f),
                                      const Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    /**
    * @brief Crea una camara 
    * @param node Nodo para renderizado
    * @param name Nombre de la camara
    * @return Puntero a la camara creada
    */
    Camera* createCamera(RenderNode* const node, std::string const& name);

    /**
    * @brief Crea un viewport a partir de una camara y un indice
    * @param camera Camara para el viewport
    * @param zOrder Orden por el cual se renderiza el viewport
    * @return Puntero al viewport creado
    */
    Viewport* createViewport(Camera* const camera, const int zOrder);

    /**
    * @brief Crea una luz direccional
    * @param node Nodo para renderizado
    * @param direction Direccion a la que apunta
    * @param color Color de la luz
    * @return Puntero a la luz direccional creada
    */
    LightDirectional* createLightDirectional(RenderNode* const node, const Vector3 direction,
                                             const Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    /**
    * @brief Crea una luz posicional
    * @param node Nodo para renderizado
    * @param color Color de la luz
    * @return Puntero a la luz posicional creada
    */
    LightPoint* createLightPoint(RenderNode* const node, const Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    /**
    * @brief Crea una luz rectangular
    * @param node Nodo para renderizado
    * @param direction Direccion a la que apunta
    * @param width Ancho del rectangulo
    * @param height Altura del rectangulo
    * @param color Color de la luz
    * @return Puntero a la luz rectangular creada
    */
    LightRectlight* createLightRectlight(RenderNode* const node, const Vector3 direction, const float width,
                                         const float height, const Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    /**
    * @brief Crea un foco de luz
    * @param node Nodo para renderizado
    * @param direction Direccion a la que apunta
    * @param color Color de la luz
    * @return Puntero al foco de luz creado
    */
    LightSpotlight* createLightSpotlight(RenderNode* const node, const Vector3 direction,
                                         const Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    /**
    * @brief Se establece la luz principal, es decir, la luz direccional que produce sombras. Solo puede haber una
    * @param Nueva luz principal
    */
    void setMainLight(LightDirectional* lightDir);

    /**
    * @brief Se elimina la luz principal
    * @param Luz principal a eliminar
    */
    void removeMainLight(LightDirectional* lightDir);

    /**
    * @brief Crea una malla
    * @param node Nodo para renderizado
    * @param meshName Nombre de la malla
    * @return Puntero a la malla creada
    */
    Mesh* createMesh(RenderNode* const node, std::string const& meshName);

    /**
    * @brief Crea un billboard
    * @param node Nodo para renderizado
    * @param name Nombre del billboard
    * @param position Posicion del billboard
    * @param colour Color del billboard
    * @return Puntero al billboard creado
    */
    Billboard* createBillboard(RenderNode* const node, std::string const& name, const Vector3 position,
                               const Vector4 colour);

    /**
    * @brief Crea un billboardSet
    * @param node Nodo para renderizado
    * @param name Nombre del billboardSet
    * @param poolSize Numero maximo de billboards que puede tener
    * @return Puntero al billboard creado
    */
    BillboardSet* createBillboardSet(RenderNode* const node, std::string const& name, unsigned int poolSize);

    /**
    * @brief Crea un billboardSet sin nombre especificado
    * @param node Nodo para renderizado
    * @param poolSize Numero maximo de billboards que puede tener
    * @return Puntero al billboard creado
    */
    BillboardSet* createBillboardSetWithName(RenderNode* const node, const unsigned int poolSize);

    /**
    * @brief Crea un particleSystem
    * @param node Nodo para renderizado
    * @param name Nombre del particleSystem
    * @param templateName TemplateName del particleSystem
    * @param emitting Indica si esta emitiendo particulas
    * @return Puntero al particleSystem creado
    */
    ParticleSystem* createParticleSystem(RenderNode* const node, std::string const& name,
                                         std::string const& templateName, const bool emitting);

    /**
    * @brief Crea un plane
    * Construye el plano a partir de la normal rkNormal y la mueve una distancia fConstant en la normal
    * @param node Nodo para renderizado
    * @param mshMgr Puntero al manager de mallas de ogre
    * @param rkNormal Normal del plano
    * @param fConstant Distancia que se mueve el plano a traves de la normal
    * @param up Vector que indica la direccion de arriba
    * @param name Nombre del plano
    * @param width Anchura del plano
    * @param height Altura del plano
    * @param xSegments Numero de segmentos del plano en la direccion x
    * @param ySegments Numero de segmentos del plano en la direccion y
    * @param material Nombre del material
    */
    Plane* createPlane(RenderNode* const node, const Vector3 rkNormal, const float fConstant, const Vector3& up,
                       std::string const& name, const float width, const float height, const int xSegments,
                       const int ySegments, std::string const& material = "");
    /**
    * @brief Crea un plane sin nombre especificado
    * Construye el plano a partir de la normal rkNormal y la mueve una distancia fConstant en la normal
    * @param node Nodo para renderizado
    * @param mshMgr Puntero al manager de mallas de ogre
    * @param rkNormal Normal del plano
    * @param fConstant Distancia que se mueve el plano a traves de la normal
    * @param up Vector que indica la direccion de arriba
    * @param width Anchura del plano
    * @param height Altura del plano
    * @param xSegments Numero de segmentos del plano en la direccion x
    * @param ySegments Numero de segmentos del plano en la direccion y
    * @param material Nombre del material
    */
    Plane* createPlaneWithName(RenderNode* const node, const Vector3 rkNormal, const float fConstant,
                               const Vector3& up, const float width, const float height, const int xSegments,
                               const int ySegments, std::string const& material = "");

    /**
    * @brief Crea un plane
    * Construye un plano usando 4 constantes
    * @param node Nodo para renderizado
    * @param mshMgr Puntero al manager de mallas de ogre
    * @param a Componente x del vector normal
    * @param b Componente y del vector normal
    * @param c Componente z del vector normal
    * @param d Distancia del plano al origen en la direccion de la normal
    * @param fConstant Distancia que se mueve el plano a traves de la normal
    * @param name Nombre del plano
    * @param width Anchura del plano
    * @param height Altura del plano
    * @param xSegments Numero de segmentos del plano en la direccion x
    * @param ySegments Numero de segmentos del plano en la direccion y
    * @param material Nombre del material
    */
    Plane* createPlane(RenderNode* const node, const float a, const float b, const float c, const float d,
                       const Vector3& up, std::string const& name, const float width, const float height,
                       const int xSegments, const int ySegments, std::string const& material = "");

    /**
    * @brief Crea una animacion
    * @param object Malla a animar
    * @param autoPlay Indica si se reproduce automaticamente
    * @param loop Indica si se reproduce en bucle
    * @return Puntero a la animacion creada
    */
    AnimationHelper* createAnimationHelper(Mesh* const object, bool autoPlay = true, const bool loop = true);
    /**
    * @brief Crea una animacion
    * @param node Nodo a animar
    * @param duration Duracion de la animacion
    * @param name Nombre de la animacion
    * @return Puntero a la animacion creada
    */
    NodeAnimator* createNodeAnimator(RenderNode* const node, const float duration, const std::string& name);
    /**
    * @brief Crea una animacion sin nombre especificado
    * @param node Nodo a animar
    * @param duration Duracion de la animacion
    */
    NodeAnimator* createNodeAnimatorWithName(RenderNode* const node, const float duration);

    /**
    * @brief Crea un skybox
    * @param node Nodo para renderizado
    * @param material Material del skybox
    * @param name Nombre del skybox
    * @param distC Distancia del plano a la camara
    * @param orderC True si se quiere que se dibuje antes que todas las geometrias de la escena
    */
    Skybox* createSkybox(RenderNode* const node, std::string const& material, std::string const& name,
                         const float distC = 5000, const bool orderC = true);

    /**
    * @brief Crea un skyplane
    * @param node Nodo para renderizado
    * @param materialName Nombre del material
    * @param name Nombre del skyplane
    * @param enable Indica si se activa el skyplane
    * @param rkNormal Normal del plano
    * @param fConstant Distancia que se mueve el plano a traves de la normal
    * @param scale Escala del skyplane
    * @param tiling Tiling del skyplane
    * @param drawFirst Indica si se dibuja primero
    * @param bow Curvatura del skyplane
    * @param xsegments Numero de segmentos en x
    * @param ysegments Numero de segmentos en y
    */
    Skyplane* createSkyplane(RenderNode* const node, std::string const& materialName, std::string const& name,
                             const bool enable, const Vector3 rkNormal, const float fConstant, const float scale,
                             const float tiling, const bool drawFirst, const float bow, const int xsegments,
                             const int ysegments);

    /**
    * @brief Crea un manual object
    * @param node Nodo para renderizado
    * @return Puntero al manual object creado
    */
    Ogre::ManualObject* createManualObject(RenderNode* const node);
    /**
    * @brief Elimina un manual object
    * @param object Manual object a eliminar
    */
    void destroyManualObject(Ogre::ManualObject* const object);
};
}
