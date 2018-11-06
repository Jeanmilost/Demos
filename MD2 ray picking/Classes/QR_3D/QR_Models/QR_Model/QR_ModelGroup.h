/****************************************************************************
 * ==> QR_ModelGroup -------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a basic class to implement a model group. A model *
 *               group is generally composed by all the items (mesh,        *
 *               textures, ...) required to load and draw the whole model   *
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

#ifndef QR_ModelGroupH
#define QR_ModelGroupH

// std
#include <map>

// qr engine
#include "QR_Vector3D.h"
#include "QR_Matrix16.h"
#include "QR_Buffer.h"
#include "QR_Texture.h"
#include "QR_AABBTree.h"
#include "QR_Renderer.h"
#include "QR_Model.h"

#ifdef USE_SHADER
    #include "QR_Shader.h"
#endif

/**
* Model group, contains all items needed to manage a whole model
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_ModelGroup
{
    public:
        /**
        * Matrix combination type enumeration
        */
        enum IECombinationType
        {
            IE_CT_Scale_Rotate_Translate,
            IE_CT_Scale_Translate_Rotate,
            IE_CT_Rotate_Translate_Scale,
            IE_CT_Rotate_Scale_Translate,
            IE_CT_Translate_Rotate_Scale,
            IE_CT_Translate_Scale_Rotate,
        };

        /**
        * Model state, can be used to share the same group to render several identical models
        */
        struct IState
        {
            /**
            * Animation state item, contains the current animation state for each sub-models
            */
            struct IAnimationStateItem
            {
                QR_SizeT  m_Gesture;
                QR_SizeT  m_FrameIndex;
                QR_SizeT  m_InterpolationFrameIndex;
                QR_Double m_InterpolationFactor;
                bool      m_Loop;

                IAnimationStateItem();
                virtual ~IAnimationStateItem();
            };

            typedef std::map<std::wstring, IAnimationStateItem*> IAnimationDictionary;

            QR_Vector3DP         m_Scaling;
            QR_Vector3DP         m_Translation;
            float                m_RotationX;
            float                m_RotationY;
            float                m_RotationZ;
            IECombinationType    m_CombinationType;
            QR_Matrix16P*        m_pInitialMatrix;
            IAnimationDictionary m_AnimDict;

            IState();
            virtual ~IState();
        };

        /**
        * Called when model hash code changed
        */
        #if defined(CP_EMBARCADERO)
            typedef void (__closure *ITfOnHashCodeChanged)();
        #else
            typedef void (*ITfOnHashCodeChanged)();
        #endif

        /**
        * Called when texture should be loaded
        *@param pSender - model group that raised the event
        *@param pRenderer - renderer used to load the texture
        *@param pBuffer - buffer containing the texture to load
        *@param[in, out] pTexture - texture info, contains loaded index when function ends
        *@return true on success, otherwise false
        */
        #if defined(CP_EMBARCADERO)
            typedef bool (__closure *ITfOnLoadTexture)(const QR_ModelGroup* pSender,
                                                       const QR_Renderer*   pRenderer,
                                                             QR_Buffer*     pBuffer,
                                                             QR_Texture*    pTexture);
        #else
            typedef bool (*ITfOnLoadTexture)(const QR_ModelGroup* pSender,
                                             const QR_Renderer*   pRenderer,
                                                   QR_Buffer*     pBuffer,
                                                   QR_Texture*    pTexture);
        #endif

        /**
        * Called when model item should be drawn
        *@param pSender - model group that raised the event
        *@param pModel - model to draw
        *@param pRenderer - renderer used to draw the model
        *@param pShader - shader used to draw the model, can be NULL
        *@param textures - textures belonging to model, in the order where they should be combined
        *@param matrix - model matrix
        *@param pMesh - mesh to draw
        *@param pNextMesh - next mesh to interpolate with, can be NULL
        *@param interpolationFactor - interpolation factor
        */
        #if defined(CP_EMBARCADERO)
            #ifdef USE_SHADER
                typedef void (__closure *ITfOnDrawItem)(const QR_ModelGroup*    pSender,
                                                        const QR_Model*         pModel,
                                                        const QR_Renderer*      pRenderer,
                                                        const QR_Shader*        pShader,
                                                        const QR_ModelTextures& textures,
                                                        const QR_Matrix16P&     matrix,
                                                        const QR_Mesh*          pMesh,
                                                        const QR_Mesh*          pNextMesh,
                                                              QR_Double         interpolationFactor);
            #else
                typedef void (__closure *ITfOnDrawItem)(const QR_ModelGroup*    pSender,
                                                        const QR_Model*         pModel,
                                                        const QR_Renderer*      pRenderer,
                                                        const QR_ModelTextures& textures,
                                                        const QR_Matrix16P&     matrix,
                                                        const QR_Mesh*          pMesh,
                                                        const QR_Mesh*          pNextMesh,
                                                              QR_Double         interpolationFactor);
            #endif
        #else
            #ifdef USE_SHADER
                typedef void (*ITfOnDrawItem)(const QR_ModelGroup*    pSender,
                                              const QR_Model*         pModel,
                                              const QR_Renderer*      pRenderer,
                                              const QR_ModelTextures& textures,
                                              const QR_Matrix16P&     matrix,
                                              const QR_Mesh*          pMesh,
                                              const QR_Mesh*          pNextMesh,
                                                    QR_Double         interpolationFactor);
            #else
                typedef void (*ITfOnDrawItem)(const QR_ModelGroup*    pSender,
                                              const QR_Model*         pModel,
                                              const QR_Renderer*      pRenderer,
                                              const QR_ModelTextures& textures,
                                              const QR_Matrix16P&     matrix,
                                              const QR_Mesh*          pMesh,
                                              const QR_Mesh*          pNextMesh,
                                                    QR_Double         interpolationFactor);
            #endif
        #endif

        /**
        * Called when collision should be detected
        *@param pSender - model group that raised the event
        *@param pMesh - mesh to check
        *@param modelMatrix - model matrix
        *@param pAABBTree - aligned-axis bounding box tree containing polygons to check
        */
        #if defined(CP_EMBARCADERO)
            typedef void (__closure *ITfOnDetectCollision)(const QR_ModelGroup* pSender,
                                                           const QR_Mesh*       pMesh,
                                                           const QR_Matrix16P&  modelMatrix,
                                                           const QR_AABBTree*   pAABBTree);
        #else
            typedef void (*ITfOnDetectCollision)(const QR_ModelGroup* pSender,
                                                 const QR_Mesh*       pMesh,
                                                 const QR_Matrix16P&  modelMatrix,
                                                 const QR_AABBTree*   pAABBTree);
        #endif

        /**
        * Constructor
        *@param pRenderer - renderer to use to draw the model
        */
        QR_ModelGroup(const QR_Renderer* pRenderer);

        /**
        * Destructor
        */
        virtual ~QR_ModelGroup();

        /**
        * Creates a new model instance
        *@return new model instance
        */
        virtual inline QR_ModelGroup* CreateInstance() const = 0;

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
        * Gets translation
        *@return Translation
        */
        virtual QR_Vector3DP GetTranslation() const;

        /**
        * Sets translation
        *@param Translation - Translation
        */
        virtual void SetTranslation(const QR_Vector3DP& translation);

        /**
        * Gets rotation on X axis
        *@return rotation angle on X axis in radians
        */
        virtual M_Precision GetRotationX() const;

        /**
        * Sets rotation on X axis
        *@param angle - rotation angle in radians
        */
        virtual void SetRotationX(const M_Precision& angle);

        /**
        * Gets rotation on Y axis
        *@return rotation angle on Y axis in radians
        */
        virtual M_Precision GetRotationY() const;

        /**
        * Sets rotation on Y axis
        *@param angle - rotation angle in radians
        */
        virtual void SetRotationY(const M_Precision& angle);

        /**
        * Gets rotation on Z axis
        *@return rotation angle on Z axis in radians
        */
        virtual M_Precision GetRotationZ() const;

        /**
        * Sets rotation on Z axis
        *@param angle - rotation angle in radians
        */
        virtual void SetRotationZ(const M_Precision& angle);

        /**
        * Gets scaling
        *@return scaling
        */
        virtual QR_Vector3DP GetScaling() const;

        /**
        * Sets scaling
        *@param scaling - scaling
        */
        virtual void SetScaling(const QR_Vector3DP& scaling);

        /**
        * Gets model matrix combination type
        *@return matrix combination type
        */
        virtual IECombinationType GetMatrixCombinationType() const;

        /**
        * Sets model matrix combination type
        *@param type - matrix combination type
        */
        virtual void SetMatrixCombinationType(IECombinationType type);

        /**
        * Gets initial model matrix
        *@return matrix
        */
        virtual QR_Matrix16P* GetInitialMatrix() const;

        /**
        * Sets initial model matrix
        *@param matrix - matrix
        */
        virtual void SetInitialMatrix(const QR_Matrix16P* pMatrix);

        /**
        * Sets pre-calculated light
        *@param pLight - pre-calculated light to apply to this model
        *@note Changing the pre-calculated light while model is running will also reset the cache
        */
        virtual void SetLight(const QR_DirectionalLight& light) = 0;

        /**
        * Enables or disables the pre-calculated light
        *@param value - if true, pre-calculated light is enabled, otherwise disabled
        *@note Enabling or disabling the pre-calculated light while model is running will also reset
        *      the cache
        */
        virtual void EnableLight(bool value) = 0;

        /**
        * Gets vertex format
        *@return vertex format
        *@note Changing the vertex format while model is running will also reset the cache
        */
        virtual QR_Vertex::IEFormat GetVertexFormat() const = 0;

        /**
        * Sets vertex format
        *@param format - vertex format
        */
        virtual void SetVertexFormat(QR_Vertex::IEFormat format) = 0;

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
        * Draws the group
        *@param elapsedTime - elapsed time since last draw
        *@param pShader - shader to use to draw the model
        */
        #ifdef USE_SHADER
            virtual void Draw(const QR_Double& elapsedTime, const QR_Shader* pShader) = 0;
        #else
            virtual void Draw(const QR_Double& elapsedTime) = 0;
        #endif

        /**
        * Gets the hash code representing the model content
        *@return the hash code representing the model content
        */
        virtual QR_UInt32 GetHashCode() const = 0;

        /**
        * Sets OnHashCodeChanged callback
        *@param fHandler - function handler
        */
        virtual void Set_OnHashCodeChanged(ITfOnHashCodeChanged fHandler);

        /**
        * Sets OnLoadTexture callback
        *@param fHandler - function handler
        */
        virtual void Set_OnLoadTexture(ITfOnLoadTexture fHandler);

        /**
        * Sets OnDrawItem callback
        *@param fHandler - function handler
        */
        virtual void Set_OnDrawItem(ITfOnDrawItem fHandler);

        /**
        * Gets OnDetectCollision callback
        *@return OnDetectCollision callback
        */
        virtual ITfOnDetectCollision Get_OnDetectCollision() const;

        /**
        * Sets OnDetectCollision callback
        *@param fHandler - function handler
        */
        virtual void Set_OnDetectCollision(ITfOnDetectCollision fHandler);

    protected:
        /**
        * Rotation data
        */
        struct IRotation
        {
            M_Precision  m_Angle;
            QR_Vector3DP m_Axis;

            /**
            * Constructor
            */
            IRotation();

            /**
            * Constructor
            *@param angle - angle
            *@param axis - rotation axis
            */
            IRotation(const M_Precision& angle, const QR_Vector3DP& axis);

            /**
            * Destructor
            */
            virtual ~IRotation();
        };

        const QR_Renderer*         m_pRenderer;
              QR_Vector3DP         m_Scaling;
              QR_Vector3DP         m_Translation;
              IRotation            m_RotationX;
              IRotation            m_RotationY;
              IRotation            m_RotationZ;
              IECombinationType    m_CombinationType;
              QR_Matrix16P*        m_pInitialMatrix;
              ITfOnHashCodeChanged m_fOnHashCodeChanged;
              ITfOnLoadTexture     m_fOnLoadTexture;
              ITfOnDrawItem        m_fOnDrawItem;
              ITfOnDetectCollision m_fOnDetectCollision;

        /**
        * Gets model matrix
        *@return matrix
        */
        virtual QR_Matrix16P GetMatrix() const;
};

#endif
