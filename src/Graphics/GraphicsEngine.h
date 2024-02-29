#pragma once
#include <string>
#include <unordered_set>
// Includes de Core
// Herencia
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
// Para parametros con valores por defecto
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

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
class RenderObject;
class Mesh;
class Vector3;
}

namespace Tapioca {
class GraphicsEngine : public Tapioca::Singleton<GraphicsEngine>, Module {
public:
    friend Singleton<GraphicsEngine>;

private:
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
    Ogre::RenderWindow* ogreWindow;   // ventan de ogre (solo para render)
    // Ventana
    uint32_t windowWidth, windowHeight;
    SDL_Window* sdlWindow;

    // Administriar objetos
    std::unordered_set<Node*> nodes;
    std::unordered_map<RenderObject*, Node*> objects;
    std::pair<Camera*, Node*> mainCamera;
    Ogre::Viewport* viewport;

    // carga plugIns especificados desde codigo
    // BORRAR
    void loadPlugIns();

    /*
    *   @brief cargar las rutas donde se ubican los assets para que ogre pueda encontrarlos y usarlos
    */
    void loadResources();

    /*
    *   @brief crea el constructor de shaders y añade el listerner al gestor de materiales para que a aquellos assets que vengan sin shaders 
    *   se les asignen shaders pass through generados automaticamente.Debe invocarse trase crear el RenderSys
    */
    void loadShaders();

    GraphicsEngine(std::string windowName = "motor", uint32_t w = 680, uint32_t h = 480);

public:
    virtual ~GraphicsEngine();

    /*
    *   @brief creat el root de Ogre y prepara los recursos para empezar a renderizar
    */
    bool init() override;

    /*
    *   @brief renderiza 1 frame
    */
    void render() override;

    /*
    *   @brief  libera la memoria que usa GraphicsEngine
    */
    void shutDown();

    // CREAR OBJETOS
    Node* createNode(Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    Node* createChildNode(
        Node* parent, Vector3 relativePos = Vector3(0.0f, 0.0f, 0.0f), Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    // eliminar un nodo por completo
    // esto quiere decir: delete del nodo y sus hijos, quitar objetos y nodos del propio nodo y de sus hijos del grafo de la escena
    void removeNode(Node* node);

    /*
    * @brief devuelve a una camara que se podra manipular
    */
    Camera* createCamera(Vector3 pos);
    /*
    * @brief crea una camara que no se modificara (la del juego de billar?)
    */
    // METODO

    void createMainCamera();
    void setBackgroundColor(Vector3 color);
    inline std::pair<Camera*, Node*> getMainCamera() { return mainCamera; }

    LightDirectional* createLightDirectional(Vector3 direction, Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    Mesh* createMesh(Node* node, std::string meshName);

    Ogre::ManualObject* createManualObject(Node* node);

    void destroyManualObject(Ogre::ManualObject* object);
};
}