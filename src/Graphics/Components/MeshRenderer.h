#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class Mesh;
class RenderNode;
class Transform;
class Animator;

class TAPIOCA_API MeshRenderer : public Component {
    friend Animator;

private:
    RenderNode* node;
    Mesh* mesh;
    Transform* transform;
    // warning C4251 'Tapioca::MeshRenderer::meshName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::MeshRenderer'
    // warning C4251 'Tapioca::MeshRenderer::materialName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::MeshRenderer'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string meshName;
    std::string materialName;
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    Vector3 initialRotation;
    bool castShadows;

    Mesh* getMesh() const;

public:
    COMPONENT_ID("MeshRenderer")
    /*
    * @brief Constructora de la clase MeshRenderer
    */
    MeshRenderer();
    /*
    * @brief Destructora de la clase MeshRenderer
    */
    ~MeshRenderer();

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    // se necesita una awake para que el animator puede obtener el mesh
    /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * Garantiza que todos los componentes iniciales esten creados
    */
    void awake() override;
    /*
    * @brief Metodo que se usa para recibir eventos.
    * Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent.
    * @param Id indica el tipo de mensaje
    * @param info puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;

    void setMeshName(const std::string& n);
    void setMaterialName(const std::string& n);

    void setVisible(const bool enable);
    bool isVisible() const;
};
}