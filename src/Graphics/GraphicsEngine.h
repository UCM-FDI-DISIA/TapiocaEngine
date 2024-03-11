#pragma once
#include <string>
#include <unordered_set>

// Includes de Core
// Herencia
#include "Utilities/Singleton.h"
#include "Structure/Module.h"

// Includes de Core para parametros con valores por defecto
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

class SGTechniqueResolverListener;
class SDL_Window;

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
class Mesh;
class Viewport;
class BillboardSet;
class Billboard;
class ParticleSystem;
class Plane;

class TAPIOCA_API GraphicsEngine : public Singleton<GraphicsEngine>, public Module {
private:
    friend Singleton<GraphicsEngine>;

    // Ogre
    Ogre::FileSystemLayer* fsLayer;                      // Sistema de busqueda de archivos de configuracion
    Ogre::RTShader::ShaderGenerator* mShaderGenerator;   // Generador de shaders
    std::string cfgPath;                                 // Ruta donde se guardar los archivos de config (bin)
    Ogre::Root* mRoot;                                   // Root de Ogre
    Ogre::SceneManager* scnMgr;                          // Gestor de escenas
    Ogre::MeshManager* mshMgr;                           // Gestor de mallas
    Ogre::RenderSystem* renderSys;                       // Sistema de render usado
    SGTechniqueResolverListener* mMaterialMgrListener;   // Listener para crear shaders para los materiales que vienen sin ellos
    std::string mwindowName;                             // Nombre de la ventana
    Ogre::RenderWindow* ogreWindow;                      // Ventana de ogre (solo para render)

    // Ventana
    uint32_t windowWidth, windowHeight;                  // Anchura y altura de la ventana, respectivamente
    SDL_Window* sdlWindow;                               // Ventana de SDL

    // UI
    Ogre::OverlaySystem* overSys;                        // Systema de overlays de Ogre
    std::unordered_set<RenderNode*> selfManagedNodes;    // Nodos gestionados por graphicsEngine
    void* glContext;

    // 
    // TODO: se carga de archivo, se puede borrar
    /*
    * @brief Carga plugIns especificados desde codigo
    */
    void loadPlugIns();

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
    * @brief Constructora de la clase GraphicsEngine.
    * @param windowName Nombre de la ventana
    * @param w Anchura de la ventana
    * @param h Altura de la ventana
    */
    GraphicsEngine(std::string const& windowName = "TapiocaEngine", const uint32_t w = 680, const uint32_t h = 480);

public:
    GraphicsEngine(GraphicsEngine&) = delete;
    GraphicsEngine(GraphicsEngine&&) = delete;
    GraphicsEngine& operator=(GraphicsEngine&) = delete;
    GraphicsEngine& operator=(GraphicsEngine&&) = delete;

    /*
    * @brief Destructora de la clase GraphicsEngine.
    */
    virtual ~GraphicsEngine();

    /*
    * @brief Crea el root de Ogre y prepara los recursos para empezar a renderizar
    */
    bool init() override;

    /*
    * @brief Renderiza 1 frame
    */
    void render() override;

    /*
    * @brief Libera la memoria que usa GraphicsEngine
    */
    void shutDown();

    /*
    * @brief Devuelve un puntero a la ventana de SDL
    * @return Puntero a la ventana de SDL
    */
    SDL_Window* getSDLWindow();
    /*
    * @brief Devuelve un puntero a la ventana de Ogre
    * @return Puntero a la ventana de Ogre
    */
    Ogre::RenderWindow* getOgreWindow();
    void* getGLContext();

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
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param name Nombre del particleSystem
    * @param templateName TemplateName del particleSystem
    * @param emitting Indica si esta emitiendo particulas
    * @return Puntero al particleSystem creado
    */
    ParticleSystem* createParticleSystem(Ogre::SceneManager* const scnMgr, RenderNode* const node,
                                         std::string const& name, std::string const& templateName, const bool emitting);

    /*
    * @brief Crea un plane
    * Construye el plano a partir de la normal rkNormal y la mueve una distancia fConstant en la normal
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param mshMgr Puntero al manager de mallas de ogre
    * @param rkNormal Normal del plano
    * @param fConstant Distancia que se mueve el plano a traves de la normal
    * @param name Nombre del plano
    * @param width Anchura del plano
    * @param height Altura del plano
    * @param xSegments Numero de segmentos del plano en la direccion x
    * @param ySegments Numero de segmentos del plano en la direccion y
    * @param normals Las normales se crean perpendiculares al plano si se pone en True
    * @param numTexCoordSets Numero de texCoordSets creados
    */
    Plane* createPlane(RenderNode* const node, const Vector3& rkNormal, const float fConstant, std::string const& name,
                       const float width, const float height, const int xSegments, const int ySegments, const float x,
                       const float y, const float z, std::string const& material = "");

    /*
    * @brief Crea un plane
    * Construye un plano usando 4 constantes
    * @param scnMgr Puntero al manager de escenas de ogre
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
    * @param normals Las normales se crean perpendiculares al plano si se pone en True
    * @param numTexCoordSets Numero de texCoordSets creados
    */
    Plane* createPlane(RenderNode* const node, const float a, const float b, const float c, const float _d,
                       std::string const& name, const float width, const float height, const int xSegments,
                       const int ySegments, const float x, const float y, const float z,
                       std::string const& material = "");

    Ogre::ManualObject* createManualObject(RenderNode* const node);

    void destroyManualObject(Ogre::ManualObject* const object);
};
}