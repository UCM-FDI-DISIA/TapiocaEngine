#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class Skyplane;
class RenderNode;
class Transform;

class TAPIOCA_API SkyplaneComponent : public Component {
private:
    RenderNode* node;
    Skyplane* skyplane;
    Transform* transform;
    std::string materialName;
    std::string skyplaneName;
    Vector3 rkNormal;
    float fConstant;
    float scale;
    float tiling;
    bool drawFirst;
    float bow;
    int xSegments;
    int ySegments;

public:
    COMPONENT_ID("SkyplaneComponent");
    /*
    * @brief Constructor de la clase SkyplaneComponent
    */
    SkyplaneComponent();
    /*
    * @brief Destructor de la clase SkyplaneComponent
    */
    ~SkyplaneComponent();

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
