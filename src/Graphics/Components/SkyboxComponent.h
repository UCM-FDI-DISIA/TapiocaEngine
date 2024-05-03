#pragma once
#include "Structure/Component.h"

namespace Tapioca {
class Skybox;
class RenderNode;
class Transform;

class TAPIOCA_API SkyboxComponent : public Component {
private:
    RenderNode* node;
    Skybox* skybox;
    Transform* transform;

    // warning C4251 'Tapioca::SkyboxComponent::materialName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita 
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::SkyboxComponent' Graphics C
    // warning C4251 'Tapioca::SkyboxComponent::skyboxName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::SkyboxComponent'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string materialName;
    std::string skyboxName;
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    float distC;
    bool orderC;

public:
    COMPONENT_ID("SkyboxComponent");
    /*
    * @brief Constructor de la clase SkyboxComponent
    */
    SkyboxComponent();
    /*
    * @brief Destructor de la clase SkyboxComponent
    */
    ~SkyboxComponent();

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /*
    * @brief Metodo que se usa para inicializar el componente.
    * Garantiza que todos los componentes iniciales esten creados
    */
    void awake() override;
};
}
