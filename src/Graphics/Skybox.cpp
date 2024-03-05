#include "Skybox.h"
#include <Ogre.h>

namespace Tapioca {

Skybox::Skybox(Ogre::SceneManager* scnMgr, Node* node, std::string const& texture, float distC, bool orderC)
    : RenderObject(node, scnMgr), texture(texture), distC(distC), orderC(orderC) 
{
    scnM = scnMgr;
    scnMgr->setSkyBox(true, texture, distC, orderC);
}

void Skybox::setEnable(bool enable) { scnM->setSkyBoxEnabled(false); }

bool Skybox::isEnable() { return scnM->isSkyBoxEnabled(); }
}
