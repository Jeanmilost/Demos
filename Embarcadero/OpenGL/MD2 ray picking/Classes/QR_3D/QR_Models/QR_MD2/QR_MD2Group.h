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

#ifndef QR_MD2GroupH
#define QR_MD2GroupH

// std
#include <string>

// qr engine
#include "QR_FramedModelGroup.h"
#include "QR_MemoryDir.h"
#include "QR_Texture.h"
#include "QR_MD2.h"
#include "QR_MD2CfgFile.h"
#include "QR_Resources.h"

/**
* MD2 model group, contains all items and functions needed to manage a complete MD2 model
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_MD2Group : public QR_FramedModelGroup
{
    public:
        /**
        * MD2 group info, contains all informations to load the group. The files representing a
        * whole MD2 model are (in general):
        * - A file containign the model (.md2). This file must always appear
        * - A file containing the model normals (.bin). If this file is not provided, the mesh
        *   vertices will be generated without normals, and the lights will not be usable with this
        *   model
        * - A file containing the model texture (.bmp). If this file is not provided, the model will
        *   be drawn without texture
        * - A file containing the model configuration (.cfg). If this file is not provided, the
        *   model gestures will be unavailable, and the model will always be drawn showing the first
        *   found frame
        */
        struct IInfo
        {
            std::wstring m_ModelFileName;
            std::wstring m_NormalsFileName;
            std::wstring m_TextureFileName;
            std::wstring m_ConfigFileName;

            IInfo();
            virtual ~IInfo();
        };

        /**
        * Constructor
        *@param pRenderer - renderer to use to draw the model
        *@param pResources - the global scene resources
        */
        QR_MD2Group(const QR_Renderer* pRenderer, const QR_Resources* pResources);

        /**
        * Destructor
        */
        virtual ~QR_MD2Group();

        /**
        * Creates a new model instance
        *@return new model instance
        */
        virtual inline QR_ModelGroup* CreateInstance() const;

        /**
        * Clears group
        */
        virtual void Clear();

        /**
        * Copies content from another group
        *@param other - other group to copy from
        */
        virtual void Copy(const QR_ModelGroup& other);

        /**
        * Loads group from dir
        *@param dir - dir containing all model files to load
        *@param info - group info
        *@return true on success, otherwise false
        */
        virtual bool Load(const std::wstring& dir, const IInfo& info);

        /**
        * Loads group from memory dir
        *@param dir - memory dir containing all model files to load
        *@param info - group info
        *@return true on success, otherwise false
        */
        virtual bool Load(QR_MemoryDir& dir, const IInfo& info);

        /**
        * Gets vertex format
        *@return vertex format
        */
        virtual QR_Vertex::IEFormat GetVertexFormat() const;

        /**
        * Sets vertex format
        *@param format - vertex format
        *@note Changing the vertex format while model is running will also reset the cache
        */
        virtual void SetVertexFormat(QR_Vertex::IEFormat format);

        /**
        * Gets gesture index
        *@return gesture index
        */
        virtual QR_SizeT GetGesture() const;

        /**
        * Selects animation to run
        *@param index - gesture index
        *@return true on success, otherwise false
        */
        virtual bool SetAnimation(QR_SizeT index);

        /**
        * Sets pre-calculated light
        *@param pLight - pre-calculated light to apply to this model
        *@note Changing the pre-calculated light while model is running will also reset the cache
        */
        virtual void SetLight(const QR_DirectionalLight& light);

        /**
        * Enables or disables the pre-calculated light
        *@param value - if true, pre-calculated light is enabled, otherwise disabled
        *@note Enabling or disabling the pre-calculated light while model is running will also reset
        *      the cache
        */
        virtual void EnableLight(bool value);

        /**
        * Draws group
        *@param elapsedTime - elapsed time since last draw
        *@param pShader - shader to use to draw the model, can be NULL
        */
        #ifdef USE_SHADER
            virtual void Draw(const QR_Double& elapsedTime, const QR_Shader* pShader);
        #else
            virtual void Draw(const QR_Double& elapsedTime);
        #endif

        /**
        * Gets the hash code representing the model content
        *@return the hash code representing the model content
        */
        virtual QR_UInt32 GetHashCode() const;

        /**
        * Gets the model state
        *@param pState - state structure that will receive the model state
        */
        virtual void GetState(IState* pState) const;

        /**
        * Sets the model state
        *@param pState - state structure containing the model state to set
        */
        virtual void SetState(const IState* pState);

        /**
        * Resets the model cache
        */
        virtual void ResetCache();

    protected:
        std::wstring             m_Name;
        QR_MD2*                  m_pModel;
        QR_MD2CfgFile            m_CfgFile;
        QR_MD2CfgFile::IGesture* m_pGesture;
        QR_ModelTextures         m_Textures;
        QR_SizeT                 m_FrameIndex;
        QR_Double                m_InterpolationFactor;

    private:
        typedef std::map<QR_SizeT, QR_Mesh*>     IMeshCache;
        typedef std::map<QR_SizeT, QR_AABBTree*> ITreeCache;

        const QR_Resources* m_pResources;
              IMeshCache    m_MeshCache;
              ITreeCache    m_TreeCache;

        /**
        * Deletes and clears all resources
        */
        void DelAndClear();

        /**
        * Gets mesh from cache, calculates and caches it if not cached
        *@param index - frame index to get or cache
        *@param[out] pMesh - mesh to get
        *@param[out] pTree - AABB tree to get
        */
        void GetOrCacheMesh(QR_SizeT index, QR_Mesh*& pMesh, QR_AABBTree*& pTree);
};

//---------------------------------------------------------------------------
// QR_MD2Group
//---------------------------------------------------------------------------
QR_ModelGroup* QR_MD2Group::CreateInstance() const
{
    return new QR_MD2Group(m_pRenderer, m_pResources);
}
//---------------------------------------------------------------------------

#endif
