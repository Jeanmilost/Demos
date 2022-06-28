/****************************************************************************
 * ==> IQMModel ------------------------------------------------------------*
 ****************************************************************************
 * Description : Inter-Quake Model (.iqm) model                             *
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
#include "Buffer.h"
#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "Vertex.h"
#include "Model.h"

#define CONVERT_ENDIANNESS

/**
* Inter-Quake Model (.iqm) model
*@author Jean-Milost Reymond
*/
class IQMModel
{
    public:
        /**
        * Constructor
        *@param meshOnly - if true, only the model meshes will be generated
        *@param meshOnly - if true, only the model pose will be generated
        */
        IQMModel(bool meshOnly = false, bool poseOnly = false);

        virtual ~IQMModel();

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
        virtual bool Open(Buffer& buffer);

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
                                       const Matrix4x4F&           initialMatrix,
                                             Matrix4x4F&           matrix) const;

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
            bool Read(Buffer& buffer);
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
            bool Read(const IHeader& header, std::size_t& start, Buffer& buffer);
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
            bool Read(Buffer& buffer);
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
            bool Read(Buffer& buffer);
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
            bool Read(Buffer& buffer);
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
            bool Read(Buffer& buffer);
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
            bool Read(Buffer& buffer);
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
            bool Read(Buffer& buffer);
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
            bool Read(Buffer& buffer);
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
            bool Read(Buffer& buffer);
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
            bool Read(const IHeader& header, std::size_t& start, Buffer& buffer);
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
            bool Read(Buffer& buffer);
        };

        typedef std::vector<IExtension*> IExtensions;

        /**
        * Inter-Quake Model vertex
        */
        struct IVertex
        {
            Vector3F      m_Position;
            Vector2F      m_TexCoord;
            Vector3F      m_Normal;
            float         m_Tangent[4]      = {0.0f};
            unsigned char m_BlendIndices[4] = {0};
            unsigned char m_BlendWeights[4] = {0};
            unsigned char m_Color[4]        = {0};

            IVertex();
            ~IVertex();
        };

        typedef std::vector<IVertex*> IVertices;

        Model*                            m_pModel            = nullptr;
        VertexFormat                      m_VertFormatTemplate;
        VertexCulling                     m_VertCullingTemplate;
        Material                          m_MaterialTemplate;
        bool                              m_MeshOnly          = false;
        bool                              m_PoseOnly          = false;
        VertexBuffer::ITfOnGetVertexColor m_fOnGetVertexColor = nullptr;
        Texture::ITfOnLoadTexture         m_fOnLoadTexture    = nullptr;

        /*REM*/
        typedef std::vector<VertexBuffer*> IVertexBuffers;
        IVertexBuffers m_SourceVB;
        /**/

        /**
        * Reads the buffer content
        *@param buffer - buffer to read
        *@return true on success, otherwise false
        */
        bool Read(Buffer& buffer);

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
        bool PopulateModel(      Buffer&        buffer,
                           const IHeader&       header,
                           const ITexts&        texts,
                           const IMeshes&       meshes,
                           const IVertexArrays& vertexArrays,
                           const ITriangles&    triangles,
                           const IAdjacencies&  adjacencies,
                           const IJoints&       joints,
                           const IPoses&        poses,
                           const IAnims&        anims,
                           const IBoundsArray&  boundsArray,
                           const IComments&     comments,
                           const IExtensions&   extensions);

        bool PopulateSkeleton(const ITexts&       texts,
                              const IJoints&      joints,
                                    int           parentIndex,
                                    Model::IBone* pRoot) const;

        bool PopulateBone(const ITexts&       texts,
                          const IJoint*       pJoint,
                                std::size_t   jointIndex,
                                Model::IBone* pBone) const;

        bool PopulateAnims(      Buffer&               buffer,
                           const IHeader&              header,
                           const ITexts&               texts,
                           const IAnims&               anims,
                           const IPoses&               poses,
                                 Model::IBone*         pRootBone,
                                 Model::IAnimationSet* pAnimSet);

        bool BuildSrcVertices(const IHeader&       header,
                              const IVertexArrays& vertexArrays,
                                    Buffer&        buffer,
                                    IVertices&     srcVertices);

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

        IVertex* GetOrAddVertex(std::size_t index, IVertices& srcVertices) const;

        void QuatToRotMat(const QuaternionF& quat, Matrix4x4F& mat) const;

        int GetTextIndex(const ITexts& texts, std::size_t offset) const;

        void GetInverseBindMatrix(const Model::IBone* pBone, Matrix4x4F& outMatrix) const;

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
                                      Matrix4x4F&           matrix) const;

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
