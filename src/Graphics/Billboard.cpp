#include "Billboard.h"
#include <Ogre.h>
#include "Node.h"

Tapioca::Billboard::Billboard(Ogre::SceneManager* scnMgr, Node* node)
    : RenderObject(node, scnMgr), mBillboard(new Ogre::Billboard()) { }