#include "Skybox.h"
#include <Ogre.h>

namespace Tapioca {

Skybox::Skybox(Ogre::SceneManager* scnMgr, Node* node, std::string const& texture)
    : RenderObject(node, scnMgr), texture(texture)
{
    scnM = scnMgr;
    scnMgr->setSkyBox(true, texture);
}

Skybox::Skybox(Ogre::SceneManager* scnMgr, Node* node, std::string const& texture, int distC)
    : RenderObject(node, scnMgr), texture(texture) , distC(distC) {
    scnM = scnMgr;
    scnMgr->setSkyBox(true, texture, distC);
}

Skybox::Skybox(Ogre::SceneManager* scnMgr, Node* node, std::string const& texture, int distC, bool orderC)
    : RenderObject(node, scnMgr), texture(texture), distC(distC), orderC(orderC) 
{
    scnM = scnMgr;
    scnMgr->setSkyBox(true, texture, distC, orderC);
}

void Tapioca::Skybox::setVisible(bool visible) { scnM->setSkyBox(false, texture, distC, orderC); }
}
