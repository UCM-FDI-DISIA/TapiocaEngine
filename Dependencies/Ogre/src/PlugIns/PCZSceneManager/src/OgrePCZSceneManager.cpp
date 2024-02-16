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
PCZSceneManager.cpp  -  description
-----------------------------------------------------------------------------
begin                : Mon Feb 19 2007
author               : Eric Cha
email                : ericc@xenopi.com
-----------------------------------------------------------------------------
*/

#include "OgrePCZSceneManager.h"
#include "OgrePCZSceneQuery.h"
#include "OgrePCZSceneNode.h"
#include "OgrePCZone.h"
#include "OgrePCZCamera.h"
#include "OgrePCZLight.h"
#include "OgrePCZoneFactory.h"
#include "OgreAntiPortal.h"
#include "OgrePortal.h"
#include "OgreLogManager.h"
#include "OgreRoot.h"

namespace Ogre
{
    PCZSceneManager::PCZSceneManager(const String& name) :
    SceneManager(name),
    mDefaultZoneTypeName("ZoneType_Default"),
    mDefaultZoneFileName("none"),
    mLastActiveCamera(0),
    mDefaultZone(0),
    mShowPortals(false),
    mZoneFactoryManager(0),
    mActiveCameraZone(0)
    {
        addShadowTextureListener(this);
    }

    PCZSceneManager::~PCZSceneManager()
    {
        // we don't delete the root scene node here because the
        // base scene manager class does that.

        // delete ALL portals
        Portal * p;
        PortalList::iterator i = mPortals.begin();
        for (i = mPortals.begin(); i != mPortals.end(); i++)
        {
            p = *i;
            OGRE_DELETE p;
        }
        mPortals.clear();

        // delete all the zones
        for (ZoneMap::iterator j = mZones.begin();
            j != mZones.end(); ++j)
        {
            OGRE_DELETE j->second;
        }
        mZones.clear();
        mDefaultZone = 0;
    }

    const String& PCZSceneManager::getTypeName(void) const
    {
        return PCZSceneManagerFactory::FACTORY_TYPE_NAME;
    }

    void PCZSceneManager::init( const String &defaultZoneTypeName,
                                const String &filename)
    {

        // delete ALL portals
        Portal * p;
        PortalList::iterator i = mPortals.begin();
        for (i = mPortals.begin(); i != mPortals.end(); i++)
        {
            p = *i;
            OGRE_DELETE p;
        }
        mPortals.clear();

        // delete all the zones
        for (ZoneMap::iterator j = mZones.begin();
            j != mZones.end(); ++j)
        {
            OGRE_DELETE j->second;
        }
        mZones.clear();

        mFrameCount = 0;

        mDefaultZoneTypeName = defaultZoneTypeName;
        mDefaultZoneFileName = filename;

        // create a new default zone
        mZoneFactoryManager = PCZoneFactoryManager::getSingletonPtr();
        mDefaultZone = createZoneFromFile(mDefaultZoneTypeName, "Default_Zone", (PCZSceneNode*)getRootSceneNode(), mDefaultZoneFileName);
    }

    // Create a portal instance
    Portal* PCZSceneManager::createPortal(const String& name, PortalBase::PORTAL_TYPE type)
    {
        Portal* newPortal = OGRE_NEW Portal(name, type);
        newPortal->_notifyCreator(Root::getSingleton().getMovableObjectFactory("Portal"));
        newPortal->_notifyManager(this);
        mPortals.push_front(newPortal);
        return newPortal;
    }

    // delete a portal instance by pointer
    void PCZSceneManager::destroyPortal(Portal * p)
    {
        // remove the portal from it's target portal
        Portal * targetPortal = p->getTargetPortal();
        if (targetPortal)
        {
            targetPortal->setTargetPortal(0); // the targetPortal will still have targetZone value, but targetPortal will be invalid
        }
        // remove the Portal from it's home zone
        PCZone * homeZone = p->getCurrentHomeZone();
        if (homeZone)
        {
            // inform zone of portal change. Do here since PCZone is abstract 
            homeZone->setPortalsUpdated(true);   
            homeZone->_removePortal(p);
        }

        // remove the portal from the master portal list
        PortalList::iterator it = std::find( mPortals.begin(), mPortals.end(), p );
        if (it != mPortals.end())
        {
            mPortals.erase(it);
        }
        // delete the portal instance
        OGRE_DELETE p;
    }

    // delete a portal instance by pointer
    void PCZSceneManager::destroyPortal(const String & portalName)
    {
        // find the portal from the master portal list
        Portal * p;
        Portal * thePortal = 0;
        PortalList::iterator it = mPortals.begin();
        while (it != mPortals.end())
        {
            p = *it;
            if (p->getName() == portalName)
            {
                thePortal = p;
                // erase entry in the master list
                mPortals.erase(it);
                break;
            }
            it++;
        }
        if (thePortal)
        {
            // remove the portal from it's target portal
            Portal * targetPortal = thePortal->getTargetPortal();
            if (targetPortal)
            {
                targetPortal->setTargetPortal(0); // the targetPortal will still have targetZone value, but targetPortal will be invalid
            }
            // remove the Portal from it's home zone
            PCZone * homeZone = thePortal->getCurrentHomeZone();
            if (homeZone)
            {
                // inform zone of portal change 
                homeZone->setPortalsUpdated(true);   
                homeZone->_removePortal(thePortal);
            }

            // delete the portal instance
            OGRE_DELETE thePortal;
        }
    }

    /** Create a new anti portal instance */
    AntiPortal* PCZSceneManager::createAntiPortal(const String& name, PortalBase::PORTAL_TYPE type)
    {
        AntiPortal* newAntiPortal = OGRE_NEW AntiPortal(name, type);
        newAntiPortal->_notifyCreator(Root::getSingleton().getMovableObjectFactory("AntiPortal"));
        newAntiPortal->_notifyManager(this);
        mAntiPortals.push_front(newAntiPortal);
        return newAntiPortal;
    }

    /** Delete a anti portal instance by pointer */
    void PCZSceneManager::destroyAntiPortal(AntiPortal * p)
    {
        // remove the Portal from it's home zone
        PCZone* homeZone = p->getCurrentHomeZone();
        if (homeZone)
        {
            // inform zone of portal change. Do here since PCZone is abstract 
            homeZone->setPortalsUpdated(true);
            homeZone->_removeAntiPortal(p);
        }

        // remove the portal from the master portal list
        AntiPortalList::iterator it = std::find(mAntiPortals.begin(), mAntiPortals.end(), p);
        if (it != mAntiPortals.end()) mAntiPortals.erase(it);

        // delete the portal instance
        OGRE_DELETE p;
    }

    /** Delete a anti portal instance by name */
    void PCZSceneManager::destroyAntiPortal(const String& portalName)
    {
        // find the anti portal from the master portal list
        AntiPortal* p;
        AntiPortal* thePortal = 0;
        AntiPortalList::iterator it = mAntiPortals.begin();
        while (it != mAntiPortals.end())
        {
            p = *it;
            if (p->getName() == portalName)
            {
                thePortal = p;
                // erase entry in the master list
                mAntiPortals.erase(it);
                break;
            }
            it++;
        }
        if (thePortal)
        {
            // remove the Portal from it's home zone
            PCZone* homeZone = thePortal->getCurrentHomeZone();
            if (homeZone)
            {
                // inform zone of portal change 
                homeZone->setPortalsUpdated(true);
                homeZone->_removeAntiPortal(thePortal);
            }

            // delete the portal instance
            OGRE_DELETE thePortal;
        }
    }

    /** Create a zone from a file (type of file
        * depends on the zone type
        * ZoneType_Default uses an Ogre Model (.mesh) file
        * ZoneType_Octree uses an Ogre Model (.mesh) file
        * ZoneType_Terrain uses a Terrain.CFG file
        */
    PCZone * PCZSceneManager::createZoneFromFile(const String &zoneTypeName,
                                                 const String &zoneName,
                                                 PCZSceneNode * parentNode,
                                                 const String &filename)
    {
        PCZone * newZone;

        // create a new default zone
        newZone = mZoneFactoryManager->createPCZone(this, zoneTypeName, zoneName);
        // add to the global list of zones
        mZones[newZone->getName()] = newZone;
        if (filename != "none")
        {
            // set the zone geometry
            newZone->setZoneGeometry(filename, parentNode);
        }
        
        return newZone;
    }

    /* Get a zone by name */
    PCZone * PCZSceneManager::getZoneByName(const String & zoneName)
    {
        ZoneMap::iterator i;
        PCZone * zone;
        i = mZones.find(zoneName);
        if (i != mZones.end())
        {
            zone = i->second;
            return zone;
        }
        return 0; // couldn't find the zone  
    }

    void PCZSceneManager::setZoneGeometry(const String & zoneName,
                                          PCZSceneNode * parentNode,
                                          const String & filename)
    {
        ZoneMap::iterator i;
        PCZone * zone;
        i = mZones.find(zoneName);
        if (i != mZones.end())
        {
            zone = i->second;
            zone->setZoneGeometry( filename, parentNode );
            return;
        }

    }

    SceneNode* PCZSceneManager::createSceneNodeImpl(void)
    {
        auto ret = new PCZSceneNode(this);
        // create any zone-specific data necessary
        createZoneSpecificNodeData(ret);
        return ret;
    }

    SceneNode* PCZSceneManager::createSceneNodeImpl(const String& name)
    {
        auto ret = new PCZSceneNode(this, name);
        // create any zone-specific data necessary
        createZoneSpecificNodeData(ret);
        return ret;
    }

    // Create a camera for the scene
    Camera * PCZSceneManager::createCamera( const String &name )
    {
        // Check name not used
        if (mCameras.find(name) != mCameras.end())
        {
            OGRE_EXCEPT(
                Exception::ERR_DUPLICATE_ITEM,
                "A camera with the name " + name + " already exists",
                "PCZSceneManager::createCamera" );
        }

        Camera * c = OGRE_NEW PCZCamera( name, this );
        mCameras.emplace(name, c);

        // create visible bounds aab map entry
        mCamVisibleObjectsMap[c] = VisibleObjectsBoundsInfo();
        
        // tell all the zones about the new camera
        ZoneMap::iterator i;
        PCZone * zone;
        for (i = mZones.begin(); i != mZones.end(); i++)
        {
            zone = i->second;
            zone->notifyCameraCreated( c );
        }

        return c;
    }

    // Destroy a Scene Node by name.
    void PCZSceneManager::destroySceneNode( const String &name )
    {
        SceneNode * on = ( getSceneNode( name ) );

        if ( on != 0 )
        {
            // destroy the node
            destroySceneNode( on );
        }
    }
    
    // Destroy a scene node
    void PCZSceneManager::destroySceneNode(SceneNode* sn)
    {
        if ( sn != 0 )
        {
            // remove references to the node from zones
            removeSceneNode( sn );
        
            // destroy the node
            SceneManager::destroySceneNode( sn );
        }
    }

    //-----------------------------------------------------------------------
    void PCZSceneManager::clearScene(void)
    {
        SceneManager::clearScene();

        // delete all the zones
        for (ZoneMap::iterator j = mZones.begin();
            j != mZones.end(); ++j)
        {
            OGRE_DELETE j->second;
        }
        mZones.clear();
        mDefaultZone = 0;

        // re-initialize
        init(mDefaultZoneTypeName, mDefaultZoneFileName);
    }

    /** Overridden from SceneManager */
    void PCZSceneManager::setWorldGeometryRenderQueue(uint8 qid)
    {
        // tell all the zones about the new WorldGeometryRenderQueue
        ZoneMap::iterator i;
        PCZone * zone;
        for (i = mZones.begin(); i != mZones.end(); i++)
        {
            zone = i->second;
            zone->notifyWorldGeometryRenderQueue( qid );
        }
        // call the regular scene manager version
        SceneManager::setWorldGeometryRenderQueue(qid);

    }
    /** Overridden from SceneManager */
    void PCZSceneManager::_renderScene(Camera* cam, Viewport *vp, bool includeOverlays)
    {
        // notify all the zones that a scene render is starting
        ZoneMap::iterator i;
        PCZone * zone;
        for (i = mZones.begin(); i != mZones.end(); i++)
        {
            zone = i->second;
            zone->notifyBeginRenderScene();
        }

        // do the regular _renderScene
        SceneManager::_renderScene(cam, vp, includeOverlays);
    }

    /* enable/disable sky rendering */
    void PCZSceneManager::enableSky(bool onoff)
    {
        if (getSkyBoxNode())
        {
            setSkyBoxEnabled(onoff);
        }
        else if (getSkyDomeNode())
        {
            setSkyDomeEnabled(onoff);
        }
        else if (getSkyPlaneNode())
        {
            setSkyPlaneEnabled(onoff);
        }
    }

    /* Set the zone which contains the sky node */
    void PCZSceneManager::setSkyZone(PCZone * zone)
    {
        if (zone == 0)
        {
            // if no zone specified, use default zone
            zone = mDefaultZone;
        }
        if (auto node = (PCZSceneNode*)getSkyBoxNode())
        {
            node->setHomeZone(zone);
            node->anchorToHomeZone(zone);
            zone->setHasSky(true);
        }
        if (auto node = (PCZSceneNode*)getSkyDomeNode())
        {
            node->setHomeZone(zone);
            node->anchorToHomeZone(zone);
            zone->setHasSky(true);
        }
        if (auto node = (PCZSceneNode*)getSkyPlaneNode())
        {
            node->setHomeZone(zone);
            node->anchorToHomeZone(zone);
            zone->setHasSky(true);
        }
        
    }

    //-----------------------------------------------------------------------
    // THIS IS THE MAIN LOOP OF THE MANAGER
    //-----------------------------------------------------------------------
    /* _updateSceneGraph does several things now:
       1) standard scene graph update (transform all nodes in the node tree)
       2) update the spatial data for all zones (& portals in the zones)
       3) Update the PCZSNMap entry for every scene node
    */
    void PCZSceneManager::_updateSceneGraph( Camera * cam )
    {
        // First do the standard scene graph update
        SceneManager::_updateSceneGraph( cam );
        // check for portal zone-related changes (portals intersecting other portals)
        _updatePortalZoneData();
        // mark nodes dirty base on portals that changed.
        _dirtyNodeByMovingPortals();
        // update all scene nodes
        _updatePCZSceneNodes();
        // calculate zones affected by each light
        _calcZonesAffectedByLights(cam);
        // clear update flags at end so user triggered updated are 
        // not cleared prematurely 
        _clearAllZonesPortalUpdateFlag(); 
    }

    /** Update the zone data for every zone portal in the scene */

    void PCZSceneManager::_updatePortalZoneData(void)
    {
        PCZone * zone;
        ZoneMap::iterator zit = mZones.begin();

        while ( zit != mZones.end() )
        {
            zone = zit->second;
            // this callchecks for portal zone changes & applies zone data changes as necessary
            zone->updatePortalsZoneData(); 
            // proceed to next zone in the list
            ++zit;
        }
    }

    /** Mark nodes dirty for every zone with moving portal in the scene */
    void PCZSceneManager::_dirtyNodeByMovingPortals(void)
    {
        PCZone * zone;
        ZoneMap::iterator zit = mZones.begin();

        while ( zit != mZones.end() )
        {
            zone = zit->second;
            // this call mark nodes dirty base on moving portals 
            zone->dirtyNodeByMovingPortals(); 
            // proceed to next zone in the list
            ++zit;
        }
    }

    /* Update all PCZSceneNodes. 
    */
    void PCZSceneManager::_updatePCZSceneNodes(void)
    {
        SceneNodeList::iterator it = mSceneNodes.begin();
        PCZSceneNode * pczsn;

        while ( it != mSceneNodes.end() )
        {
            pczsn = (PCZSceneNode*)*it;
            if (pczsn->isMoved() && pczsn->isEnabled())
            {
                // Update a single entry 
                _updatePCZSceneNode(pczsn);

                // reset moved state.
                pczsn->setMoved(false);
            }
            // proceed to next entry in the list
            ++it;
        }
    }

    /*
    */
    void PCZSceneManager::_calcZonesAffectedByLights(Camera * cam)
    {
        MovableObjectCollection* lights =
            getMovableObjectCollection(PCZLightFactory::FACTORY_TYPE_NAME);
        {
                OGRE_LOCK_MUTEX(lights->mutex);

            MovableObjectIterator it(lights->map.begin(), lights->map.end());

            while(it.hasMoreElements())
            {
                PCZLight* l = static_cast<PCZLight*>(it.getNext());
                if(l->getNeedsUpdate()) 
                {
                    // only update if necessary 
                    l->updateZones(((PCZSceneNode*)(cam->getParentSceneNode()))->getHomeZone(), mFrameCount);   
                }
                // clear update flag 
                l->clearNeedsUpdate();   
            }
        }
    }

    //-----------------------------------------------------------------------
    // Update all the zone info for a given node.  This function
    // makes sure the home zone of the node is correct, and references
    // to any zones it is visiting are added and a reference to the 
    // node is added to the visitor lists of any zone it is visiting.
    //
    void PCZSceneManager::_updatePCZSceneNode( PCZSceneNode * pczsn )
    {
        // Skip if root Zone has been destroyed (shutdown conditions)
        if (!mDefaultZone)
            return;

        // Skip if the node is the sceneroot node 
        if (pczsn == getRootSceneNode())
            return;

        // clear all references to visiting zones
        pczsn->clearNodeFromVisitedZones();

        // Find the current home zone of the node associated with the pczsn entry.
        _updateHomeZone( pczsn, false );

        /* The following function does the following:
        * 1) Check all portals in the home zone - if the node is touching the portal
        *    then add the node to the connected zone as a visitor
        * 2) Recurse into visited zones in case the node spans several zones
        */
        // (recursively) check each portal of home zone to see if the node is touching 
        if (pczsn->getHomeZone() &&
            pczsn->allowedToVisit() == true)
        {
            pczsn->getHomeZone()->_checkNodeAgainstPortals(pczsn, 0);
        }

        // update zone-specific data for the node for any zones that require it
        pczsn->updateZoneData();
    }

    /** Removes all references to the node from every zone in the scene.  
    */
    void PCZSceneManager::removeSceneNode( SceneNode * sn )
    {
        // Skip if mDefaultZone has been destroyed (shutdown conditions)
        if (!mDefaultZone)
            return;

        PCZSceneNode * pczsn = (PCZSceneNode*)sn;

        // clear all references to the node in visited zones
        pczsn->clearNodeFromVisitedZones();

        // tell the node it's not in a zone
        pczsn->setHomeZone(0);
    }

    // Set the home zone for a node
    void PCZSceneManager::addPCZSceneNode(PCZSceneNode * sn, PCZone * homeZone)
    {
        // set the home zone
        sn->setHomeZone(homeZone);
        // add the node
        homeZone->_addNode(sn);
    }

    //-----------------------------------------------------------------------
    /* Create a zone with the given name and parent zone */
    PCZone * PCZSceneManager::createZone(const String& zoneType, const String& instanceName)
    {
        if (mZones.find(instanceName) != mZones.end())
        {
            OGRE_EXCEPT(
                Exception::ERR_DUPLICATE_ITEM,
                "A zone with the name " + instanceName + " already exists",
                "PCZSceneManager::createZone" );
        }
        PCZone * newZone = mZoneFactoryManager->createPCZone(this, zoneType, instanceName);
        if (newZone)
        {
            // add to the global list of zones
            mZones[instanceName] = newZone;

            if (newZone->requiresZoneSpecificNodeData())
            {
                createZoneSpecificNodeData(newZone);
            }
        }
        return newZone;
    }

    /* destroy an existing zone within the scene */
    /* if destroySceneNodes is true, then all nodes which have the destroyed
       zone as their homezone are desroyed too.  If destroySceneNodes is false
       then all scene nodes which have the zone as their homezone will have
       their homezone pointer set to 0, which will allow them to be re-assigned
       either by the user or via the automatic re-assignment routine */
    void PCZSceneManager::destroyZone(PCZone* zone, bool destroySceneNodes)
    {
        // need to remove this zone from all lights affected zones list,
        // otherwise next frame _calcZonesAffectedByLights will call PCZLight::getNeedsUpdate()
        // which will try to access the zone pointer and will cause an access violation
        MovableObjectCollection* lights =
        getMovableObjectCollection(PCZLightFactory::FACTORY_TYPE_NAME);
        {
                    OGRE_LOCK_MUTEX(lights->mutex); // Is locking necessary in destroyZone? I don't know..

            MovableObjectIterator it(lights->map.begin(), lights->map.end());

            while(it.hasMoreElements())
            {
                PCZLight* l = static_cast<PCZLight*>(it.getNext());
                if(l) 
                {
                    // no need to check, this function does that anyway. if exists, is erased.
                    l->removeZoneFromAffectedZonesList(zone);   
                }
            }
        }
        // if not destroying scene nodes, then make sure any nodes who have
        // this zone as homezone are set to have 0 for a homezone
        for (SceneNodeList::iterator i = mSceneNodes.begin();
            i != mSceneNodes.end(); ++i)
        {
            PCZSceneNode * pczsn = (PCZSceneNode*)*i;
            if (!destroySceneNodes)
            {
                if (pczsn->getHomeZone() == zone)
                {
                    pczsn->setHomeZone(0);
                }
            }
            // reset all node visitor lists
            // note, it might be more efficient to only do this to nodes which
            // are actually visiting the zone being destroyed, but visitor lists
            // get cleared every frame anyway, so it's not THAT big a deal.
            pczsn->clearNodeFromVisitedZones();
        }

        ZoneMap::iterator it;
        it = mZones.find(zone->getName());
        if (it != mZones.end())
        {
            mZones.erase(zone->getName());
        }
        OGRE_DELETE zone;
    }

    /* The following function checks if a node has left it's current home zone.
    * This is done by checking each portal in the zone.  If the node has crossed
    * the portal, then the current zone is no longer the home zone of the node.  The
    * function then recurses into the connected zones.  Once a zone is found where
    * the node does NOT cross out through a portal, that zone is the new home zone.
    * When this function is done, the node should have the correct home zone already
    * set.  A pointer is returned to this zone as well.
    *
    * NOTE: If the node does not have a home zone when this function is called on it,
    *       the function will do its best to find the proper zone for the node using
    *       bounding box volume testing.  This CAN fail to find the correct zone in
    *       some scenarios, so it is best for the user to EXPLICITLY set the home
    *       zone of the node when the node is added to the scene using 
    *       PCZSceneNode::setHomeZone()
    */
    void PCZSceneManager::_updateHomeZone( PCZSceneNode * pczsn, bool allowBackTouches )
    {
        // Skip if root PCZoneTree has been destroyed (shutdown conditions)
        if (!mDefaultZone)
            return;

        PCZone * startzone;
        PCZone * newHomeZone;

        // start with current home zone of the node
        startzone = pczsn->getHomeZone();

        if (startzone)
        {
            if (!pczsn->isAnchored())
            {
                newHomeZone = startzone->updateNodeHomeZone(pczsn, false);
            }
            else
            {
                newHomeZone = startzone;
            }

            if (newHomeZone != startzone)
            {
                // add the node to the home zone
                newHomeZone->_addNode(pczsn);
            }
        }
        else
        {
            // the node hasn't had it's home zone set yet, so do our best to
            // find the home zone using volume testing.  
            Vector3 nodeCenter = pczsn->_getDerivedPosition();
            PCZone * bestZone = findZoneForPoint(nodeCenter);
            // set the best zone as the node's home zone
            pczsn->setHomeZone(bestZone);
            // add the node to the zone
            bestZone->_addNode(pczsn);
        }

        return;

    }

    /* Find the best (smallest) zone that contains a point
    */
    PCZone * PCZSceneManager::findZoneForPoint(Vector3 & point)
    {
        PCZone * zone;
        PCZone * bestZone = mDefaultZone;
        Real bestVolume = Ogre::Math::POS_INFINITY;

        ZoneMap::iterator zit = mZones.begin();

        while ( zit != mZones.end() )
        {
            zone = zit->second;
            AxisAlignedBox aabb;
            zone->getAABB(aabb);
            SceneNode * enclosureNode = zone->getEnclosureNode();
            if (enclosureNode != 0)
            {
                // since this is the "local" AABB, add in world translation of the enclosure node
                aabb.setMinimum(aabb.getMinimum() + enclosureNode->_getDerivedPosition());
                aabb.setMaximum(aabb.getMaximum() + enclosureNode->_getDerivedPosition());
            }
            if (aabb.contains(point))
            {
                if (aabb.volume() < bestVolume)
                {
                    // this zone is "smaller" than the current best zone, so make it
                    // the new best zone
                    bestZone = zone;
                    bestVolume = aabb.volume();
                }
            }
            // proceed to next zone in the list
            ++zit;
        }
        return bestZone;
    }

    // create any zone-specific data necessary for all zones for the given node
    void PCZSceneManager::createZoneSpecificNodeData(PCZSceneNode * node)
    {
        ZoneMap::iterator i;
        PCZone * zone;
        for (i = mZones.begin(); i != mZones.end(); i++)
        {
            zone = i->second;
            if (zone->requiresZoneSpecificNodeData())
            {
                zone->createNodeZoneData(node);
            }
        }       
    }

    // create any zone-specific data necessary for all nodes for the given zone
    void PCZSceneManager::createZoneSpecificNodeData(PCZone * zone)
    {
        SceneNodeList::iterator it = mSceneNodes.begin();
        PCZSceneNode * pczsn;
        if (zone->requiresZoneSpecificNodeData())
        {
            while ( it != mSceneNodes.end() )
            {
                pczsn = (PCZSceneNode*)*it;
                // create zone specific data for the node 
                zone->createNodeZoneData(pczsn);
                // proceed to next entry in the list
                ++it;
            }
        }
    }

    // set the home zone for a scene node
    void PCZSceneManager::setNodeHomeZone(SceneNode *node, PCZone *zone)
    {
        // cast the Ogre::SceneNode to a PCZSceneNode
        PCZSceneNode * pczsn = (PCZSceneNode*)node;
        pczsn->setHomeZone(zone);
    }

    //-----------------------------------------------------------------------
    Light* PCZSceneManager::createLight(const String& name)
    {
        return static_cast<Light*>(
            createMovableObject(name, PCZLightFactory::FACTORY_TYPE_NAME));
    }
    //-----------------------------------------------------------------------
    Light* PCZSceneManager::getLight(const String& name) const
    {
        return static_cast<Light*>(
            getMovableObject(name, PCZLightFactory::FACTORY_TYPE_NAME));
    }
    //-----------------------------------------------------------------------
    bool PCZSceneManager::hasLight(const String& name) const
    {
        return hasMovableObject(name, PCZLightFactory::FACTORY_TYPE_NAME);
    }
    //-----------------------------------------------------------------------
    void PCZSceneManager::destroyLight(const String& name)
    {
        destroyMovableObject(name, PCZLightFactory::FACTORY_TYPE_NAME);
    }
    //-----------------------------------------------------------------------
    void PCZSceneManager::destroyAllLights(void)
    {
        destroyAllMovableObjectsByType(PCZLightFactory::FACTORY_TYPE_NAME);
    }
    //---------------------------------------------------------------------
    void PCZSceneManager::findLightsAffectingFrustum(const Camera* camera)
    {
        // Similar to the basic SceneManager, we iterate through
        // lights to see which ones affect the frustum.  However,
        // since we have camera & lights partitioned by zones,
        // we can check only those lights which either affect the
        // zone the camera is in, or affect zones which are visible to
        // the camera

        MovableObjectCollection* lights =
            getMovableObjectCollection(PCZLightFactory::FACTORY_TYPE_NAME);


        {
                    OGRE_LOCK_MUTEX(lights->mutex);

            // Pre-allocate memory
            mTestLightInfos.clear();
            mTestLightInfos.reserve(lights->map.size());

            MovableObjectIterator it(lights->map.begin(), lights->map.end());

            while(it.hasMoreElements())
            {
                PCZLight* l = static_cast<PCZLight*>(it.getNext());
                if (l->isVisible() &&
                    l->affectsVisibleZone())
                {
                    LightInfo lightInfo;
                    lightInfo.light = l;
                    lightInfo.type = l->getType();
                    if (lightInfo.type == Light::LT_DIRECTIONAL)
                    {
                        // Always visible
                        lightInfo.position = Vector3::ZERO;
                        lightInfo.range = 0;
                        mTestLightInfos.push_back(lightInfo);
                    }
                    else
                    {
                        // NB treating spotlight as point for simplicity
                        // Just see if the lights attenuation range is within the frustum
                        lightInfo.range = l->getAttenuationRange();
                        lightInfo.position = l->getDerivedPosition();
                        Sphere sphere(lightInfo.position, lightInfo.range);
                        if (camera->isVisible(sphere))
                        {
                            mTestLightInfos.push_back(lightInfo);
                        }
                    }
                }
            }
        } // release lock on lights collection

        updateCachedLightInfos(camera);
    }
    //---------------------------------------------------------------------
    void PCZSceneManager::ensureShadowTexturesCreated()
    {
        SceneManager::ensureShadowTexturesCreated();
        if (!isShadowTextureConfigDirty()) return;

        for (auto cam : getShadowTextureCameras())
        {
            auto node = (PCZSceneNode*)cam->getParentSceneNode();
            addPCZSceneNode(node, mDefaultZone);
        }
    }
    //---------------------------------------------------------------------
    void PCZSceneManager::shadowTextureCasterPreViewProj(Light* light, Camera* camera, size_t iteration)
    {
        PCZSceneNode* camNode = (PCZSceneNode*)camera->getParentSceneNode();

        if (light->getType() == Light::LT_DIRECTIONAL)
        {
            if (camNode->getHomeZone() != mActiveCameraZone)
                addPCZSceneNode(camNode, mActiveCameraZone);
        }
        else
        {
            PCZSceneNode* lightNode = (PCZSceneNode*)light->getParentSceneNode();
            assert(lightNode);
            PCZone* lightZone = lightNode->getHomeZone();
            if (camNode->getHomeZone() != lightZone)
                addPCZSceneNode(camNode, lightZone);
        }
    }

    /* Attempt to automatically connect unconnected portals to proper target zones 
        * by looking for matching portals in other zones which are at the same location
        */
    void PCZSceneManager::connectPortalsToTargetZonesByLocation(void)
    {
        // go through every zone to find portals
        ZoneMap::iterator i, iend;
        PCZone* zone;
        iend = mZones.end();
        bool foundMatch;
        for (i = mZones.begin(); i != iend; i++)
        {
            zone = i->second;
            // go through all the portals in the zone
            Portal* portal;
            PortalList::iterator pi, piend;
            piend = zone->mPortals.end();
            for (pi = zone->mPortals.begin(); pi != piend; pi++)
            {
                portal = *pi;
                //portal->updateDerivedValues();
                if (portal->getTargetZone() == 0)
                {
                    // this is a portal without a connected zone - look for 
                    // a matching portal in another zone
                    PCZone* zone2;
                    ZoneMap::iterator j= mZones.begin();
                    foundMatch = false;
                    while (!foundMatch && j != mZones.end())
                    {
                        zone2 = j->second;
                        if (zone2 != zone) // make sure we don't look in the same zone
                        {
                            Portal * portal2 = zone2->findMatchingPortal(portal);
                            if (portal2)
                            {
                                // found a match!
                                Ogre::LogManager::getSingletonPtr()->logMessage("Connecting portal "+portal->getName()+" to portal "+portal2->getName());
                                foundMatch = true;
                                portal->setTargetZone(zone2);
                                portal->setTargetPortal(portal2);
                                portal2->setTargetZone(zone);
                                portal2->setTargetPortal(portal);
                            }
                        }
                        j++;
                    }
                    if (foundMatch == false)
                    {
                        // error, didn't find a matching portal!
                        OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                            "Could not find matching portal for portal " + portal->getName(), 
                            "PCZSceneManager::connectPortalsToTargetZonesByLocation");

                    }
                }
            }           
        }
    }

    // main visibility determination & render queue filling routine
    // over-ridden from base/default scene manager.  This is *the*
    // main call.
    void PCZSceneManager::_findVisibleObjects(Camera* cam,
                                              VisibleObjectsBoundsInfo* visibleBounds,
                                              bool onlyShadowCasters)
    {
        // clear the render queue
        getRenderQueue()->clear();

        // if we are re-rendering the scene again with the same camera, we can just use the cache.
        // this helps post processing compositors.
        unsigned long frameCount = Root::getSingleton().getNextFrameNumber();
        if (mLastActiveCamera == cam && mFrameCount == frameCount)
        {
            RenderQueue* queue = getRenderQueue();
            size_t count = mVisible.size();
            for (size_t i = 0; i < count; ++i)
            {
                ((PCZSceneNode*)mVisible[i])->_addToRenderQueue(
                    cam, queue, onlyShadowCasters, visibleBounds);
            }
            return;
        }

        // increment the visibility frame counter
        mFrameCount = frameCount;
        mLastActiveCamera = cam;

        // clear the list of visible nodes
        mVisible.clear();

        // turn off sky 
        enableSky(false);

        // remove all extra culling planes
        ((PCZCamera*)cam)->removeAllExtraCullingPlanes();

        // update the camera
        ((PCZCamera*)cam)->update();

        // get the home zone of the camera
        PCZone* cameraHomeZone = ((PCZSceneNode*)(cam->getParentSceneNode()))->getHomeZone();

        // walk the zones, starting from the camera home zone,
        // adding all visible scene nodes to the mVisibles list
        cameraHomeZone->setLastVisibleFrame(mFrameCount);
        cameraHomeZone->findVisibleNodes((PCZCamera*)cam, 
                                          mVisible, 
                                          getRenderQueue(),
                                          visibleBounds, 
                                          onlyShadowCasters,
                                          getDisplaySceneNodes(),
                                          getShowBoundingBoxes());
    }

    void PCZSceneManager::findNodesIn( const AxisAlignedBox &box, 
                                       PCZSceneNodeList &list, 
                                       PCZone * startZone, 
                                       PCZSceneNode *exclude )
    {
        PortalList visitedPortals;
        if (startZone)
        {
            // start in startzone, and recurse through portals if necessary
            startZone->_findNodes(box, list, visitedPortals, true, true, exclude);
        }
        else
        {
            // no start zone specified, so check all zones
            ZoneMap::iterator i;
            PCZone * zone;
            for (i = mZones.begin(); i != mZones.end(); i++)
            {
                zone = i->second;
                zone->_findNodes( box, list, visitedPortals, false, false, exclude );
            }
        }
    }

    void PCZSceneManager::findNodesIn( const Sphere &sphere, 
                                       PCZSceneNodeList &list, 
                                       PCZone * startZone, 
                                       PCZSceneNode *exclude  )
    {
        PortalList visitedPortals;
        if (startZone)
        {
            // start in startzone, and recurse through portals if necessary
            startZone->_findNodes(sphere, list, visitedPortals, true, true, exclude);
        }
        else
        {
            // no start zone specified, so check all zones
            ZoneMap::iterator i;
            PCZone * zone;
            for (i = mZones.begin(); i != mZones.end(); i++)
            {
                zone = i->second;
                zone->_findNodes( sphere, list, visitedPortals, false, false, exclude );
            }
        }
    }

    void PCZSceneManager::findNodesIn( const PlaneBoundedVolume &volume, 
                                       PCZSceneNodeList &list, 
                                       PCZone * startZone, 
                                       PCZSceneNode *exclude )
    {
        PortalList visitedPortals;
        if (startZone)
        {
            // start in startzone, and recurse through portals if necessary
            startZone->_findNodes(volume, list, visitedPortals, true, true, exclude);
        }
        else
        {
            // no start zone specified, so check all zones
            ZoneMap::iterator i;
            PCZone * zone;
            for (i = mZones.begin(); i != mZones.end(); i++)
            {
                zone = i->second;
                zone->_findNodes( volume, list, visitedPortals, false, false, exclude );
            }
        }
    }

    void PCZSceneManager::findNodesIn( const Ray &r, 
                                       PCZSceneNodeList &list, 
                                       PCZone * startZone, 
                                       PCZSceneNode *exclude  )
    {
        PortalList visitedPortals;
        if (startZone)
        {
            // start in startzone, and recurse through portals if necessary
            startZone->_findNodes(r, list, visitedPortals, true, true, exclude);
        }
        else
        {
            ZoneMap::iterator i;
            PCZone * zone;
            for (i = mZones.begin(); i != mZones.end(); i++)
            {
                zone = i->second;
                zone->_findNodes( r, list, visitedPortals, false, false, exclude );
            }
        }
    }

    // get the current value of a scene manager option
    bool PCZSceneManager::getOptionValues( const String & key, StringVector  &refValueList )
    {
        return SceneManager::getOptionValues( key, refValueList );
    }

    // get option keys (base along with PCZ-specific)
    bool PCZSceneManager::getOptionKeys( StringVector & refKeys )
    {
        SceneManager::getOptionKeys( refKeys );
        refKeys.push_back( "ShowBoundingBoxes" );
        refKeys.push_back( "ShowPortals" );

        return true;
    }

    bool PCZSceneManager::setOption( const String & key, const void * val )
    {
        if ( key == "ShowBoundingBoxes" )
        {
            showBoundingBoxes(* static_cast < const bool * > ( val ));
            return true;
        }

        else if ( key == "ShowPortals" )
        {
            mShowPortals = * static_cast < const bool * > ( val );
            return true;
        }
        // send option to each zone
        ZoneMap::iterator i;
        PCZone * zone;
        for (i = mZones.begin(); i != mZones.end(); i++)
        {
            zone = i->second;
             if (zone->setOption(key, val ) == true)
             {
                 return true;
             }
        }
        
        // try regular scenemanager option
        return SceneManager::setOption( key, val );


    }

    bool PCZSceneManager::getOption( const String & key, void *val )
    {
        if ( key == "ShowBoundingBoxes" )
        {

            * static_cast < bool * > ( val ) = getShowBoundingBoxes();
            return true;
        }
        if ( key == "ShowPortals" )
        {

            * static_cast < bool * > ( val ) = mShowPortals;
            return true;
        }
        return SceneManager::getOption( key, val );

    }

    //---------------------------------------------------------------------
    AxisAlignedBoxSceneQuery*
    PCZSceneManager::createAABBQuery(const AxisAlignedBox& box, uint32 mask)
    {
        PCZAxisAlignedBoxSceneQuery* q = OGRE_NEW PCZAxisAlignedBoxSceneQuery(this);
        q->setBox(box);
        q->setQueryMask(mask);
        return q;
    }
    //---------------------------------------------------------------------
    SphereSceneQuery*
    PCZSceneManager::createSphereQuery(const Sphere& sphere, uint32 mask)
    {
        PCZSphereSceneQuery* q = OGRE_NEW PCZSphereSceneQuery(this);
        q->setSphere(sphere);
        q->setQueryMask(mask);
        return q;
    }
    //---------------------------------------------------------------------
    PlaneBoundedVolumeListSceneQuery*
    PCZSceneManager::createPlaneBoundedVolumeQuery(const PlaneBoundedVolumeList& volumes,
            uint32 mask)
    {
        PCZPlaneBoundedVolumeListSceneQuery* q = OGRE_NEW PCZPlaneBoundedVolumeListSceneQuery(this);
        q->setVolumes(volumes);
        q->setQueryMask(mask);
        return q;
    }

    //---------------------------------------------------------------------
    RaySceneQuery*
    PCZSceneManager::createRayQuery(const Ray& ray, uint32 mask)
    {
        PCZRaySceneQuery* q = OGRE_NEW PCZRaySceneQuery(this);
        q->setRay(ray);
        q->setQueryMask(mask);
        return q;
    }
    //---------------------------------------------------------------------
    IntersectionSceneQuery*
    PCZSceneManager::createIntersectionQuery(uint32 mask)
    {

        PCZIntersectionSceneQuery* q = OGRE_NEW PCZIntersectionSceneQuery(this);
        q->setQueryMask(mask);
        return q;
    }
    //---------------------------------------------------------------------
    // clear portal update flag from all zones
    void PCZSceneManager::_clearAllZonesPortalUpdateFlag(void)
    {
        ZoneMap::iterator zoneIterator = mZones.begin();

        while ( zoneIterator != mZones.end() )
        {
            (zoneIterator->second)->setPortalsUpdated(false);
            zoneIterator++;
        }
    }
    //---------------------------------------------------------------------
    /// See SceneManager::prepareShadowTextures.
    void PCZSceneManager::prepareShadowTextures(Camera* cam, Viewport* vp, const LightList* lightList)
    {
        mActiveCameraZone = ((PCZSceneNode*)cam->getParentSceneNode())->getHomeZone();
        SceneManager::prepareShadowTextures(cam, vp);
    }

    //-----------------------------------------------------------------------
    const String PCZSceneManagerFactory::FACTORY_TYPE_NAME = "PCZSceneManager";
    //-----------------------------------------------------------------------
    SceneManager* PCZSceneManagerFactory::createInstance(
        const String& instanceName)
    {
        return OGRE_NEW PCZSceneManager(instanceName);
    }

}
