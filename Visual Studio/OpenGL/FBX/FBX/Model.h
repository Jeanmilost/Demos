/****************************************************************************
 * ==> Model ---------------------------------------------------------------*
 ****************************************************************************
 * Description : 3D model                                                   *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - fbx model reader                                           *
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
        enum IEAnimKeyType
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
            IBone*      m_pParent;  // bone parent, root bone if 0
            IBones      m_Children; // bone children

            IBone();
            virtual ~IBone();
        };

        /**
        * Weights
        */
        typedef std::vector<float> IWeights;

        /**
        * Vertex weight influence table, it's a table containing the indices of each vertex influenced by a weight
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
            std::string       m_BoneName;         // linked bone name (required to find the bone in skeleton)
            IBone*            m_pBone;            // linked bone
            Matrix4x4F        m_Matrix;           // matrix to transform the mesh vertices to the bone space
            IWeightInfluences m_WeightInfluences; // table allowing to retrieve the vertices influenced by a weight
            IWeights          m_Weights;          // weights indicating the bone influence on vertices, between 0.0f and 1.0f

            ISkinWeights();
            virtual ~ISkinWeights();
        };

        /**
        * Skin weights belonging to a mesh
        */
        struct IMeshSkinWeights
        {
            typedef std::vector<ISkinWeights*> ISkinWeightsData;

            ISkinWeightsData m_SkinWeights;

            IMeshSkinWeights();
            virtual ~IMeshSkinWeights();
        };

        /**
        * Animation key, may be a rotation, a translation, a scale, a matrix, ...
        */
        struct IAnimationKey
        {
            typedef std::vector<float> IValues;

            std::size_t m_Frame;
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

            IAnimations m_Animations;

            IAnimationSet();
            virtual ~IAnimationSet();
        };

        std::vector<Mesh*>             m_Mesh;         // meshes composing the model
        std::vector<IMeshSkinWeights*> m_MeshWeights;  // mesh skin weights, sorted in the same order as the meshes
        std::vector<IAnimationSet*>    m_AnimationSet; // set of animations to apply to bones
        IBone*                         m_pSkeleton;    // model skeleton
        bool                           m_MeshOnly;     // if activated, only the mesh will be drawn. All other data will be ignored
        bool                           m_PoseOnly;     // if activated, the model will take the default pose but will not be animated

        Model();
        virtual ~Model();
};