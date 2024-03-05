#include "Skyplane.h"
#include <Ogre.h>


namespace Tapioca {

Skyplane::Skyplane(Ogre::SceneManager* scnMgr, Node* node, std::string const& materialName, bool enable,
    Vector3 rkNormal, float fConstant, float scale, float tiling, bool drawFirst, float bow,
    int xsegments, int ysegments)
    : RenderObject(node, scnMgr), scnM(scnMgr), material(materialName), rkNormal(rkNormal), fConstant(fConstant), 
    scale(scale), tiling(tiling), drawFirst(drawFirst), bow(bow), xSegments(xsegments), ySegments(ysegments) 
{
    scnM = scnMgr;
    scnM->setSkyPlane(enable, Ogre::Plane({rkNormal.x, rkNormal.y, rkNormal.z}, fConstant), material, scale, tiling,
                      drawFirst, bow, xsegments, ysegments);
}

void Skyplane::setEnable(bool enable) { scnM->setSkyPlaneEnabled(enable); }

bool Skyplane::isEnable() { return scnM->isSkyPlaneEnabled(); }
}


