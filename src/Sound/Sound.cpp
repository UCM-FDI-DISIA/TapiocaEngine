#include "Sound.h"
#include "SoundManager.h"
#include <irrKlang.h>

namespace Tapioca {
Sound::Sound(std::string source) {


    sourceFile = source;

    //is3D = load3D; ira al audio source

    //irrKlang se asegura de solo cargar los arvhivos con un mismo nombre 1 sola vez

    mysource = Tapioca::SoundManager::instance()->soundEngine->addSoundSourceFromFile(source.c_str(),
      
                                                                                irrklang::ESM_AUTO_DETECT, true);
    
    //si el sonido ya se ha cargado
    if (mysource == nullptr) {
        mysource = Tapioca::SoundManager::instance()->soundEngine->getSoundSource(source.c_str());
    }
    length = mysource->getPlayLength();
}
}
