/****************************************************************************
 * ==> Model ---------------------------------------------------------------*
 ****************************************************************************
 * Description : 3D model                                                   *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - iqm model reader                                           *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sub-license, and/or sell copies of the Software, and to      *
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

#pragma once

// classes
#include "Color.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Vertex.h"

/**
* Model
*@author Jean-Milost Reymond
*/
class Model
{
    public:
        /**
        * Animation key type
        */
        enum class IEAnimKeyType
        {
            IE_KT_Unknown    = -1,
            IE_KT_Rotation   =  0,
            IE_KT_Scale      =  1,
            IE_KT_Position   =  2,
            IE_KT_MatrixKeys =  4
        };

        /**
        * Bone, it's a hierarchical local transformation to apply to a mesh
        */
        struct IBone
        {
            typedef std::vector<IBone*> IBones;

            std::string m_Name;     // bone name
            Matrix4x4F  m_Matrix;   // matrix containing the bone transformation to apply
            IBone*      m_pParent;  // bone parent, root bone if nullptr
            IBones      m_Children; // bone children
            void*       m_pCustom;  // custom data, depends on implementation

            IBone();
            virtual ~IBone();
        };

        /**
        * Weights
        */
        typedef std::vector<float> IWeights;

        /**
        * Vertex weight influence, it's a table containing the indices of each vertex influenced by a weight
        */
        struct IWeightInfluence
        {
            typedef std::vector<std::size_t> IVertexIndex;

            IVertexIndex m_VertexIndex;

            IWeightInfluence();
            virtual ~IWeightInfluence();
        };

        /**
        * Vertex weight influences
        */
        typedef std::vector<IWeightInfluence*> IWeightInfluences;

        /**
        * Skin weights, it's a group of vertices influenced by a bone
        */
        struct ISkinWeights
        {
            std::string       m_BoneName;            // linked bone name (required to find the bone in skeleton)
            IBone*            m_pBone;               // linked bone
            Matrix4x4F        m_Matrix;              // matrix to transform the mesh vertices to the bone space
            Matrix4x4F        m_TransformMatrix;     // transform matrix (used in FBX files)
            Matrix4x4F        m_TransformLinkMatrix; // transform link matrix (used in FBX files)
            IWeightInfluences m_WeightInfluences;    // table allowing to retrieve the vertices influenced by a weight
            IWeights          m_Weights;             // weights indicating the bone influence on vertices, between 0.0f and 1.0f

            ISkinWeights();
            virtual ~ISkinWeights();
        };

        /**
        * Mesh deformers, it's a list of skin weights belonging to a mesh
        */
        struct IDeformers
        {
            typedef std::vector<ISkinWeights*> ISkinWeightsData;

            ISkinWeightsData m_SkinWeights;

            IDeformers();
            virtual ~IDeformers();
        };

        /**
        * Animation key, may be a rotation, a translation, a scale, a matrix, ...
        */
        struct IAnimationKey
        {
            typedef std::vector<float> IValues;

            std::size_t m_Frame;
            long long   m_TimeStamp;
            IValues     m_Values;

            IAnimationKey();
            virtual ~IAnimationKey();
        };

        /**
        * Animation key list
        */
        struct IAnimationKeys
        {
            typedef std::vector<IAnimationKey*> IKeys;

            IEAnimKeyType m_Type;
            IKeys         m_Keys;

            IAnimationKeys();
            virtual ~IAnimationKeys();
        };

        /**
        * Animation
        */
        struct IAnimation
        {
            typedef std::vector<IAnimationKeys*> IKeys;

            std::string m_BoneName;
            IBone*      m_pBone;
            IKeys       m_Keys;

            IAnimation();
            virtual ~IAnimation();
        };

        /**
        * Set of animations
        */
        struct IAnimationSet
        {
            typedef std::vector<IAnimation*> IAnimations;

            IAnimations m_Animations; // animations belonging to this set
            long long   m_MaxValue;   // maximum value the animation may reach before looping or stopping

            IAnimationSet();
            virtual ~IAnimationSet();
        };

        std::vector<Mesh*>          m_Mesh;         // meshes composing the model
        std::vector<IDeformers*>    m_Deformers;    // mesh deformers, sorted in the same order as the meshes
        std::vector<IAnimationSet*> m_AnimationSet; // set of animations to apply to bones
        IBone*                      m_pSkeleton;    // model skeleton
        bool                        m_MeshOnly;     // if activated, only the mesh will be drawn. All other data will be ignored
        bool                        m_PoseOnly;     // if activated, the model will take the default pose but will not be animated

        Model();
        virtual ~Model();

        /**
        * Finds a bone in the skeleton
        *@param pBone - root bone to search from
        *@param name - bone name to find
        *@return the bone, nullptr if not found or on error
        */
        virtual IBone* FindBone(IBone* pBone, const std::string& name) const;

        /**
        * Gets the bone animation matrix
        *@param pBone - skeleton root bone
        *@param initialMatrix - the initial matrix
        *@param[out] matrix - animation matrix
        */
        virtual void GetBoneMatrix(const IBone* pBone, const Matrix4x4F& initialMatrix, Matrix4x4F& matrix) const;
};
