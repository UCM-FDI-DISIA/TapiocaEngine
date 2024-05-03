#pragma once
#include <string>
#include "defs.h"

namespace Ogre {
class AnimationState;
class AnimationStateSet;
}

namespace Tapioca {
class RenderNode;
class Mesh;

/*
* @brief Clase que gestiona las animaciones de un objeto
*/
class TAPIOCA_API AnimationHelper {
private:
    Ogre::AnimationState* animState;         // Animacion actual
    Ogre::AnimationStateSet* animStateSet;   // Conjunto de animaciones
    bool playing;                            // Indica si la animacion esta en reproduccion
    bool looping;                            // Indica si la animacion esta en bucle

public:
    /*
    * @brief Inicializa los valores de la animacion
    * @param object Objeto al que se le aplicara la animacion
    * @param autoPlay Indica si la animacion se reproduce automaticamente
    * @param loop Indica si la animacion esta en bucle
    */
    AnimationHelper(Mesh* const object, const bool autoPlay, const bool loop);

    /*
    * @brief Destructor por defecto
    */
    ~AnimationHelper();

    /*
    * @brief Actualiza la animacion
    * @param delt Tiempo transcurrido desde el ultimo frame
    * @param speed Velocidad de la animacion
    */
    void updateAnim(const uint64_t delt, const float speed);

    /*
    * @brief Establece la animacion a reproducir
    * @param anim Nombre de la animacion
    */
    void playAnim(std::string const& anim);

    /*
    * @brief Establece si la animacion esta en bucle
    * @param l True si la animacion esta en bucle, false en caso contrario
    */
    void setLoop(const bool l);

    /*
    * @brief Establece si la animacion esta en reproduccion
    * @param p True si la animacion esta en reproduccion, false en caso contrario
    */
    void setPlaying(const bool p);

    /*
    * @brief Devuelve si la animacion esta en bucle
    * @return True si la animacion esta en bucle, false en caso contrario
    */
    inline bool getLoop() const { return looping; }

    /*
    * @brief Devuelve si la animacion esta en reproduccion
    * @return True si la animacion esta en reproduccion, false en caso contrario
    */
    inline bool getPlaying() const { return playing; }
};
}
