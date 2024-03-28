#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class Plane;
class RenderNode;
class Transform;

class TAPIOCA_API PlaneComponent : public Component {
private:
    RenderNode* node;
    Plane* plane;
    Transform* transform;
    Vector3 rkNormal;
    float fConstant;
    float width;
    float height;
    int xSegments;
    int ySegments;


#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    std::string planeName;
    std::string materialName;
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    Plane* getPlane() const;
    Vector3 initialRotation;

public:
    COMPONENT_ID("PlaneComponent")
    /*
    * @brief Constructora de la clase PlaneComponent
    */
    PlaneComponent();
    /*
    * @brief Destructora de la clase PlaneComponent
    */
    ~PlaneComponent();

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
    void start() override;
    /*
    * @brief Metodo que se usa para recibir eventos.
    * Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent.
    * @param Id indica el tipo de mensaje
    * @param info puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;
};
}
