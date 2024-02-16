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
#include "OgreInstanceManager.h"
#include "OgreInstanceBatchHW.h"
#include "OgreInstanceBatchHW_VTF.h"
#include "OgreInstanceBatchShader.h"
#include "OgreInstanceBatchVTF.h"

namespace Ogre
{
    InstanceManager::InstanceManager( const String &customName, SceneManager *sceneManager,
                                        const String &meshName, const String &groupName,
                                        InstancingTechnique instancingTechnique, uint16 instancingFlags,
                                        size_t instancesPerBatch, unsigned short subMeshIdx, bool useBoneMatrixLookup ) :
                mName( customName ),
                mIdCount( 0 ),
                mInstancesPerBatch( instancesPerBatch ),
                mInstancingTechnique( instancingTechnique ),
                mInstancingFlags( instancingFlags ),
                mSubMeshIdx( subMeshIdx ),
                mSceneManager( sceneManager ),
                mMaxLookupTableInstances(16),
                mNumCustomParams( 0 )
    {
        mMeshReference = MeshManager::getSingleton().load( meshName, groupName );

        if(mMeshReference->sharedVertexData)
            unshareVertices(mMeshReference);

        if( mMeshReference->hasSkeleton() && mMeshReference->getSkeleton() )
            mMeshReference->getSubMesh(mSubMeshIdx)->_compileBoneAssignments();
    }
                
    InstanceManager::~InstanceManager()
    {
        //Remove all batches from all materials we created
        for (auto& i : mInstanceBatches)
        {
            for (auto *it : i.second)
                OGRE_DELETE it;
        }
    }
    //----------------------------------------------------------------------
    void InstanceManager::setInstancesPerBatch( size_t instancesPerBatch )
    {
        OgreAssert(mInstanceBatches.empty(), "can only be changed before building the batch");
        mInstancesPerBatch = instancesPerBatch;
    }

    //----------------------------------------------------------------------
    void InstanceManager::setMaxLookupTableInstances( size_t maxLookupTableInstances )
    {
        OgreAssert(mInstanceBatches.empty(), "can only be changed before building the batch");
        mMaxLookupTableInstances = maxLookupTableInstances;
    }
    
    //----------------------------------------------------------------------
    void InstanceManager::setNumCustomParams( unsigned char numCustomParams )
    {
        OgreAssert(mInstanceBatches.empty(), "can only be changed before building the batch");
        mNumCustomParams = numCustomParams;
    }
    //----------------------------------------------------------------------
    size_t InstanceManager::getMaxOrBestNumInstancesPerBatch( const String &materialName, size_t suggestedSize,
                                                                uint16 flags )
    {
        //Get the material
        MaterialPtr mat = MaterialManager::getSingleton().getByName( materialName,
                                                                    mMeshReference->getGroup() );
        InstanceBatch *batch = 0;

        //Base material couldn't be found
        if( !mat )
            return 0;

        switch( mInstancingTechnique )
        {
        case ShaderBased:
            batch = OGRE_NEW InstanceBatchShader( this, mMeshReference, mat, suggestedSize,
                                                    0, mName + "/TempBatch" );
            break;
        case TextureVTF:
            batch = OGRE_NEW InstanceBatchVTF( this, mMeshReference, mat, suggestedSize,
                                                    0, mName + "/TempBatch" );
            static_cast<InstanceBatchVTF*>(batch)->setBoneDualQuaternions((mInstancingFlags & IM_USEBONEDUALQUATERNIONS) != 0);
            static_cast<InstanceBatchVTF*>(batch)->setUseOneWeight((mInstancingFlags & IM_USEONEWEIGHT) != 0);
            static_cast<InstanceBatchVTF*>(batch)->setForceOneWeight((mInstancingFlags & IM_FORCEONEWEIGHT) != 0);
            break;
        case HWInstancingBasic:
            batch = OGRE_NEW InstanceBatchHW( this, mMeshReference, mat, suggestedSize,
                                                    0, mName + "/TempBatch" );
            break;
        case HWInstancingVTF:
            batch = OGRE_NEW InstanceBatchHW_VTF( this, mMeshReference, mat, suggestedSize,
                                                    0, mName + "/TempBatch" );
            static_cast<InstanceBatchHW_VTF*>(batch)->setBoneMatrixLookup((mInstancingFlags & IM_VTFBONEMATRIXLOOKUP) != 0, mMaxLookupTableInstances);
            static_cast<InstanceBatchHW_VTF*>(batch)->setBoneDualQuaternions((mInstancingFlags & IM_USEBONEDUALQUATERNIONS) != 0);
            static_cast<InstanceBatchHW_VTF*>(batch)->setUseOneWeight((mInstancingFlags & IM_USEONEWEIGHT) != 0);
            static_cast<InstanceBatchHW_VTF*>(batch)->setForceOneWeight((mInstancingFlags & IM_FORCEONEWEIGHT) != 0);
            break;
        default:
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
                    "Unimplemented instancing technique: " +
                    StringConverter::toString(mInstancingTechnique),
                    "InstanceBatch::getMaxOrBestNumInstancesPerBatches()");
        }

        const size_t retVal = batch->calculateMaxNumInstances( mMeshReference->getSubMesh(mSubMeshIdx),
                                                                flags );

        OGRE_DELETE batch;

        return retVal;
    }
    //----------------------------------------------------------------------
    InstancedEntity* InstanceManager::createInstancedEntity( const String &materialName )
    {
        InstanceBatch *instanceBatch;

        if( mInstanceBatches.empty() ||
            (mInstanceBatches[materialName].size () == 0))
            instanceBatch = buildNewBatch( materialName, true );
        else
            instanceBatch = getFreeBatch( materialName );

        return instanceBatch->createInstancedEntity();
    }
    //-----------------------------------------------------------------------
    inline InstanceBatch* InstanceManager::getFreeBatch( const String &materialName )
    {
        InstanceBatchVec &batchVec = mInstanceBatches[materialName];
        for (auto *b : batchVec)
        {
            if(!b->isBatchFull())
                return b;
        }

        //None found, or they're all full
        return buildNewBatch( materialName, false );
    }
    //-----------------------------------------------------------------------
    InstanceBatch* InstanceManager::buildNewBatch( const String &materialName, bool firstTime )
    {
        //Get the bone to index map for the batches
        Mesh::IndexMap &idxMap = mMeshReference->getSubMesh(mSubMeshIdx)->blendIndexToBoneIndexMap;
        idxMap = idxMap.empty() ? mMeshReference->sharedBlendIndexToBoneIndexMap : idxMap;

        //Get the material
        MaterialPtr mat = MaterialManager::getSingleton().getByName( materialName,
                                                                    mMeshReference->getGroup() );

        //Get the array of batches grouped by this material
        InstanceBatchVec &materialInstanceBatch = mInstanceBatches[materialName];

        InstanceBatch *batch = 0;

        switch( mInstancingTechnique )
        {
        case ShaderBased:
            batch = OGRE_NEW InstanceBatchShader( this, mMeshReference, mat, mInstancesPerBatch,
                                                    &idxMap, mName + "/InstanceBatch_" +
                                                    StringConverter::toString(mIdCount++) );
            break;
        case TextureVTF:
            batch = OGRE_NEW InstanceBatchVTF( this, mMeshReference, mat, mInstancesPerBatch,
                                                    &idxMap, mName + "/InstanceBatch_" +
                                                    StringConverter::toString(mIdCount++) );
            static_cast<InstanceBatchVTF*>(batch)->setBoneDualQuaternions((mInstancingFlags & IM_USEBONEDUALQUATERNIONS) != 0);
            static_cast<InstanceBatchVTF*>(batch)->setUseOneWeight((mInstancingFlags & IM_USEONEWEIGHT) != 0);
            static_cast<InstanceBatchVTF*>(batch)->setForceOneWeight((mInstancingFlags & IM_FORCEONEWEIGHT) != 0);
            break;
        case HWInstancingBasic:
            batch = OGRE_NEW InstanceBatchHW( this, mMeshReference, mat, mInstancesPerBatch,
                                                    &idxMap, mName + "/InstanceBatch_" +
                                                    StringConverter::toString(mIdCount++) );
            break;
        case HWInstancingVTF:
            batch = OGRE_NEW InstanceBatchHW_VTF( this, mMeshReference, mat, mInstancesPerBatch,
                                                    &idxMap, mName + "/InstanceBatch_" +
                                                    StringConverter::toString(mIdCount++) );
            static_cast<InstanceBatchHW_VTF*>(batch)->setBoneMatrixLookup((mInstancingFlags & IM_VTFBONEMATRIXLOOKUP) != 0, mMaxLookupTableInstances);
            static_cast<InstanceBatchHW_VTF*>(batch)->setBoneDualQuaternions((mInstancingFlags & IM_USEBONEDUALQUATERNIONS) != 0);
            static_cast<InstanceBatchHW_VTF*>(batch)->setUseOneWeight((mInstancingFlags & IM_USEONEWEIGHT) != 0);
            static_cast<InstanceBatchHW_VTF*>(batch)->setForceOneWeight((mInstancingFlags & IM_FORCEONEWEIGHT) != 0);
            break;
        default:
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
                    "Unimplemented instancing technique: " +
                    StringConverter::toString(mInstancingTechnique),
                    "InstanceBatch::buildNewBatch()");
        }

        batch->_notifyManager( mSceneManager );


        if( !firstTime )
        {
            //TODO: Check different materials have the same mInstancesPerBatch upper limit
            //otherwise we can't share
            batch->buildFrom( mMeshReference->getSubMesh(mSubMeshIdx), mSharedRenderOperation );
        }
        else
        {
            //Ensure we don't request more than we can
            const size_t maxInstPerBatch = batch->calculateMaxNumInstances( mMeshReference->
                                                        getSubMesh(mSubMeshIdx), mInstancingFlags );
            mInstancesPerBatch = std::min( maxInstPerBatch, mInstancesPerBatch );
            batch->_setInstancesPerBatch( mInstancesPerBatch );

            OgreAssert(mInstancesPerBatch, "unsupported instancing technique");
            //TODO: Create a "merge" function that merges all submeshes into one big submesh
            //instead of just sending submesh #0

            //Get the RenderOperation to be shared with further instances.
            mSharedRenderOperation = batch->build( mMeshReference->getSubMesh(mSubMeshIdx) );
        }

        const BatchSettings &batchSettings = mBatchSettings[materialName];
        batch->setCastShadows( batchSettings.setting[CAST_SHADOWS] );

        //Batches need to be part of a scene node so that their renderable can be rendered
        SceneNode *sceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
        sceneNode->attachObject( batch );
        sceneNode->showBoundingBox( batchSettings.setting[SHOW_BOUNDINGBOX] );

        materialInstanceBatch.push_back( batch );

        return batch;
    }
    //-----------------------------------------------------------------------
    void InstanceManager::cleanupEmptyBatches(void)
    {
        //Do this now to avoid any dangling pointer inside mDirtyBatches
        _updateDirtyBatches();

        InstanceBatchMap::iterator itor = mInstanceBatches.begin();
        InstanceBatchMap::iterator end  = mInstanceBatches.end();

        while( itor != end )
        {
            InstanceBatchVec::iterator it = itor->second.begin();
            InstanceBatchVec::iterator en = itor->second.end();

            while( it != en )
            {
                if( (*it)->isBatchUnused() )
                {
                    OGRE_DELETE *it;
                    //Remove it from the list swapping with the last element and popping back
                    size_t idx = it - itor->second.begin();
                    *it = itor->second.back();
                    itor->second.pop_back();

                    //Restore invalidated iterators
                    it = itor->second.begin() + idx;
                    en = itor->second.end();
                }
                else
                    ++it;
            }

            ++itor;
        }

        //By this point it may happen that all mInstanceBatches' objects are also empty
        //however if we call mInstanceBatches.clear(), next time we'll create an InstancedObject
        //we'll end up calling buildFirstTime() instead of buildNewBatch(), which is not the idea
        //(takes more time and will leak the shared render operation)
    }
    //-----------------------------------------------------------------------
    void InstanceManager::defragmentBatches( bool optimizeCull,
                                                InstanceBatch::InstancedEntityVec &usedEntities,
                                                InstanceBatch::CustomParamsVec &usedParams,
                                                InstanceBatchVec &fragmentedBatches )
    {
        InstanceBatchVec::iterator itor = fragmentedBatches.begin();
        InstanceBatchVec::iterator end  = fragmentedBatches.end();

        while( itor != end && !usedEntities.empty() )
        {
            if( !(*itor)->isStatic() )
                (*itor)->_defragmentBatch( optimizeCull, usedEntities, usedParams );
            ++itor;
        }

        InstanceBatchVec::iterator lastImportantBatch = itor;

        while( itor != end )
        {
            if( !(*itor)->isStatic() )
            {
                //If we get here, this means we hit remaining batches which will be unused.
                //Destroy them
                //Call this to avoid freeing InstancedEntities that were just reparented
                (*itor)->_defragmentBatchDiscard();
                OGRE_DELETE *itor;
            }
            else
            {
                //This isn't a meaningless batch, move it forward so it doesn't get wipe
                //when we resize the container (faster than removing element by element)
                *lastImportantBatch++ = *itor;
            }

            ++itor;
        }

        //Remove remaining batches all at once from the vector
        const size_t remainingBatches = end - lastImportantBatch;
        fragmentedBatches.resize( fragmentedBatches.size() - remainingBatches );
    }
    //-----------------------------------------------------------------------
    void InstanceManager::defragmentBatches( bool optimizeCulling )
    {
        //Do this now to avoid any dangling pointer inside mDirtyBatches
        _updateDirtyBatches();

        //Do this for every material
        for (auto& b : mInstanceBatches)
        {
            InstanceBatch::InstancedEntityVec   usedEntities;
            InstanceBatch::CustomParamsVec      usedParams;
            usedEntities.reserve(b.second.size() * mInstancesPerBatch );

            //Collect all Instanced Entities being used by _all_ batches from this material
            for (auto *it : b.second)
            {
                //Don't collect instances from static batches, we assume they're correctly set
                //Plus, we don't want to put InstancedEntities from non-static into static batches
                if (!it->isStatic())
                    it->getInstancedEntitiesInUse(usedEntities, usedParams);
            }

            defragmentBatches(optimizeCulling, usedEntities, usedParams, b.second );
        }
    }
    //-----------------------------------------------------------------------
    void InstanceManager::setSetting( BatchSettingId id, bool value, const String &materialName )
    {
        assert( id < NUM_SETTINGS );

        if( materialName == BLANKSTRING )
        {
            //Setup all existing materials
            for (auto& i : mInstanceBatches)
            {
                mBatchSettings[i.first].setting[id] = value;
                applySettingToBatches( id, value, i.second );
            }
        }
        else
        {
            //Setup a given material
            mBatchSettings[materialName].setting[id] = value;

            InstanceBatchMap::const_iterator itor = mInstanceBatches.find( materialName );
            //Don't crash or throw if the batch with that material hasn't been created yet
            if( itor != mInstanceBatches.end() )
                applySettingToBatches( id, value, itor->second );
        }
    }
    //-----------------------------------------------------------------------
    bool InstanceManager::getSetting( BatchSettingId id, const String &materialName ) const
    {
        assert( id < NUM_SETTINGS );

        BatchSettingsMap::const_iterator itor = mBatchSettings.find( materialName );
        if( itor != mBatchSettings.end() )
            return itor->second.setting[id]; //Return current setting

        //Return default
        return BatchSettings().setting[id];
    }
    bool InstanceManager::hasSettings(const String& materialName) const
    {
        return mBatchSettings.find(materialName) != mBatchSettings.end();
    }
    //-----------------------------------------------------------------------
    void InstanceManager::applySettingToBatches( BatchSettingId id, bool value,
                                                 const InstanceBatchVec &container )
    {
        for (auto *s : container)
        {
            switch(id)
            {
            case CAST_SHADOWS:
                s->setCastShadows(value);
                break;
            case SHOW_BOUNDINGBOX:
                s->getParentSceneNode()->showBoundingBox(value);
                break;
            default:
                break;
            }
        }
    }
    //-----------------------------------------------------------------------
    void InstanceManager::setBatchesAsStaticAndUpdate( bool bStatic )
    {
        for (auto& b : mInstanceBatches)
        {
            for (auto *it : b.second)
            {
                it->setStaticAndUpdate(bStatic);
            }
        }
    }
    //-----------------------------------------------------------------------
    void InstanceManager::_addDirtyBatch( InstanceBatch *dirtyBatch )
    {
        if( mDirtyBatches.empty() )
            mSceneManager->_addDirtyInstanceManager( this );

        mDirtyBatches.push_back( dirtyBatch );
    }
    //-----------------------------------------------------------------------
    void InstanceManager::_updateDirtyBatches(void)
    {
        for (auto *d : mDirtyBatches)
        {
            d->_updateBounds();
        }
        mDirtyBatches.clear();
    }
    //-----------------------------------------------------------------------
    // Helper functions to unshare the vertices
    //-----------------------------------------------------------------------
    typedef std::map<uint32, uint32> IndicesMap;

    template< typename TIndexType >
    void collectUsedIndices(IndicesMap& indicesMap, IndexData* idxData)
    {
        HardwareBufferLockGuard indexLock(idxData->indexBuffer,
                                          idxData->indexStart * sizeof(TIndexType),
                                          idxData->indexCount * sizeof(TIndexType),
                                          HardwareBuffer::HBL_READ_ONLY);
        TIndexType *data = (TIndexType*)indexLock.pData;

        for (size_t i = 0; i < idxData->indexCount; i++)
        {
            TIndexType index = data[i];
            if (indicesMap.find(index) == indicesMap.end())
            {
                //We need to guarantee that the size is read before an entry is added, hence these are on separate lines.
                uint32 size = (uint32)(indicesMap.size());
                indicesMap[index] = size;
            }
        }
    }
    //-----------------------------------------------------------------------
    template< typename TIndexType >
    void copyIndexBuffer(IndexData* idxData, const IndicesMap& indicesMap, uint32 indexStart)
    {
        size_t start = std::max(indexStart, idxData->indexStart);
        size_t count = idxData->indexCount - (start - idxData->indexStart);

        //We get errors if we try to lock a zero size buffer.
        if (count == 0) {
            return;
        }
        HardwareBufferLockGuard indexLock(idxData->indexBuffer,
                                          start * sizeof(TIndexType),
                                          count * sizeof(TIndexType),
                                          HardwareBuffer::HBL_NORMAL);
        TIndexType *data = (TIndexType*)indexLock.pData;

        for (size_t i = 0; i < count; i++)
        {
            //Access data and write on two separate lines, to avoid compiler confusion.
            auto originalPos = data[i];
            auto indexEntry = indicesMap.find(originalPos);
            data[i] = (TIndexType)(indexEntry->second);
        }
    }
    //-----------------------------------------------------------------------
    void InstanceManager::unshareVertices(const Ogre::MeshPtr &mesh)
    {
        // Retrieve data to copy bone assignments
        const Mesh::VertexBoneAssignmentList& boneAssignments = mesh->getBoneAssignments();
        size_t curVertexOffset = 0;

        // Access shared vertices
        VertexData* sharedVertexData = mesh->sharedVertexData;

        for (size_t subMeshIdx = 0; subMeshIdx < mesh->getNumSubMeshes(); subMeshIdx++)
        {
            SubMesh *subMesh = mesh->getSubMesh(subMeshIdx);

            IndexData *indexData = subMesh->indexData;
            HardwareIndexBuffer::IndexType idxType = indexData->indexBuffer->getType();
            IndicesMap indicesMap;
            if (idxType == HardwareIndexBuffer::IT_16BIT) {
                collectUsedIndices<uint16>(indicesMap, indexData);
            } else {
                collectUsedIndices<uint32>(indicesMap, indexData);
            }

            //Also collect indices for all LOD faces.
            for (auto& lodIndex : subMesh->mLodFaceList) {
                //Typically the LOD indices would use the same buffer type as the main index. But we'll check to make extra sure.
                if (lodIndex->indexBuffer->getType() == HardwareIndexBuffer::IT_16BIT) {
                    collectUsedIndices<uint16>(indicesMap, lodIndex);
                } else {
                    collectUsedIndices<uint32>(indicesMap, lodIndex);
                }
            }

            VertexData *newVertexData = new VertexData();
            newVertexData->vertexCount = indicesMap.size();
            newVertexData->vertexDeclaration = sharedVertexData->vertexDeclaration->clone();

            for (uint16 bufIdx = 0; bufIdx < uint16(sharedVertexData->vertexBufferBinding->getBufferCount()); bufIdx++)
            {
                HardwareVertexBufferSharedPtr sharedVertexBuffer = sharedVertexData->vertexBufferBinding->getBuffer(bufIdx);
                size_t vertexSize = sharedVertexBuffer->getVertexSize();                

                HardwareVertexBufferSharedPtr newVertexBuffer = HardwareBufferManager::getSingleton().createVertexBuffer
                    (vertexSize, newVertexData->vertexCount, sharedVertexBuffer->getUsage(), sharedVertexBuffer->hasShadowBuffer());

                HardwareBufferLockGuard oldLock(sharedVertexBuffer, 0, sharedVertexData->vertexCount * vertexSize, HardwareBuffer::HBL_READ_ONLY);
                HardwareBufferLockGuard newLock(newVertexBuffer, 0, newVertexData->vertexCount * vertexSize, HardwareBuffer::HBL_NORMAL);

                for (auto& id : indicesMap)
                {
                    memcpy((uint8*)newLock.pData + vertexSize * id.second,
                           (uint8*)oldLock.pData + vertexSize * id.first, vertexSize);
                }

                newVertexData->vertexBufferBinding->setBinding(bufIdx, newVertexBuffer);
            }

            if (idxType == HardwareIndexBuffer::IT_16BIT)
            {
                copyIndexBuffer<uint16>(indexData, indicesMap, 0);
            }
            else
            {
                copyIndexBuffer<uint32>(indexData, indicesMap, 0);
            }

            //Need to adjust all of the LOD faces too.
            size_t lastIndexEnd = 0;
            for (size_t i = 0; i < subMesh->mLodFaceList.size(); ++i) {
				auto lodIndex = subMesh->mLodFaceList[i];
				//When using "generated" mesh lods, the indices are shared, so that index[n] would overlap with
				//index[n]. We need to take this into account to make sure we don't process data twice (with incorrect
				//data as a result). We check if the index either is the first one, or if it has a different buffer
				//than the previous one we processed. Since the overlap seems to be progressing we only need to keep
				// track of the last used index.
                if (i == 0 || lodIndex->indexBuffer != subMesh->mLodFaceList[i - 1]->indexBuffer) {
                    lastIndexEnd = 0;
                }

                //Typically the LOD indices would use the same buffer type as the main index. But we'll check to make extra sure.
                if (lodIndex->indexBuffer->getType() == HardwareIndexBuffer::IT_16BIT) {
                    copyIndexBuffer<uint16>(lodIndex, indicesMap, lastIndexEnd);
                } else {
                    copyIndexBuffer<uint32>(lodIndex, indicesMap, lastIndexEnd);
                }
                lastIndexEnd = lodIndex->indexStart + lodIndex->indexCount;
			}

            // Store new attributes
            subMesh->resetVertexData(newVertexData);

            // Transfer bone assignments to the submesh
            size_t offset = curVertexOffset + newVertexData->vertexCount;
            for (auto& b : boneAssignments)
            {
                size_t vertexIdx = b.first;
                if (vertexIdx > offset)
                    break;

                VertexBoneAssignment boneAssignment = b.second;
                boneAssignment.vertexIndex = static_cast<unsigned int>(boneAssignment.vertexIndex - curVertexOffset);
                subMesh->addBoneAssignment(boneAssignment);
            }
            curVertexOffset = newVertexData->vertexCount + 1;
        }

        // Release shared vertex data
        mesh->resetVertexData();
        mesh->clearBoneAssignments();

        if( mesh->isEdgeListBuilt() )
        {
            mesh->freeEdgeList();
            mesh->buildEdgeList();
        }
    }
    //-----------------------------------------------------------------------
    InstanceManager::InstanceBatchIterator InstanceManager::getInstanceBatchIterator( const String &materialName ) const
    {
        InstanceBatchMap::const_iterator it = mInstanceBatches.find( materialName );
        if(it != mInstanceBatches.end())
            return InstanceBatchIterator( it->second.begin(), it->second.end() );

        OGRE_EXCEPT(Exception::ERR_INVALID_STATE, "Cannot create instance batch iterator. "
                    "Material " + materialName + " cannot be found");
    }
}
