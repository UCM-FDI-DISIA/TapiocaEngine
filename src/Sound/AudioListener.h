#pragma once
#include "defs.h"
#include "../Core/Utilities/Vector3.h"
namespace Tapioca {
class SoundEngine;
class TAPIOCA_API AudioListener {
//const vec3df &pos, const vec3df &lookdir, const vec3df &velPerSecond=vec3df(0, 0, 0), const vec3df &upVector=vec3df(0, 1, 0))=0
private:
friend SoundEngine;
Vector3 position;
Vector3 look;
Vector3 velocity;//en metros por segundo
Vector3 up;
	
public:
/*
* @brief Crea un listener en @param pos que mira en direccion @param ldir y se mueve a velocidad @param vel
*/
AudioListener(Vector3 pos , Vector3 ldir, Vector3 vel , Vector3 upvec);
    void setPosition(Vector3 p) { position = p; };
	void setDirection(Vector3 d) { look = d; };
    void setVelocity(Vector3 v) { velocity = v; };
        void setUp(Vector3 u) { up = u; };
};
}
