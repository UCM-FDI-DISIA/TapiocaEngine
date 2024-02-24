#pragma once
#include <string>

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
class GraphicsEngine { 

private:
    //Ogre
    Ogre::FileSystemLayer* fsLayer;
    Ogre::RTShader::ShaderGenerator* mShaderGenerator;
    std::string cfgPath;
    Ogre::Root* mRoot;
    Ogre::SceneManager* scnMgr;
   // Ogre::GL3PlusRenderSystem* renderSys;
    Ogre::RenderSystem* renderSys;
    SGTechniqueResolverListener* mMaterialMgrListener;
    std::string mwindowName;
    Ogre::RenderWindow* ogreWindow;
    //Ventana
    uint32_t windowWidth, windowHeight;
    SDL_Window* sdlWindow;

    /*
    /// @brief carga plugIns especificados desde codigo
    */
	void loadPlugIns();

    /*
    * @brief cargar las rutas donde se ubican los assets para que ogre pueda encontrarlos y usarlos
    */
    void loadResources();

    /*
    /// @brief crea el constructor de shaders y añade el listerner al gestor de materiales para que a aquellos assets que vengan sin shaders 
   /// se les asignen shaders pass through generados automaticamente.Debe invocarse trase crear el RenderSys
    */
    void loadShaders();

 

public:
 
    GraphicsEngine();
    /*
    /// @brief creat el root de Ogre y prepara los recursos para empezar a renderizar
    */
    void init(std::string windowName = "motor", uint32_t w = 680, uint32_t h = 480);
    /*
    /// @brief renderiza 1 frame
    */
    void renderFrame();

    /*
    /// @brief  libera la memoria que usa GraphicsEngine
    */
    void shutDown();

    void testScene(); //escena basica para hacer pruebas luego borramos este metodo entero
    void clearTestScene();
};
