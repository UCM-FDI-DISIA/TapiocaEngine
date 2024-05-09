#include "Sound.h"
#include "SoundManager.h"
#include <irrKlang.h>
#include "checkML.h"

namespace Tapioca {
Sound::Sound(std::string sourceFile) : source(nullptr), sourceFile(sourceFile), length(0) {
    soundManager = SoundManager::instance();
    if (soundManager != nullptr) {
        source = soundManager->soundEngine->addSoundSourceFromFile(
            (soundManager->getAudioPath()+sourceFile).c_str(), irrklang::ESM_AUTO_DETECT, true);

        // Si el sonido ya se ha cargado
        if (source == nullptr) {
            std::string src = soundManager->getAudioPath() + sourceFile;
            source = soundManager->soundEngine->getSoundSource(src.c_str());
        }
        length = source->getPlayLength();
    }
}
}
