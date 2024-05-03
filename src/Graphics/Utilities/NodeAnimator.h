#pragma once
#include "defs.h"
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"
#include <vector>

namespace Ogre {
class AnimationState;
class SceneManager;
}

namespace Tapioca {
class GraphicsManager;
class RenderNode;

struct KeyFrame {
    Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
    Quaternion rot = Quaternion(Vector3(0.0f, 0.0f, 1.0f));
};

/*
* Clase que se encarga de animar un nodo de la escena
*/
class TAPIOCA_API NodeAnimator {
private:
    friend GraphicsManager;

    Ogre::SceneManager* sceneManager;       // Referencia al SceneManager
    RenderNode* node;                       // Referencia al nodo que se va a animar
    Ogre::AnimationState* animationState;   // Referencia al estado de la animacion

    float duration;                    // Duracion de la animacion en segundos
    std::vector<KeyFrame> keyFrames;   // Lista de keyframes
    bool enabled;                      // Indica si la animacion esta activa
    bool loop;                         // Indica si la animacion se repite
    std::string name;                  // Nombre de la animacion

    /*
    * @brief Constructor de la clase NodeAnimator
    * @param scnMngr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param duration Duracion de la animacion
    * @param name Nombre de la animacion
    */
    NodeAnimator(Ogre::SceneManager* const scnMngr, RenderNode* const node, float duration, std::string name);

public:
    /*
    * @brief Inicializa la animacion. Crea los keyframes y la animacion en ogre. 
    * Establece si esta activado y si se quiere repetir la animacion
    */
    void init();

    /*
    * @brief Actualiza la animacion
    * @param delt Tiempo transcurrido desde el ultimo frame
    * @param speed Velocidad de la animacion
    */
    void updateAnim(const uint64_t delt, const float speed);

    /*
    * @brief Reproduce la animacion
    * @param loop Indica si la animacion se repite
    */
    void play(const bool loop);

    /*
    * @brief Establece si la animacion esta activa
    * @param enabled Indica si la animacion esta activa
    */
    void enable(const bool enabled);

    /*
    * @brief Establece si la animacion se repite
    * @param loop Indica si la animacion se repite
    */
    void setLoop(const bool loop);

    /*
    * @brief Devuelve si la animacion se repite
    * @return true si la animacion se repite, false en caso contrario
    */
    inline bool isLooping() const { return loop; }

    /*
    * @brief Devuelve si la animacion esta activa
    * @return true si la animacion esta activa, false en caso contrario
    */
    inline bool isEnabled() const { return enabled; }

    /*
    * @brief Devuelve si la animacion ha terminado
    * @return true si la animacion ha terminado, false en caso contrario
    */
    bool hasEnded() const;

    /*
    * @brief Anade un keyframe a la animacion y lo devuelve
    * @return Puntero al keyframe anadido
    */
    KeyFrame* addKeyFrame();
    /*
    * @brief Anade un keyframe a la animacion con la posicion, escala y rotacion dadas
    * @param pos Posicion del keyframe
    * @param scale Escala del keyframe
    * @param rot Rotacion del keyframe (en grados)
    */
    void addKeyFramePosScaleRot(Vector3 pos, Vector3 scale, Vector3 rot);
    /*
    * @brief Anade un keyframe a la animacion con la posicion, escala y rotacion dadas
    * @param pos Posicion del keyframe
    * @param scale Escala del keyframe
    * @param rot Rotacion del keyframe (en quaternion)
    */
    void addKeyFramePosScaleRot(Vector3 pos, Vector3 scale, Quaternion rot);
    /*
    * @brief Anade un keyframe a la animacion con la posicion y escala dadas
    * @param pos Posicion del keyframe
    * @param scale Escala del keyframe
    */
    void addKeyFramePosScale(Vector3 pos, Vector3 scale);
    /*
    * @brief Anade un keyframe a la animacion con la posicion y rotacion dadas
    * @param pos Posicion del keyframe
    * @param rot Rotacion del keyframe (en grados)
    */
    void addKeyFramePosRot(Vector3 pos, Vector3 rot);
    /*
    * @brief Anade un keyframe a la animacion con la posicion y rotacion dadas
    * @param pos Posicion del keyframe
    * @param rot Rotacion del keyframe (en quaternion)
    */
    void addKeyFramePosRot(Vector3 pos, Quaternion rot);
    /*
    * @brief Anade un keyframe a la animacion con la escala y rotacion dadas
    * @param scale Escala del keyframe
    * @param rot Rotacion del keyframe (en grados)
    */
    void addKeyFrameScaleRot(Vector3 scale, Vector3 rot);
    /*
    * @brief Anade un keyframe a la animacion con la escala y rotacion dadas
    * @param scale Escala del keyframe
    * @param rot Rotacion del keyframe (en quaternion)
    */
    void addKeyFrameScaleRot(Vector3 scale, Quaternion rot);
    /*
    * @brief Anade un keyframe a la animacion con la posicion dada
    * @param pos Posicion del keyframe
    */
    void addKeyFramePos(Vector3 pos);
    /*
    * @brief Anade un keyframe a la animacion con la escala dada
    * @param scale Escala del keyframe
    */
    void addKeyFrameScale(Vector3 scale);
    /*
    * @brief Anade un keyframe a la animacion con la rotacion dada
    * @param rot Rotacion del keyframe (en grados)
    */
    void addKeyFrameRot(Vector3 rot);
    /*
    * @brief Anade un keyframe a la animacion con la rotacion dada
    * @param rot Rotacion del keyframe (en quaternion)
    */
    void addKeyFrameRot(Quaternion rot);
};
}
