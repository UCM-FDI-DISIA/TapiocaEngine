#ifndef __Sample_PBR_H__
#define __Sample_PBR_H__

#include "SdkSample.h"
#include "SamplePlugin.h"

using namespace Ogre;
using namespace OgreBites;

class _OgreSampleClassExport Sample_PBR : public SdkSample
{
public:

    Sample_PBR()
    {
        mInfo["Title"] = "Physically Based Rendering";
        mInfo["Description"] = "Shows how to do physically based rendering using a custom material";
        mInfo["Thumbnail"] = "thumb_pbr.png";
        mInfo["Category"] = "Lighting";
    }

protected:
    GpuProgramParametersSharedPtr mParams;
    Entity* mEntity;

    void setupControls()
    {
        int gui_width = 250;

        SelectMenu* objectType = mTrayMgr->createThickSelectMenu(TL_TOPLEFT, "term", "Material", gui_width, 9);
        objectType->addItem("glTF2 Shader");
        objectType->addItem("RTSS");
        if(GpuProgramManager::getSingleton().isSyntaxSupported("glsl330"))
            objectType->addItem("Filament Shader");
        objectType->selectItem(0, false);

        mTrayMgr->createCheckBox(TL_TOPLEFT, "ibl", "Image Based Lighting", gui_width)->setChecked(true, false);
    }

    void setupContent() override
    {
        // Make this viewport work with shader generator
        mViewport->setMaterialScheme(MSN_SHADERGEN);
        setupControls();

        mCamera->setNearClipDistance(0.1);
        mCameraMan->setStyle(CS_ORBIT);
        mCameraMan->setYawPitchDist(Degree(-30), Degree(30), 3);
        mTrayMgr->showCursor();

        Light* light = mSceneMgr->createLight();
        light->setDiffuseColour(ColourValue::White);
        light->setSpecularColour(ColourValue::White);

        mSceneMgr->getRootSceneNode()
            ->createChildSceneNode(Vector3(4, 1, 6))
            ->attachObject(light);

        mEntity = mSceneMgr->createEntity("DamagedHelmet.mesh");

        unsigned short src;
        if (!mEntity->getMesh()->suggestTangentVectorBuildParams(src))
        {
            // ensure that we have tangent vectors
            mEntity->getMesh()->buildTangentVectors(src);
        }

        SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(mEntity);

        mViewport->setBackgroundColour(ColourValue(0.05, 0.05, 0.05));

        MaterialPtr mat = MaterialManager::getSingleton().getByName("DamagedHelmet");
        mParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
    }

    void itemSelected(SelectMenu* menu) override
    {
        static const char* materials[] = {"DamagedHelmet", "DamagedHelmet_RTSS", "DamagedHelmet_Filament"};
        int n = menu->getSelectionIndex();

        mEntity->setMaterialName(materials[n]);
    }
        
    void checkBoxToggled(CheckBox* box) override
    {
        bool checked = box->isChecked();
        mParams->setNamedConstant("u_ScaleIBLAmbient", Vector4(float(checked)));

        using namespace RTShader;
        MaterialPtr mat = MaterialManager::getSingleton().getByName("DamagedHelmet_RTSS");
        const auto& renderstate = any_cast<TargetRenderStatePtr>(
            mat->getTechnique(1)->getPass(0)->getUserObjectBindings().getUserAny(TargetRenderState::UserKey));
        renderstate->getSubRenderState(SRS_IMAGE_BASED_LIGHTING)->setParameter("luminance", checked ? "4" : "0");
    }
};

#endif
