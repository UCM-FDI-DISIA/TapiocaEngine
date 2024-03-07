#include "Skybox.h"
#include <Ogre.h>

namespace Tapioca {

Skybox::Skybox(Ogre::SceneManager* const scnMgr, std::string const& texture, const float distC, const bool orderC)
    : texture(texture), distC(distC), orderC(orderC) {
    scnM = scnMgr;
    scnMgr->setSkyBox(true, texture, distC, orderC);
}

void Skybox::setEnable(const bool enable) { scnM->setSkyBoxEnabled(false); }

bool Skybox::isEnabled() { return scnM->isSkyBoxEnabled(); }
}
