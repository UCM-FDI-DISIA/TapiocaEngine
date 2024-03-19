#include "Skyplane.h"
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

Skyplane::Skyplane(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& materialName,
                   const bool enable,
                   const Vector3 rkNormal, const float fConstant, const float scale, const float tiling,
                   const bool drawFirst, const float bow, const int xsegments, const int ysegments)
    : RenderObject(node, scnMgr), scnM(scnMgr), material(materialName), rkNormal(rkNormal), fConstant(fConstant), scale(scale), tiling(tiling),
      drawFirst(drawFirst), bow(bow), xSegments(xsegments), ySegments(ysegments) {

    scnM = scnMgr;
    scnM->setSkyPlane(enable, Ogre::Plane({rkNormal.x, rkNormal.y, rkNormal.z}, fConstant), material, scale, tiling,
                      drawFirst, bow, xsegments, ysegments);
}

void Skyplane::setEnable(const bool enable) { scnM->setSkyPlaneEnabled(enable); }

bool Skyplane::isEnabled() { return scnM->isSkyPlaneEnabled(); }
}
