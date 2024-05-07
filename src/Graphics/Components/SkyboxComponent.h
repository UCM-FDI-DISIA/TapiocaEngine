#pragma once
#include "Structure/Component.h"

namespace Tapioca {
class Skybox;
class RenderNode;
class Transform;

/**
* @brief Componente que se encarga de renderizar un skybox
*/
class TAPIOCA_API SkyboxComponent : public Component {
private:
    RenderNode* node;       // Nodo de renderizado
    Transform* transform;   // Transform del skybox
    Skybox* skybox;         // Skybox a renderizar

    // warning C4251 'Tapioca::SkyboxComponent::materialName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::SkyboxComponent' Graphics C
    // warning C4251 'Tapioca::SkyboxComponent::skyboxName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::SkyboxComponent'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string materialName;   // Nombre del material del skybox
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    float distC;   // Distancia hasta la camara
    bool orderC;   // True se pinta el plano antes que la escena False despues

public:
    COMPONENT_ID("SkyboxComponent");
    /**
    * @brief Constructor por defecto
    */
    SkyboxComponent();

    /**
    * @brief Destructor por defecto
    */
    ~SkyboxComponent();

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
