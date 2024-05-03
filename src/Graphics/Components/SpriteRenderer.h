#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector4.h"

namespace Tapioca {
class Billboard;
class BillboardSet;
class RenderNode;
class Transform;

/*
* @brief Componente que se encarga de renderizar un sprite en pantalla
*/
class TAPIOCA_API SpriteRenderer : public Component {
private:
    RenderNode* node;             // Nodo de renderizado
    Transform* transform;         // Transform del objeto
    BillboardSet* billboardSet;   // Conjunto de billboards
    Billboard* billboard;         // Billboard
    Vector4 color;                // Color del sprite

public:
    COMPONENT_ID("SpriteRenderer");

    /*
    * @brief Constructor por defecto
    */
    SpriteRenderer();

    /*
    * @brief Destructor por defecto
    */
    ~SpriteRenderer();

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /*
    * @brief 
    */
    void start() override;
    /*
    * @brief Metodo que se usa para recibir eventos.
    * Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent.
    * @param Id indica el tipo de mensaje
    * @param info puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;

    /*
    * @brief Hace que el sistema de particulas sea o no visible
    * @param v True para que sea visible, false para que no lo sea
    */
    void setVisible(const bool v);
    /*
    * @brief Devuelve si el sistema de particulas es visible o no
    * @return True si es visible, false en caso contrario
    */
    bool isVisible() const;
};
}