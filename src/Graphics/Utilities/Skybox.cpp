#include "Skybox.h"
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

Skybox::Skybox(Ogre::SceneManager* const scnMngr, RenderNode* const node, std::string const& material,
               std::string const& skyboxName, const float distC, const bool orderC)
    : RenderObject(node, scnMngr), sceneManager(scnMngr), material(material), distC(distC), orderC(orderC) {
    Ogre::MaterialPtr m = Ogre::MaterialManager::getSingleton().getByName(
        material, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (!m)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Sky box material '" + material + "' not found.",
                    "SceneManager::setSkyBox");
    m->load();

    bool valid = m->getBestTechnique() && m->getBestTechnique()->getNumPasses();
    if (valid) {
        Ogre::Pass* pass = m->getBestTechnique()->getPass(0);
        valid = valid && pass->getNumTextureUnitStates() &&
            pass->getTextureUnitState(0)->getTextureType() == Ogre::TEX_TYPE_CUBE_MAP;
    }

    if (!valid) {
        Ogre::LogManager::getSingleton().logWarning("skybox material " + material + " is not supported, defaulting");
        m = Ogre::MaterialManager::getSingleton().getDefaultSettings();
    }

    if (!mSkyBoxObj) {
        mSkyBoxObj = std::make_unique<Ogre::ManualObject>(skyboxName);
        mSkyBoxObj->setCastShadows(false);
    }
    else {
        if (!mSkyBoxObj->isAttached()) {
        }
        mSkyBoxObj->clear();
    }

    mSkyBoxObj->setRenderQueueGroup(0);
    mSkyBoxObj->begin(material, Ogre::RenderOperation::OT_TRIANGLE_STRIP,
                      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    const Ogre::Vector3 cube_strip[14] = {
        {-1.f, 1.f, 1.f},     // Front-top-left
        {1.f, 1.f, 1.f},      // Front-top-right
        {-1.f, -1.f, 1.f},    // Front-bottom-left
        {1.f, -1.f, 1.f},     // Front-bottom-right
        {1.f, -1.f, -1.f},    // Back-bottom-right
        {1.f, 1.f, 1.f},      // Front-top-right
        {1.f, 1.f, -1.f},     // Back-top-right
        {-1.f, 1.f, 1.f},     // Front-top-left
        {-1.f, 1.f, -1.f},    // Back-top-left
        {-1.f, -1.f, 1.f},    // Front-bottom-left
        {-1.f, -1.f, -1.f},   // Back-bottom-left
        {1.f, -1.f, -1.f},    // Back-bottom-right
        {-1.f, 1.f, -1.f},    // Back-top-left
        {1.f, 1.f, -1.f}      // Back-top-right
    };

    for (const auto& vtx : cube_strip) {
        mSkyBoxObj->position(Ogre::Vector3(0, 0, 1) * (vtx * distC));
        mSkyBoxObj->textureCoord(vtx.normalisedCopy() * Ogre::Vector3(1, 1, -1));
    }

    init(mSkyBoxObj.get());

    mSkyBoxObj->end();
}

void Skybox::setEnable(const bool enable) { sceneManager->setSkyBoxEnabled(false); }

bool Skybox::isEnabled() { return sceneManager->isSkyBoxEnabled(); }
}
