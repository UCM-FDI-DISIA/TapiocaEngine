#include "SoundManager.h"
#include "irrKlang.h"

// TEMPORAL
#include "Sound.h"
#include "AudioSource.h"
#include "AudioListener.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<SoundManager>;
template<>
SoundManager* Singleton<SoundManager>::instance_ = nullptr;

SoundManager::SoundManager() : al(nullptr), soundEngine(nullptr), snd(nullptr), au(nullptr) { }

void SoundManager::setListener(AudioListener al) {
    soundEngine->setListenerPosition(irrklang::vec3df(al.position.x, al.position.y, al.position.z),
                                     irrklang::vec3df(al.look.x, al.look.y, al.look.z),
                                     irrklang::vec3df(al.velocity.x, al.velocity.y, al.velocity.z),
                                     irrklang::vec3df(al.up.x, al.up.y, al.up.z));
}

SoundManager::~SoundManager() {
    if (al != nullptr) delete al;
    al = nullptr;
    if (soundEngine != nullptr) soundEngine->drop();
    soundEngine = nullptr;
    // TEMPORAL
    if (snd != nullptr) delete snd;
    snd = nullptr;
    if (au != nullptr) delete au;
    au = nullptr;
}

bool SoundManager::init() {
    soundEngine = irrklang::createIrrKlangDevice();
    if (soundEngine == nullptr) {
        logError("SoundManager: Error al inciar el motor de audio, no se encuentra dispositivo de salida.");
        return false;
    }
    return true;
}

void SoundManager::testsample() {
    // irrklang::vec3df position(0, 0, 0);
    // //por defecto se parte de la ruta de la solucion
    // //irrklang::ISound* snd = SoundManager->play3D("../bin/assets/audio/BadBlood.mp3", position, true,true);   //tienes que crearlo pausado si luego quieres acceder al el
    //  snd = new Sound("../bin/assets/audio/BadBlood.mp3 ");
    //// snd->setIsPaused(false);
    //// SoundManager->play2D(snd->mysource);
    //  au = new AudioSource(*snd, Vector3(1,1,1), false, true);
    // //parameros para el listener
    // irrklang::vec3df lookDirection(10, 0, 10);   // the direction the listener looks into
    // irrklang::vec3df velPerSecond(0, 0, 0);      // only relevant for doppler effects ojoooo que te calcula el doppler
    // irrklang::vec3df upVector(0, 1, 0);
    // soundEngine->setListenerPosition(position, lookDirection, velPerSecond, upVector);
    // au->setSpeed(1.2f);
    // al = new AudioListener(Vector3(10, 0, 0), Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
    // setListener(*al);
    //au->setPan(0);
    //un source cargado una vez se puede reproducir simultaneamente en varios sound distintos
    /** int i = 0;
    
    while (i<100023203) {
        i++;
    }
    SoundManager->play2D(snd->mysource);
    */
}
}
