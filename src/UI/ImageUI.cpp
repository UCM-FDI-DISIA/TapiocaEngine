#include "ImageUI.h"
#include "OgreGLTextureCommon.h"
#include "OgreTextureManager.h"
namespace Tapioca { 

	ImageUI::ImageUI(std::string file, Tapioca::Vector2 widthandheigth, Tapioca::Vector2 xandy, ImGuiWindowFlags flags) {
	

			sourcefile = file;
            size = widthandheigth;
            position = xandy;
		//puede qeu la imagen no este o que hayamso introducido la ruta mal
          try{
                myTexture = (Ogre::GLTexture*) Ogre::TextureManager::getSingleton().load(file, "General").get();
              GLuint* image_textureid = new GLuint();
              myTexture->getCustomAttribute("GLID", image_textureid);
              textureID = *image_textureid;
              textHeitgh = myTexture->getHeight();
              textWidth = myTexture->getWidth();
              delete image_textureid;
		  }  
		  catch (Ogre::Exception oe) {
                std::cout << "ERROR AL CARGAR" << file << " IMAGEN PARA INTERFAZ: \n " << oe.getDescription();
   
		  }    
		  myflags = flags;
	}

   ImageUI::~ImageUI() { 
  // delete textureID;
   delete canClose;
   }	  

}

