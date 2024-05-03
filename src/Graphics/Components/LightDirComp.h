#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

namespace Tapioca {
class RenderNode;
class LightDirectional;

/*
* @brief Componente que se encarga de la iluminacion de tipo direccional
*/
class TAPIOCA_API LightDirComp : public Component {
private:
    const Vector3 INITIAL_DIR = Vector3(0.0f, 0.0f, -1.0f);

    RenderNode* node;          // Nodo que contiene el LightDirectional
    LightDirectional* light;   // LightDirectional que se encarga de la iluminacion
    Vector3 direction;         // Direccion de la luz
    bool shadows;              // Indica si la luz produce sombras
    Vector4 color;             // Color de la luz
    float powerScale;          // Intensidad de la luz

public:
    COMPONENT_ID("LightDirComp")

    /*
    * @brief Constructor por defecto
    */
    LightDirComp();

    /*
    * @brief Destructor por defecto
    */
    ~LightDirComp();

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;

    /*
    * @brief Crea un LightDirectional, crea un nodo y le asigna el LightDirectional
    */
    void start() override;

    /*
    * @brief Establece el color de la luz
    * @param color Vector4 con el color de la luz
    */
    void setColor(const Vector4 color);

    /*
    * @brief Establece la intensidad de la luz
    * @param power float con la intensidad de la luz
    */
    void setPowerScale(const float power);

    /*
    * @brief Establece la direccion de la luz
    * @param direction Vector3 con la direccion de la luz
    */
    void setDirection(const Vector3 direction);

    /*
    * @brief Activa o desactiva las sombras
    * @param enable bool que indica si se activan o desactivan las sombras
    */
    void produceShadows(const bool enable);
};
}
