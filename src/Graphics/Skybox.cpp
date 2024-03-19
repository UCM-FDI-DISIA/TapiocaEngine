#include "Skybox.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "checkML.h"

namespace Tapioca {

Skybox::Skybox(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& texture, const float distC,
               const bool orderC)
    : RenderObject(node, scnMgr), texture(texture), distC(distC), orderC(orderC) {
    scnM = scnMgr;
    scnM->setSkyBox(true, texture, distC, orderC);
}

void Skybox::setEnable(const bool enable) { scnM->setSkyBoxEnabled(false); }

bool Skybox::isEnabled() { return scnM->isSkyBoxEnabled(); }
}
