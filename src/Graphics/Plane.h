#pragma once
//#include "OgreMath.h"
#include "RenderObject.h"
#include "OgrePlane.h"
//#include "OgreMovablePlane.h"

namespace Ogre {
class SceneManager;
class Plane;
class Entity;
class MeshManager;
}

namespace Tapioca {
class RenderNode;
class Vector3;
class GraphicsManager;

/*
* @brief Wrapper de la clase Plane de Ogre.
*/
class TAPIOCA_API Plane : public RenderObject {
private:
    friend GraphicsManager;

    Ogre::Entity* mPlane;    // Entidad con la malla del plano
    Ogre::Plane mPlaneAux;   // Plano de Ogre
    // warning C4251 'Tapioca::Plane::name' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Plane'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string name;   // Nombre del plano
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    //Construye un plano con todo inicializado a 0
    //Plane(Ogre::SceneManager* scnMgr, Node* node, Ogre::MeshManager* mshMgr, std::string name, float width, float height,
    //      int xSegments, int ySegments, float x, float y, float z, std::string material);

    /*
    * @brief Constructora de la clase Plane.
    * Construye el plano a partir de la normal rkNormal y la mueve una distancia fConstant en la normal
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param mshMgr Puntero al manager de mallas de ogre
    * @param rkNormal Normal del plano
    * @param fConstant Distancia que se mueve el plano a traves de la normal
    * @param name Nombre del plano
    * @param width Anchura del plano
    * @param height Altura del plano
    * @param xSegments Numero de segmentos del plano en la direccion x
    * @param ySegments Numero de segmentos del plano en la direccion y
    * @param normals Las normales se crean perpendiculares al plano si se pone en True
    * @param numTexCoordSets Numero de texCoordSets creados
    */
    Plane(Ogre::SceneManager* const scnMgr, RenderNode* const node, Ogre::MeshManager* const mshMgr,
          const Vector3& rkNormal, const float fConstant, const Vector3& up_, std::string const& name,
          const float width,
          const float height, const int xSegments, const int ySegments);
    /*
    * @brief Constructora de la clase Plane.
    * Construye un plano usando 4 constantes
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param mshMgr Puntero al manager de mallas de ogre
    * @param a Componente x del vector normal
    * @param b Componente y del vector normal
    * @param c Componente z del vector normal
    * @param _d Distancia del plano al origen en la direccion de la normal
    * @param fConstant Distancia que se mueve el plano a traves de la normal
    * @param name Nombre del plano
    * @param width Anchura del plano
    * @param height Altura del plano
    * @param xSegments Numero de segmentos del plano en la direccion x
    * @param ySegments Numero de segmentos del plano en la direccion y
    * @param normals Las normales se crean perpendiculares al plano si se pone en True
    * @param numTexCoordSets Numero de texCoordSets creados
    */
    Plane(Ogre::SceneManager* const scnMgr, RenderNode* const node, Ogre::MeshManager* const mshMgr, const float a,
          const float b, const float c, const float _d, const Vector3& up_, std::string const& name, const float width,
          const float height,
          const int xSegments, const int ySegments);

public:
    /*
    * @brief Destructora de la clase Plane.
    */
    virtual ~Plane() {};
    /*
    * @brief Devuelve la normal que forma el plano
    */
    Vector3 getNormal() const;
    /*
    * @brief Devuelve la distancia desde el plano al origen
    */
    float getD() const;
    /*
    * @brief Devuelve distancia desde un punto al plano
    */
    float getDistance(const Vector3& rkPoint) const;
    /*
    * @brief Redefine el plano a partir de 3 puntos
    */
    void redefine(const Vector3& p0, const Vector3& p1, const Vector3& p2);
    /*
    * @brief Redefine el plano a partir de una normal y un punto
    */
    void redefine(const Vector3& rkNormal, const Vector3& rkPoint);
    /*
    * @brief Proyecta un vector sobre el plano
    */
    Vector3 projectVector(const Vector3& v) const;
    /*
    * @brief Normaliza la normal y la distancia d del plano. Devuelve la longitud de la normal antes de ser normalizada
    */
    float normalise(void);

    /*
    * @brief Cambia el material del plano
    * @params material Nombre del material al que se quiere cambiar
    */
    void setMaterial(std::string const& material);

    void castShadows(bool enable);

    //Comparadores del plano
    bool operator==(const Plane& rhs) const;
    bool operator!=(const Plane& rhs) const;
};

}
