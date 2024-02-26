#pragma once
#include <string>
#include <unordered_set>
// Includes de Core
// Herencia
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
// Para parametros con valores por defecto
#include "Utilities/Vector3.h"

class SGTechniqueResolverListener;
class SDL_Window;

namespace Ogre {
class FileSystemLayer;
class Root;
class SceneManager;
class GL3PlusRenderSystem;
class RenderSystem;
class RenderWindow;

namespace RTShader {
    class ShaderGenerator;
}
}

namespace Tapioca {
class Node;
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

    // Administrar los nodos propios
    std::unordered_set<Node*> nodes;

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
    void init() override;

    /*
    *   @brief renderiza 1 frame
    */
    void render() override;

    /*
    *   @brief  libera la memoria que usa GraphicsEngine
    */
    void shutDown();

    // escena basica para hacer pruebas luego borramos este metodo entero
    // BORRAR
    void testScene();

    Node* createNode(Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    Node* createChildNode(
        Node* parent, Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    void removeNode(Node* node);

    // BORRAR cuando se haya creado el wrapper
    inline Ogre::SceneManager* getSceneManager() { return scnMgr; }
};
}