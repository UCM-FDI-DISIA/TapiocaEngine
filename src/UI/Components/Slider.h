#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"

namespace Tapioca {
/**
* @brief Clase que representa un slider en la interfaz de usuario
*/
class TAPIOCA_API Slider : public BaseWidget, public Component {
private:
    bool isVertical;      // Flag que indica si el slider es vertical
    float currentValue;   // Valor actual del slider
    float maxLimit;       // Limite superior del slider
    float minLimit;       // Limite inferior del slider

public:
    COMPONENT_ID("Slider")

    /**
    * @brief Constructor por defecto
    */
    Slider();

    /**
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param Variables unordered_map con los parametros iniciales
    */
    bool initComponent(const CompMap& variables) override;

    /**
    * @brief Asigna el transform del objeto al texto e inicializa la fuente del texto
    */
    void start() override;

    /**
    * @brief Metodo que se usa para renderizar el texto
    */
    void render() const override;

    /**
    * @brief Establece el flag de verticalidad
    * @param isVertical Flag de verticalidad
    */
    inline void setIsVertical(bool isVertical) { this->isVertical = isVertical; }

    /**
    * @brief Establece el valor del slider actual
    * @param currentValue Valor del slider actual
    */
    inline void setCurrentValue(float currentValue) { this->currentValue = currentValue; }

    /**
    * @brief Establece el limite superior del slider
    * @param maxLimit Limite superior del slider
    */
    inline void setMaxLimit(float maxLimit) { this->maxLimit = maxLimit; }

    /**
    * @brief Establece el limite inferior del slider
    * @param minLimit Limite inferior del slider
    */
    inline void setMinLimit(float minLimit) { this->minLimit = minLimit; }

    /**
    * @brief Devuelve el flag de verticalidad
    * @return Flag de verticalidad
    */
    inline bool getIsVertical() const { return isVertical; }

    /**
    * @brief Devuelve el valor del slider actual
    * @return Valor del slider actual
    */
    inline float getCurrentValue() const { return currentValue; }

    /**
    * @brief Devuelve el limite superior del slider 
    * @return Limite superior del slider
    */
    inline float getMaxLimit() const { return maxLimit; }

    /**
    * @brief Devuelve el limite inferior del slider
    * @return Limite inferior del slider
    */
    inline float getMinLimit() const { return minLimit; }
};
}
