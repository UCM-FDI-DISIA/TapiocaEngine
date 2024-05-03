#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class GraphicsManager;
class Mesh;
class RenderNode;
class Transform;
class Animator;
class KeyFrameAnimator;

/*
* @brief Componente que renderiza un mesh en la escena
*/
class TAPIOCA_API MeshRenderer : public Component {
    friend Animator;
    friend KeyFrameAnimator;

private:
    GraphicsManager* graphicsManager;   // Puntero al graphicsManager
    RenderNode* node;                   // Nodo de renderizado
    Transform* transform;               // Transform del objeto
    Mesh* mesh;                         // Mesh a renderizar

    // warning C4251 'Tapioca::MeshRenderer::meshName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::MeshRenderer'
    // warning C4251 'Tapioca::MeshRenderer::materialName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::MeshRenderer'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string materialName;   // Nombre del material
    std::string meshName;       // Nombre de la mesh
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    Vector3 initialRotation;   // Rotacion inicial
    bool castShadows;          // Indica si el objeto emite sombras

    /*
    * @brief Devuelve la mesh
    * @return Puntero a la mesh
    */
    inline Mesh* getMesh() const { return mesh; }

    /*
    * @brief Devuelve el nodo de renderizado
    * @return Puntero al nodo de renderizado
    */
    inline RenderNode* getNode() const { return node; }

public:
    COMPONENT_ID("MeshRenderer")

    /*
    * @brief Constructor por defecto
    */
    MeshRenderer();

    /*
    * @brief Destructor por defecto
    */
    ~MeshRenderer();

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
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

    /*
    * @brief Establece el nombre del material
    * @param name Nombre del material
    */
    void setMaterialName(const std::string& name) { materialName = name; }

    /*
    * @brief Establece el nombre de la mesh
    * @param name Nombre de la mesh
    */
    void setMeshName(const std::string& name);

    /*
    * @brief Establece si la luz es visible o no
    * @param enable Indica si la luz es visible o no
    */
    void setVisible(const bool enable);
    /*
    * @brief Devuelve si la luz es visible o no
    * @return true si la luz es visible, false en caso contrario
    */
    bool isVisible() const;
};
}
