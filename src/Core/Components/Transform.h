#pragma once
#include <unordered_set>
#include "../Structure/Component.h"
#include "../Utilities/Vector3.h"
#include "../Utilities/defs.h"

namespace Tapioca {
class INode;
class TransformBuilder;

/*
    * @brief Guarda la posicion, rotacion y escala del objeto, ademas de la interfaz del nodo.
    * Este componente esta en todos los objetos, y si se intenta borrar, se borrara todo el objeto.
    * @param scene Escena a la que se le van a cargar los gameobjects
    * @return Devuelve true si se ha cargado correctamente
*/
class TAPIOCA_API Transform : public Component {
    friend TransformBuilder;

private:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    /*
    * @brief Envia un evento para informar que se ha modificado el transform
    */
    void changed();

    Transform* parent;
    std::unordered_set<Transform*> children;

    /*
    * @brief Devuelve en el vector que se pasa como parametro los hijos de este transfom
    * @param allChildren vector en el que se devuelven todos los hijos
    */
    void getAllChildrenAux(std::vector<Transform*>& allChildren) const;
    /*
    * @brief Aniade un hijo a este transform
    * @param child puntero al hijo que de aniade a este transform
    */
    void addChild(Transform* const child);
    /*
    * @brief Elimina un hijo de este transform
    * @param child puntero al hijo que se elimina del transform
    */
    void removeChild(Transform* const child);
    /*
    * @brief Desconecta este transform, elimina todos los hijos y se elimina a si mismo de su padre
    */
    void removeConnections();
    /*
    * @brief Elimina su relacion de parentesco con su padre
    */
    void removeParent();

public:
    COMPONENT_ID("Transform")
    /*
    * @brief Constructora de la clase Transform. Se inicializa con posicion y rotacion 0 y escala 1
    */
    Transform();
    /*
    * @brief Destrucora de transform. Se elimina la relacion de parentesco y se eliminan los hijos
    */
    ~Transform();
    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /*
    * @brief Procesa un evento recibido
    * @param id String que indica el tipo de evento
    * @param info Parametros del mensaje, cada evento gestiona sus propios parametros
    */
    void handleEvent(std::string const& id, void* info) override;

    /*
    * @brief Devuelve la posicion local del transform
    * @return La posicion local del transform
    */
    inline Vector3 getPosition() const { return position; }
    /*
    * @brief Devuelve la posicion global del transform
    * @return La posicion global del transform
    */
    Vector3 getGlobalPosition() const;
    /*
    * @brief Devuelve la rotacion local del transform
    * @return La posicion local del transform
    */
    inline Vector3 getRotation() const { return rotation; }
    /*
    * @brief Devuelve la rotacion global del transform
    * @return La rotacion global del transform
    */
    Vector3 getGlobalRotation() const;
    /*
    * @brief Devuelve la escala local del transform
    * @return La escala local del transform
    */
    inline Vector3 getScale() const { return scale; }
    /*
    * @brief Devuelve la escala global del transform
    * @return La escala global del transform
    */
    Vector3 getGlobalScale() const;

    // Todos estos metodos generan el evento "transformChanged"
    /*
    * @brief Cambia la posicion del transform
    * @param Posicion a la que se quiere mover el transform
    */
    void setPosition(const Vector3& p);
    /*
    * @brief Cambia la rotacion del transform
    * @param Rotacion a la que se quiere rotar el transform
    */
    void setRotation(const Vector3& r);
    /*
    * @brief Cambia la escala del transform
    * @param Tamanio al que se quiere escalar el transform
    */
    void setScale(const Vector3& s);

    /*
    * @brief Mueve el objeto tanto como el vector dado indica
    * @param p distancia que se quiere mover el transform
    */
    void translate(const Vector3& p);
    /*
    * @brief Rota el objeto sobre los ejes tanto como el vector dado indica
    * @param r rotacion que se quiere aplicar sobre el transform
    */
    void rotate(const Vector3& r);

    /*
    * @brief Devuelve el eje X de coordenadas locales en coordenadas globales
    * @return Eje X de coordenadas locales en coordenadas globales
    */
    Vector3 right();
    /*
    * @brief Devuelve el eje Y de coordenadas locales en coordenadas globales
    * @return Eje Y de coordenadas locales en coordenadas globales
    */
    Vector3 up();
    /*
    * @brief Devuelve el eje Z de coordenadas locales en coordenadas globales
    * @return Eje Z de coordenadas locales en coordenadas globales
    */
    Vector3 forward();

    /*
    * @brief Establece el transform dado como el padre de este
    * @param transform Transform al que se quiere hacer padre de este
    */
    void setParent(Transform* const transform);
    /*
    * @brief Devuelve el padre de este transform
    * @return Padre de este transform (nullptr si no tiene)
    */
    Transform* getParent() const;

    /*
    * @brief Devuelve los hijos directos de este transform
    * @return Vector que contiene los punteros los hijos directos de este transform
    */
    std::vector<Transform*> getChildren() const;
    /*
    * @brief Devuelve tanto los hijos directos como los indirectos de este transform
    * @return Vector que contiene los punteros de tanto los hijos directos como los indirectos de este transform
    */
    std::vector<Transform*> getAllChildren() const;
};
}