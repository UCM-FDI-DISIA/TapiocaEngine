#include "SoundManager.h"
#include <filesystem>
#include <irrKlang.h>
#include "AudioListener.h"
#include "checkML.h"

namespace Tapioca {
SoundManager::SoundManager() : al(nullptr), soundEngine(nullptr), audioFolderPath("assets/audio/") { }

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
    createAudioFolder();
    return true;
}

void SoundManager::setListener(AudioListener al) {
    soundEngine->setListenerPosition(irrklang::vec3df(al.position.x, al.position.y, al.position.z),
                                     irrklang::vec3df(al.look.x, al.look.y, al.look.z),
                                     irrklang::vec3df(al.velocity.x, al.velocity.y, al.velocity.z),
                                     irrklang::vec3df(al.up.x, al.up.y, al.up.z));
}
bool SoundManager::checkAudioFolder() const {
    if (!std::filesystem::exists(audioFolderPath)) {
        logWarn(("SoundManager: la carpeta " + audioFolderPath + " no existe").c_str());
        return false;
    }
    else return true;
}
void SoundManager::createAudioFolder() const {
    if (!checkAudioFolder()) {
        try {
            if (std::filesystem::create_directory(audioFolderPath)) {
                logInfo(("SoundManager: Carpeta de audio " + audioFolderPath + " creada correctamente").c_str());
            }

        } catch (const std::filesystem::filesystem_error& e) {
            logError(("SoundManager: No se pudo crear la carpeta de sonidos" + std::string(e.what())).c_str());
        }
    }
}
std::string SoundManager::getAudioPath() { return audioFolderPath; }
}
