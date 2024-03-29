#include "Skyplane.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "RenderNode.h"
#include "checkML.h"

namespace Tapioca {

Skyplane::Skyplane(Ogre::SceneManager* const scnMgr, RenderNode* const node, Ogre::MeshManager* const mshMgr,
                   std::string const& materialName, const bool enable, const Vector3 rkNormal, const float fConstant,
                   const float scale, const float tiling, const bool drawFirst, const float bow, const int xsegments,
                   const int ysegments)
    : RenderObject(node, scnMgr), scnM(scnMgr), material(materialName), rkNormal(rkNormal), fConstant(fConstant),
      scale(scale), tiling(tiling), drawFirst(drawFirst), bow(bow), xSegments(xsegments), ySegments(ysegments) {

    scnM = scnMgr;

    if (enable) {
        string meshName = "SkyPlane";

        mSkyPlaneAux = Ogre::Plane(Ogre::Vector3(rkNormal.x, rkNormal.y, rkNormal.z), Ogre::Real(0));

        Ogre::MaterialPtr m = Ogre::MaterialManager::getSingleton().getByName(
            materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        if (!m) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Sky plane material '" + materialName + "' not found.",
                        "SceneManager::setSkyPlane");
        }
        // Make sure the material doesn't update the depth buffer
        m->setDepthWriteEnabled(false);
        // Ensure loaded
        m->load();

        // Set up the plane
        Ogre::MeshPtr planeMesh = Ogre::MeshManager::getSingleton().getByName(
            meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        if (planeMesh) {
            // Destroy the old one
            Ogre::MeshManager::getSingleton().remove(planeMesh);
        }

        // Create up vector
        Ogre::Vector3 up = mSkyPlaneAux.normal.crossProduct(Ogre::Vector3::UNIT_X);
        if (up == Ogre::Vector3::ZERO) up = mSkyPlaneAux.normal.crossProduct(-Ogre::Vector3::UNIT_Z);

        // Create skyplane
        if (bow > 0) {
            // Build a curved skyplane
            planeMesh = Ogre::MeshManager::getSingleton().createCurvedPlane(
                meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSkyPlaneAux, scale * 100,
                scale * 100, scale * bow * 100, xsegments, ysegments, false, 1, tiling, tiling, up);
        }
        else {
            planeMesh = Ogre::MeshManager::getSingleton().createPlane(
                meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSkyPlaneAux, scale * 100,
                scale * 100, xsegments, ysegments, false, 1, tiling, tiling, up);
        }

        // Create entity
        if (mSkyPlane) {
            // destroy old one, do it by name for speed
            scnM->destroyEntity(meshName);
            mSkyPlane = 0;
        }

        mSkyPlane = scnMgr->createEntity(meshName);

        init(mSkyPlane);
    }
}

void Skyplane::setEnable(const bool enable) { scnM->setSkyPlaneEnabled(enable); }

bool Skyplane::isEnabled() { return scnM->isSkyPlaneEnabled(); }
}
