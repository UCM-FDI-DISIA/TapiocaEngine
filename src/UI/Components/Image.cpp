#include "imgui.h"
#include "Image.h"
#include "UIManager.h"
#include "OgreGLTextureCommon.h"
#include "OgreTextureManager.h"
#include "OgreGLTexture.h"

namespace Tapioca {
Image::Image() { }

Image::~Image() { }

bool Image::initComponent(const CompMap& variables) { 
    if (!setValueFromMap(name, "name", variables)) {
        logError("ImageUI: No se pudo inicializar el nombre.");
        return false;
    }
    if (!setValueFromMap(sourcefile, "sourcefile", variables)) {
        logError("ImageUI: No se ha especificado el archivo de origen para la imagen." );
       
    }
    //puede qeu la imagen no este o que hayamso introducido la ruta mal
    try {
        myTexture =
            (Ogre::GLTexture*)Ogre::TextureManager::getSingleton().load("textures/"+ sourcefile, "General").get();
        GLuint* image_textureid = new GLuint();
        myTexture->getCustomAttribute("GLID", image_textureid);
        textureID = *image_textureid;
        textHeitgh = myTexture->getHeight();
        textWidth = myTexture->getWidth();
        delete image_textureid;
    } catch (Ogre::Exception oe) {
        logError(("Image: Error al cargar \"" + sourcefile + "\" (Imagen) para interfaz: " + oe.getDescription()).c_str());
    }
    return true;
}
void Image::start() { 
setTransform(object->getComponent<Transform>());
}
void Image::render() const { 

      float scaleFactorX = uiManager->getScaleFactorX();
    float scaleFactorY = uiManager->getScaleFactorY();

    ImVec2 imageSize(getSize().x * scaleFactorX, getSize().y * scaleFactorY);
    ImVec2 imagePos(getPosition().x * scaleFactorX - imageSize.x / 2.0f,
                     getPosition().y * scaleFactorY - imageSize.y / 2.0f);

    //ImGui::SetNextWindowPos(ImVec2(getPosition().x, getPosition().y));
    //ImGui::SetNextWindowSize(ImVec2(getSize().x, getSize().y));

  

    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del boton
    ImGui::SetNextWindowPos(imagePos);
    ImGui::SetNextWindowSize(imageSize);


    ImGui::Begin(name.c_str(), nullptr, windowFlags);
   
    ImVec2 imagesize = ImVec2(getSize().x, getSize().y);
    ImVec2 offset = ImGui::GetContentRegionMax();
    ImGui::Image((void*)(intptr_t) textureID,
                 ImVec2(getSize().x - (getSize().x - offset.x),
                        getSize().y - (getSize().y - offset.y)));

    ImGui::End();


}

}
