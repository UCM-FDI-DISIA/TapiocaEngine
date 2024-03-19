#pragma once
#include <string>
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class Skybox;
}

namespace Tapioca {

/*
* @brief Wrapper de la clase Skybox de Ogre.
*/
class TAPIOCA_API Skybox : public RenderObject {
protected:
    // warning C4251 'Tapioca::Skybox::texture' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Skybox'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string texture;            // Nombre de la textura
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    float distC;                    // 
    bool orderC;
    Ogre::SceneManager* scnM; 
public:
    /*
    * @brief Construye un skybox con la textura texture a una distancia distC de la cámara
    * e indicando si se pinta antes o después del resto de elementos en función de orderC
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param texture Nombre de la textura
    * @param distC Distancia del plano a la camara
    * @param orderC True si se quiere que se dibuje antes que todas las geometrias de la escena
    */
    Skybox(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& texture,
           const float distC = 5000, const bool orderC = true);

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
