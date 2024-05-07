#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class Skyplane;
class RenderNode;
class Transform;

/**
* @brief Componente que se encarga de crear un plano en el cielo
*/
class TAPIOCA_API SkyplaneComponent : public Component {
private:
    RenderNode* node;           // Nodo de renderizado
    Transform* transform;       // Transform del nodo
    Skyplane* skyplane;         // Plano en el cielo

    // warning C4251 'Tapioca::SkyplaneComponent::materialName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::SkyplaneComponent'
    // warning C4251 'Tapioca::SkyplaneComponent::skyplaneName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::SkyplaneComponent'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string materialName;   // Nombre del material
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    
    Vector3 rkNormal;           // Vector normal del plano
    float fConstant;            // Distancia que se desplaza el plano en la direccion de la normal
    float scale;                // Escala del plano
    float tiling;               // Tiling del plano
    bool drawFirst;             // Si se dibuja primero
    float bow;                  // Curvatura del plano
    int xSegments;              // Numero de segmentos en x
    int ySegments;              // Numero de segmentos en y

public:
    COMPONENT_ID("SkyplaneComponent");

    /**
    * @brief Constructor por defecto
    */
    SkyplaneComponent();

    /**
    * @brief Destructor por defecto
    */
    ~SkyplaneComponent();

    /**
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /**
    * @brief Metodo que se usa para inicializar el componente.
    * Garantiza que todos los componentes iniciales esten creados
    */
    void awake() override;
};
}
