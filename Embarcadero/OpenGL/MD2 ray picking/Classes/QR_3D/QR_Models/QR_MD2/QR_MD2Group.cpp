/****************************************************************************
 * ==> QR_MD2Group ---------------------------------------------------------*
 ****************************************************************************
 * Description : MD2 model group, contains all items and functions needed   *
 *               to manage a complete MD2 model                             *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - QR Engine                                                  *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sublicense, and/or sell copies of the Software, and to       *
 * permit persons to whom the Software is furnished to do so, subject to    *
 * the following conditions:                                                *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     *
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     *
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        *
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   *
 ****************************************************************************/

#include "QR_MD2Group.h"

// qr engine
#include "QR_STDTools.h"
#include "QR_StdFileBuffer.h"
#include "QR_FileTools.h"
#include "QR_Texture.h"

//---------------------------------------------------------------------------
// QR_MD2Group::IInfo
//---------------------------------------------------------------------------
QR_MD2Group::IInfo::IInfo()
{}
//---------------------------------------------------------------------------
QR_MD2Group::IInfo::~IInfo()
{}
//---------------------------------------------------------------------------
// QR_MD2Group
//---------------------------------------------------------------------------
QR_MD2Group::QR_MD2Group(const QR_Renderer* pRenderer, const QR_Resources* pResources) :
    QR_FramedModelGroup(pRenderer),
    m_Name(L"qr_md2"),
    m_pModel(NULL),
    m_pGesture(NULL),
    m_FrameIndex(0),
    m_InterpolationFactor(0.0),
    m_pResources(pResources)
{
    M_ASSERT(m_pResources);
}
//---------------------------------------------------------------------------
QR_MD2Group::~QR_MD2Group()
{
    DelAndClear();
}
//---------------------------------------------------------------------------
void QR_MD2Group::Clear()
{
    QR_FramedModelGroup::Clear();

    DelAndClear();

    m_CfgFile.Clear();

    m_pGesture            = NULL;
    m_FrameIndex          = 0;
    m_InterpolationFactor = 0.0;
}
//---------------------------------------------------------------------------
void QR_MD2Group::Copy(const QR_ModelGroup& other)
{
    Clear();

    QR_FramedModelGroup::Copy(other);

    // try to get source model
    const QR_MD2Group* pSource = dynamic_cast<const QR_MD2Group*>(&other);

    // found it?
    if (!pSource)
    {
        Clear();
        return;
    }

    // copy the model
    std::auto_ptr<QR_MD2> pModel(new QR_MD2());
    pModel->Copy(*pSource->m_pModel);
    m_pModel = pModel.release();

    // copy the model configuration
    m_CfgFile.Copy(pSource->m_CfgFile);

    // get the texture dictionary from the resources
    QR_Resources::ITextureDictionary* pTextures = m_pResources->GetTextureDictionary();

    // initialize the texture list
    const QR_SizeT textureCount = pSource->m_Textures.size();
    m_Textures.reserve(textureCount);

    // iterate through textures to copy
    for (QR_SizeT i = 0; i < textureCount; ++i)
    {
        const QR_SizeT hashCode = pSource->m_Textures[i]->m_pTexture->m_HashCode;

        // get the matching texture from the resources
        QR_Resources::ITextureDictionary::iterator it = pTextures->find(hashCode);

        // found it?
        if (it != pTextures->end())
        {
            // keep trace of it if found
            std::auto_ptr<QR_ModelTexture> pModelTexture(new QR_ModelTexture());
            pModelTexture->m_Name     = pSource->m_Textures[i]->m_Name;
            pModelTexture->m_Enabled  = pSource->m_Textures[i]->m_Enabled;
            pModelTexture->m_pTexture = it->second;
            m_Textures.push_back(pModelTexture.get());
            pModelTexture.release();
        }
    }

    // copy the current running animation
    SetAnimation(pSource->GetGesture());

    // copy the group attributes
    m_FrameIndex          =  pSource->m_FrameIndex;
    m_InterpolationFactor =  pSource->m_InterpolationFactor;
}
//---------------------------------------------------------------------------
bool QR_MD2Group::Load(const std::wstring& dir, const IInfo& info)
{
    Clear();

    QR_Vertex::IEFormat vertexFormat = QR_Vertex::IE_VF_Colors;

    // build model file name
    const std::wstring modelFileName = QR_FileTools::AppendDelimiter(dir) + info.m_ModelFileName;

    // model file exists?
    if (!QR_FileTools::FileExists(modelFileName))
        return false;

    // create new MD2 model
    m_pModel = new QR_MD2();

    // succeeded?
    if (!m_pModel)
        return false;

    // load model
    if (!m_pModel->Load(modelFileName))
    {
        Clear();
        return false;
    }

    // build model normals file name
    const std::wstring normalsFileName = QR_FileTools::AppendDelimiter(dir) + info.m_NormalsFileName;

    // model normals file exists?
    if (QR_FileTools::FileExists(normalsFileName))
    {
        // load model normals
        if (!m_pModel->LoadNormals(normalsFileName))
        {
            Clear();
            return false;
        }

        vertexFormat = QR_Vertex::IEFormat(vertexFormat | QR_Vertex::IE_VF_Normals);
    }

    // build model texture file name
    const std::wstring textureFileName = QR_FileTools::AppendDelimiter(dir) + info.m_TextureFileName;

    // model texture file exists?
    if (QR_FileTools::FileExists(textureFileName))
    {
        // generate a new texture
        std::auto_ptr<QR_Texture> pTexture(m_pRenderer->GenerateTexture());

        // user want to load texture itself?
        if (m_fOnLoadTexture)
        {
            // create a file buffer and open it for read
            QR_StdFileBuffer buffer;
            buffer.Open(textureFileName, QR_FileBuffer::IE_M_Read);
            buffer.Seek(0, 0);

            if (!m_fOnLoadTexture(this, m_pRenderer, &buffer, pTexture.get()))
            {
                Clear();
                return false;
            }
        }
        else
        {
            // for now only bitmaps are supported
            if (QR_StringTools::ToLowerCase(QR_FileTools::GetFileExtension(textureFileName)) != L"bmp")
            {
                Clear();
                return false;
            }

            QR_SizeT width, height, format, length;
            void*    pPixels = NULL;

            try
            {
                // load texture
                if (!QR_TextureHelper::LoadFromBitmap(textureFileName,
                                                      width,
                                                      height,
                                                      format,
                                                      length,
                                                      pPixels))
                {
                    if (pPixels)
                        delete[] pPixels;

                    Clear();
                    return false;
                }

                // configure texture
                pTexture->m_Width  = width;
                pTexture->m_Height = height;

                switch (format)
                {
                    case 24: pTexture->m_Format = QR_Texture::IE_FT_24bit; break;
                    case 32: pTexture->m_Format = QR_Texture::IE_FT_32bit; break;
                    default: M_THROW_EXCEPTION("Unknown format");
                }

                // create model texture
                if (!pTexture->Create(pPixels))
                {
                    if (pPixels)
                        delete[] pPixels;

                    Clear();
                    return false;
                }
            }
            catch (...)
            {
                if (pPixels)
                    delete[] pPixels;

                throw;
            }

            if (pPixels)
                delete[] pPixels;
        }

        // create a new model texture
        std::auto_ptr<QR_ModelTexture> pModelTexture(new QR_ModelTexture());
        pModelTexture->m_Name     = m_Name;
        pModelTexture->m_Enabled  = true;

        // get the texture dictionary from the resources
        QR_Resources::ITextureDictionary* pTextures = m_pResources->GetTextureDictionary();

        // try to get a matching texture from the resources
        QR_Resources::ITextureDictionary::iterator it = pTextures->find(pTexture->m_HashCode);

        // found a matching texture in the global resources?
        if (it != pTextures->end())
            // yes, keep the reference to the matching texture
            pModelTexture->m_pTexture = it->second;
        else
        {
            // no, add it in the global resources and keep his reference
            (*pTextures)[pTexture->m_HashCode] = pTexture.get();
            pModelTexture->m_pTexture          = pTexture.release();
        }

        // add model texture to local list
        m_Textures.push_back(pModelTexture.get());
        pModelTexture.release();

        vertexFormat = QR_Vertex::IEFormat(vertexFormat | QR_Vertex::IE_VF_TexCoords);
    }

    // build model config file name
    const std::wstring configFileName = QR_FileTools::AppendDelimiter(dir) + info.m_ConfigFileName;

    // model config file exists?
    if (QR_FileTools::FileExists(configFileName))
        // load configuration file and populate gesture dictionary
        if (!m_CfgFile.Load(configFileName))
        {
            Clear();
            return false;
        }

    // set vertex format used by model
    m_pModel->SetVertexFormat(vertexFormat);

    return true;
}
//---------------------------------------------------------------------------
bool QR_MD2Group::Load(QR_MemoryDir& dir, const IInfo& info)
{
    Clear();

    QR_Vertex::IEFormat vertexFormat = QR_Vertex::IE_VF_Colors;

    // model file exists?
    if (!dir.FileExists(info.m_ModelFileName))
        return false;

    // create new MD2 model
    m_pModel = new QR_MD2();

    // succeeded?
    if (!m_pModel)
        return false;

    // get file from memory dir
    QR_Buffer* pBuffer = dir.GetFile(info.m_ModelFileName);

    // succeeded?
    if (!pBuffer)
    {
        Clear();
        return false;
    }

    // seek to start
    pBuffer->Seek(0, 0);

    // load model
    if (!m_pModel->Load(*pBuffer, pBuffer->GetSize()))
    {
        Clear();
        return false;
    }

    // model normals file exists?
    if (dir.FileExists(info.m_NormalsFileName))
    {
        // get file from memory dir
        QR_Buffer* pBuffer = dir.GetFile(info.m_NormalsFileName);

        // succeeded?
        if (!pBuffer)
        {
            Clear();
            return false;
        }

        // seek to start
        pBuffer->Seek(0, 0);

        // load model normals
        if (!m_pModel->LoadNormals(*pBuffer, pBuffer->GetSize()))
        {
            Clear();
            return false;
        }

        vertexFormat = QR_Vertex::IEFormat(vertexFormat | QR_Vertex::IE_VF_Normals);
    }

    // model texture file exists?
    if (dir.FileExists(info.m_TextureFileName))
    {
        // get file from memory dir
        QR_Buffer* pBuffer = dir.GetFile(info.m_TextureFileName);

        // succeeded?
        if (!pBuffer)
        {
            Clear();
            return false;
        }

        // seek to start
        pBuffer->Seek(0, 0);

        // generate a new texture
        std::auto_ptr<QR_Texture> pTexture(m_pRenderer->GenerateTexture());

        // user want to load texture itself?
        if (m_fOnLoadTexture)
        {
            if (!m_fOnLoadTexture(this, m_pRenderer, pBuffer, pTexture.get()))
            {
                Clear();
                return false;
            }
        }
        else
        {
            // for now only bitmaps are supported
            if (QR_StringTools::ToLowerCase(QR_FileTools::GetFileExtension(info.m_TextureFileName)) != L"bmp")
            {
                Clear();
                return false;
            }

            QR_SizeT width, height, format, length;
            void*    pPixels = NULL;

            try
            {
                // load texture
                if (!QR_TextureHelper::LoadFromBitmap(*pBuffer,
                                                       width,
                                                       height,
                                                       format,
                                                       length,
                                                       pPixels))
                {
                    if (pPixels)
                        delete[] pPixels;

                    Clear();
                    return false;
                }

                // configure texture
                pTexture->m_Width  = width;
                pTexture->m_Height = height;

                switch (format)
                {
                    case 24: pTexture->m_Format = QR_Texture::IE_FT_24bit; break;
                    case 32: pTexture->m_Format = QR_Texture::IE_FT_32bit; break;
                    default: M_THROW_EXCEPTION("Unknown format");
                }

                // create model texture
                if (!pTexture->Create(pPixels))
                {
                    if (pPixels)
                        delete[] pPixels;

                    Clear();
                    return false;
                }
            }
            catch (...)
            {
                if (pPixels)
                    delete[] pPixels;

                throw;
            }

            if (pPixels)
                delete[] pPixels;
        }

        // create a new model texture
        std::auto_ptr<QR_ModelTexture> pModelTexture(new QR_ModelTexture());
        pModelTexture->m_Name     = m_Name;
        pModelTexture->m_Enabled  = true;

        // get the texture dictionary from the resources
        QR_Resources::ITextureDictionary* pTextures = m_pResources->GetTextureDictionary();

        // try to get a matching texture from the resources
        QR_Resources::ITextureDictionary::iterator it = pTextures->find(pTexture->m_HashCode);

        // found a matching texture in the global resources?
        if (it != pTextures->end())
            // yes, keep the reference to the matching texture
            pModelTexture->m_pTexture = it->second;
        else
        {
            // no, add it in the global resources and keep his reference
            (*pTextures)[pTexture->m_HashCode] = pTexture.get();
            pModelTexture->m_pTexture          = pTexture.release();
        }

        // add model texture to local list
        m_Textures.push_back(pModelTexture.get());
        pModelTexture.release();

        vertexFormat = QR_Vertex::IEFormat(vertexFormat | QR_Vertex::IE_VF_TexCoords);
    }

    // model config file exists?
    if (dir.FileExists(info.m_ConfigFileName))
    {
        // get file from memory dir
        QR_Buffer* pBuffer = dir.GetFile(info.m_ConfigFileName);

        // succeeded?
        if (!pBuffer)
        {
            Clear();
            return false;
        }

        // seek to start
        pBuffer->Seek(0, 0);

        // load configuration file and populate gesture dictionary
        if (!m_CfgFile.Load(*pBuffer, pBuffer->GetSize()))
        {
            Clear();
            return false;
        }
    }

    // set vertex format used by model
    m_pModel->SetVertexFormat(vertexFormat);

    return true;
}
//---------------------------------------------------------------------------
QR_Vertex::IEFormat QR_MD2Group::GetVertexFormat() const
{
    if (m_pModel)
        return m_pModel->GetVertexFormat();

    return QR_Vertex::IEFormat(0);
}
//---------------------------------------------------------------------------
void QR_MD2Group::SetVertexFormat(QR_Vertex::IEFormat format)
{
    if (!m_pModel)
        return;

    m_pModel->SetVertexFormat(format);

    ResetCache();

    // notify that hash code changed
    if (m_fOnHashCodeChanged)
        m_fOnHashCodeChanged();
}
//---------------------------------------------------------------------------
QR_SizeT QR_MD2Group::GetGesture() const
{
    if (!m_pGesture)
        return 0;

    const QR_SizeT gestureCount = m_CfgFile.GetGestureCount();

    for (QR_SizeT i = 0; i < gestureCount; ++i)
        if (m_pGesture == m_CfgFile.GetGesture(i))
            return i;

    return 0;
}
//---------------------------------------------------------------------------
bool QR_MD2Group::SetAnimation(QR_SizeT index)
{
    m_pGesture = m_CfgFile.GetGesture(index);

    m_FrameIndex          = m_pGesture ? m_pGesture->m_StartFrame : 0;
    m_InterpolationFactor = 0.0;

    return true;
}
//---------------------------------------------------------------------------
void QR_MD2Group::SetLight(const QR_DirectionalLight& light)
{
    if (!m_pModel)
        return;

    m_pModel->SetLight(light);

    ResetCache();

    // notify that hash code changed
    if (m_fOnHashCodeChanged)
        m_fOnHashCodeChanged();
}
//---------------------------------------------------------------------------
void QR_MD2Group::EnableLight(bool value)
{
    if (!m_pModel)
        return;

    m_pModel->EnableLight(value);

    ResetCache();

    // notify that hash code changed
    if (m_fOnHashCodeChanged)
        m_fOnHashCodeChanged();
}
//---------------------------------------------------------------------------
#ifdef USE_SHADER
    void QR_MD2Group::Draw(const QR_Double& elapsedTime, const QR_Shader* pShader)
#else
    void QR_MD2Group::Draw(const QR_Double& elapsedTime)
#endif
{
    // model loaded?
    if (!m_pModel)
        return;

    const QR_Matrix16P matrix = GetMatrix();

    // no gesture defined?
    if (!m_pGesture)
    {
        QR_Mesh*     pMesh;
        QR_AABBTree* pTree;

        // only one frame is allowed or exists. Get mesh from cache, or cache it if needed
        GetOrCacheMesh(0, pMesh, pTree);

        // is user responsible to draw the model?
        if (m_fOnDrawItem)
            #ifdef USE_SHADER
                m_fOnDrawItem(this,
                              m_pModel,
                              m_pRenderer,
                              pShader,
                              m_Textures,
                              matrix,
                              pMesh,
                              NULL,
                              0.0);
            #else
                m_fOnDrawItem(this,
                              m_pModel,
                              m_pRenderer,
                              m_Textures,
                              matrix,
                              pMesh,
                              NULL,
                              0.0);
            #endif
        else
        {
            #ifdef USE_SHADER
                // do use shader?
                if (pShader)
                    // draw mesh
                    m_pRenderer->Draw(*pMesh, matrix, m_Textures, pShader);
                else
            #endif
                    // draw mesh
                    m_pRenderer->Draw(*pMesh, matrix, m_Textures);
        }

        // notify that collisions should be detected
        if (m_fOnDetectCollision)
            m_fOnDetectCollision(this, pMesh, matrix, pTree);

        return;
    }

    const IAnimation animation = GetAnimation(elapsedTime,
                                              m_pGesture->m_FramesPerSecond,
                                              m_pGesture->m_StartFrame,
                                              m_pGesture->m_StartFrame + m_pGesture->m_FrameCount,
                                              m_pGesture->m_FrameLoop,
                                              m_FrameIndex,
                                              m_InterpolationFactor);

    QR_Mesh*     pMesh;
    QR_AABBTree* pTree;
    QR_Mesh*     pNextMesh;
    QR_AABBTree* pNextTree;

    // get meshes and trees to draw
    GetOrCacheMesh(animation.m_FrameIndex,              pMesh,     pTree);
    GetOrCacheMesh(animation.m_InterpolationFrameIndex, pNextMesh, pNextTree);

    // is user responsible to draw the model?
    if (m_fOnDrawItem)
        #ifdef USE_SHADER
            m_fOnDrawItem(this,
                          m_pModel,
                          m_pRenderer,
                          pShader,
                          m_Textures,
                          matrix,
                          pMesh,
                          pNextMesh,
                          animation.m_InterpolationFactor);
        #else
            m_fOnDrawItem(this,
                          m_pModel,
                          m_pRenderer,
                          m_Textures,
                          matrix,
                          pMesh,
                          pNextMesh,
                          animation.m_InterpolationFactor);
        #endif
    else
    {
        #ifdef USE_SHADER
            // do use shader?
            if (pShader)
                // draw mesh
                m_pRenderer->Draw(*pMesh,
                                  *pNextMesh,
                                   matrix,
                                   animation.m_InterpolationFactor,
                                   m_Textures,
                                   pShader);
            else
        #endif
            {
                QR_Mesh meshToDraw;

                try
                {
                    // interpolate the meshes
                    if (!QR_FramedModel::Interpolate( animation.m_InterpolationFactor,
                                                     *pMesh,
                                                     *pNextMesh,
                                                      meshToDraw))
                        return;

                    // draw mesh
                    m_pRenderer->Draw(meshToDraw, matrix, m_Textures);
                }
                catch (...)
                {
                    QR_STDTools::DelAndClear(meshToDraw);
                    throw;
                }

                QR_STDTools::DelAndClear(meshToDraw);
            }
    }

    // notify that collisions should be detected
    if (m_fOnDetectCollision)
        m_fOnDetectCollision(this, pMesh, matrix, pTree);
}
//---------------------------------------------------------------------------
QR_UInt32 QR_MD2Group::GetHashCode() const
{
    // get model hash code
    QR_UInt32 hashCode = m_pModel->GetParser()->GetHashCode();

    // get texture count
    const QR_SizeT textureCount = m_Textures.size();

    // iterate through textures and update hash code
    for (QR_SizeT i = 0; i < textureCount; ++i)
    {
        const QR_UInt32 texHash = m_Textures[i]->m_pTexture->m_HashCode;
        hashCode                = QR_Hash::CRC32((QR_UInt8*)&texHash, sizeof(QR_UInt32), hashCode);
    }

    const QR_Vertex::IEFormat vertexFormat = GetVertexFormat();

    // include the vertex format in the hash code (vertex format influences on which the vertex are
    // generated, making thus the model unique)
    hashCode = QR_Hash::CRC32((QR_UInt8*)&vertexFormat, sizeof(QR_Vertex::IEFormat), hashCode);

    const QR_UInt32 modelColor = m_pModel->GetColor().GetARGB();

    // include the model color in the hash code (model color influences on which the vertex are
    // generated, making thus the model unique)
    hashCode = QR_Hash::CRC32((QR_UInt8*)&modelColor, sizeof(QR_UInt32), hashCode);

    const QR_UInt32 color   = m_pModel->GetLight().m_Color.GetARGB();
    const QR_UInt32 ambient = m_pModel->GetLight().m_Ambient.GetARGB();

    // include the pre-calculated light state in the hash code (pre-calculated light influences on
    // which the vertex are generated, making thus the model unique)
    hashCode = QR_Hash::CRC32((QR_UInt8*)&color,                                sizeof(QR_UInt32), hashCode);
    hashCode = QR_Hash::CRC32((QR_UInt8*)&ambient,                              sizeof(QR_UInt32), hashCode);
    hashCode = QR_Hash::CRC32((QR_UInt8*)&m_pModel->GetLight().m_Direction.m_X, sizeof(float),     hashCode);
    hashCode = QR_Hash::CRC32((QR_UInt8*)&m_pModel->GetLight().m_Direction.m_Y, sizeof(float),     hashCode);
    hashCode = QR_Hash::CRC32((QR_UInt8*)&m_pModel->GetLight().m_Direction.m_Z, sizeof(float),     hashCode);
    hashCode = QR_Hash::CRC32((QR_UInt8*)&m_pModel->GetLight().m_Enabled,       sizeof(bool),      hashCode);

    return hashCode;
}
//---------------------------------------------------------------------------
void QR_MD2Group::GetState(IState* pState) const
{
    QR_ModelGroup::GetState(pState);

    if (!pState)
        return;

    // clear previous animation dictionary
    QR_STDTools::DelAndClear(pState->m_AnimDict);

    // copy animation from model into animation state item
    std::auto_ptr<IState::IAnimationStateItem> pItem(new IState::IAnimationStateItem());
    pItem->m_Gesture             = GetGesture();
    pItem->m_FrameIndex          = m_FrameIndex;
    pItem->m_InterpolationFactor = m_InterpolationFactor;
    pState->m_AnimDict[m_Name]   = pItem.get();
    pItem.release();
}
//---------------------------------------------------------------------------
void QR_MD2Group::SetState(const IState* pState)
{
    QR_ModelGroup::SetState(pState);

    if (!pState)
        return;

    // get animation state item to restore
    IState::IAnimationDictionary::const_iterator it = pState->m_AnimDict.find(m_Name);

    // found it?
    if (it == pState->m_AnimDict.end())
        return;

    // restore model animation from state item
    m_pGesture            = m_CfgFile.GetGesture(it->second->m_Gesture);
    m_FrameIndex          = it->second->m_FrameIndex;
    m_InterpolationFactor = it->second->m_InterpolationFactor;
}
//---------------------------------------------------------------------------
void QR_MD2Group::ResetCache()
{
    // delete mesh cache content
    for (IMeshCache::iterator it = m_MeshCache.begin(); it != m_MeshCache.end(); ++it)
        QR_STDTools::DelAndClear(*it->second);

    QR_STDTools::DelAndClear(m_MeshCache);
    QR_STDTools::DelAndClear(m_TreeCache);
}
//---------------------------------------------------------------------------
void QR_MD2Group::DelAndClear()
{
    ResetCache();

    QR_STDTools::DelAndClear(m_Textures);

    // delete model, if exists
    if (m_pModel)
    {
        delete m_pModel;
        m_pModel = NULL;
    }
}
//---------------------------------------------------------------------------
void QR_MD2Group::GetOrCacheMesh(QR_SizeT index, QR_Mesh*& pMesh, QR_AABBTree*& pTree)
{
    pMesh = NULL;
    pTree = NULL;

    // get mesh from cache
    IMeshCache::iterator itMesh = m_MeshCache.find(index);

    // found it?
    if (itMesh != m_MeshCache.end())
        pMesh = itMesh->second;

    // get tree from cache
    ITreeCache::iterator itTree = m_TreeCache.find(index);

    // found it?
    if (itTree != m_TreeCache.end())
        pTree = itTree->second;

    // can use mesh and tree from cache?
    if (pMesh && pTree)
        return;

    M_ASSERT(!pMesh && !pTree);

    std::auto_ptr<QR_Mesh>     pNewMesh(new QR_Mesh());
    std::auto_ptr<QR_AABBTree> pNewTree(new QR_AABBTree());

    // calculate mesh and tree
    m_pModel->GetMesh(index, *pNewMesh.get(), pNewTree.get());

    const QR_SizeT verticesCount = pNewMesh->size();

    for (QR_SizeT i = 0; i < verticesCount; ++i)
        (*pNewMesh)[i]->m_Name = m_Name;

    // cache newly calculated mesh
    m_MeshCache[index] = pNewMesh.get();
    pMesh              = pNewMesh.release();

    // cache newly calculated tree
    m_TreeCache[index] = pNewTree.get();
    pTree              = pNewTree.release();
}
//---------------------------------------------------------------------------
