#pragma once
#include "Structure/Component.h"
#include <unordered_set>
#include "Utilities/Vector2.h"
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"
#include "defs.h"

namespace Tapioca {
/*
* @brief Guarda la posicion, rotacion y escala del objeto, ademas de la interfaz del nodo.
* Este componente esta en todos los objetos, y si se intenta borrar, se borrara todo el objeto.
* @param scene Escena a la que se le van a cargar los gameobjects
* @return true si se ha cargado correctamente, false si no
*/
class TAPIOCA_API Transform : public Component {
private:
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;

    Transform* parent;

    // warning C4251 'Tapioca::Transform::children' :
    // class 'std::unordered_set<Tapioca::Transform *,std::hash<Tapioca::Transform *>,
    // std::equal_to<Tapioca::Transform *>,std::allocator<Tapioca::Transform *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Transform'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_set<Transform*> children;
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    /*
    * @brief Envia un evento para informar que se ha modificado el transform
    */
    void changed(bool rb = false);

    /*
    * @brief Devuelve en el vector que se pasa como parametro, los hijos de este transfom
    * @param allChildren Vector en el que se devuelven todos los hijos
    */
    void getAllChildrenAux(std::vector<Transform*>& allChildren) const;
    /*
    * @brief Anade un hijo a este transform
    * @param child Puntero al hijo que de anade a este transform
    */
    void addChild(Transform* const child);
    /*
    * @brief Elimina un hijo de este transform
    * @param child Puntero al hijo que se elimina del transform
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

    Vector3 localRight();

    Vector3 localUp();

    Vector3 localForward();

    Vector3 getGlobalPositionWithoutRotationAux(Vector3 point) const;

    Vector3 getGlobalPositionAux(Vector3 aux) const;

public:
    COMPONENT_ID("Transform")

    /*
    * @brief Constructor de la clase Transform. Se inicializa con posicion y rotacion 0 y escala 1
    */
    Transform();
    /*
    * @brief Destructor de transform. Se elimina la relacion de parentesco y se eliminan los hijos
    */
    ~Transform();

    /*
    * @brief Recibe los parametros iniciales y se inicializan los atributos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return true si se ha inicializado correctamente, false si no
    */
    bool initComponent(const CompMap& variables) override;

    /*
    */
    void start() override;

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
    * @brief Devuelve la posicion XY local del transform
    * @return La posicion XY local del transform
    */
    inline Vector2 getPositionXY() const { return Vector2(position.x, position.y); }

    /*
    * @brief Devuelve la posicion XY local del transform
    * @return La posicion XY local del transform
    */
    //inline ImVec2 getImPositionXY() const { return ImVec2(position.x, position.y); }

    /*
    * @brief Devuelve la posicion global del transform
    * @return La posicion global del transform
    */
    Vector3 getGlobalPositionWithoutRotation() const;

    Vector3 getGlobalPosition() const;
    /*
    * @brief Devuelve la rotacion local del transform
    * @return La posicion local del transform
    */
    inline Quaternion getRotation() const { return rotation; }
    /*
    * @brief Devuelve la rotacion global del transform
    * @return La rotacion global del transform
    */
    Quaternion getGlobalRotation() const;
    /*
    * @brief Devuelve la escala local del transform
    * @return La escala local del transform
    */
    inline Vector3 getScale() const { return scale; }
    /*
    * @brief Devuelve la escala XY local del transform
    * @return La escala XY local del transform
    */
    inline Vector2 getScaleXY() const { return Vector2(scale.x, scale.y); }
    /*
    * @brief Devuelve la escala XY local del transform
    * @return La escala XY local del transform
    */
    //inline ImVec2 getImScaleXY() const { return ImVec2(scale.x, scale.y); }
    /*
    * @brief Devuelve la escala global del transform
    * @return La escala global del transform
    */
    Vector3 getGlobalScale() const;

    // Todos estos metodos generan el evento "transformChanged"
    /*
    * @brief Cambia la posicion del transform
    * @param p Posicion a la que se quiere mover el transform
    * @param rb Indica si es movido por f�sicas
    */
    void setPosition(const Vector3& p, bool rb = false);
    /*
    * @brief Cambia la posicion XY del transform
    * @param pXY Posicion XY a la que se quiere mover el transform
    * @param rb Indica si es movido por f�sicas
    */
    void setPositionXY(const Vector2& pXY, bool rb = false);
    /*
    * @brief Cambia la rotacion del transform
    * @param r Rotacion a la que se quiere rotar el transform
    * @param rb Indica si es movido por f�sicas
    */
    void setRotation(const Vector3& r, bool rb = false);
    /*
    * @brief Cambia la escala del transform
    * @param s Tamanio al que se quiere escalar el transform
    */
    void setScale(const Vector3& s);
    /*
    * @brief Cambia la escala XY del transform
    * @param s Tamanio XY al que se quiere escalar el transform
    */
    void setScaleXY(const Vector2& s);

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
    * @brief Devuelve el eje X de coordenadas locales en coordenadas globales, inicialmente su eje X es 1,0,0
    * @return Eje X de coordenadas locales en coordenadas globales
    */
    Vector3 right();
    /*
    * @brief Devuelve el eje Y de coordenadas locales en coordenadas globales, inicialmente su eje Y es 0,1,0
    * @return Eje Y de coordenadas locales en coordenadas globales
    */
    Vector3 up();
    /*
    * @brief Devuelve el eje Z de coordenadas locales en coordenadas globales, inicialmente su eje Z es 0,0,1
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