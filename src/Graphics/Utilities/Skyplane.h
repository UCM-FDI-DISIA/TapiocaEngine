#pragma once
#include <string>
#include "RenderObject.h"
#include "OgreMath.h"
#include "OgrePlane.h"
#include "Utilities/Vector3.h"

using namespace std;

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
class RenderNode;
/*
* @brief Wrapper de la clase Skyplane de Ogre.
*/
class TAPIOCA_API Skyplane : public RenderObject {
private:
    Ogre::Entity* mSkyPlane;    // Entidad con la malla del SkyPlane
    Ogre::Plane mSkyPlaneAux;   // Plano de Ogre
    Ogre::SceneManager* scnM;
    // warning C4251 'Tapioca::Skyplane::material' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Skyplane'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string material;
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    Vector3 rkNormal;
    float fConstant;
    float scale;
    float tiling;
    bool drawFirst;
    float bow;
    int xSegments;
    int ySegments;

public:
    /*
    * @brief Constructor de la clase Skyplane.
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param materialName Nombre del material
    * @param enable True si esta activo, false en caso contrario
    * @param rkNormal Vector normal del plano
    * @param fConstant Distancia que se desplaza el plano en la direccion de la normal
    * @param scale Escala del skyplane
    */
    Skyplane(Ogre::SceneManager* const scnMgr, RenderNode* const node, Ogre::MeshManager* const mshMgr,
             std::string const& materialName, std::string const& skyplaneName, const bool enable = true,
             const Vector3 rkNormal = {0.0f, 0.0f, 1.0f},
             const float fConstant = -20.0f, const float scale = 0.3f, const float tiling = 1.0f,
             const bool drawFirst = true, const float bow = 1.0f, const int xsegments = 100, const int ysegments = 100);
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
