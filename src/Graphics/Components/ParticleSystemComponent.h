#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class RenderNode;
class Transform;
class ParticleSystem;

class TAPIOCA_API ParticleSystemComponent : public Component {
private:
    RenderNode* node;
    Transform* transform;
    ParticleSystem* pSys;

       // warning C4251 'Tapioca::ParticleSystemComponent::pSysName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::ParticleSystemComponent' Graphics
    // warning C4251 'Tapioca::ParticleSystemComponent::templateName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::ParticleSystemComponent'
//#ifdef _MSC_VER
//#pragma warning(disable : 4251)
//#endif
    std::string pSysName;
    std::string templateName;
//#ifdef _MSC_VER
//#pragma warning(default : 4251)
//#endif
    bool emitting;
    Vector3 initialRotation;

public:
    COMPONENT_ID("ParticleSystemComponent")

    /*
    * @brief Constructora de la clase ParticleSystemComponent
    */
    ParticleSystemComponent();
    /*
    * @brief Destructora de la clase ParticleSystemComponent
    */
    ~ParticleSystemComponent();

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * Garantiza que todos los componentes iniciales esten creados
    */
    void awake() override;

    /*
    * @brief Metodo que se usa para recibir eventos.
    * Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent.
    * @param Id indica el tipo de mensaje
    * @param info puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;


    /*
    * @brief Comienza o para el sistema de particulas
    * @param emitting True para que emita particulas, false para que no emita
    */
    void setEmitting(const bool emitting);
    /*
    * @brief Devuelve si el sistema de particulas esta emitiendo o no
    * @return True si emite particulas, false en caso contrario
    */
    bool isEmitting();

    /*
    * @brief Cambia el numero maximo de particulas que puede haber en el sistema
    * @param q Quota nueva del sistem a de particulas
    */
    void setQuota(const int q);
    /*
    * @brief Devuelve el numero maximo de particulas que puede haber en el sistema
    * @return Quota actual del sistema de particulas
    */
    int getQuota();

    /*
    * @brief Hace que el sistema de particulas sea o no visible
    * @param v True para que sea visible, false para que no lo sea
    */
    void setVisible(const bool v);
    /*
    * @brief Devuelve si el sistema de particulas es visible o no
    * @return True si es visible, false en caso contrario
    */
    bool isVisible();

    /* 
    * @brief Acelera el sistema de particulas. Puede usarse para hacer que un sistema de particulas
    * que tarda cierto tiempo en comenzar a funcionar completamente lo haga al momento.
    * @param time Tiempo en segundos que se adelantan
    * @param interval Muestreo para generar las particulas (cuanto menor sea, mas realista, pero tambien mas costoso)
    */
    void fastForward(const float time, const float interval);

};


}