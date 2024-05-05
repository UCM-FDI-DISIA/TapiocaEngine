#include "SoundManager.h"
#include <irrKlang.h>
#include "AudioListener.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<SoundManager>;
template<>
SoundManager* Singleton<SoundManager>::instance_ = nullptr;

SoundManager::SoundManager() : al(nullptr), soundEngine(nullptr) { }

SoundManager::~SoundManager() {
    if (al != nullptr) delete al;
    al = nullptr;
    if (soundEngine != nullptr) soundEngine->drop();
    soundEngine = nullptr;
}

bool SoundManager::init() {
    soundEngine = irrklang::createIrrKlangDevice();
    if (soundEngine == nullptr) {
        logError("SoundManager: Error al inciar el motor de audio, no se encuentra dispositivo de salida.");
        return false;
    }
    return true;
}

void SoundManager::setListener(AudioListener al) {
    soundEngine->setListenerPosition(irrklang::vec3df(al.position.x, al.position.y, al.position.z),
                                     irrklang::vec3df(al.look.x, al.look.y, al.look.z),
                                     irrklang::vec3df(al.velocity.x, al.velocity.y, al.velocity.z),
                                     irrklang::vec3df(al.up.x, al.up.y, al.up.z));
}
}
