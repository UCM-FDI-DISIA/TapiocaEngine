#pragma once
#include <string>
#include <memory>
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class ManualObject;
}

namespace Tapioca {

/*
* @brief Wrapper de la clase Skybox de Ogre.
*/
class TAPIOCA_API Skybox : public RenderObject {
private:
    std::unique_ptr<Ogre::ManualObject> mSkyBoxObj;
    Ogre::SceneManager* scnM;
    // warning C4251 'Tapioca::Skybox::texture' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Skybox'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string material;      // Nombre del material
    std::string skyboxName;   // Nombre skybox
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    float distC;   // Distancia entre la c�mara y el skybox
    bool orderC;   // True se pinta el plano antes que la escena False despu�s
public:
    /*
    * @brief Construye un skybox con la textura texture a una distancia distC de la c�mara
    * e indicando si se pinta antes o despu�s del resto de elementos en funci�n de orderC
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param texture Nombre de la textura
    * @param distC Distancia del plano a la camara
    * @param orderC True si se quiere que se dibuje antes que todas las geometrias de la escena
    */
    Skybox(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& material,
           std::string const& skyboxName, const float distC = 5000, const bool orderC = true);

    /*
    * @brief Activa o desactiva el Skybox
    * @param enable True si se quiere activar, false en caso contrario
    */
    void setEnable(const bool enable);
    /*
    * @brief Devuelve si esta activo
    * @return True si esta activo, false en caso contrario
    */
    bool isEnabled();
};
}