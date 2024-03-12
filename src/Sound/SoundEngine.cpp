#include "SoundEngine.h"
#include <iostream>

Tapioca::SoundEngine::SoundEngine() { }

bool Tapioca::SoundEngine::init() {
    soundEngine = irrklang::createIrrKlangDevice();
    if (!soundEngine) {
        std::cout << "error al inciar el motor de audio";
        return false;
    }
    return true;
}

Tapioca::SoundEngine::~SoundEngine() {
    if (soundEngine != nullptr) soundEngine->drop();
}

void Tapioca::SoundEngine::testsample() {
    irrklang::vec3df position(0, 0, 0);
    //por defecto se parte de la ruta de la solucion
    irrklang::ISound* snd = soundEngine->play3D("../bin/assets/audio/BadBlood.mp3", position, true,
                                                true);   //tienes que crearlo pausado si luego quieres acceder al el
    snd->setIsPaused(false);
    //parameros para el listener
    irrklang::vec3df lookDirection(10, 0, 10);   // the direction the listener looks into
    irrklang::vec3df velPerSecond(0, 0, 0);      // only relevant for doppler effects ojoooo que te calcula el doppler
    irrklang::vec3df upVector(0, 1, 0);
    soundEngine->setListenerPosition(position, lookDirection, velPerSecond, upVector);
}
