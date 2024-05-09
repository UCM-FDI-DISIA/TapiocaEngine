#pragma once
#include "Structure/Component.h"
#include <unordered_set>
#include "Utilities/Vector2.h"
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"
#include "defs.h"

namespace Tapioca {
/**
* @brief Guarda la posicion, rotacion y escala del objeto, ademas de la interfaz del nodo.
* Este componente esta en todos los objetos, y si se intenta borrar, se borrara todo el objeto
*/
class TAPIOCA_API Transform : public Component {
private:
    Vector3 position;      // Posicion del objeto
    Quaternion rotation;   // Rotacion del objeto
    Vector3 scale;         // Escala del objeto

    Transform* parent;   // Padre del objeto

    // warning C4251 'Tapioca::Transform::children' :
    // class 'std::unordered_set<Tapioca::Transform *,std::hash<Tapioca::Transform *>,
    // std::equal_to<Tapioca::Transform *>,std::allocator<Tapioca::Transform *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Transform'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_set<Transform*> children;   // Hijos del objeto
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    /**
    * @brief Genera el evento "posChanged"
    * @param rb Indica si es movido por fisicas
    */
    void posChanged(bool rb = false);

    /**
    * @brief Genera el evento "rotChanged"
    * @param rb Indica si es movido por fisicas
    */
    void rotChanged(bool rb = false);

    /**
    * @brief Genera el evento "scaleChanged"
    */
    void scaleChanged();

    /**
    * @brief Devuelve en el vector que se pasa como parametro, los hijos de este transform
    * @param allChildren Vector en el que se devuelven todos los hijos
    */
    void getAllChildrenAux(std::vector<Transform*>& allChildren) const;
    /**
    * @brief Anade un hijo a este transform
    * @param child Puntero al hijo que de anade a este transform
    */
    void addChild(Transform* const child);
    /**
    * @brief Elimina un hijo de este transform
    * @param child Puntero al hijo que se elimina del transform
    */
    void removeChild(Transform* const child);
    /**
    * @brief Desconecta este transform, elimina todos los hijos y se elimina a si mismo de su padre
    */
    void removeConnections();
    /**
    * @brief Elimina su relacion de parentesco con su padre
    */
    void removeParent();

    /**
    * @brief Devuelve la direccion derecha relativa a la orientacion actual del objeto, considerando su rotacion (Eje X)
    * @return Direccion derecha relativa a la orientacion actual del objeto
    */
    Vector3 localRight();

    /**
    * @brief Devuelve la direccion arriba relativa a la orientacion actual del objeto, considerando su rotacion (Eje Y)
    * @return Direccion arriba relativa a la orientacion actual del objeto
    */
    Vector3 localUp();

    /**
    * @brief Devuelve la direccion delante relativa a la orientacion actual del objeto, considerando su rotacion (Eje Z)
    * @return Direccion delante relativa a la orientacion actual del objeto
    */
    Vector3 localForward();

    /**
    * @brief Devuelve la posicion global del transform sin tener en cuenta la rotacion
    * @param point Punto al que se le quiere calcular la posicion global
    * @return Posicion global del transform sin tener en cuenta la rotacion
    */
    Vector3 getGlobalPositionWithoutRotationAux(Vector3 point) const;

    /**
    * @brief Devuelve la posicion global del transform
    * @param aux Vector auxiliar
    * @return Posicion global del transform
    */
    Vector3 getGlobalPositionAux(Vector3 aux) const;

    /**
    * @brief Devuelve la posicion local del transform
    * @param point Vector en coordenadas global
    * @return Coordenadas locales respecto del transform
    */
    Vector3 getLocalFromGlobalPos(Vector3 point) const;

public:
    COMPONENT_ID("Transform")

    /**
    * @brief Inicializa con posicion 0, rotacion 0 y escala 1
    */
    Transform();
    /**
    * @brief Elimina la relacion de parentesco y se eliminan los hijos
    */
    ~Transform();

    /**
    * @brief Recibe los parametros iniciales y se inicializan los atributos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return true si se ha inicializado correctamente, false si no
    */
    bool initComponent(const CompMap& variables) override;
    /**
    * @brief Manda eventos de cambio de transform
    */
    void start() override;

    /**
    * @brief Procesa un evento recibido
    * @param id Indica el tipo de evento
    * @param info Parametros del mensaje, cada evento gestiona sus propios parametros
    */
    void handleEvent(std::string const& id, void* info) override;

    /**
    * @brief Devuelve la posicion local del transform
    * @return Posicion local del transform
    */
    inline Vector3 getPosition() const { return position; }

    /**
    * @brief Devuelve la posicion XY local del transform
    * @return Posicion XY local del transform
    */
    inline Vector2 getPositionXY() const { return Vector2(position.x, position.y); }

    /**
    * @brief Devuelve la posicion global del transform
    * @return Posicion global del transform
    */
    Vector3 getGlobalPositionWithoutRotation() const;

    /**
    * @brief Devuelve la posicion global del transform
    * @return Posicion global del transform
    */
    Vector3 getGlobalPosition() const;

    /**
    * @brief Devuelve la rotacion local del transform
    * @return Posicion local del transform
    */
    inline Quaternion getRotation() const { return rotation; }
    /**
    * @brief Devuelve la rotacion global del transform
    * @return Rotacion global del transform
    */
    Quaternion getGlobalRotation() const;
    /**
    * @brief Devuelve la escala local del transform
    * @return Escala local del transform
    */
    inline Vector3 getScale() const { return scale; }
    /**
    * @brief Devuelve la escala XY local del transform
    * @return Escala XY local del transform
    */
    inline Vector2 getScaleXY() const { return Vector2(scale.x, scale.y); }
    /**
    * @brief Devuelve la escala global del transform
    * @return Escala global del transform
    */
    Vector3 getGlobalScale() const;

    /**
    * @brief Cambia la posicion del transform. Genera el evento "posChanged"
    * @param p Posicion a la que se quiere mover el transform
    * @param rb Indica si es movido por f�sicas
    */
    void setPosition(const Vector3& p, bool rb = false);

    /**
    * @brief Cambia la posicion del transform. Genera el evento "posChanged"
    * @param p Posicion en coordenadas global a la que se quiere mover el transform
    * @param rb Indica si es movido por fisicas
    */
    void setGlobalPosition(const Vector3& p, bool rb = false);

    /**
    * @brief Cambia la posicion XY del transform. Genera el evento "posChanged"
    * @param pXY Posicion XY a la que se quiere mover el transform
    * @param rb Indica si es movido por fisicas
    */
    void setPositionXY(const Vector2& pXY, bool rb = false);
    /**
    * @brief Cambia la rotacion del transform. Genera el evento "posChanged" y "rotChanged"
    * @param r Rotacion a la que se quiere rotar el transform
    * @param rb Indica si es movido por fisicas
    */
    void setRotation(const Vector3& r, bool rb = false);
    /**
    * @brief Cambia la rotacion del transform. Genera el evento "posChanged" y "rotChanged"
    * @param q Rotacion en Quaterniones a la que se quiere rotar el transform
    * @param rb Indica si es movido por fisicas
    */
    void setRotation(const Quaternion& q, bool rb = false);
    /**
    * @brief Cambia la escala del transform. Genera el evento "posChanged" y "scaleChanged"
    * @param s Tamano al que se quiere escalar el transform
    */
    void setScale(const Vector3& s);
    /**
    * @brief Cambia la escala XY del transform. Genera el evento "posChanged" y "scaleChanged"
    * @param s Tamano XY al que se quiere escalar el transform
    */
    void setScaleXY(const Vector2& s);

    /**
    * @brief Mueve el objeto tanto como el vector dado indica
    * @param p distancia que se quiere mover el transform
    */
    void translate(const Vector3& p);
    /**
    * @brief Rota el objeto sobre los ejes tanto como el vector dado indica
    * @param r rotacion que se quiere aplicar sobre el transform
    */
    void rotate(const Vector3& r);
    /**
    * @brief Rota el objeto sobre los ejes tanto como el vector dado indica
    * @param r rotacion que se quiere aplicar sobre el transform
    */
    void rotate(const Quaternion& r);

    /**
    * @brief Devuelve el eje X de coordenadas locales en coordenadas globales, inicialmente su eje X es 1,0,0
    * @return Eje X de coordenadas locales en coordenadas globales
    */
    Vector3 right() const;
    /**
    * @brief Devuelve el eje Y de coordenadas locales en coordenadas globales, inicialmente su eje Y es 0,1,0
    * @return Eje Y de coordenadas locales en coordenadas globales
    */
    Vector3 up() const;
    /**
    * @brief Devuelve el eje Z de coordenadas locales en coordenadas globales, inicialmente su eje Z es 0,0,1
    * @return Eje Z de coordenadas locales en coordenadas globales
    */
    Vector3 forward() const;

    /**
    * @brief Establece el transform dado como el padre de este
    * @param transform Transform al que se quiere hacer padre de este
    */
    void setParent(Transform* const transform);
    /**
    * @brief Devuelve el padre de este transform
    * @return Padre de este transform (nullptr si no tiene)
    */
    Transform* getParent() const;

    /**
    * @brief Devuelve los hijos directos de este transform
    * @return Vector que contiene los punteros los hijos directos de este transform
    */
    std::vector<Transform*> getChildren() const;
    /**
    * @brief Devuelve tanto los hijos directos como los indirectos de este transform
    * @return Vector que contiene los punteros de tanto los hijos directos como los indirectos de este transform
    */
    std::vector<Transform*> getAllChildren() const;
};
}
