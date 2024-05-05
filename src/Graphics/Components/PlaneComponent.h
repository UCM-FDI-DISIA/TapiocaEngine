#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class Plane;
class RenderNode;
class Transform;

/**
* @brief Clase que se encarga de crear un plano en la escena
*/
class TAPIOCA_API PlaneComponent : public Component {
private:
    RenderNode* node;           // Nodo de renderizado
    Transform* transform;       // Transform del nodo
    Plane* plane;               // Plano
    Vector3 rkNormal;           // Vector normal del plano
    Vector3 up;                 // Vector perpendicular del plano
    float width;                // Ancho del plano
    float height;               // Alto del plano
    int xSegments;              // Numero de segmentos en x
    int ySegments;              // Numero de segmentos en y

    // warning C4251 'Tapioca::PlaneComponent::planeName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::PlaneComponent'
    // warning C4251 'Tapioca::PlaneComponent::materialName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::PlaneComponent'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string planeName;      // Nombre del plano
    std::string materialName;   // Nombre del material
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    
public:
    COMPONENT_ID("PlaneComponent")

    /**
    * @brief Constructor por defecto
    */
    PlaneComponent();

    /**
    * @brief Destructor por defecto
    */
    ~PlaneComponent();

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
    /**
    * @brief Metodo que se usa para recibir eventos.
    * Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent.
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;

    /**
    * @brief Hace que el sistema de particulas sea o no visible
    * @param v True para que sea visible, false para que no lo sea
    */
    void setVisible(const bool v);
    /**
    * @brief Devuelve si el sistema de particulas es visible o no
    * @return True si es visible, false en caso contrario
    */
    bool isVisible() const;
};
}
