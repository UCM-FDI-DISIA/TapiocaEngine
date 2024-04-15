#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include "Core.h"
#include <string>

namespace Tapioca {
class TAPIOCA_API Slider : public BaseWidget, public Component {
private:
    bool vertical;
    float currentValue;
    float max;
    float min;
    //a e y
    Vector2 position;
    //width y heitgh
    Vector2 size;

public:
    COMPONENT_ID("Slider")
    //@brief constructora vacia
    Slider();
    //@brief destructora por defecto
    ~Slider();
    /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param variables unordered_map con los parametros iniciales
    */
    virtual bool initComponent(const CompMap& variables) override;
    /*
    * @brief Asigna el transform del objeto al texto e inicializa la fuente del texto
    */
    virtual void start() override;

    /*
    * @brief Metodo que se usa para renderizar el texto
    */
    virtual void render() const override;
    /*
    * @brief devuelve el valor de progreso actual
    */
    float* getCurrentValue();
    /*
    * @brief Devuelve el limite superior del slider 
    */
    float getMax() { return max; }
    /*
    * @brief Devuelve el limite inferior del slider
    */
    float getMin() { return min; }
    /*
    *  @brief Devuelve el flag de veritcalidad
    */
    bool isVertical() { return vertical; }
};
}
