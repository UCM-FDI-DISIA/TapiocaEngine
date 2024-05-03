#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector4.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class RenderNode;
class LightSpotlight;
class Transform;

/*
* @brief Componente que se encarga de la iluminacion de tipo spot
*/
class TAPIOCA_API LightSpotComp : public Component {
private:
    RenderNode* node;          // Nodo que contiene el LightPoint
    Transform* transform;      // Transform del nodo
    LightSpotlight* light;     // LightPoint que se encarga de la iluminacion
    Vector3 direction;         // Direccion de la luz
    Vector4 color;             // Color de la luz
    float powerScale;          // Escala de la potencia de la luz
    float attenuationFactor;   // Factor de atenuacion de la luz
    bool attenuationSet;       // Flag que indica si se ha seteado el factor de atenuacion
    float nearClipDist;        // Distancia de corte cercano
    float falloff;             // Falloff de la luz
    float innerAngle;          // Angulo interno de la luz
    bool innerAngleSet;        // Flag que indica si se ha seteado el angulo interno
    float outerAngle;          // Angulo externo de la luz
    bool outerAngleSet;        // Flag que indica si se ha seteado el angulo externo

public:
    COMPONENT_ID("LightSpotComp")

    /*
    * @brief Constructor por defecto
    */
    LightSpotComp();

    /*
    * @brief Destructor por defecto
    */
    ~LightSpotComp();

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;

    /*
    * @brief Crea un LightSpotlight, crea un nodo y le asigna el LightSpotlight
    */
    void awake() override;

    /*
    * @brief Recibe eventos. Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;

    /*
    * @brief Establece el color de la luz
    * @param color Color de la luz
    */
    void setColor(const Vector4 color);

    /*
    * @brief Establece la potencia de la luz
    * @param power Escala de la potencia de la luz
    */
    void setPowerScale(const float power);

    /*
    * @brief Establece el factor de atenuacion de la luz
    * @param attenuationFactor Factor de atenuacion de la luz
    */
    void setAttenuation(const float attenuationFactor);

    /*
    * @brief Establece la direccion de la luz
    * @param direction Direccion de la luz
    */
    void setDirection(const Vector3 direction);

    /*
    * @brief Establece la distancia de corte cercano
    * @param distance Distancia de corte cercano
    */
    void setNearClipDistance(float distance);

    /*
    * @brief Establece el angulo interno de la luz
    * @param degrees Angulo interno de la luz
    */
    void setInnerAngle(float degrees);

    /*
    * @brief Establece el angulo externo de la luz
    * @param degrees Angulo externo de la luz
    */
    void setOuterAngle(float degrees);

    /*
    * @brief Establece el falloff de la luz
    * @param falloff Falloff de la luz
    */
    void setFalloff(float falloff);

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
