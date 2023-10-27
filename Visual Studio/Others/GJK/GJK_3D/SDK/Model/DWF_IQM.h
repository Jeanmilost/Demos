/****************************************************************************
 * ==> DWF_IQM -------------------------------------------------------------*
 ****************************************************************************
 * Description : Inter-Quake Model (.iqm) model                             *
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
#include "DWF_Buffer.h"
#include "DWF_Color.h"
#include "DWF_Vector2.h"
#include "DWF_Vector3.h"
#include "DWF_Quaternion.h"
#include "DWF_Matrix4x4.h"
#include "DWF_Model.h"

#define CONVERT_ENDIANNESS

namespace DWF_Model
{
    /**
    * Inter-Quake Model (.iqm) model
    *@author Jean-Milost Reymond
    */
    class IQM
    {
        public:
            /**
            * Constructor
            *@param meshOnly - if true, only the model meshes will be generated
            *@param poseOnly - if true, only the model pose will be generated
            */
            IQM(bool meshOnly = false, bool poseOnly = false);

            virtual ~IQM();

            /**
            * Clears the model
            */
            virtual void Clear();

            /**
            * Opens an .iqm model from file
            *@param fileName - iqm file to open
            *@return true on success, otherwise false
            */
            virtual bool Open(const std::string& fileName);

            /**
            * Opens an .iqm model from file
            *@param fileName - iqm file to open
            *@return true on success, otherwise false
            */
            virtual bool Open(const std::wstring& fileName);

            /**
            * Opens an .iqm model from buffer
            *@param buffer - buffer containing the iqm data to open
            *@return true on success, otherwise false
            *@note the buffer should be positioned to the data start
            */
            virtual bool Open(DWF_Buffer::Buffer& buffer);

            /**
            * Gets the model
            *@param animSetIndex - animation set index
            *@param frameCount - frame count
            *@param frameInex - frame index
            *@return the model, nullptr if no model or on error
            */
            virtual Model* GetModel(int animSetIndex, int frameCount, int frameIndex) const;

            /**
            * Gets the bone animation matrix
            *@param pBone - skeleton root bone
            *@param pAnimSet - animation set containing the animation to get
            *@param frameIndex - animation frame index
            *@param initialMatrix - the initial matrix
            *@param[out] matrix - animation matrix
            */
            virtual void GetBoneAnimMatrix(const Model::IBone*         pBone,
                                           const Model::IAnimationSet* pAnimSet,
                                                 std::size_t           frameIndex,
                                           const DWF_Math::Matrix4x4F& initialMatrix,
                                                 DWF_Math::Matrix4x4F& matrix) const;

            /**
            * Copies an animation set from a source iqm
            *@param srcIQM - source iqm containing the animation set to copy from
            *@param srcAnimSetIndex - source animation set index
            *@note If the source and destination models doesn't contain the same skeleton,
            *      the animation set will not work
            */
            virtual bool CopyAnim(const IQM& srcIQM, std::size_t srcAnimSetIndex);

            /**
            * Copies the materials from a source iqm
            *@param srcIQM - source iqm containing the materials to copy from
            *@note The source and destination should share the same basic model, otherwise
            *      the materials may fail to be copied, or the result may be unexpected
            *@note As the material may contain a reference to a texture loaded in the source
            *      material, the source model should not be deleted, until this model is no longer used
            */
            virtual bool CopyMaterials(const IQM& srcIQM);

            /**
            * Changes the vertex format template
            *@param vertFormatTemplate - new vertex format template
            */
            virtual void SetVertFormatTemplate(const VertexFormat& vertFormatTemplate);

            /**
            * Changes the vertex culling template
            *@param vertCullingTemplate - new vertex culling template
            */
            virtual void SetVertCullingTemplate(const VertexCulling& vertCullingTemplate);

            /**
            * Changes the material template
            *@param materialTemplate - new material template
            */
            virtual void SetMaterial(const Material& materialTemplate);

            /**
            * Sets the OnGetVertexColor callback
            *@param fOnGetVertexColor - callback function handle
            */
            void Set_OnGetVertexColor(VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor);

            /**
            * Sets the OnLoadTexture callback
            *@param fOnLoadTexture - callback function handle
            */
            void Set_OnLoadTexture(Texture::ITfOnLoadTexture fOnLoadTexture);

        private:
            /**
            * Memory endianness type
            */
            enum class IEEndianness
            {
                IE_E_LittleEndian,
                IE_E_BigEndian
            };

            /**
            * Vertex array type
            *@note All vertex array entries must ordered as defined below, if present.
            *      I.e. position comes before normal, comes before ..., comes before custom,
            *      where a format and size is given, this means models intended for portable
            *      use should use these an IQM implementation is not required to honor any
            *      other format/size than those recommended. However, it may support other
            *      format/size combinations for these types if it desires
            */
            enum class IEVertexArrayType
            {
                IE_VT_Position     = 0,   // float, 3
                IE_VT_TexCoord     = 1,   // float, 2
                IE_VT_Normal       = 2,   // float, 3
                IE_VT_Tangent      = 3,   // float, 4
                IE_VT_BlendIndexes = 4,   // unsigned byte, 4
                IE_VT_BlendWeights = 5,   // unsigned byte, 4
                IE_VT_Color        = 6,   // unsigned byte, 4
                IE_VT_Custom       = 0x10 // all values up to CSR_IQM_CUSTOM are reserved for future use
                                          // any value >= CSR_IQM_CUSTOM is interpreted as CUSTOM type
                                          // the value then defines an offset into the string table, where
                                          // offset = value - CSR_IQM_CUSTOM this must be a valid string
                                          // naming the type
            };

            /**
            * Vertex array format
            */
            enum class IEVertexArrayFormat
            {
                IE_VF_Byte   = 0,
                IE_VF_UByte  = 1,
                IE_VF_Short  = 2,
                IE_VF_UShort = 3,
                IE_VF_Int    = 4,
                IE_VF_UInt   = 5,
                IE_VF_Half   = 6,
                IE_VF_Float  = 7,
                IE_VF_Double = 8,
            };

            /**
            * Animation flags
            */
            enum class IEAnimFlags
            {
                IE_AF_LOOP = 1 << 0
            };

            /**
            * Inter-Quake Model (.iqm) header
            *@note Offset fields are relative to the beginning of the CSR_IQMHeader struct
            *@note Offset fields must be set to 0 when the particular data is empty
            *@note Offset fields must be aligned to at least 4 byte boundaries
            */
            struct IHeader
            {
                char     m_ID[16]            = {0}; // the "INTERQUAKEMODEL\0" identifier, 0 terminated
                unsigned m_Version           =  0;  // version, must be 2
                unsigned m_FileSize          =  0;
                unsigned m_Flags             =  0;
                unsigned m_TextCount         =  0;
                unsigned m_TextOffset        =  0;
                unsigned m_MeshCount         =  0;
                unsigned m_MeshOffset        =  0;
                unsigned m_VertexArrayCount  =  0;
                unsigned m_VertexCount       =  0;
                unsigned m_VertexArrayOffset =  0;
                unsigned m_TriangleCount     =  0;
                unsigned m_TriangleOffset    =  0;
                unsigned m_AdjacencyOffset   =  0;
                unsigned m_JointCount        =  0;
                unsigned m_JointOffset       =  0;
                unsigned m_PoseCount         =  0;
                unsigned m_PoseOffset        =  0;
                unsigned m_AnimCount         =  0;
                unsigned m_AnimOffset        =  0;
                unsigned m_FrameCount        =  0;
                unsigned m_FrameChannelCount =  0;
                unsigned m_FrameOffset       =  0;
                unsigned m_BoundsOffset      =  0;
                unsigned m_CommentCount      =  0;
                unsigned m_CommentOffset     =  0;
                unsigned m_ExtensionCount    =  0;
                unsigned m_ExtensionOffset   =  0;  // these are stored as a linked list, not as a contiguous array

                IHeader();
                ~IHeader();

                /**
                * Reads the structure content
                *@param buffer - buffer to read from
                *@return true on success, otherwise false
                */
                bool Read(DWF_Buffer::Buffer& buffer);
            };

            /**
            * Inter-Quake Model text
            */
            struct IText
            {
                std::string m_Text;
                std::size_t m_Offset = 0;

                IText();
                ~IText();

                /**
                * Reads the structure content
                *@param header - IQM header
                *@param[in, out] start - start offset in buffer, new start position on function ends
                *@param count - total text count, as read from
                *@return true on success, otherwise false
                */
                bool Read(const IHeader& header, std::size_t& start, DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<IText*> ITexts;

            /**
            * Inter-Quake Model mesh
            */
            struct IMesh
            {
                unsigned m_Name          = 0; // mesh unique name, if desired
                unsigned m_Material      = 0; // set to a non-unique material or texture name
                unsigned m_FirstVertex   = 0;
                unsigned m_VertexCount   = 0;
                unsigned m_FirstTriangle = 0;
                unsigned m_TriangleCount = 0;

                IMesh();
                ~IMesh();

                /**
                * Reads the structure content
                *@param buffer - buffer to read from
                *@return true on success, otherwise false
                */
                bool Read(DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<IMesh*> IMeshes;

            /**
            * Inter-Quake Model vertex array
            */
            struct IVertexArray
            {
                unsigned m_Type   = 0; // type or custom name
                unsigned m_Flags  = 0;
                unsigned m_Format = 0; // component format
                unsigned m_Size   = 0; // component count
                unsigned m_Offset = 0; // offset to array of tightly packed components, with
                                       // num_vertexes * size total entries, must be aligned
                                       // to max(sizeof(format), 4)

                IVertexArray();
                ~IVertexArray();

                /**
                * Reads the structure content
                *@param buffer - buffer to read from
                *@return true on success, otherwise false
                */
                bool Read(DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<IVertexArray*> IVertexArrays;

            /**
            * Inter-Quake Model triangle
            */
            struct ITriangle
            {
                unsigned m_Vertex[3] = {0};

                ITriangle();
                ~ITriangle();

                /**
                * Reads the structure content
                *@param buffer - buffer to read from
                *@return true on success, otherwise false
                */
                bool Read(DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<ITriangle*> ITriangles;

            /**
            * Inter-Quake Model adjacency
            *@note Each value is the index of the adjacent triangle for edge 0, 1, and 2,
            *      where ~0 (= -1) indicates no adjacent triangle. Indexes are relative
            *      to the CSR_IQMHeader.m_TriangleOffset array and span all meshes, where
            *      0 is the first triangle, 1 is the second, 2 is the third, etc...
            */
            struct IAdjacency
            {
                unsigned m_Triangle[3] = {0};

                IAdjacency();
                ~IAdjacency();

                /**
                * Reads the structure content
                *@param buffer - buffer to read from
                *@return true on success, otherwise false
                */
                bool Read(DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<IAdjacency*> IAdjacencies;

            /**
            * Inter-Quake Model joint
            *@note Translate is translation <Tx, Ty, Tz>, and rotate is quaternion rotation
            *      <Qx, Qy, Qz, Qw>. Rotation is in relative/parent local space. Scale is
            *      pre-scaling <Sx, Sy, Sz>, so:
            *      output = (input * scale) * rotation + translation
            */
            struct IJoint
            {
                unsigned m_Name         =  0;
                int      m_Parent       = -1; // parent < 0 means this is a root bone
                float    m_Translate[3] = {0.0f};
                float    m_Rotate[4]    = {0.0f};
                float    m_Scale[3]     = {0.0f};

                IJoint();
                ~IJoint();

                /**
                * Reads the structure content
                *@param buffer - buffer to read from
                *@return true on success, otherwise false
                */
                bool Read(DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<IJoint*> IJoints;

            /**
            * Inter-Quake Model pose
            *@note Channels 0..2 are translation <Tx, Ty, Tz> and channels 3..6 are quaternion
            *      rotation <Qx, Qy, Qz, Qw>. Rotation is in relative/parent local space.
            *      Channels 7..9 are scale <Sx, Sy, Sz>, so:
            *      output = (input * scale) * rotation + translation
            */
            struct IPose
            {
                int      m_Parent            = -1; // parent < 0 means this is a root bone
                unsigned m_ChannelMask       =  0; // mask of which 10 channels are present for this joint pose
                float    m_ChannelOffset[10] = {0.0f};
                float    m_ChannelScale[10]  = {0.0f};

                IPose();
                ~IPose();

                /**
                * Reads the structure content
                *@param buffer - buffer to read from
                *@return true on success, otherwise false
                */
                bool Read(DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<IPose*> IPoses;

            /**
            * Inter-Quake Model animation
            */
            struct IAnim
            {
                unsigned m_Name       = 0;
                unsigned m_FirstFrame = 0;
                unsigned m_FrameCount = 0;
                float    m_FrameRate  = 0.0f;
                unsigned m_Flags      = 0;

                IAnim();
                ~IAnim();

                /**
                * Reads the structure content
                *@param buffer - buffer to read from
                *@return true on success, otherwise false
                */
                bool Read(DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<IAnim*> IAnims;

            /**
            * Inter-Quake Model bounds
            */
            struct IBounds
            {
                float m_Bbmin[3] = {0.0f}; // the minimum coordinates of the bounding box for this animation frame
                float m_Bbmax[3] = {0.0f}; // the maximum coordinates of the bounding box for this animation frame
                float m_Xyradius =  0.0f;  // the circular radius in the X-Y plane
                float m_Radius   =  0.0f;  // the spherical radius

                IBounds();
                ~IBounds();

                /**
                * Reads the structure content
                *@param buffer - buffer to read from
                *@return true on success, otherwise false
                */
                bool Read(DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<IBounds*> IBoundsArray;

            /**
            * Inter-Quake Model comment
            */
            struct IComment
            {
                std::string m_Text;
                std::size_t m_Offset = 0;

                IComment();
                ~IComment();

                /**
                * Reads the structure content
                *@param header - IQM header
                *@param[in, out] start - start offset in buffer, new start position on function ends
                *@param count - total text count, as read from
                *@return true on success, otherwise false
                */
                bool Read(const IHeader& header, std::size_t& start, DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<IComment*> IComments;

            /**
            * Inter-Quake Model extension
            */
            struct IExtension
            {
                unsigned m_Name          = 0;
                unsigned m_DataCount     = 0;
                unsigned m_DataOffset    = 0;
                unsigned m_NextExtOffset = 0; // pointer to next extension

                IExtension();
                ~IExtension();

                /**
                * Reads the structure content
                *@param buffer - buffer to read from
                *@return true on success, otherwise false
                */
                bool Read(DWF_Buffer::Buffer& buffer);
            };

            typedef std::vector<IExtension*> IExtensions;

            /**
            * Inter-Quake Model vertex
            */
            struct IVertex
            {
                DWF_Math::Vector3F m_Position;
                DWF_Math::Vector2F m_TexCoord;
                DWF_Math::Vector3F m_Normal;
                float              m_Tangent[4]      = {0.0f};
                unsigned char      m_BlendIndices[4] = {0};
                unsigned char      m_BlendWeights[4] = {0};
                unsigned char      m_Color[4]        = {0};

                IVertex();
                ~IVertex();
            };

            typedef std::vector<IVertex*> IVertices;

            /**
            * Animated bone cache dictionary
            */
            typedef std::map<const Model::IBone*, DWF_Math::Matrix4x4F> IAnimBoneCacheDict;

            /**
            * Source vertex buffer cache
            */
            typedef std::vector<VertexBuffer::IData*> IVBCache;

            Model*                            m_pModel            = nullptr;
            VertexFormat                      m_VertFormatTemplate;
            VertexCulling                     m_VertCullingTemplate;
            Material                          m_MaterialTemplate;
            IAnimBoneCacheDict                m_AnimBoneCacheDict;
            IVBCache                          m_VBCache;
            bool                              m_MeshOnly          = false;
            bool                              m_PoseOnly          = false;
            VertexBuffer::ITfOnGetVertexColor m_fOnGetVertexColor = nullptr;
            Texture::ITfOnLoadTexture         m_fOnLoadTexture    = nullptr;

            /**
            * Reads the buffer content
            *@param buffer - buffer to read
            *@return true on success, otherwise false
            */
            bool Read(DWF_Buffer::Buffer& buffer);

            /**
            * Populates the model
            *@param buffer - buffer from which model data may be read
            *@param header - header
            *@param texts - texts
            *@param meshes - meshes
            *@param vertexArrays - vertex arrays
            *@param triangles - triangles
            *@param adjacencies - adjacencies
            *@param joints - joints
            *@param poses - poses
            *@param anims - anims
            *@param boundsArray - bounds array
            *@param comments - comments
            *@param extensions - extensions
            *@return true on success, otherwise false
            */
            bool PopulateModel(      DWF_Buffer::Buffer& buffer,
                               const IHeader&            header,
                               const ITexts&             texts,
                               const IMeshes&            meshes,
                               const IVertexArrays&      vertexArrays,
                               const ITriangles&         triangles,
                               const IAdjacencies&       adjacencies,
                               const IJoints&            joints,
                               const IPoses&             poses,
                               const IAnims&             anims,
                               const IBoundsArray&       boundsArray,
                               const IComments&          comments,
                               const IExtensions&        extensions);

            /**
            * Populates the skeleton
            *@param texts - texts
            *@param joints - joints
            *@param parentIndex - parent index
            *@param pRoot - root bone
            *@return true on success, otherwise false
            */
            bool PopulateSkeleton(const ITexts&       texts,
                                  const IJoints&      joints,
                                        int           parentIndex,
                                        Model::IBone* pRoot) const;

            /**
            * Populates a bone
            *@param texts - texts
            *@param joints - joints
            *@param jointIndex - joint index
            *@param pBone - bone to populate
            *@return true on success, otherwise false
            */
            bool PopulateBone(const ITexts&       texts,
                              const IJoint*       pJoint,
                                    std::size_t   jointIndex,
                                    Model::IBone* pBone) const;

            /**
            * Populates the animations
            *@param buffer - source buffer to read from
            *@param header - header
            *@param texts - texts
            *@param anims - animations
            *@param poses - poses
            *@param pRootBone - root bone
            *@param pAnimSet - animation set to populate
            *@return true on success, otherwise false
            */
            bool PopulateAnims(      DWF_Buffer::Buffer&   buffer,
                               const IHeader&              header,
                               const ITexts&               texts,
                               const IAnims&               anims,
                               const IPoses&               poses,
                                     Model::IBone*         pRootBone,
                                     Model::IAnimationSet* pAnimSet);

            /**
            * Builds the source vertices
            *@param header - header
            *@param vertexArrays - vertex arrays
            *@param buffer - source buffer to read from
            *@param srcVertices - source vertices to populate
            *@return true on success, otherwise false
            */
            bool BuildSrcVertices(const IHeader&            header,
                                  const IVertexArrays&      vertexArrays,
                                        DWF_Buffer::Buffer& buffer,
                                        IVertices&          srcVertices);

            /**
            * Builds the weights from the skeleton
            *@param pBone - parent bone
            *@param meshIndex - mesh index
            *@param pDeformers - deformers (i.e. weights groups) to populate
            *@return true on success, otherwise false
            */
            bool BuildWeightsFromSkeleton(Model::IBone*      pBone,
                                          std::size_t        meshIndex,
                                          Model::IDeformers* pDeformers);

            /**
            * Finds a bone in the skeleton
            *@param pBone - parent bone to search from
            *@param index - bone index to find
            *@return bone matching with index, nullptr if not found or on error
            */
            Model::IBone* FindBone(Model::IBone* pBone, std::size_t index) const;

            /**
            * Gets an existing vertex at index, or adds it if still not exists
            *@param index - index
            *@param[in, out] srcVertices - source vertices, vertices in which the vertex will be added on function ends
            *@return get or newly added vertex
            */
            IVertex* GetOrAddVertex(std::size_t index, IVertices& srcVertices) const;

            /**
            * Converts a quaternion to a matrix
            *@param quat - quaternion to convert
            *@param[out] mat - matrix
            */
            void QuatToRotMat(const DWF_Math::QuaternionF& quat, DWF_Math::Matrix4x4F& mat) const;

            /**
            * Gets the text index
            *@param texts - texts
            *@param offset - text offset to find
            *@return text index
            */
            int GetTextIndex(const ITexts& texts, std::size_t offset) const;

            /**
            * Gets the inverse bind matrix
            *@param pBone - bone from which the inverse bind matrix should be get
            *@param[out] outMatrix - resulting inverse matrix
            */
            void GetInverseBindMatrix(const Model::IBone* pBone, DWF_Math::Matrix4x4F& outMatrix) const;

            /**
            * Gets the animation matrix
            *@param pAnimSet - animation set containing the animation to search
            *@param pBone - skeleton root bone
            *@param frame - animation frame
            *@param[out] matrix - animation matrix
            *@return true on success, otherwise false
            */
            bool GetAnimationMatrix(const Model::IAnimationSet* pAnimSet,
                                    const Model::IBone*         pBone,
                                          std::size_t           frame,
                                          DWF_Math::Matrix4x4F& matrix) const;

            /**
            * Gets the memory endianness
            *@return the memory endianness
            */
            #ifdef CONVERT_ENDIANNESS
                static IEEndianness GetEndianness();
            #endif

            /**
            * Swaps the memory content between big and little endian
            *@param[in, out] pMemory - memory to swap, swapped memory on function ends
            *@paran size - size of memory to swap in bytes
            */
            #ifdef CONVERT_ENDIANNESS
                static void SwapMemory(void* pMemory, std::size_t size);
            #endif
    };
}
