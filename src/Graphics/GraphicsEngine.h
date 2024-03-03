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

#include <string>
#include <unordered_map>
#include <unordered_set>

class SGTechniqueResolverListener;
class SDL_Window;

namespace Ogre {
class FileSystemLayer;
class Root;
class SceneManager;
class GL3PlusRenderSystem;
class RenderSystem;
class RenderWindow;
class ManualObject;
class Viewport;

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


class GraphicsEngine : public Tapioca::Singleton<GraphicsEngine>, Tapioca::Module {
private:
    friend Singleton<GraphicsEngine>;

    // Ogre
    Ogre::FileSystemLayer* fsLayer;                      // sistema de busqueda de archivos de configuracion
    Ogre::RTShader::ShaderGenerator* mShaderGenerator;   // generador de shaders
    std::string cfgPath;                                 // ruta donde se guardar los archivos de config (bin)
    Ogre::Root* mRoot;                                   // root de Ogre
    Ogre::SceneManager* scnMgr;                          // gestor de escenas
    Ogre::RenderSystem* renderSys;                       // sistema de render usado
    SGTechniqueResolverListener*
        mMaterialMgrListener;         // listener para crear shaders para los materiales que vienen sin ellos
    std::string mwindowName;          // nombre de la ventana
    Ogre::RenderWindow* ogreWindow;   // ventana de ogre (solo para render)

    // Ventana
    uint32_t windowWidth, windowHeight;
    SDL_Window* sdlWindow;
    void* gl_context;

    std::unordered_set<Node*> selfManagedNodes;
    // TODO: ADMINISTRAR OBJETOS, SE PUEDE BORRAR
    /*std::unordered_map<RenderObject*, Node*> objects;
    Ogre::Viewport* viewport;*/

    // carga plugIns especificados desde codigo
    // TODO: SE CARGA DE ARCHIVO, SE PUEDE BORRAR
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

    GraphicsEngine(std::string windowName = "TapiocaEngine", uint32_t w = 680, uint32_t h = 480);

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
    Tapioca::Node* createNode(Tapioca::Vector3 pos = Tapioca::Vector3(0.0f, 0.0f, 0.0f),
        Tapioca::Vector3 scale = Tapioca::Vector3(1.0f, 1.0f, 1.0f));

    // solo para manual object
    Tapioca::Node* createSelfManagedNode(Tapioca::Vector3 pos = Tapioca::Vector3(0.0f, 0.0f, 0.0f),
        Tapioca::Vector3 scale = Tapioca::Vector3(1.0f, 1.0f, 1.0f));

    Tapioca::Node* createChildNode(Tapioca::Node* parent,
        Tapioca::Vector3 relativePos = Tapioca::Vector3(0.0f, 0.0f, 0.0f),
        Tapioca::Vector3 scale = Tapioca::Vector3(1.0f, 1.0f, 1.0f));

    // eliminar un nodo por completo
    // esto quiere decir: delete del nodo y sus hijos, quitar objetos y nodos del propio nodo y de sus hijos del grafo de la escena
    //void removeNode(Node* node);

    /*
    * @brief devuelve a una camara que se podra manipular
    */
    Tapioca::Camera* createCamera(Tapioca::Node* node, std::string name);

    Tapioca::Viewport* createViewport(Tapioca::Camera* camera, int zOrder);

    // TODO: SOLO PARA PRUEBAS, SE PUEDE BORRAR
    void createMainCamera();

    Tapioca::LightDirectional* createLightDirectional(Tapioca::Node* node, Tapioca::Vector3 direction,
        Tapioca::Vector4 color = Tapioca::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    Tapioca::Mesh* createMesh(Tapioca::Node* node, std::string meshName);

    Ogre::ManualObject* createManualObject(Tapioca::Node* node);

    void destroyManualObject(Ogre::ManualObject* object);
    
    inline SDL_Window* getSDLWindow() const { return sdlWindow; };

    void* getGLContext() const;

    //void removeObject(Tapioca::RenderObject* object);
};
}