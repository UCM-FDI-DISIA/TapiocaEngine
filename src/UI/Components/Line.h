#pragma once
#include "Structure/Component.h"
#include <string>
#include "Utilities/Vector2.h"
#include "Utilities/Vector4.h"

namespace Tapioca {
class UIManager;

/**
* @brief Clase que representa una linea en la interfaz grafica
*/
class TAPIOCA_API Line : public Component {
private:
    Vector2 startPosition;   // Posicion inicial de la linea
    Vector2 endPosition;     // Posicion final de la linea
    Vector4 lineColor;       // Color de la linea
    float lineThickness;     // Grosor de la linea
    bool addBorder;          // Si se le agrega un borde a la linea
    Vector4 borderColor;     // Color del borde de la linea
    float borderThickness;   // Grosor del borde de la linea

    UIManager* uiManager;   // Puntero al UIManager

public:
    COMPONENT_ID("Line")

    /**
    * @brief Constructor por defecto
    */
    Line();

    /**
    * @brief Destructor por defecto
    */
    ~Line();

    /**
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param variables unordered_map con los parametros iniciales
    */
    bool initComponent(const CompMap& variables) override;

    /**
    * @brief Coge referencia al UIManager
    */
    void start() override;

    /**
    * @brief Metodo que se usa para renderizar la linea
    */
    void render() const override;

    /**
    * @brief Establece la posicion inicial de la linea
    * @param startPosition Posicion inicial de la linea
    */
    inline void setStartPosition(const Vector2& startPosition) { this->startPosition = startPosition; }

    /**
    * @brief Establece la posicion final de la linea
    * @param endPosition Posicion final de la linea
    */
    inline void setEndPosition(const Vector2& endPosition) { this->endPosition = endPosition; }

    /**
    * @brief Establece el color de la linea
    * @param lineColor Color de la linea
    */
    inline void setLineColor(const Vector4& lineColor) { this->lineColor = lineColor; }

    /**
    * @brief Establece el grosor de la linea
    * @param lineThickness Grosor de la linea
    */
    inline void setThickness(float lineThickness) { this->lineThickness = lineThickness; }

    /**
    * @brief Establece si se le agrega un borde a la linea
    * @param addBorder Si se le agrega un borde a la linea
    */
    inline void setAddBorder(bool addBorder) { this->addBorder = addBorder; }

    /**
    * @brief Establece el color del borde de la linea
    * @param borderColor Color del borde de la linea
    */
    inline void setBorderColor(const Vector4& borderColor) { this->borderColor = borderColor; }

    /**
    * @brief Establece el grosor del borde de la linea
    * @param borderThickness Grosor del borde de la linea
    */
    inline void setBorderThickness(float borderThickness) { this->borderThickness = borderThickness; }

    /**
    * @brief Devuelve la posicion inicial de la linea
    * @return Posicion inicial de la linea
    */
    inline Vector2 getStartPosition() const { return startPosition; }

    /**
    * @brief Devuelve la posicion final de la linea
    * @return Posicion final de la linea
    */
    inline Vector2 getEndPosition() const { return endPosition; }

    /**
    * @brief Devuelve el color de la linea
    * @return Color de la linea
    */
    inline Vector4 getLineColor() const { return lineColor; }

    /**
    * @brief Devuelve el grosor de la linea
    * @return Grosor de la linea
    */
    inline float getLineThickness() const { return lineThickness; }

    /**
    * @brief Devuelve si se le agrega un borde a la linea
    * @return Si se le agrega un borde a la linea
    */
    inline bool getAddBorder() const { return addBorder; }

    /**
    * @brief Devuelve el color del borde de la linea
    * @return Color del borde de la linea
    */
    inline Vector4 getBorderColor() const { return borderColor; }

    /**
    * @brief Devuelve el grosor del borde de la linea
    * @return Grosor del borde de la linea
    */
    inline float getBorderThickness() const { return borderThickness; }
};
}
