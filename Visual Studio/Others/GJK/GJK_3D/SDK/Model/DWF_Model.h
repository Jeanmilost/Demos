/****************************************************************************
 * ==> DWF_Model -----------------------------------------------------------*
 ****************************************************************************
 * Description : 3D model                                                   *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - DwarfStar Game Engine                                      *
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
#include "DWF_Color.h"
#include "DWF_Material.h"
#include "DWF_Vector2.h"
#include "DWF_Vector3.h"
#include "DWF_Matrix4x4.h"

namespace DWF_Model
{
    /**
    * Vertex format
    *@author Jean-Milost Reymond
    */
    class VertexFormat
    {
        public:
            /**
            * Vertex buffer type enumeration
            */
            enum class IEType
            {
                IE_VT_Unknown = 0,
                IE_VT_Triangles,
                IE_VT_TriangleStrip,
                IE_VT_TriangleFan,
                IE_VT_Quads,
                IE_VT_QuadStrip,
            };

            /**
            * Vertex format enumeration
            *@note Flags can be combined
            */
            enum class IEFormat
            {
                IE_VF_None      = 0x00,
                IE_VF_Normals   = 0x01, // each vertex contains a normal
                IE_VF_TexCoords = 0x02, // each vertex contains an UV texture coordinate
                IE_VF_Colors    = 0x04  // each vertex contains its own color
            };

            std::size_t m_Stride; // vertex stride (i.e. length between each vertex) in bytes
            IEType      m_Type;   // vertex type (i.e. how vertex is organized: triangle list, triangle fan, ...)
            IEFormat    m_Format; // vertex format (i.e. what data vertex contains: position, normal, texture, ...)

            VertexFormat();
            virtual ~VertexFormat();

            /**
            * Calculates the vertex stride
            */
            virtual void CalculateStride();

            /**
            * Compares vertex and determine if their format are equivalent
            *@param other - other vertex to compare with
            *@return true if both vertex header are equivalent, otherwise false
            */
            virtual bool CompareFormat(const VertexFormat& other) const;
    };

    /**
    * Vertex culling
    *@author Jean-Milost Reymond
    */
    class VertexCulling
    {
        public:
            /**
            * Culling type
            */
            enum class IECullingType
            {
                IE_CT_None,
                IE_CT_Front,
                IE_CT_Back,
                IE_CT_Both
            };

            /**
            * Culling face
            */
            enum class IECullingFace
            {
                IE_CF_CW,
                IE_CF_CCW
            };

            IECullingType m_Type;
            IECullingFace m_Face;

            VertexCulling();
            virtual ~VertexCulling();
    };

    /**
    * Vertex descriptor, contains global enumeration and types
    *@author Jean-Milost Reymond
    */
    class VertexBuffer
    {
        public:
            typedef std::vector<float> IData;

            std::string   m_Name;
            VertexFormat  m_Format;
            VertexCulling m_Culling;
            Material      m_Material;
            IData         m_Data;

            /**
            * Called when a vertex color should be get
            *@param pVB - vertex buffer that will contain the vertex for which the color should be get
            *@param pNormal - vertex normal
            *@param groupIndex - the vertex group index (e.g. the inner and outer vertices of a ring)
            *@return RGBA color to apply to the vertex
            *@note This callback will be called only if the per-vertex color option is activated in the vertex
            *      buffer
            */
            typedef ColorF(*ITfOnGetVertexColor)(const VertexBuffer* pVB, const DWF_Math::Vector3F* pNormal, std::size_t groupIndex);

            VertexBuffer();
            virtual ~VertexBuffer();

            /**
            * Clones vertex in such manner that vertex info are copied, but not vertex buffer
            *@param includeData - if true, vertex buffer data will also be cloned
            *@return cloned vertex
            *@note Cloned vertex should be deleted when useless
            */
            virtual VertexBuffer* Clone(bool includeData = false) const;

            /**
            * Adds a vertex to a vertex buffer
            *@param pVertex - vertex
            *@param pNormal - normal
            *@param pUV - texture coordinate
            *@param groupIndex - the vertex group index (e.g. the inner and outer vertices of a ring)
            *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
            *@return true on success, otherwise false
            */
            virtual bool Add(const DWF_Math::Vector3F* pVertex,
                             const DWF_Math::Vector3F* pNormal,
                             const DWF_Math::Vector2F* pUV,
                                   std::size_t         groupIndex,
                             const ITfOnGetVertexColor fOnGetVertexColor);
    };

    /**
    * Mesh
    *@author Jean-Milost Reymond
    */
    class Mesh
    {
        public:
            std::vector<VertexBuffer*> m_VB;

            Mesh();
            virtual ~Mesh();
    };

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

                std::string          m_Name;     // bone name
                DWF_Math::Matrix4x4F m_Matrix;   // matrix containing the bone transformation to apply
                IBone*               m_pParent;  // bone parent, root bone if nullptr
                IBones               m_Children; // bone children
                void*                m_pCustom;  // custom data, depends on implementation

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
                std::string          m_BoneName;            // linked bone name (required to find the bone in skeleton)
                IBone*               m_pBone;               // linked bone
                DWF_Math::Matrix4x4F m_Matrix;              // matrix to transform the mesh vertices to the bone space
                DWF_Math::Matrix4x4F m_TransformMatrix;     // transform matrix (used in FBX files)
                DWF_Math::Matrix4x4F m_TransformLinkMatrix; // transform link matrix (used in FBX files)
                IWeightInfluences    m_WeightInfluences;    // table allowing to retrieve the vertices influenced by a weight
                IWeights             m_Weights;             // weights indicating the bone influence on vertices, between 0.0f and 1.0f

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
                bool          m_ColOverRow;

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
            virtual void GetBoneMatrix(const IBone* pBone, const DWF_Math::Matrix4x4F& initialMatrix, DWF_Math::Matrix4x4F& matrix) const;
    };
}
