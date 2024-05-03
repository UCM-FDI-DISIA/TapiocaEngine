#include "Sound.h"
#include "SoundManager.h"
#include <irrKlang.h>

namespace Tapioca {
Sound::Sound(std::string sourceFile) : source(nullptr), sourceFile(sourceFile), length(0) {
    soundManager = Tapioca::SoundManager::instance();
    if (soundManager != nullptr) {
        source = soundManager->soundEngine->addSoundSourceFromFile(sourceFile.c_str(), irrklang::ESM_AUTO_DETECT, true);

        // Si el sonido ya se ha cargado
        if (source == nullptr)
            source = Tapioca::SoundManager::instance()->soundEngine->getSoundSource(sourceFile.c_str());
        length = source->getPlayLength();
    }
}
}
