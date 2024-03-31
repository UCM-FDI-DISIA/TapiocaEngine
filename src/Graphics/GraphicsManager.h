#pragma once
#include <string>
#include <unordered_set>
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

class WindowManager;

class TAPIOCA_API GraphicsManager : public Singleton<GraphicsManager>, public WindowModule {
private:
    friend Singleton<GraphicsManager>;

    // Ogre
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
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string cfgPath;       // Ruta donde se guardar los archivos de config (bin)
    std::string mwindowName;   // Nombre de la ventana
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    Ogre::RenderWindow* ogreWindow;   // Ventana de ogre (solo para render)

    // Ventana
    WindowManager* windowManager;
    SDL_Window* sdlWindow;   // Ventana de SDL
    void* glContext;         // Contexto de OpenGL

    // warning C4251 'Tapioca::GraphicsManager::selfManagedNodes' :
    // class 'std::unordered_set<Tapioca::RenderNode *,std::hash<Tapioca::RenderNode *>,
    // std::equal_to<Tapioca::RenderNode *>,std::allocator<Tapioca::RenderNode *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::GraphicsManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_set<RenderNode*> selfManagedNodes;   // Nodos gestionados por GraphicsManager
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    // UI
    Ogre::OverlaySystem* overSys;   // Systema de overlays de Ogre

    // TODO: se carga de archivo, se puede borrar
    /*
    * @brief Carga plugIns especificados desde codigo
    */
    void loadPlugIns();

    void loadConfigFiles();

    void setUpShadows();

    /*
    * @brief cargar las rutas donde se ubican los assets para que ogre pueda encontrarlos y usarlos
    */
    void loadResources();

    /*
    * @brief crea el constructor de shaders y aniade el listerner al gestor de materiales para que a aquellos assets que vengan sin shaders 
    * se les asignen shaders pass through generados automaticamente. Debe invocarse tras crear el RenderSys
    */
    void loadShaders();

    /*
    * @brief Constructora de la clase GraphicsManager.
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

    /*
    * @brief Destructora de la clase GraphicsManager.
    */
    virtual ~GraphicsManager();

    /*
    * @brief Crea el root de Ogre y prepara los recursos para empezar a renderizar
    */
    bool init() override;

    /*
    * @brief Renderiza 1 frame
    */
    void render() override;

    /*
    * @brief Maneja los eventos de SDL
    * @param event Evento de SDL
    * @return true si se ha manejado el evento, false si no
    */
    bool handleEvents(const SDL_Event& event) override;

    /*
    * @brief Libera la memoria que usa GraphicsManager
    */
    void shutDown();

    /*
    * @brief Devuelve un puntero al gestor de escenas de Ogre
    */
    Ogre::SceneManager* getSceneManager();

    /*
    * @brief Crea un nodo
    * @param pos Posicion inicial del nodo. Se crea en el origen (0, 0, 0) si no se especifica
    * @param scale Escala inicial del nodo. Se inicia con escala "normal" (1, 1, 1) si no se especifica
    * @return Puntero a la ventana de SDL
    */
    RenderNode* createNode(const Vector3 pos = Vector3(0.0f, 0.0f, 0.0f),
                           const Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    /*
    * @brief Crea un nodo solo para manual object
    * @param pos Posicion inicial del nodo. Se crea en el origen (0, 0, 0) si no se especifica
    * @param scale Escala inicial del nodo. Se inicia con escala "normal" (1, 1, 1) si no se especifica
    * @return Puntero a la ventana de SDL
    */
    RenderNode* createSelfManagedNode(const Vector3 pos = Vector3(0.0f, 0.0f, 0.0f),
                                      const Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    /*
    * @brief Crea una camara 
    * @param node Nodo para renderizado
    * @param name Nombre de la camara
    * @return Puntero a la camara creada
    */
    Camera* createCamera(RenderNode* const node, std::string const& name);

    /*
    * @brief Crea un viewport a partir de una camara y un indice
    * @param camera Camara para el viewport
    * @param zOrder Orden por el cual se renderiza el viewport
    * @return Puntero al viewport creado
    */
    Viewport* createViewport(Camera* const camera, const int zOrder);

    /*
    * @brief Crea una luz direccional
    * @param node Nodo para renderizado
    * @param direction Direccion a la que apunta
    * @param color Color de la luz
    * @return Puntero a la luz direccional creada
    */
    LightDirectional* createLightDirectional(RenderNode* const node, const Vector3 direction,
                                             const Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    /*
    * @brief Crea una luz posicional
    * @param node Nodo para renderizado
    * @param color Color de la luz
    * @return Puntero a la luz posicional creada
    */
    LightPoint* createLightPoint(RenderNode* const node, const Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    /*
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

    /*
    * @brief Crea un foco de luz
    * @param node Nodo para renderizado
    * @param direction Direccion a la que apunta
    * @param color Color de la luz
    * @return Puntero al foco de luz creado
    */
    LightSpotlight* createLightSpotlight(RenderNode* const node, const Vector3 direction,
                                         const Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    /*
    * @brief Crea una malla
    * @param node Nodo para renderizado
    * @param meshName Nombre de la malla
    * @return Puntero a la malla creada
    */
    Mesh* createMesh(RenderNode* const node, std::string const& meshName);

    /*
    * @brief Crea un billboard
    * @param node Nodo para renderizado
    * @param name Nombre del billboard
    * @param position Posicion del billboard
    * @param colour Color del billboard
    * @return Puntero al billboard creado
    */
    Billboard* createBillboard(RenderNode* const node, std::string const& name, const Vector3 position,
                               const Vector4 colour);

    /*
    * @brief Crea un billboardSet
    * @param node Nodo para renderizado
    * @param name Nombre del billboardSet
    * @param poolSize Numero maximo de billboards que puede tener
    * @return Puntero al billboard creado
    */
    BillboardSet* createBillboardSet(RenderNode* const node, std::string const& name, unsigned int poolSize);

    /*
    * @brief Crea un particleSystem
    * @param node Nodo para renderizado
    * @param name Nombre del particleSystem
    * @param templateName TemplateName del particleSystem
    * @param emitting Indica si esta emitiendo particulas
    * @return Puntero al particleSystem creado
    */
    ParticleSystem* createParticleSystem(RenderNode* const node, std::string const& name,
                                         std::string const& templateName, const bool emitting);

    /*
    * @brief Crea un plane
    * Construye el plano a partir de la normal rkNormal y la mueve una distancia fConstant en la normal
    * @param node Nodo para renderizado
    * @param mshMgr Puntero al manager de mallas de ogre
    * @param rkNormal Normal del plano
    * @param fConstant Distancia que se mueve el plano a traves de la normal
    * @param name Nombre del plano
    * @param width Anchura del plano
    * @param height Altura del plano
    * @param xSegments Numero de segmentos del plano en la direccion x
    * @param ySegments Numero de segmentos del plano en la direccion y
    * @param material Nombre del material
    */
    Plane* createPlane(RenderNode* const node, const Vector3 rkNormal, const float fConstant, const Vector3& up_,
                       std::string const& name, const float width, const float height, const int xSegments,
                       const int ySegments, std::string const& material = "");

    /*
    * @brief Crea un plane
    * Construye un plano usando 4 constantes
    * @param node Nodo para renderizado
    * @param mshMgr Puntero al manager de mallas de ogre
    * @param a Componente x del vector normal
    * @param b Componente y del vector normal
    * @param c Componente z del vector normal
    * @param _d Distancia del plano al origen en la direccion de la normal
    * @param fConstant Distancia que se mueve el plano a traves de la normal
    * @param name Nombre del plano
    * @param width Anchura del plano
    * @param height Altura del plano
    * @param xSegments Numero de segmentos del plano en la direccion x
    * @param ySegments Numero de segmentos del plano en la direccion y
    * @param material Nombre del material
    */
    Plane* createPlane(RenderNode* const node, const float a, const float b, const float c, const float _d,
                       const Vector3& up_, std::string const& name, const float width, const float height,
                       const int xSegments, const int ySegments, std::string const& material = "");

    AnimationHelper* createAnimationHelper(Mesh* const object, bool autoPlay = true, const bool loop = true);

    Skybox* createSkybox(RenderNode* const node, std::string const& texture, const float distC = 5000,
                         const bool orderC = true);

    Skyplane* createSkyplane(RenderNode* const node, std::string const& materialName, const bool enable,
                             const Vector3 rkNormal, const float fConstant, const float scale, const float tiling,
                             const bool drawFirst, const float bow, const int xsegments, const int ysegments);

    Ogre::ManualObject* createManualObject(RenderNode* const node);

    void destroyManualObject(Ogre::ManualObject* const object);
};
}