#include "Sound.h"
#include "SoundManager.h"
#include <irrKlang.h>

Tapioca::Sound::Sound(std::string source) { 
	
	
		sourceFile = source;
       
        //is3D = load3D; ira al audio source

		//irrKlang se asegura de solo cargar los arvhivos con un mismo nombre 1 sola vez
		mysource = Tapioca::SoundManager::instance()->soundEngine->addSoundSourceFromFile(source.c_str(), irrklang::ESM_AUTO_DETECT, true);
        length= mysource->getPlayLength();
    
	}
