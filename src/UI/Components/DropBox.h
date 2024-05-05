#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include <vector>
#include <string>

namespace Tapioca {
/**
* @brief Clase que representa un DropBox en la interfaz grafica con el que el usuario puede interactuar para seleccionar una opcion
*/
class TAPIOCA_API DropBox : public BaseWidget, public Component {
private:

    // warning C4251 'Tapioca::DropBox::items' : 
    // class 'std::vector<std::string,std::allocator<std::string>>' necesita tener una interfaz DLL
    // para que la utilicen los clientes de class 'Tapioca::DropBox'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::vector<std::string> items;   // Vector con las distintas opciones
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif

    int currentItem;                  // Indice del elemento seleccionado

public:
    COMPONENT_ID("DropBox")

    /**
    * @brief Constructor por defecto
    */
    DropBox();

    /**
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param variables unordered_map con los parametros iniciales
    */
    bool initComponent(const CompMap& variables) override;

    /**
    * @brief Asigna el transform del objeto al texto
    */
    void start() override;

    /**
    * @brief Metodo que se usa para renderizar 
    */
    void render() const override;

    /**
    * @brief Establece el vector con las distintas opciones
    * @param items Vector con las distintas opciones
    */
    inline void setItems(const std::vector<std::string>& items) { this->items = items; }

    /**
    * @brief Establece el indice del elemento seleccionado
    * @param currentItem Indice del elemento seleccionado
    */
    inline void setCurrentItem(int currentItem) { this->currentItem = currentItem; }

    /**
    * @brief Devuelve el vector con las distintas opciones
    * @return Vector con las distintas opciones
    */
    inline std::vector<std::string> getItems() { return items; }

    /**
    * @brief Devuelve el indice del elemento seleccionado
    * @return Indice del elemento seleccionado
    */
    inline int* getCurrentItem() { return &currentItem; }
};
}
