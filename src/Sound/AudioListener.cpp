#include "AudioListener.h"
#include "checkML.h"

namespace Tapioca {
AudioListener::AudioListener(Vector3 pos, Vector3 dir, Vector3 vel, Vector3 up)
    : position(pos), look(dir), velocity(vel), up(up) { }
}