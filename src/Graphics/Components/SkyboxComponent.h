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
    std::string materialName;
    std::string skyboxName;
    float distC;   
    bool orderC; 

public:
    COMPONENT_ID("SkyboxComponent");
    /*
    * @brief Constructora de la clase SkyboxComponent
    */
    SkyboxComponent();
    /*
    * @brief Destructora de la clase SkyboxComponent
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
    void update(uint64_t delt) override;
};
}
