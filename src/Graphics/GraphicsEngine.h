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
    Ogre::FileSystemLayer* fsLayer;                      // sistema de busqueda de archivos de configuracion
    Ogre::RTShader::ShaderGenerator* mShaderGenerator;   // generador de shaders
    std::string cfgPath;                                 // ruta donde se guardar los archivos de config (bin)
    Ogre::Root* mRoot;                                   // root de Ogre
    Ogre::SceneManager* scnMgr;                          // gestor de escenas
    Ogre::MeshManager* mshMgr;                           // gestor de mallas
    Ogre::RenderSystem* renderSys;                       // sistema de render usado
    SGTechniqueResolverListener*
        mMaterialMgrListener;         // listener para crear shaders para los materiales que vienen sin ellos
    std::string mwindowName;          // nombre de la ventana
    Ogre::RenderWindow* ogreWindow;   // ventana de ogre (solo para render)

    // Ventana
    uint32_t windowWidth, windowHeight;
    SDL_Window* sdlWindow;

    // UI
    Ogre::OverlaySystem* overSys;
    std::unordered_set<RenderNode*> selfManagedNodes;
    void* glContext;

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

    GraphicsEngine(std::string const& windowName = "TapiocaEngine", const uint32_t w = 680, const uint32_t h = 480);

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

    SDL_Window* getSDLWindow();
    Ogre::RenderWindow* getOgreWindow();
    void* getGLContext();

    // CREAR OBJETOS
    RenderNode* createNode(const Vector3 pos = Vector3(0.0f, 0.0f, 0.0f),
                           const Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    // solo para manual object
    RenderNode* createSelfManagedNode(const Vector3 pos = Vector3(0.0f, 0.0f, 0.0f),
                                      const Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));

    /*
    * @brief devuelve a una camara que se podra manipular
    */
    Camera* createCamera(RenderNode* const node, std::string const& name);

    Viewport* createViewport(Camera* const camera, const int zOrder);

    LightDirectional* createLightDirectional(RenderNode* const node, const Vector3 direction,
                                             const Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    Mesh* createMesh(RenderNode* const node, std::string const& meshName);

    Billboard* createBillboard(RenderNode* const node, std::string const& name, const Vector3 position,
                               const Vector4 colour);

    BillboardSet* createBillboardSet(RenderNode* const node, std::string const& name, unsigned int poolSize);

    ParticleSystem* createParticleSystem(Ogre::SceneManager* const scnMgr, RenderNode* const node,
                                         std::string const& name, std::string const& templateName, const bool emitting);

    Plane* createPlane(RenderNode* const node, const Vector3& rkNormal, const float fConstant, std::string const& name,
                       const float width, const float height, const int xSegments, const int ySegments, const float x,
                       const float y, const float z, std::string const& material = "");

    Plane* createPlane(RenderNode* const node, const float a, const float b, const float c, const float _d,
                       std::string const& name, const float width, const float height, const int xSegments,
                       const int ySegments, const float x, const float y, const float z,
                       std::string const& material = "");

    Ogre::ManualObject* createManualObject(RenderNode* const node);

    void destroyManualObject(Ogre::ManualObject* const object);
};
}