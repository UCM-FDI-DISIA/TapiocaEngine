#pragma once
#include <string>
#include "RenderObject.h"
#include "Utilities/Vector3.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {

/*
* @brief Wrapper de la clase Skyplane de Ogre.
*/
class TAPIOCA_API Skyplane {
protected:
    Ogre::SceneManager* scnM;
    std::string material;
    Vector3 rkNormal;
    float fConstant;
    float scale;
    float tiling;
    bool drawFirst;
    float bow;
    int xSegments;
    int ySegments;

    /*
    * @brief Constructora de la clase Skyplane.
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param materialName Nombre del material
    * @param enable True si esta activo, false en caso contrario
    * @param rkNormal Vector normal del plano
    * @param fConstant Distancia que se desplaza el plano en la direccion de la normal
    * @param scale Escala del skyplane
    */
    Skyplane(Ogre::SceneManager* const scnMgr, std::string const& materialName, const bool enable = true,
             const Vector3 rkNormal = {0.0f, 0.0f, 1.0f}, const float fConstant = -20.0f, const float scale = 0.3f,
             const float tiling = 1.0f, const bool drawFirst = true, const float bow = 1.0f, const int xsegments = 100,
             const int ysegments = 100);

public:
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
