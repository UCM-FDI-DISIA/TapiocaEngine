#pragma once
#include "RenderObject.h"
#include "OgrePlane.h"

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
* @brief Wrapper de la clase Plane de Ogre
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

    /*
    * @brief Constructor de la clase Plane.
    * Construye el plano a partir de la normal rkNormal y la mueve una distancia fConstant en la normal
    * @param scnMngr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param mshMngr Puntero al manager de mallas de ogre
    * @param rkNormal Normal del plano
    * @param fConstant Distancia que se mueve el plano a traves de la normal
    * @param up Vector que indica la direccion perpendicular del plano
    * @param name Nombre del plano
    * @param width Anchura del plano
    * @param height Altura del plano
    * @param xSegments Numero de segmentos del plano en la direccion x
    * @param ySegments Numero de segmentos del plano en la direccion y
    */
    Plane(Ogre::SceneManager* const scnMngr, RenderNode* const node, Ogre::MeshManager* const mshMngr,
          const Vector3& rkNormal, const float fConstant, const Vector3& up, std::string const& name, const float width,
          const float height, const int xSegments, const int ySegments);
    /*
    * @brief Constructor de la clase Plane.
    * Construye un plano usando 4 constantes
    * @param scnMngr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param mshMngr Puntero al manager de mallas de ogre
    * @param a Componente x del vector normal
    * @param b Componente y del vector normal
    * @param c Componente z del vector normal
    * @param d Distancia del plano al origen en la direccion de la normal
    * @param up Vector que indica la direccion perpendicular del plano
    * @param name Nombre del plano
    * @param width Anchura del plano
    * @param height Altura del plano
    * @param xSegments Numero de segmentos del plano en la direccion x
    * @param ySegments Numero de segmentos del plano en la direccion y
    */
    Plane(Ogre::SceneManager* const scnMngr, RenderNode* const node, Ogre::MeshManager* const mshMngr, const float a,
          const float b, const float c, const float d, const Vector3& up, std::string const& name, const float width,
          const float height, const int xSegments, const int ySegments);

public:
    /*
    * @brief Destructor vacio
    */
    virtual ~Plane() {};
    /*
    * @brief Devuelve la normal que forma el plano
    * @return Vector3 con la normal del plano
    */
    Vector3 getNormal() const;
    /*
    * @brief Devuelve la distancia desde el plano al origen
    * @return Distancia desde el plano al origen
    */
    inline float getD() const { return mPlaneAux.d; }
    /*
    * @brief Devuelve distancia desde un punto al plano
    * @param rkPoint Punto del que se quiere calcular la distancia
    * @return Distancia desde el punto al plano
    */
    float getDistance(const Vector3& rkPoint) const;
    /*
    * @brief Redefine el plano a partir de 3 puntos
    * @param p0 Primer punto
    * @param p1 Segundo punto
    * @param p2 Tercer punto
    */
    void redefine(const Vector3& p0, const Vector3& p1, const Vector3& p2);
    /*
    * @brief Redefine el plano a partir de una normal y un punto
    * @param rkNormal Normal del plano
    * @param rkPoint Punto del plano
    */
    void redefine(const Vector3& rkNormal, const Vector3& rkPoint);
    /*
    * @brief Proyecta un vector sobre el plano
    * @param v Vector que se quiere proyectar
    * @return Vector proyectado sobre el plano
    */
    Vector3 projectVector(const Vector3& v) const;
    /*
    * @brief Normaliza la normal y la distancia d del plano. Devuelve la longitud de la normal antes de ser normalizada
    * @return Longitud de la normal antes de ser normalizada
    */
    float normalise();

    /*
    * @brief Cambia el material del plano
    * @params material Nombre del material al que se quiere cambiar
    */
    void setMaterial(std::string const& material);

    /*
    * @brief Activa o desactiva las sombras del plano
    * @param enable True para activar las sombras, false para desactivarlas
    */
    void castShadows(bool enable);

    /*
    * @brief Devuelve si el plano es igual a otro
    * @param rhs Plano con el que se quiere comparar
    * @return True si los planos son iguales, false en caso contrario
    */
    bool operator==(const Plane& rhs) const;
    /*
    * @brief Devuelve si el plano es distinto a otro
    * @param rhs Plano con el que se quiere comparar
    * @return True si los planos son distintos, false en caso contrario
    */
    bool operator!=(const Plane& rhs) const;
};
}
