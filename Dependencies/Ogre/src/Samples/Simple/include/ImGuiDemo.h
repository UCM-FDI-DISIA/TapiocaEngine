#pragma once

#include "SdkSample.h"
#include "OgreImGuiOverlay.h"
#include <OgreImGuiInputListener.h>

using namespace Ogre;
using namespace OgreBites;

class _OgreSampleClassExport Sample_ImGui : public SdkSample, public RenderTargetListener
{
    std::unique_ptr<ImGuiInputListener> mImguiListener;
    InputListenerChain mListenerChain;
public:
    // Basic constructor
    Sample_ImGui()
    {
        mInfo["Title"] = "Dear ImGui integration";
        mInfo["Description"] = "Overlay ImGui interactions";
        mInfo["Category"] = "Unsorted";
        mInfo["Thumbnail"] = "thumb_imgui.png";
    }

    void preViewportUpdate(const RenderTargetViewportEvent& evt) override
    {
        if(!evt.source->getOverlaysEnabled()) return;
        if(!mTrayMgr->getTraysLayer()->isVisible()) return;

        ImGuiOverlay::NewFrame();

        ImGui::ShowDemoWindow();
    }

    bool keyPressed(const KeyboardEvent& evt) override { return mListenerChain.keyPressed(evt); }
    bool keyReleased(const KeyboardEvent& evt) override { return mListenerChain.keyReleased(evt); }
    bool mouseMoved(const MouseMotionEvent& evt) override { return mListenerChain.mouseMoved(evt); }
    bool mouseWheelRolled(const MouseWheelEvent& evt) override { return mListenerChain.mouseWheelRolled(evt); }
    bool mousePressed(const MouseButtonEvent& evt) override { return mListenerChain.mousePressed(evt); }
    bool mouseReleased(const MouseButtonEvent& evt) override { return mListenerChain.mouseReleased(evt); }
    bool textInput (const TextInputEvent& evt) override { return mListenerChain.textInput (evt); }

    void setupContent(void) override
    {
        auto imguiOverlay = new ImGuiOverlay();

        // handle DPI scaling
        float vpScale = OverlayManager::getSingleton().getPixelRatio();
        ImGui::GetIO().FontGlobalScale = std::round(vpScale); // default font does not work with fractional scaling
        ImGui::GetStyle().ScaleAllSizes(vpScale);

        imguiOverlay->setZOrder(300);
        imguiOverlay->show();
        OverlayManager::getSingleton().addOverlay(imguiOverlay); // now owned by overlaymgr

        /*
            NOTE:
            Custom apps will ASSERT on ImGuiOverlay::NewFrame() and not display any UI if they
            have not registered the overlay system by calling mSceneMgr->addRenderQueueListener(mOverlaySystem).
            OgreBites::SampleBrowser does this on behalf of the ImGuiDemo but custom applications will need to
            call this themselves.  See ApplicationContextBase::createDummyScene().
        */
        mWindow->addListener(this);

        mImguiListener.reset(new ImGuiInputListener());
        mListenerChain = InputListenerChain({mTrayMgr.get(), mImguiListener.get(), mCameraMan.get()});

        mTrayMgr->showCursor();
        mCameraMan->setStyle(OgreBites::CS_ORBIT);
        mCameraMan->setYawPitchDist(Degree(0), Degree(0), 15);

        SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        lightNode->setPosition(0, 10, 15);
        lightNode->attachObject(mSceneMgr->createLight("MainLight"));

        Entity* ent = mSceneMgr->createEntity("Sinbad.mesh");
        SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(ent);
    }

    void cleanupContent() override
    {
        OverlayManager::getSingleton().destroy("ImGuiOverlay");
        mWindow->removeListener(this);
    }
};
