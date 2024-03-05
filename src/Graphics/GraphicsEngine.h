#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>

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
class Node;
class Camera;
class LightDirectional;
class Mesh;
class Viewport;
class BillboardSet;
class Billboard;
class ParticleSystem;
class Plane;

class GraphicsEngine : public Singleton<GraphicsEngine>, public Module {
private:
    friend Singleton<GraphicsEngine>;

    // Ogre
    Ogre::FileSystemLayer* fsLayer;                      // sistema de busqueda de archivos de configuracion
    Ogre::RTShader::ShaderGenerator* mShaderGenerator;   // generador de shaders
    std::string cfgPath;                                 // ruta donde se guardar los archivos de config (bin)
    Ogre::Root* mRoot;                                   // root de Ogre
    Ogre::SceneManager* scnMgr;                          // gestor de escenas
    Ogre::MeshManager* mshMgr;                           // gestor de mallas
    Ogre::RenderSystem* renderSys;                       // sistema de render usado
    SGTechniqueResolverListener* mMaterialMgrListener;   // listener para crear shaders para los materiales que vienen sin ellos
    std::string mwindowName;          // nombre de la ventana
    Ogre::RenderWindow* ogreWindow;   // ventana de ogre (solo para render)

    // Ventana
    uint32_t windowWidth, windowHeight;
    SDL_Window* sdlWindow;

    // UI
    Ogre::OverlaySystem* overSys;
    std::unordered_set<Node*> selfManagedNodes;
    void* glContext;

    // TODO: se puede borrar
    /*std::unordered_map<RenderObject*, Node*> objects;
    Ogre::Viewport* viewport;*/

    // carga plugIns especificados desde codigo
    // TODO: se carga de archivo, se puede borrar
    void loadPlugIns();

    /*
    * @brief cargar las rutas donde se ubican los assets para que ogre pueda encontrarlos y usarlos
    */
    void loadResources();

    /*
    * @brief crea el constructor de shaders y a�ade el listerner al gestor de materiales para que a aquellos assets que vengan sin shaders 
    * se les asignen shaders pass through generados automaticamente.Debe invocarse trase crear el RenderSys
    */
    void loadShaders();

    GraphicsEngine(std::string const& windowName = "TapiocaEngine", uint32_t w = 680, uint32_t h = 480);

public:
    GraphicsEngine(GraphicsEngine&) = delete;
    GraphicsEngine(GraphicsEngine&&) = delete;
    GraphicsEngine& operator=(GraphicsEngine&) = delete;
    GraphicsEngine& operator=(GraphicsEngine&&) = delete;

    virtual ~GraphicsEngine();

    /*
    * @brief creat el root de Ogre y prepara los recursos para empezar a renderizar
    */
    bool init() override;

    /*
    * @brief renderiza 1 frame
    */
    void render() override;

    /*
    * @brief  libera la memoria que usa GraphicsEngine
    */
    void shutDown();


    // CREAR OBJETOS
    Node* createNode(Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    // solo para manual object
    Node* createSelfManagedNode(Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    Node* createChildNode(Node* parent, Vector3 relativePos = Vector3(0.0f, 0.0f, 0.0f), Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    // eliminar un nodo por completo
    // esto quiere decir: delete del nodo y sus hijos, quitar objetos y nodos del propio nodo y de sus hijos del grafo de la escena
    //void removeNode(Node* node);

    
    /*
    * @brief devuelve a una camara que se podra manipular
    */
    Camera* createCamera(Node* node, std::string const& name);

    Viewport* createViewport(Camera* camera, int zOrder);

    LightDirectional* createLightDirectional(Node* node, Vector3 direction, Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    Mesh* createMesh(Node* node, std::string const& meshName);

    Billboard* createBillboard(Node* node, std::string const& name, Vector3 position, Vector4 colour);

    BillboardSet* createBillboardSet(Node* node, std::string const& name, unsigned int poolSize);

    ParticleSystem* createParticleSystem(Ogre::SceneManager* scnMgr, Node* node, std::string const& name,
                                         std::string const& templateName,
                                         bool emitting);

    //Plane* createPlane(Node* node, std::string name, float width, float height, int xSegments, int ySegments, float x,
    //                   float y, float z, std::string material = "");

    Plane* createPlane(Node* node, const Vector3& rkNormal,
                       float fConstant, std::string name, float width, float height, int xSegments, int ySegments, float x, float y, float z,
                       std::string material = "");

    Plane* createPlane(Node* node, float a, float b, float c,
                       float _d, std::string name, float width, float height, int xSegments, int ySegments, float x, float y, float z, std::string material = "");

    Ogre::ManualObject* createManualObject(Node* node);

    void destroyManualObject(Ogre::ManualObject* object);

    SDL_Window* getSDLWindow();
    Ogre::RenderWindow* getOgreWindow();
    void* getGLContext();

    //void removeObject(RenderObject* object);
};
}