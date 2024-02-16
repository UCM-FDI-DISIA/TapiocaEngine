/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"

#include "OgreRenderSystemCapabilitiesManager.h"
#include "OgreRenderSystemCapabilitiesSerializer.h"
#include "OgreConfigFile.h"
#include "OgreFileSystemLayer.h"

namespace Ogre {

    //-----------------------------------------------------------------------
    template<> RenderSystemCapabilitiesManager* Singleton<RenderSystemCapabilitiesManager>::msSingleton = 0;
    RenderSystemCapabilitiesManager* RenderSystemCapabilitiesManager::getSingletonPtr(void)
    {
        return msSingleton;
    }
    RenderSystemCapabilitiesManager& RenderSystemCapabilitiesManager::getSingleton(void)
    {
        assert( msSingleton );  return ( *msSingleton );
    }

    //-----------------------------------------------------------------------
    RenderSystemCapabilitiesManager::RenderSystemCapabilitiesManager() : mSerializer(0), mScriptPattern("*.rendercaps")
    {
        mSerializer = OGRE_NEW RenderSystemCapabilitiesSerializer();
    }
    //-----------------------------------------------------------------------
    RenderSystemCapabilitiesManager::~RenderSystemCapabilitiesManager()
    {
        for (auto& cm : mCapabilitiesMap)
        {
        // free memory in RenderSystemCapabilities*
            OGRE_DELETE cm.second;
        }

        OGRE_DELETE mSerializer;
    }

    RenderSystemCapabilities* RenderSystemCapabilitiesManager::loadCapabilitiesConfig(const String& customConfig)
    {
        ConfigFile cfg;
        cfg.load(customConfig, "\t:=", false);

        // resolve relative path with regards to configfile
        String baseDir, unused;
        StringUtil::splitFilename(customConfig, unused, baseDir);

        // Capabilities Database setting must be in the same format as
        // resources.cfg in Ogre examples.
        for(auto& it : cfg.getSettings("Capabilities Database"))
        {
            String filename = it.second;
            if(filename.empty() || filename[0] == '.')
                filename = baseDir + it.second;

            filename = FileSystemLayer::resolveBundlePath(filename);
            parseCapabilitiesFromArchive(filename, it.first, true);
        }

        String capsName = cfg.getSetting("Custom Capabilities");
        // The custom capabilities have been parsed, let's retrieve them
        RenderSystemCapabilities* rsc = loadParsedCapabilities(capsName);
        if (rsc == 0)
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Cannot load a RenderSystemCapability named '" + capsName + "'");
        }

        return rsc;
    }

    //-----------------------------------------------------------------------
    void RenderSystemCapabilitiesManager::parseCapabilitiesFromArchive(const String& filename, const String& archiveType, bool recursive)
    {
        // get the list of .rendercaps files
        Archive* arch = ArchiveManager::getSingleton().load(filename, archiveType, true);
        StringVectorPtr files = arch->find(mScriptPattern, recursive);

        // loop through .rendercaps files and load each one
        for (auto& f : *files)
        {
            DataStreamPtr stream = arch->open(f);
            mSerializer->parseScript(stream);
            stream->close();
        }
    }

    RenderSystemCapabilities* RenderSystemCapabilitiesManager::loadParsedCapabilities(const String& name)
    {
        return mCapabilitiesMap[name];
    }

    const std::map<String, RenderSystemCapabilities*> &RenderSystemCapabilitiesManager::getCapabilities() const
    {
        return mCapabilitiesMap;
    }

    /** Method used by RenderSystemCapabilitiesSerializer::parseScript */
    void RenderSystemCapabilitiesManager::_addRenderSystemCapabilities(const String& name, RenderSystemCapabilities* caps)
    {
        mCapabilitiesMap.emplace(name, caps);
    }
}





