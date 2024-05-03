#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector4.h"

namespace Tapioca {
class RenderNode;
class LightPoint;
class Transform;

/**
* @brief Componente que se encarga de la iluminacion de tipo puntual
*/
class TAPIOCA_API LightPointComp : public Component {
private:
    RenderNode* node;          // Nodo que contiene el LightPoint
    Transform* transform;      // Transform del nodo
    LightPoint* light;         // LightPoint que se encarga de la iluminacion
    Vector4 color;             // Color de la luz
    float powerScale;          // Escala de la potencia de la luz
    float attenuationFactor;   // Factor de atenuacion de la luz
    bool attenuationSet;       // Flag que indica si se ha seteado el factor de atenuacion

public:
    COMPONENT_ID("LightPointComp")

    /**
    * @brief Constructor por defecto
    */
    LightPointComp();

    /**
    * @brief Destructor por defecto
    */
    ~LightPointComp();

    /**
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;

    /**
    * @brief Crea un LightPoint, crea un nodo y le asigna el LightPoint
    */
    void awake() override;

    /**
    * @brief Recibe eventos. Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;

    /**
    * @brief Establece el color de la luz
    * @param color Color de la luz
    */
    void setColor(const Vector4 color);

    /**
    * @brief Establece la potencia de la luz
    * @param power Escala de la potencia de la luz
    */
    void setPowerScale(const float power);

    /**
    * @brief Establece el factor de atenuacion de la luz
    * @param attenuationFactor Factor de atenuacion de la luz
    */
    void setAttenuation(const float attenuationFactor);

    /**
    * @brief Establece si la luz es visible o no
    * @param enable Indica si la luz es visible o no
    */
    void setVisible(const bool enable);
    /**
    * @brief Devuelve si la luz es visible o no
    * @return true si la luz es visible, false en caso contrario
    */
    bool isVisible() const;
};
}
