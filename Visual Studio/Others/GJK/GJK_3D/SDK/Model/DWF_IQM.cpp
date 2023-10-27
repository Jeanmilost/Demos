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

#include "DWF_IQM.h"

// std
#include <stdlib.h>
#include <math.h>
#include <string.h>

// classes
#include "DWF_Quaternion.h"
#include "DWF_StdFileBuffer.h"

#pragma warning(disable: 26812)

using namespace DWF_Model;

//---------------------------------------------------------------------------
// Global defines
//---------------------------------------------------------------------------
#define M_IQM_ID            "INTERQUAKEMODEL\0"
#define M_IQM_File_Version  2

//---------------------------------------------------------------------------
// IQM::IHeader
//---------------------------------------------------------------------------
IQM::IHeader::IHeader()
{}
//---------------------------------------------------------------------------
IQM::IHeader::~IHeader()
{}
//---------------------------------------------------------------------------
bool IQM::IHeader::Read(DWF_Buffer::Buffer& buffer)
{
    if (!buffer.Read(&m_ID, sizeof(m_ID)))
        return false;

    if (!buffer.Read(&m_Version, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_FileSize, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_Flags, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_TextCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_TextOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_MeshCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_MeshOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_VertexArrayCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_VertexCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_VertexArrayOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_TriangleCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_TriangleOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_AdjacencyOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_JointCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_JointOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_PoseCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_PoseOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_AnimCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_AnimOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_FrameCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_FrameChannelCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_FrameOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_BoundsOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_CommentCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_CommentOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_ExtensionCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_ExtensionOffset, sizeof(unsigned)))
        return false;

    #ifdef CONVERT_ENDIANNESS
        // the read bytes are inverted and should be swapped if the target system is big endian
        if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
        {
            // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
            //SwapMemory(&m_ID,                sizeof(m_ID)); // todo -cCheck -oJean: check if required
            SwapMemory(&m_Version,           sizeof(unsigned));
            SwapMemory(&m_FileSize,          sizeof(unsigned));
            SwapMemory(&m_Flags,             sizeof(unsigned));
            SwapMemory(&m_TextCount,         sizeof(unsigned));
            SwapMemory(&m_TextOffset,        sizeof(unsigned));
            SwapMemory(&m_MeshCount,         sizeof(unsigned));
            SwapMemory(&m_MeshOffset,        sizeof(unsigned));
            SwapMemory(&m_VertexArrayCount,  sizeof(unsigned));
            SwapMemory(&m_VertexCount,       sizeof(unsigned));
            SwapMemory(&m_VertexArrayOffset, sizeof(unsigned));
            SwapMemory(&m_TriangleCount,     sizeof(unsigned));
            SwapMemory(&m_TriangleOffset,    sizeof(unsigned));
            SwapMemory(&m_AdjacencyOffset,   sizeof(unsigned));
            SwapMemory(&m_JointCount,        sizeof(unsigned));
            SwapMemory(&m_JointOffset,       sizeof(unsigned));
            SwapMemory(&m_PoseCount,         sizeof(unsigned));
            SwapMemory(&m_PoseOffset,        sizeof(unsigned));
            SwapMemory(&m_AnimCount,         sizeof(unsigned));
            SwapMemory(&m_AnimOffset,        sizeof(unsigned));
            SwapMemory(&m_FrameCount,        sizeof(unsigned));
            SwapMemory(&m_FrameChannelCount, sizeof(unsigned));
            SwapMemory(&m_FrameOffset,       sizeof(unsigned));
            SwapMemory(&m_BoundsOffset,      sizeof(unsigned));
            SwapMemory(&m_CommentCount,      sizeof(unsigned));
            SwapMemory(&m_CommentOffset,     sizeof(unsigned));
            SwapMemory(&m_ExtensionCount,    sizeof(unsigned));
            SwapMemory(&m_ExtensionOffset,   sizeof(unsigned));
        }
    #endif

    return true;
}
//---------------------------------------------------------------------------
// IQM::IText
//---------------------------------------------------------------------------
IQM::IText::IText()
{}
//---------------------------------------------------------------------------
IQM::IText::~IText()
{}
//---------------------------------------------------------------------------
bool IQM::IText::Read(const IHeader& header, std::size_t& start, DWF_Buffer::Buffer& buffer)
{
    m_Offset = start;

    // iterate through chars
    for (std::size_t i = start; i < header.m_TextCount; ++i)
    {
        char c;

        // read next char
        if (!buffer.Read(&c, sizeof(char)))
            return false;

        // dispatch char
        switch (c)
        {
            case '\0':
                // end reached
                start = i + 1;
                return true;

            default:
                // copy next char
                m_Text += c;
                continue;
        }
    }

    return true;
}
//---------------------------------------------------------------------------
// IQM::IMesh
//---------------------------------------------------------------------------
IQM::IMesh::IMesh()
{}
//---------------------------------------------------------------------------
IQM::IMesh::~IMesh()
{}
//---------------------------------------------------------------------------
bool IQM::IMesh::Read(DWF_Buffer::Buffer& buffer)
{
    // read the mesh content
    if (!buffer.Read(&m_Name, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_Material, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_FirstVertex, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_VertexCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_FirstTriangle, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_TriangleCount, sizeof(unsigned)))
        return false;

    #ifdef CONVERT_ENDIANNESS
        // the read bytes are inverted and should be swapped if the target system is big endian
        if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
        {
            // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
            SwapMemory(&m_Name,          sizeof(unsigned));
            SwapMemory(&m_Material,      sizeof(unsigned));
            SwapMemory(&m_FirstVertex,   sizeof(unsigned));
            SwapMemory(&m_VertexCount,   sizeof(unsigned));
            SwapMemory(&m_FirstTriangle, sizeof(unsigned));
            SwapMemory(&m_TriangleCount, sizeof(unsigned));
        }
    #endif

    return true;
}
//---------------------------------------------------------------------------
// IQM::IVertexArray
//---------------------------------------------------------------------------
IQM::IVertexArray::IVertexArray()
{}
//---------------------------------------------------------------------------
IQM::IVertexArray::~IVertexArray()
{}
//---------------------------------------------------------------------------
bool IQM::IVertexArray::Read(DWF_Buffer::Buffer& buffer)
{
    // read the vertex array content
    if (!buffer.Read(&m_Type, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_Flags, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_Format, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_Size, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_Offset, sizeof(unsigned)))
        return false;

    #ifdef CONVERT_ENDIANNESS
        // the read bytes are inverted and should be swapped if the target system is big endian
        if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
        {
            // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
            SwapMemory(&m_Type,   sizeof(unsigned));
            SwapMemory(&m_Flags,  sizeof(unsigned));
            SwapMemory(&m_Format, sizeof(unsigned));
            SwapMemory(&m_Size,   sizeof(unsigned));
            SwapMemory(&m_Offset, sizeof(unsigned));
        }
    #endif

    return true;
}
//---------------------------------------------------------------------------
// IQM::ITriangle
//---------------------------------------------------------------------------
IQM::ITriangle::ITriangle()
{}
//---------------------------------------------------------------------------
IQM::ITriangle::~ITriangle()
{}
//---------------------------------------------------------------------------
bool IQM::ITriangle::Read(DWF_Buffer::Buffer& buffer)
{
    // read the triangle content
    for (std::size_t i = 0; i < 3; ++i)
    {
        if (!buffer.Read(&m_Vertex[i], sizeof(unsigned)))
            return false;

        #ifdef CONVERT_ENDIANNESS
            // the read bytes are inverted and should be swapped if the target system is big endian
            if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
                // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
                SwapMemory(&m_Vertex[i], sizeof(unsigned));
        #endif
    }

    return true;
}
//---------------------------------------------------------------------------
// IQM::IAdjacency
//---------------------------------------------------------------------------
IQM::IAdjacency::IAdjacency()
{}
//---------------------------------------------------------------------------
IQM::IAdjacency::~IAdjacency()
{}
//---------------------------------------------------------------------------
bool IQM::IAdjacency::Read(DWF_Buffer::Buffer& buffer)
{
    // read the triangle content
    for (std::size_t i = 0; i < 3; ++i)
    {
        if (!buffer.Read(&m_Triangle[i], sizeof(unsigned)))
            return false;

        #ifdef CONVERT_ENDIANNESS
            // the read bytes are inverted and should be swapped if the target system is big endian
            if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
                // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
                SwapMemory(&m_Triangle[i], sizeof(unsigned));
        #endif
    }

    return true;
}
//---------------------------------------------------------------------------
// IQM::IJoint
//---------------------------------------------------------------------------
IQM::IJoint::IJoint()
{}
//---------------------------------------------------------------------------
IQM::IJoint::~IJoint()
{}
//---------------------------------------------------------------------------
bool IQM::IJoint::Read(DWF_Buffer::Buffer& buffer)
{
    // read the joint content
    if (!buffer.Read(&m_Name, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_Parent, sizeof(int)))
        return false;

    for (std::size_t i = 0; i < 3; ++i)
        if (!buffer.Read(&m_Translate[i], sizeof(float)))
            return false;

    for (std::size_t i = 0; i < 4; ++i)
        if (!buffer.Read(&m_Rotate[i], sizeof(float)))
            return false;

    for (std::size_t i = 0; i < 3; ++i)
        if (!buffer.Read(&m_Scale[i], sizeof(float)))
            return false;

    #ifdef CONVERT_ENDIANNESS
        // the read bytes are inverted and should be swapped if the target system is big endian
        if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
        {
            // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
            SwapMemory(&m_Name,   sizeof(unsigned));
            SwapMemory(&m_Parent, sizeof(int));

            for (std::size_t i = 0; i < 3; ++i)
                SwapMemory(&m_Translate[i], sizeof(float));

            for (std::size_t i = 0; i < 4; ++i)
                SwapMemory(&m_Rotate[i], sizeof(float));

            for (std::size_t i = 0; i < 3; ++i)
                SwapMemory(&m_Scale[i], sizeof(float));
        }
    #endif

    return true;
}
//---------------------------------------------------------------------------
// IQM::IPose
//---------------------------------------------------------------------------
IQM::IPose::IPose()
{}
//---------------------------------------------------------------------------
IQM::IPose::~IPose()
{}
//---------------------------------------------------------------------------
bool IQM::IPose::Read(DWF_Buffer::Buffer& buffer)
{
    // read the pose content
    if (!buffer.Read(&m_Parent, sizeof(int)))
        return false;

    if (!buffer.Read(&m_ChannelMask, sizeof(unsigned)))
        return false;

    for (std::size_t i = 0; i < 10; ++i)
        if (!buffer.Read(&m_ChannelOffset[i], sizeof(float)))
            return false;

    for (std::size_t i = 0; i < 10; ++i)
        if (!buffer.Read(&m_ChannelScale[i], sizeof(float)))
            return false;

    #ifdef CONVERT_ENDIANNESS
        // the read bytes are inverted and should be swapped if the target system is big endian
        if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
        {
            // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
            SwapMemory(&m_Parent, sizeof(int));
            SwapMemory(&m_ChannelMask, sizeof(unsigned));

            for (std::size_t i = 0; i < 10; ++i)
                SwapMemory(&m_ChannelOffset[i], sizeof(float));

            for (std::size_t i = 0; i < 10; ++i)
                SwapMemory(&m_ChannelScale[i], sizeof(float));
        }
    #endif

    return true;
}
//---------------------------------------------------------------------------
// IQM::IAnim
//---------------------------------------------------------------------------
IQM::IAnim::IAnim()
{}
//---------------------------------------------------------------------------
IQM::IAnim::~IAnim()
{}
//---------------------------------------------------------------------------
bool IQM::IAnim::Read(DWF_Buffer::Buffer& buffer)
{
    // read the animation content
    if (!buffer.Read(&m_Name, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_FirstFrame, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_FrameCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_FrameRate, sizeof(float)))
        return false;

    if (!buffer.Read(&m_Flags, sizeof(unsigned)))
        return false;

    #ifdef CONVERT_ENDIANNESS
        // the read bytes are inverted and should be swapped if the target system is big endian
        if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
        {
            // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
            SwapMemory(&m_Name,       sizeof(unsigned));
            SwapMemory(&m_FirstFrame, sizeof(unsigned));
            SwapMemory(&m_FrameCount, sizeof(unsigned));
            SwapMemory(&m_FrameRate,  sizeof(float));
            SwapMemory(&m_Flags,      sizeof(unsigned));
        }
    #endif

    return true;
}
//---------------------------------------------------------------------------
// IQM::IBounds
//---------------------------------------------------------------------------
IQM::IBounds::IBounds()
{}
//---------------------------------------------------------------------------
IQM::IBounds::~IBounds()
{}
//---------------------------------------------------------------------------
bool IQM::IBounds::Read(DWF_Buffer::Buffer& buffer)
{
    // read the bounds content
    for (std::size_t i = 0; i < 3; ++i)
        if (!buffer.Read(&m_Bbmin[i], sizeof(float)))
            return false;

    for (std::size_t i = 0; i < 3; ++i)
        if (!buffer.Read(&m_Bbmax[i], sizeof(float)))
            return false;

    if (!buffer.Read(&m_Xyradius, sizeof(float)))
        return false;

    if (!buffer.Read(&m_Radius, sizeof(float)))
        return false;

    #ifdef CONVERT_ENDIANNESS
        // the read bytes are inverted and should be swapped if the target system is big endian
        if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
        {
            // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
            for (std::size_t i = 0; i < 3; ++i)
                SwapMemory(&m_Bbmin[i], sizeof(float));

            for (std::size_t i = 0; i < 3; ++i)
                SwapMemory(&m_Bbmax[i], sizeof(float));

            SwapMemory(&m_Xyradius, sizeof(float));
            SwapMemory(&m_Radius, sizeof(float));
        }
    #endif

    return true;
}
//---------------------------------------------------------------------------
// IQM::IComment
//---------------------------------------------------------------------------
IQM::IComment::IComment()
{}
//---------------------------------------------------------------------------
IQM::IComment::~IComment()
{}
//---------------------------------------------------------------------------
bool IQM::IComment::Read(const IHeader& header, std::size_t& start, DWF_Buffer::Buffer& buffer)
{
    m_Offset = start;

    // iterate through chars
    for (std::size_t i = start; i < header.m_TextCount; ++i)
    {
        char c;

        // read next char
        if (!buffer.Read(&c, sizeof(char)))
            return false;

        // dispatch char
        switch (c)
        {
            case '\0':
                // end reached
                start = i + 1;
                return true;

            default:
                // copy next char
                m_Text += c;
                continue;
        }
    }

    return true;
}
//---------------------------------------------------------------------------
// IQM::IExtension
//---------------------------------------------------------------------------
IQM::IExtension::IExtension()
{}
//---------------------------------------------------------------------------
IQM::IExtension::~IExtension()
{}
//---------------------------------------------------------------------------
bool IQM::IExtension::Read(DWF_Buffer::Buffer& buffer)
{
    // read the extension content
    if (!buffer.Read(&m_Name, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_DataCount, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_DataOffset, sizeof(unsigned)))
        return false;

    if (!buffer.Read(&m_NextExtOffset, sizeof(unsigned)))
        return false;

    #ifdef CONVERT_ENDIANNESS
        // the read bytes are inverted and should be swapped if the target system is big endian
        if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
        {
            // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
            SwapMemory(&m_Name,          sizeof(unsigned));
            SwapMemory(&m_DataCount,     sizeof(unsigned));
            SwapMemory(&m_DataOffset,    sizeof(unsigned));
            SwapMemory(&m_NextExtOffset, sizeof(unsigned));
        }
    #endif

    return true;
}
//---------------------------------------------------------------------------
// IQM::IVertex
//---------------------------------------------------------------------------
IQM::IVertex::IVertex()
{}
//---------------------------------------------------------------------------
IQM::IVertex::~IVertex()
{}
//---------------------------------------------------------------------------
// IQM
//---------------------------------------------------------------------------
IQM::IQM(bool meshOnly, bool poseOnly) :
    m_MeshOnly(meshOnly),
    m_PoseOnly(poseOnly)
{
    // configure the default vertex format
    m_VertFormatTemplate.m_Format = (VertexFormat::IEFormat)((unsigned)VertexFormat::IEFormat::IE_VF_Colors |
                                                             (unsigned)VertexFormat::IEFormat::IE_VF_TexCoords);
    m_VertFormatTemplate.m_Type   =  VertexFormat::IEType::IE_VT_Triangles;

    // configure the default vertex culling
    m_VertCullingTemplate.m_Type = VertexCulling::IECullingType::IE_CT_Back;
    m_VertCullingTemplate.m_Face = VertexCulling::IECullingFace::IE_CF_CW;

    // configure the default material
    m_MaterialTemplate.m_Color = ColorF(1.0f, 1.0f, 1.0f, 1.0f);
}
//---------------------------------------------------------------------------
IQM::~IQM()
{
    if (m_pModel)
        delete m_pModel;

    for (IVBCache::iterator it = m_VBCache.begin(); it != m_VBCache.end(); ++it)
        delete (*it);
}
//---------------------------------------------------------------------------
void IQM::Clear()
{
    if (m_pModel)
        delete m_pModel;

    m_pModel = nullptr;

    for (IVBCache::iterator it = m_VBCache.begin(); it != m_VBCache.end(); ++it)
        delete (*it);

    m_VBCache.clear();
}
//---------------------------------------------------------------------------
bool IQM::Open(const std::string& fileName)
{
    // clear the previous model, if exists
    Clear();

    // no file name?
    if (fileName.empty())
        return false;

    DWF_Buffer::StdFileBuffer fileBuffer;

    // open the file
    if (!fileBuffer.Open(fileName, DWF_Buffer::FileBuffer::IEMode::IE_M_Read))
        return false;

    // seek to start
    fileBuffer.Seek(0, 0);

    // read the content
    return Read(fileBuffer);
}
//---------------------------------------------------------------------------
bool IQM::Open(const std::wstring& fileName)
{
    // delete the previous model, if exists
    if (m_pModel)
    {
        delete m_pModel;
        m_pModel = nullptr;
    }

    // no file name?
    if (fileName.empty())
        return false;

    DWF_Buffer::StdFileBuffer fileBuffer;

    // open the file
    if (!fileBuffer.Open(fileName, DWF_Buffer::FileBuffer::IEMode::IE_M_Read))
        return false;

    // seek to start
    fileBuffer.Seek(0, 0);

    // read the content
    return Read(fileBuffer);
}
//---------------------------------------------------------------------------
bool IQM::Open(DWF_Buffer::Buffer& buffer)
{
    // delete the previous model, if exists
    if (m_pModel)
    {
        delete m_pModel;
        m_pModel = nullptr;
    }

    // read the buffer content
    return Read(buffer);
}
//---------------------------------------------------------------------------
Model* IQM::GetModel(int animSetIndex, int frameCount, int frameIndex) const
{
    // no model?
    if (!m_pModel)
        return nullptr;

    // if mesh has no skeleton, or if only the pose is required, perform a simple draw
    if (!m_pModel->m_pSkeleton || m_pModel->m_PoseOnly)
        return m_pModel;

    // clear the animation matrix cache
    const_cast<IAnimBoneCacheDict&>(m_AnimBoneCacheDict).clear();

    const std::size_t meshCount = m_pModel->m_Mesh.size();

    // iterate through model meshes
    for (std::size_t i = 0; i < meshCount; ++i)
    {
        // get model mesh
        Mesh* pMesh = m_pModel->m_Mesh[i];

        // found it?
        if (!pMesh)
            continue;

        // normally each mesh should contain only one vertex buffer
        if (pMesh->m_VB.size() != 1)
            // unsupported if not (because cannot know which texture should be binded. If a such model
            // exists, a custom version of this function should also be written for it)
            continue;

        // malformed deformers?
        if (meshCount != m_pModel->m_Deformers.size())
            return nullptr;

        const std::size_t weightCount = m_pModel->m_Deformers[i]->m_SkinWeights.size();

        // mesh contains skin weights?
        if (!weightCount)
            return nullptr;

        // clear the previous vertex buffer vertices in order to rebuild them
        for (std::size_t j = 0; j < pMesh->m_VB[0]->m_Data.size(); j += pMesh->m_VB[0]->m_Format.m_Stride)
        {
            pMesh->m_VB[0]->m_Data[j]     = 0.0f;
            pMesh->m_VB[0]->m_Data[j + 1] = 0.0f;
            pMesh->m_VB[0]->m_Data[j + 2] = 0.0f;
        }

        // iterate through mesh skin weights
        for (std::size_t j = 0; j < weightCount; ++j)
        {
            DWF_Math::Matrix4x4F boneMatrix;

            // get the bone matrix
            if (m_pModel->m_PoseOnly)
                m_pModel->GetBoneMatrix(m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_pBone, DWF_Math::Matrix4x4F::Identity(), boneMatrix);
            else
                GetBoneAnimMatrix(m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_pBone,
                                  m_pModel->m_AnimationSet[animSetIndex],
                                  frameIndex,
                                  DWF_Math::Matrix4x4F::Identity(),
                                  boneMatrix);

            // get the final matrix after bones transform
            const DWF_Math::Matrix4x4F finalMatrix = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Matrix.Multiply(boneMatrix);

            // get the weight influence count
            const std::size_t weightInfluenceCount = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences.size();

            // apply the bone and its skin weights to each vertices
            for (std::size_t k = 0; k < weightInfluenceCount; ++k)
            {
                // get the vertex index count
                const std::size_t vertexIndexCount =
                    m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex.size();

                // iterate through weights influences vertex indices
                for (std::size_t l = 0; l < vertexIndexCount; ++l)
                {
                    // get the next vertex to which the next skin weight should be applied
                    const std::size_t iX = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l];
                    const std::size_t iY = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l] + 1;
                    const std::size_t iZ = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l] + 2;

                    DWF_Math::Vector3F inputVertex;

                    // get input vertex
                    inputVertex.m_X = (*m_VBCache[i])[iX];
                    inputVertex.m_Y = (*m_VBCache[i])[iY];
                    inputVertex.m_Z = (*m_VBCache[i])[iZ];

                    // apply bone transformation to vertex
                    const DWF_Math::Vector3F outputVertex = finalMatrix.Transform(inputVertex);

                    // apply the skin weights and calculate the final output vertex
                    pMesh->m_VB[0]->m_Data[iX] += (outputVertex.m_X * (float)m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Weights[k]);
                    pMesh->m_VB[0]->m_Data[iY] += (outputVertex.m_Y * (float)m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Weights[k]);
                    pMesh->m_VB[0]->m_Data[iZ] += (outputVertex.m_Z * (float)m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Weights[k]);
                }
            }
        }
    }

    return m_pModel;
}
//---------------------------------------------------------------------------
void IQM::GetBoneAnimMatrix(const Model::IBone*         pBone,
                            const Model::IAnimationSet* pAnimSet,
                                  std::size_t           frameIndex,
                            const DWF_Math::Matrix4x4F& initialMatrix,
                                  DWF_Math::Matrix4x4F& matrix) const
{
    // no bone?
    if (!pBone)
        return;

    // set the output matrix as identity
    matrix = DWF_Math::Matrix4x4F::Identity();

    DWF_Math::Matrix4x4F animMatrix;

    // iterate through bones
    while (pBone)
    {
        // get the previously stacked matrix as base to calculate the new one
        const DWF_Math::Matrix4x4F localMatrix = matrix;

        // get the animated bone matrix matching with frame. If not found use the identity one
        if (!GetAnimationMatrix(pAnimSet, pBone, frameIndex, animMatrix))
            animMatrix = DWF_Math::Matrix4x4F::Identity();

        // stack the previously calculated matrix with the current bone one
        matrix = localMatrix.Multiply(animMatrix);

        // go to parent bone
        pBone = pBone->m_pParent;
    }

    // initial matrix provided?
    if (!initialMatrix.IsIdentity())
    {
        // get the previously stacked matrix as base to calculate the new one
        const DWF_Math::Matrix4x4F localMatrix = matrix;

        // stack the previously calculated matrix with the initial one
        matrix = localMatrix.Multiply(initialMatrix);
    }
}
//---------------------------------------------------------------------------
bool IQM::CopyAnim(const IQM& srcIQM, std::size_t srcAnimSetIndex)
{
    // no model?
    if (!m_pModel)
        return false;

    // no source model?
    if (!srcIQM.m_pModel)
        return false;

    // is source animation index out of bounds?
    if (srcAnimSetIndex >= srcIQM.m_pModel->m_AnimationSet.size())
        return false;

    // get animation set to copy
    const DWF_Model::Model::IAnimationSet* pSrcAnimSet = srcIQM.m_pModel->m_AnimationSet[srcAnimSetIndex];

    // could not get source animation set?
    if (!pSrcAnimSet)
        return false;

    std::unique_ptr<DWF_Model::Model::IAnimationSet> pDestAnimSet = std::make_unique<DWF_Model::Model::IAnimationSet>();

    pDestAnimSet->m_MaxValue = pSrcAnimSet->m_MaxValue;

    for (std::size_t i = 0; i < pSrcAnimSet->m_Animations.size(); ++i)
    {
        const DWF_Model::Model::IAnimation* pSrcAnim = pSrcAnimSet->m_Animations[i];

        if (!pSrcAnim)
            return false;

        std::unique_ptr<DWF_Model::Model::IAnimation> pDestAnim = std::make_unique<DWF_Model::Model::IAnimation>();
        pDestAnim->m_BoneName                                   = pSrcAnim->m_BoneName;
        pDestAnim->m_pBone                                      = pSrcAnim->m_pBone;

        for (std::size_t j = 0; j < pSrcAnim->m_Keys.size(); ++j)
        {
            const DWF_Model::Model::IAnimationKeys* pSrcAnimKeys = pSrcAnim->m_Keys[j];

            if (!pSrcAnimKeys)
                return false;

            std::unique_ptr<DWF_Model::Model::IAnimationKeys> pDestAnimKeys = std::make_unique<DWF_Model::Model::IAnimationKeys>();
            pDestAnimKeys->m_Type                                           = pSrcAnimKeys->m_Type;
            pDestAnimKeys->m_ColOverRow                                     = pSrcAnimKeys->m_ColOverRow;

            for (std::size_t k = 0; k < pSrcAnimKeys->m_Keys.size(); ++k)
            {
                const DWF_Model::Model::IAnimationKey* pSrcAnimKey = pSrcAnimKeys->m_Keys[k];

                if (!pSrcAnimKey)
                    return false;

                std::unique_ptr<DWF_Model::Model::IAnimationKey> pDestAnimKey = std::make_unique<DWF_Model::Model::IAnimationKey>();
                pDestAnimKey->m_Frame                                         = pSrcAnimKey->m_Frame;
                pDestAnimKey->m_TimeStamp                                     = pSrcAnimKey->m_TimeStamp;
                pDestAnimKey->m_Values                                        = pSrcAnimKey->m_Values;

                pDestAnimKeys->m_Keys.push_back(pDestAnimKey.get());
                pDestAnimKey.release();
            }

            pDestAnim->m_Keys.push_back(pDestAnimKeys.get());
            pDestAnimKeys.release();
        }

        pDestAnimSet->m_Animations.push_back(pDestAnim.get());
        pDestAnim.release();
    }

    // set the newly copied animation set
    m_pModel->m_AnimationSet.push_back(pDestAnimSet.get());
    pDestAnimSet.release();

    return true;
}
//---------------------------------------------------------------------------
bool IQM::CopyMaterials(const IQM& srcIQM)
{
    // no model?
    if (!m_pModel)
        return false;

    // no source model?
    if (!srcIQM.m_pModel)
        return false;

    // mesh count differs?
    if (m_pModel->m_Mesh.size() != srcIQM.m_pModel->m_Mesh.size())
        return false;

    // iterate through meshes
    for (std::size_t i = 0; i < srcIQM.m_pModel->m_Mesh.size(); ++i)
    {
        // vertex buffer count differs?
        if (m_pModel->m_Mesh[i]->m_VB.size() != srcIQM.m_pModel->m_Mesh[i]->m_VB.size())
            return false;

        // iterate through vertex buffers
        for (std::size_t j = 0; j < srcIQM.m_pModel->m_Mesh[i]->m_VB.size(); ++j)
        {
            // copy the material
            m_pModel->m_Mesh[i]->m_VB[j]->m_Material.m_Color       = srcIQM.m_pModel->m_Mesh[i]->m_VB[j]->m_Material.m_Color;
            m_pModel->m_Mesh[i]->m_VB[j]->m_Material.m_Transparent = srcIQM.m_pModel->m_Mesh[i]->m_VB[j]->m_Material.m_Transparent;
            m_pModel->m_Mesh[i]->m_VB[j]->m_Material.m_Wireframe   = srcIQM.m_pModel->m_Mesh[i]->m_VB[j]->m_Material.m_Wireframe;
            m_pModel->m_Mesh[i]->m_VB[j]->m_Material.m_pTexture    = srcIQM.m_pModel->m_Mesh[i]->m_VB[j]->m_Material.m_pTexture->Clone();
        }
    }

    return true;
}
//---------------------------------------------------------------------------
void IQM::SetVertFormatTemplate(const VertexFormat& vertFormatTemplate)
{
    m_VertFormatTemplate = vertFormatTemplate;
}
//---------------------------------------------------------------------------
void IQM::SetVertCullingTemplate(const VertexCulling& vertCullingTemplate)
{
    m_VertCullingTemplate = vertCullingTemplate;
}
//---------------------------------------------------------------------------
void IQM::SetMaterial(const Material& materialTemplate)
{
    m_MaterialTemplate = materialTemplate;
}
//---------------------------------------------------------------------------
void IQM::Set_OnGetVertexColor(VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    m_fOnGetVertexColor = fOnGetVertexColor;
}
//---------------------------------------------------------------------------
void IQM::Set_OnLoadTexture(Texture::ITfOnLoadTexture fOnLoadTexture)
{
    m_fOnLoadTexture = fOnLoadTexture;
}
//---------------------------------------------------------------------------
bool IQM::Read(DWF_Buffer::Buffer& buffer)
{
    IHeader header;

    if (!header.Read(buffer))
        return false;

    // is iqm file and version correct?
    if (std::strcmp(header.m_ID, M_IQM_ID) != 0 || (header.m_Version != M_IQM_File_Version))
        return false;

    ITexts        texts;
    IMeshes       meshes;
    IVertexArrays vertexArrays;
    ITriangles    triangles;
    IAdjacencies  adjacencies;
    IJoints       joints;
    IPoses        poses;
    IAnims        anims;
    IBoundsArray  boundsArray;
    IComments     comments;
    IExtensions   extensions;
    bool          result = false;

    try
    {
        // do read texts?
        if (header.m_TextCount)
        {
            // seek to buffer data start position
            buffer.Seek(0, header.m_TextOffset);

            std::size_t start = 0;

            // iterate through text chars to read
            while (start < header.m_TextCount)
            {
                std::unique_ptr<IText> pText = std::make_unique<IText>();

                // read next text
                if (!pText->Read(header, start, buffer))
                    return false;

                texts.push_back(pText.get());
                pText.release();
            }
        }

        // do read meshes?
        if (header.m_MeshCount)
        {
            // seek to buffer data start position
            buffer.Seek(0, header.m_MeshOffset);

            // reserve memory for the meshes
            meshes.reserve(header.m_MeshCount);

            // iterate through meshes to read
            for (std::size_t i = 0; i < header.m_MeshCount; ++i)
            {
                std::unique_ptr<IMesh> pMesh = std::make_unique<IMesh>();

                // read next mesh
                if (!pMesh->Read(buffer))
                    return false;

                meshes.push_back(pMesh.get());
                pMesh.release();
            }
        }

        // do read vertex arrays?
        if (header.m_VertexArrayCount)
        {
            // seek to buffer data start position
            buffer.Seek(0, header.m_VertexArrayOffset);

            // reserve memory for the vertex arrays
            vertexArrays.reserve(header.m_VertexArrayCount);

            // iterate through vertex arrays to read
            for (std::size_t i = 0; i < header.m_VertexArrayCount; ++i)
            {
                std::unique_ptr<IVertexArray> pVertexArray = std::make_unique<IVertexArray>();

                // read next vertex array
                if (!pVertexArray->Read(buffer))
                    return false;

                vertexArrays.push_back(pVertexArray.get());
                pVertexArray.release();
            }
        }

        // do read triangles and adjacencies?
        if (header.m_TriangleCount)
        {
            // seek to buffer data start position
            buffer.Seek(0, header.m_TriangleOffset);

            // reserve memory for the triangles
            triangles.reserve(header.m_TriangleCount);

            // iterate through triangles to read
            for (std::size_t i = 0; i < header.m_TriangleCount; ++i)
            {
                std::unique_ptr<ITriangle> pTriangle = std::make_unique<ITriangle>();

                // read next triangle
                if (!pTriangle->Read(buffer))
                    return false;

                triangles.push_back(pTriangle.get());
                pTriangle.release();
            }

            // seek to buffer data start position
            buffer.Seek(0, header.m_AdjacencyOffset);

            // reserve memory for the adjacencies
            adjacencies.reserve(header.m_TriangleCount);

            // iterate through adjacencies to read
            for (std::size_t i = 0; i < header.m_TriangleCount; ++i)
            {
                std::unique_ptr<IAdjacency> pAdjacency = std::make_unique<IAdjacency>();

                // read next adjacency
                if (!pAdjacency->Read(buffer))
                    return false;

                adjacencies.push_back(pAdjacency.get());
                pAdjacency.release();
            }
        }

        // do read joints?
        if (header.m_JointCount)
        {
            // seek to buffer data start position
            buffer.Seek(0, header.m_JointOffset);

            // reserve memory for the joints
            joints.reserve(header.m_JointCount);

            // iterate through joints to read
            for (std::size_t i = 0; i < header.m_JointCount; ++i)
            {
                std::unique_ptr<IJoint> pJoint = std::make_unique<IJoint>();

                // read next joint
                if (!pJoint->Read(buffer))
                    return false;

                joints.push_back(pJoint.get());
                pJoint.release();
            }
        }

        // do read poses?
        if (header.m_PoseCount)
        {
            // seek to buffer data start position
            buffer.Seek(0, header.m_PoseOffset);

            // reserve memory for the poses
            poses.reserve(header.m_PoseCount);

            // iterate through poses to read
            for (std::size_t i = 0; i < header.m_PoseCount; ++i)
            {
                std::unique_ptr<IPose> pPose = std::make_unique<IPose>();

                // read next pose
                if (!pPose->Read(buffer))
                    return false;

                poses.push_back(pPose.get());
                pPose.release();
            }
        }

        // do read animations?
        if (header.m_AnimCount)
        {
            // seek to buffer data start position
            buffer.Seek(0, header.m_AnimOffset);

            // reserve memory for the animations
            anims.reserve(header.m_AnimCount);

            // iterate through animations to read
            for (std::size_t i = 0; i < header.m_AnimCount; ++i)
            {
                std::unique_ptr<IAnim> pAnim = std::make_unique<IAnim>();

                // read next animation
                if (!pAnim->Read(buffer))
                    return false;

                anims.push_back(pAnim.get());
                pAnim.release();
            }
        }

        // do read bounds arrays?
        if (header.m_FrameCount)
        {
            // seek to buffer data start position
            buffer.Seek(0, header.m_BoundsOffset);

            // reserve memory for the bounds arrays
            boundsArray.reserve(header.m_FrameCount);

            // iterate through bounds arrays to read
            for (std::size_t i = 0; i < header.m_FrameCount; ++i)
            {
                std::unique_ptr<IBounds> pBounds = std::make_unique<IBounds>();

                // read next bounds
                if (!pBounds->Read(buffer))
                    return false;

                boundsArray.push_back(pBounds.get());
                pBounds.release();
            }
        }

        // do read comments?
        if (header.m_CommentCount)
        {
            // seek to buffer data start position
            buffer.Seek(0, header.m_CommentOffset);

            std::size_t start = 0;

            // iterate through comment chars to read
            while (start < header.m_CommentCount)
            {
                std::unique_ptr<IComment> pComment = std::make_unique<IComment>();

                // read next comment
                if (!pComment->Read(header, start, buffer))
                    return false;

                comments.push_back(pComment.get());
                pComment.release();
            }
        }

        // do read extensions?
        if (header.m_ExtensionCount)
        {
            // seek to buffer data start position
            buffer.Seek(0, header.m_ExtensionOffset);

            // reserve memory for the extensions
            boundsArray.reserve(header.m_ExtensionCount);

            // iterate through extensions to read
            for (std::size_t i = 0; i < header.m_ExtensionCount; ++i)
            {
                std::unique_ptr<IExtension> pExtension = std::make_unique<IExtension>();

                // read next extension
                if (!pExtension->Read(buffer))
                    return false;

                extensions.push_back(pExtension.get());
                pExtension.release();
            }
        }

        // populate the model
        result = PopulateModel(buffer,
                               header,
                               texts,
                               meshes,
                               vertexArrays,
                               triangles,
                               adjacencies,
                               joints,
                               poses,
                               anims,
                               boundsArray,
                               comments,
                               extensions);
    }
    catch (...)
    {}

    for (std::size_t i = 0; i < texts.size(); ++i)
        delete texts[i];

    for (std::size_t i = 0; i < meshes.size(); ++i)
        delete meshes[i];

    for (std::size_t i = 0; i < vertexArrays.size(); ++i)
        delete vertexArrays[i];

    for (std::size_t i = 0; i < triangles.size(); ++i)
        delete triangles[i];

    for (std::size_t i = 0; i < adjacencies.size(); ++i)
        delete adjacencies[i];

    for (std::size_t i = 0; i < joints.size(); ++i)
        delete joints[i];

    for (std::size_t i = 0; i < poses.size(); ++i)
        delete poses[i];

    for (std::size_t i = 0; i < anims.size(); ++i)
        delete anims[i];

    for (std::size_t i = 0; i < boundsArray.size(); ++i)
        delete boundsArray[i];

    for (std::size_t i = 0; i < comments.size(); ++i)
        delete comments[i];

    for (std::size_t i = 0; i < extensions.size(); ++i)
        delete extensions[i];

    return result;
}
//---------------------------------------------------------------------------
bool IQM::PopulateModel(      DWF_Buffer::Buffer& buffer,
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
                        const IExtensions&        extensions)
{
    // create the model
    m_pModel             = new Model();
    m_pModel->m_MeshOnly = m_MeshOnly;
    m_pModel->m_PoseOnly = m_PoseOnly;

    // do create mesh only?
    if (!m_MeshOnly)
    {
        // create the root bone
        std::unique_ptr<Model::IBone> pRootBone = std::make_unique<Model::IBone>();
        pRootBone->m_Name = "RootBone";

        // populate the skeleton
        if (!PopulateSkeleton(texts, joints, -1, pRootBone.get()))
            return false;

        // set the skeleton in the model
        m_pModel->m_pSkeleton = pRootBone.release();

        // model contains animations and should create them?
        if (anims.size() && m_pModel->m_pSkeleton && !m_PoseOnly)
        {
            // create the animation sets
            m_pModel->m_AnimationSet.reserve(anims.size());

            // iterate through animation sets to initialize
            for (std::size_t i = 0; i < anims.size(); ++i)
            {
                // create a new animation set
                std::unique_ptr<Model::IAnimationSet> pAnimSet = std::make_unique<Model::IAnimationSet>();

                // reserve memory for animations
                pAnimSet->m_Animations.reserve(joints.size());

                // iterate through bone animations to create
                for (std::size_t j = 0; j < joints.size(); ++j)
                {
                    // create bone animation
                    std::unique_ptr<Model::IAnimation> pAnim = std::make_unique<Model::IAnimation>();

                    // add it to animation set. NOTE the animation will be populated by PopulateAnims() below
                    pAnimSet->m_Animations.push_back(pAnim.get());
                    pAnim.release();
                }

                // populate the animations
                if (!PopulateAnims(buffer, header, texts, anims, poses, m_pModel->m_pSkeleton, pAnimSet.get()))
                    return false;

                // add animation set to model
                m_pModel->m_AnimationSet.push_back(pAnimSet.get());
                pAnimSet.release();
            }
        }
    }

    // create a source vertices container
    IVertices srcVertices;

    // read the source vertices
    if (!BuildSrcVertices(header, vertexArrays, buffer, srcVertices))
        return false;

    // reserve memory for the meshes
    m_pModel->m_Mesh.reserve(meshes.size());

    // do build the weights?
    if (!m_MeshOnly)
        // reserve memory for the mesh deformers
        m_pModel->m_Deformers.reserve(meshes.size());

    // should take care of the pose only?
    if (!m_PoseOnly)
        m_VBCache.resize(meshes.size());

    // iterate through the source meshes
    for (std::size_t i = 0; i < meshes.size(); ++i)
    {
        std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

        // create the vertex buffer
        std::unique_ptr<VertexBuffer> pVB = std::make_unique<VertexBuffer>();

        // apply the user wished vertex format
        pVB->m_Format = m_VertFormatTemplate;

        // apply the user wished vertex culling
        pVB->m_Culling = m_VertCullingTemplate;

        // apply the user wished material
        pVB->m_Material = m_MaterialTemplate;

        // set the vertex format type
        pVB->m_Format.m_Type = VertexFormat::IEType::IE_VT_Triangles;

        // calculate the vertex stride
        pVB->m_Format.CalculateStride();

        Model::IDeformers* pCurDeformers = nullptr;

        // do build the weights?
        if (!m_MeshOnly)
        {
            std::unique_ptr<Model::IDeformers> pDeformers = std::make_unique<Model::IDeformers>();

            // populate the skin weights group
            if (!BuildWeightsFromSkeleton(m_pModel->m_pSkeleton, i, pDeformers.get()))
                return false;

            // add the deformers to the model
            m_pModel->m_Deformers.push_back(pDeformers.get());
            pCurDeformers = pDeformers.release();
        }

        // iterate through source mesh triangles
        for (std::size_t j = 0; j < meshes[i]->m_TriangleCount; ++j)
        {
            // get triangle index
            const size_t index = meshes[i]->m_FirstTriangle + j;

            // iterate through vertices to create
            for (std::size_t k = 0; k < 3; ++k)
            {
                // get vertex index
                const size_t vertIndex = triangles[index]->m_Vertex[k];

                // add next vertex to vertex buffer
                pVB->Add(&srcVertices[vertIndex]->m_Position,
                         &srcVertices[vertIndex]->m_Normal,
                         &srcVertices[vertIndex]->m_TexCoord,
                          0,
                          m_fOnGetVertexColor);

                // do build the weights?
                if (!m_MeshOnly)
                {
                    if (!pCurDeformers)
                        return false;

                    // iterate through vertex weights
                    for (std::size_t l = 0; l < 4 && srcVertices[vertIndex]->m_BlendWeights[l]; ++l)
                    {
                        // get the bone matching with weight to animate
                        Model::IBone* pBone = FindBone(m_pModel->m_pSkeleton,
                                                       srcVertices[vertIndex]->m_BlendIndices[l]);

                        // found it?
                        if (!pBone)
                            return false;

                        Model::ISkinWeights* pWeights = nullptr;

                        // get the skin weights to populate
                        for (std::size_t m = 0; m < pCurDeformers->m_SkinWeights.size(); ++m)
                            if (pCurDeformers->m_SkinWeights[m]->m_BoneName == pBone->m_Name)
                            {
                                pWeights = pCurDeformers->m_SkinWeights[m];
                                break;
                            }

                        // found it?
                        if (!pWeights)
                            return false;

                        // number of weights should always equals the index table one
                        if (pWeights->m_Weights.size() != pWeights->m_WeightInfluences.size())
                            return false;

                        // write the weight value
                        pWeights->m_Weights.push_back((float)srcVertices[vertIndex]->m_BlendWeights[l] / 255.0f);

                        std::unique_ptr<Model::IWeightInfluence> pWeightInfluence = std::make_unique<Model::IWeightInfluence>();

                        // populate the index
                        pWeightInfluence->m_VertexIndex.push_back((std::size_t)((j * 3) + k) * pVB->m_Format.m_Stride);

                        pWeights->m_WeightInfluences.push_back(pWeightInfluence.get());
                        pWeightInfluence.release();
                    }
                }
            }
        }

        // do load the texture?
        if ((std::size_t)pVB->m_Format.m_Format & (std::size_t)VertexFormat::IEFormat::IE_VF_TexCoords &&
            meshes[i]->m_Material                                                                      &&
            m_fOnLoadTexture)
        {
            const size_t materialIndex              = GetTextIndex(texts, meshes[i]->m_Material);
                         pVB->m_Material.m_pTexture = m_fOnLoadTexture(texts[materialIndex]->m_Text, true);
        }

        // should take care of the pose only?
        if (!m_PoseOnly)
        {
            // cache the vertex buffer
             m_VBCache[i] = new VertexBuffer::IData();
            *m_VBCache[i] = pVB->m_Data;
        }

        // set the vertex buffer in the mesh
        pMesh->m_VB.push_back(pVB.get());
        pVB.release();

        // add mesh to model meshes
        m_pModel->m_Mesh.push_back(pMesh.get());
        pMesh.release();
    }

    return true;
}
//---------------------------------------------------------------------------
bool IQM::PopulateSkeleton(const ITexts&       texts,
                           const IJoints&      joints,
                                 int           parentIndex,
                                 Model::IBone* pRoot) const
{
    // iterate through source joints
    for (std::size_t i = 0; i < joints.size(); ++i)
    {
        // create new child bone
        std::unique_ptr<Model::IBone> pBone = std::make_unique<Model::IBone>();

        // is root?
        if (joints[i]->m_Parent >= 0)
        {
            // find the parent bone to link with
            Model::IBone* pParent = FindBone(pRoot, joints[i]->m_Parent);

            // found it?
            if (!pParent)
                return false;

            // set the parent
            pBone->m_pParent = pParent;

            // populate the child bone
            if (!PopulateBone(texts, joints[i], i, pBone.get()))
                return false;

            // set it in parent bone
            pParent->m_Children.push_back(pBone.get());
            pBone.release();
        }
        else
        {
            // set the parent
            pBone->m_pParent = pRoot;

            // populate the child bone
            if (!PopulateBone(texts, joints[i], i, pBone.get()))
                return false;

            // set it in parent bone
            pRoot->m_Children.push_back(pBone.get());
            pBone.release();
        }
    }

    return true;
}
//---------------------------------------------------------------------------
bool IQM::PopulateBone(const ITexts&       texts,
                       const IJoint*       pJoint,
                             std::size_t   jointIndex,
                             Model::IBone* pBone) const
{
    if (!pJoint || !pBone)
        return false;

    // get the bone name index in the text array
    const std::size_t boneNameIndex = GetTextIndex(texts, pJoint->m_Name);

    // found it?
    if (boneNameIndex >= texts.size())
        return false;

    // get the bone name
    pBone->m_Name = texts[boneNameIndex]->m_Text;

    DWF_Math::Vector3F position;

    // get the position
    position.m_X = pJoint->m_Translate[0];
    position.m_Y = pJoint->m_Translate[1];
    position.m_Z = pJoint->m_Translate[2];

    DWF_Math::QuaternionF rotation;

    // get the rotation
    rotation.m_X = pJoint->m_Rotate[0];
    rotation.m_Y = pJoint->m_Rotate[1];
    rotation.m_Z = pJoint->m_Rotate[2];
    rotation.m_W = pJoint->m_Rotate[3];

    DWF_Math::Vector3F scaling;

    // get the scaling
    scaling.m_X = pJoint->m_Scale[0];
    scaling.m_Y = pJoint->m_Scale[1];
    scaling.m_Z = pJoint->m_Scale[2];

    DWF_Math::Matrix4x4F scaleMatrix;
    DWF_Math::Matrix4x4F rotateMatrix;
    DWF_Math::Matrix4x4F translateMatrix;

    // get the rotation quaternion and the scale and translate vectors
    scaleMatrix     = DWF_Math::Matrix4x4F::Identity().Scale(scaling);
    QuatToRotMat(rotation, rotateMatrix);
    translateMatrix = DWF_Math::Matrix4x4F::Identity().Translate(position);

    // build the final matrix
    const DWF_Math::Matrix4x4F buildMatrix     = scaleMatrix.Multiply(rotateMatrix);
                               pBone->m_Matrix = buildMatrix.Multiply(translateMatrix);

    // allocate memory for the joint index
    pBone->m_pCustom = new std::size_t();

    // succeeded?
    if (!pBone->m_pCustom)
        return false;

    // link the mesh to the bone owning it
    *static_cast<std::size_t*>(pBone->m_pCustom) = jointIndex;

    return true;
}
//---------------------------------------------------------------------------
bool IQM::PopulateAnims(      DWF_Buffer::Buffer&   buffer,
                        const IHeader&              header,
                        const ITexts&               texts,
                        const IAnims&               anims,
                        const IPoses&               poses,
                              Model::IBone*         pRootBone,
                              Model::IAnimationSet* pAnimSet)
{
    // seek to buffer data start position
    buffer.Seek(0, header.m_FrameOffset);

    // iterate through animation sets to create
    for (std::size_t i = 0; i < anims.size(); ++i)
    {
        const IAnim* pSrcAnim = anims[i];

        // iterate through bone animations to create
        for (std::size_t j = 0; j < pSrcAnim->m_FrameCount; ++j)
            // iterate through frames to create
            for (std::size_t k = 0; k < poses.size(); ++k)
            {
                // get next pose
                const IPose* pPose = poses[k];

                // get destination animation to populate
                Model::IAnimation* pDstAnim = pAnimSet->m_Animations[k];

                // do initialize the animation set?
                if (!j)
                {
                    // find the bone to link with animation
                    pDstAnim->m_pBone = FindBone(pRootBone, k);

                    // found it?
                    if (!pDstAnim->m_pBone)
                        return false;

                    // copy the bone name
                    pDstAnim->m_BoneName = pDstAnim->m_pBone->m_Name;

                    // create the animation keys
                    std::unique_ptr<Model::IAnimationKeys> pKeys = std::make_unique<Model::IAnimationKeys>();

                    // initialize the keys container
                    pKeys->m_Type = Model::IEAnimKeyType::IE_KT_MatrixKeys;

                    // reserve memory for the key list
                    pKeys->m_Keys.reserve(pSrcAnim->m_FrameCount);

                    pDstAnim->m_Keys.push_back(pKeys.get());
                    pKeys.release();
                }

                unsigned short value;

                DWF_Math::Vector3F position;
                position.m_X = pPose->m_ChannelOffset[0];

                // is position x value animated?
                if (pPose->m_ChannelMask & 0x01)
                {
                    // read the animation data
                    if (!buffer.Read(&value, sizeof(unsigned short)))
                        return false;

                    position.m_X += value * pPose->m_ChannelScale[0];
                }

                position.m_Y = pPose->m_ChannelOffset[1];

                // is position y value animated?
                if (pPose->m_ChannelMask & 0x02)
                {
                    // read the animation data
                    if (!buffer.Read(&value, sizeof(unsigned short)))
                        return false;

                    position.m_Y += value * pPose->m_ChannelScale[1];
                }

                position.m_Z = pPose->m_ChannelOffset[2];

                // is position z value animated?
                if (pPose->m_ChannelMask & 0x04)
                {
                    // read the animation data
                    if (!buffer.Read(&value, sizeof(unsigned short)))
                        return false;

                    position.m_Z += value * pPose->m_ChannelScale[2];
                }

                DWF_Math::QuaternionF rotation;
                rotation.m_X = pPose->m_ChannelOffset[3];

                // is rotation x value animated?
                if (pPose->m_ChannelMask & 0x08)
                {
                    // read the animation data
                    if (!buffer.Read(&value, sizeof(unsigned short)))
                        return false;

                    rotation.m_X += value * pPose->m_ChannelScale[3];
                }

                rotation.m_Y = pPose->m_ChannelOffset[4];

                // is rotation y value animated?
                if (pPose->m_ChannelMask & 0x10)
                {
                    // read the animation data
                    if (!buffer.Read(&value, sizeof(unsigned short)))
                        return false;

                    rotation.m_Y += value * pPose->m_ChannelScale[4];
                }

                rotation.m_Z = pPose->m_ChannelOffset[5];

                // is rotation z animation animated?
                if (pPose->m_ChannelMask & 0x20)
                {
                    // read the frame data
                    if (!buffer.Read(&value, sizeof(unsigned short)))
                        return false;

                    rotation.m_Z += value * pPose->m_ChannelScale[5];
                }

                rotation.m_W = pPose->m_ChannelOffset[6];

                // is rotation w value animated?
                if (pPose->m_ChannelMask & 0x40)
                {
                    // read the animation data
                    if (!buffer.Read(&value, sizeof(unsigned short)))
                        return false;

                    rotation.m_W += value * pPose->m_ChannelScale[6];
                }

                DWF_Math::Vector3F scaling;
                scaling.m_X = pPose->m_ChannelOffset[7];

                // is scaling x value animated?
                if (pPose->m_ChannelMask & 0x80)
                {
                    // read the animation data
                    if (!buffer.Read(&value, sizeof(unsigned short)))
                        return false;

                    scaling.m_X += value * pPose->m_ChannelScale[7];
                }

                scaling.m_Y = pPose->m_ChannelOffset[8];

                // is scaling y value animated?
                if (pPose->m_ChannelMask & 0x100)
                {
                    // read the animation data
                    if (!buffer.Read(&value, sizeof(unsigned short)))
                        return false;

                    scaling.m_Y += value * pPose->m_ChannelScale[8];
                }

                scaling.m_Z = pPose->m_ChannelOffset[9];

                // is scaling z value animated?
                if (pPose->m_ChannelMask & 0x200)
                {
                    // read the animation data
                    if (!buffer.Read(&value, sizeof(unsigned short)))
                        return false;

                    scaling.m_Z += value * pPose->m_ChannelScale[9];
                }

                DWF_Math::Matrix4x4F scaleMatrix;
                DWF_Math::Matrix4x4F rotateMatrix;
                DWF_Math::Matrix4x4F translateMatrix;
                DWF_Math::Matrix4x4F buildMatrix;
                DWF_Math::Matrix4x4F matrix;

                // get the rotation quaternion and the scale and translate vectors
                scaleMatrix     = DWF_Math::Matrix4x4F::Identity().Scale(scaling);
                QuatToRotMat(rotation, rotateMatrix);
                translateMatrix = DWF_Math::Matrix4x4F::Identity().Translate(position);

                // build the final matrix
                buildMatrix = scaleMatrix.Multiply(rotateMatrix);
                matrix      = buildMatrix.Multiply(translateMatrix);

                // create the animation key
                std::unique_ptr<Model::IAnimationKey> pKey = std::make_unique<Model::IAnimationKey>();

                // reserve memory for the matrix
                pKey->m_Values.reserve(16);

                // set the matrix content in the animation key
                for (std::size_t x = 0; x < 4; ++x)
                    for (std::size_t y = 0; y < 4; ++y)
                        pKey->m_Values.push_back(matrix.m_Table[x][y]);

                // set the frame number
                pKey->m_Frame = pDstAnim->m_Keys[0]->m_Keys.size();

                // add the new animation key to the key set
                pDstAnim->m_Keys[0]->m_Keys.push_back(pKey.get());
                pKey.release();
            }
    }

    return true;
}
//---------------------------------------------------------------------------
bool IQM::BuildSrcVertices(const IHeader&            header,
                           const IVertexArrays&      vertexArrays,
                                 DWF_Buffer::Buffer& buffer,
                                 IVertices&          srcVertices)
{
    // reserve memory for the vertex array
    srcVertices.reserve(header.m_VertexCount);

    // iterate through vertex arrays
    for (std::size_t i = 0; i < vertexArrays.size(); ++i)
    {
        // get source vertex array
        IVertexArray* pVertexArray = vertexArrays[i];

        // seek to buffer data start position
        buffer.Seek(0, pVertexArray->m_Offset);

        // search for vertex data type to read
        switch ((IQM::IEVertexArrayType)pVertexArray->m_Type)
        {
            case IQM::IEVertexArrayType::IE_VT_Position:
                // is vertex format valid?
                if (pVertexArray->m_Format != (int)IQM::IEVertexArrayFormat::IE_VF_Float || pVertexArray->m_Size != 3)
                    return false;

                // iterate through vertices to read
                for (std::size_t j = 0; j < (std::size_t)header.m_VertexCount; ++j)
                {
                    IVertex* pSrcVertex = GetOrAddVertex(j, srcVertices);

                    // iterate through vertex data size
                    for (std::size_t k = 0; k < (std::size_t)pVertexArray->m_Size; ++k)
                    {
                        float value;

                        // read vertex data from buffer
                        if (!buffer.Read(&value, sizeof(float)))
                            return false;

                        #ifdef CONVERT_ENDIANNESS
                            // the read bytes are inverted and should be swapped if the target system is big endian
                            if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
                                // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
                                SwapMemory(&value, sizeof(float));
                        #endif

                        switch (k)
                        {
                            case 0: pSrcVertex->m_Position.m_X = value; break;
                            case 1: pSrcVertex->m_Position.m_Y = value; break;
                            case 2: pSrcVertex->m_Position.m_Z = value; break;
                        }
                    }
                }

                break;

            case IQM::IEVertexArrayType::IE_VT_TexCoord:
                // is vertex format valid?
                if (pVertexArray->m_Format != (int)IQM::IEVertexArrayFormat::IE_VF_Float || pVertexArray->m_Size != 2)
                    return false;

                // iterate through vertices to read
                for (std::size_t j = 0; j < (std::size_t)header.m_VertexCount; ++j)
                {
                    IVertex* pSrcVertex = GetOrAddVertex(j, srcVertices);

                    // iterate through vertex data size
                    for (std::size_t k = 0; k < (std::size_t)pVertexArray->m_Size; ++k)
                    {
                        float value;

                        // read vertex data from buffer
                        if (!buffer.Read(&value, sizeof(float)))
                            return false;

                        #ifdef CONVERT_ENDIANNESS
                            // the read bytes are inverted and should be swapped if the target system is big endian
                            if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
                                // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
                                SwapMemory(&value, sizeof(float));
                        #endif

                        switch (k)
                        {
                            case 0: pSrcVertex->m_TexCoord.m_X = value; break;
                            case 1: pSrcVertex->m_TexCoord.m_Y = value; break;
                        }
                    }
                }

                break;

            case IQM::IEVertexArrayType::IE_VT_Normal:
                // is vertex format valid?
                if (pVertexArray->m_Format != (int)IQM::IEVertexArrayFormat::IE_VF_Float || pVertexArray->m_Size != 3)
                    return false;

                // iterate through vertices to read
                for (std::size_t j = 0; j < (std::size_t)header.m_VertexCount; ++j)
                {
                    IVertex* pSrcVertex = GetOrAddVertex(j, srcVertices);

                    // iterate through vertex data size
                    for (std::size_t k = 0; k < (std::size_t)pVertexArray->m_Size; ++k)
                    {
                        float value;

                        // read vertex data from buffer
                        if (!buffer.Read(&value, sizeof(float)))
                            return false;

                        #ifdef CONVERT_ENDIANNESS
                            // the read bytes are inverted and should be swapped if the target system is big endian
                            if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
                                // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
                                SwapMemory(&value, sizeof(float));
                        #endif

                        switch (k)
                        {
                            case 0: pSrcVertex->m_Normal.m_X = value; break;
                            case 1: pSrcVertex->m_Normal.m_Y = value; break;
                            case 2: pSrcVertex->m_Normal.m_Z = value; break;
                        }
                    }
                }

                break;

            case IQM::IEVertexArrayType::IE_VT_Color:
                // is vertex format valid?
                if (pVertexArray->m_Format != (int)IQM::IEVertexArrayFormat::IE_VF_UByte || pVertexArray->m_Size != 4)
                    return false;

                // iterate through vertices to read
                for (std::size_t j = 0; j < (std::size_t)header.m_VertexCount; ++j)
                {
                    IVertex* pSrcVertex = GetOrAddVertex(j, srcVertices);

                    // iterate through vertex data size
                    for (std::size_t k = 0; k < (std::size_t)pVertexArray->m_Size; ++k)
                        // read vertex data from buffer
                        if (!buffer.Read(&pSrcVertex->m_Color[k], sizeof(unsigned char)))
                            return false;
                }

                break;

            case IQM::IEVertexArrayType::IE_VT_Tangent:
                // is vertex format valid?
                if (pVertexArray->m_Format != (int)IQM::IEVertexArrayFormat::IE_VF_Float || pVertexArray->m_Size != 4)
                    return false;

                // iterate through vertices to read
                for (std::size_t j = 0; j < (std::size_t)header.m_VertexCount; ++j)
                {
                    IVertex* pSrcVertex = GetOrAddVertex(j, srcVertices);

                    // iterate through vertex data size
                    for (std::size_t k = 0; k < (std::size_t)pVertexArray->m_Size; ++k)
                    {
                        // read vertex data from buffer
                        if (!buffer.Read(&pSrcVertex->m_Tangent[k], sizeof(float)))
                            return false;

                        #ifdef CONVERT_ENDIANNESS
                            // the read bytes are inverted and should be swapped if the target system is big endian
                            if (GetEndianness() == IQM::IEEndianness::IE_E_BigEndian)
                                // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
                                SwapMemory(&pSrcVertex->m_Tangent[k], sizeof(float));
                        #endif
                    }
                }

                break;

            case IQM::IEVertexArrayType::IE_VT_BlendIndexes:
                // is vertex format valid?
                if (pVertexArray->m_Format != (int)IQM::IEVertexArrayFormat::IE_VF_UByte || pVertexArray->m_Size != 4)
                    return false;

                // iterate through vertices to read
                for (std::size_t j = 0; j < (std::size_t)header.m_VertexCount; ++j)
                {
                    IVertex* pSrcVertex = GetOrAddVertex(j, srcVertices);

                    // iterate through vertex data size
                    for (std::size_t k = 0; k < (size_t)pVertexArray->m_Size; ++k)
                        // read vertex data from buffer
                        if (!buffer.Read(&pSrcVertex->m_BlendIndices[k], sizeof(unsigned char)))
                            return false;
                }

                break;

            case IQM::IEVertexArrayType::IE_VT_BlendWeights:
                // is vertex format valid?
                if (pVertexArray->m_Format != (int)IQM::IEVertexArrayFormat::IE_VF_UByte || pVertexArray->m_Size != 4)
                    return false;

                // iterate through vertices to read
                for (std::size_t j = 0; j < (std::size_t)header.m_VertexCount; ++j)
                {
                    IVertex* pSrcVertex = GetOrAddVertex(j, srcVertices);

                    // iterate through vertex data size
                    for (std::size_t k = 0; k < (size_t)pVertexArray->m_Size; ++k)
                        // read vertex data from buffer
                        if (!buffer.Read(&pSrcVertex->m_BlendWeights[k], sizeof(unsigned char)))
                            return false;
                }

                break;
        }
    }

    return true;
}
//---------------------------------------------------------------------------
bool IQM::BuildWeightsFromSkeleton(Model::IBone*      pBone,
                                   std::size_t        meshIndex,
                                   Model::IDeformers* pDeformers)
{
    if (!pBone)
        return false;

    if (!pDeformers)
        return false;

    std::unique_ptr<Model::ISkinWeights> pSkinWeights = std::make_unique<Model::ISkinWeights>();

    // populate the bone infos
    pSkinWeights->m_pBone = pBone;

    if (pBone->m_Name.empty())
        return false;

    // copy the bone name
    pSkinWeights->m_BoneName = pBone->m_Name;

    // get the inverse bind matrix
    GetInverseBindMatrix(pBone, pSkinWeights->m_Matrix);

    // iterate through children bones to create
    for (std::size_t i = 0; i < pBone->m_Children.size(); ++i)
        // build from next child bone
        if (!BuildWeightsFromSkeleton(pBone->m_Children[i], meshIndex, pDeformers))
            return false;

    // add the skin weights in the deformers
    pDeformers->m_SkinWeights.push_back(pSkinWeights.get());
    pSkinWeights.release();

    return true;
}
//---------------------------------------------------------------------------
IQM::IVertex* IQM::GetOrAddVertex(std::size_t index, IVertices& vertices) const
{
    // vertex exists in vertices table?
    if (index < vertices.size())
        return vertices[index];

    // add it
    std::unique_ptr<IVertex> pVertex = std::make_unique<IVertex>();
    vertices.push_back(pVertex.get());

    return pVertex.release();
}
//---------------------------------------------------------------------------
Model::IBone* IQM::FindBone(Model::IBone* pBone, std::size_t index) const
{
    if (!pBone)
        return nullptr;

    if (pBone->m_pCustom && *static_cast<std::size_t*>(pBone->m_pCustom) == index)
        return pBone;

    for (std::size_t i = 0; i < pBone->m_Children.size(); ++i)
    {
        Model::IBone* pChild = FindBone(pBone->m_Children[i], index);

        if (pChild)
            return pChild;
    }

    return nullptr;
}
//---------------------------------------------------------------------------
void IQM::QuatToRotMat(const DWF_Math::QuaternionF& quat, DWF_Math::Matrix4x4F& mat) const
{
    // initialize the constants to use for conversion
    const float tx  = 2.0f     * quat.m_X;
    const float ty  = 2.0f     * quat.m_Y;
    const float tz  = 2.0f     * quat.m_Z;
    const float txx = tx       * quat.m_X;
    const float tyy = ty       * quat.m_Y;
    const float tzz = tz       * quat.m_Z;
    const float txy = tx       * quat.m_Y;
    const float txz = tx       * quat.m_Z;
    const float tyz = ty       * quat.m_Z;
    const float twx = quat.m_W * tx;
    const float twy = quat.m_W * ty;
    const float twz = quat.m_W * tz;

    // initialize the rotation matrix
    mat = DWF_Math::Matrix4x4F::Identity();

    // build it
    mat.m_Table[0][0] = 1.0f - (tyy + tzz);
    mat.m_Table[1][0] = txy  - twz;
    mat.m_Table[2][0] = txz  + twy;

    mat.m_Table[0][1] = txy  + twz;
    mat.m_Table[1][1] = 1.0f - (txx + tzz);
    mat.m_Table[2][1] = tyz  - twx;

    mat.m_Table[0][2] = txz  - twy;
    mat.m_Table[1][2] = tyz  + twx;
    mat.m_Table[2][2] = 1.0f - (txx + tyy);
}
//---------------------------------------------------------------------------
int IQM::GetTextIndex(const ITexts& texts, std::size_t offset) const
{
    for (std::size_t i = 0; i < texts.size(); ++i)
        if (texts[i]->m_Offset == offset)
            return (int)i;

    return -1;
}
//---------------------------------------------------------------------------
void IQM::GetInverseBindMatrix(const Model::IBone* pBone, DWF_Math::Matrix4x4F& outMatrix) const
{
    // no bone?
    if (!pBone)
        return;

    // set the intermediate matrix as identity
    DWF_Math::Matrix4x4F matrix = DWF_Math::Matrix4x4F::Identity();
    DWF_Math::Matrix4x4F localMatrix;

    // iterate through bones
    while (pBone)
    {
        // get the previously stacked matrix as base to calculate the new one
        localMatrix = matrix;

        // stack the previously calculated matrix with the current bone one
        matrix = localMatrix.Multiply(pBone->m_Matrix);

        // go to parent bone
        pBone = pBone->m_pParent;
    }

    float determinant;

    // get the inverse bind matrix
    outMatrix = matrix.Inverse(determinant);
}
//---------------------------------------------------------------------------
bool IQM::GetAnimationMatrix(const Model::IAnimationSet* pAnimSet,
                             const Model::IBone*         pBone,
                                   std::size_t           frame,
                                   DWF_Math::Matrix4x4F& matrix) const
{
    // no animation set?
    if (!pAnimSet)
        return false;

    // no bone?
    if (!pBone)
        return false;

    // iterate through animations
    for (std::size_t i = 0; i < pAnimSet->m_Animations.size(); ++i)
    {
        // found the animation matching with the bone for which the matrix should be get?
        if (pAnimSet->m_Animations[i]->m_pBone != pBone)
            continue;

        std::size_t           rotFrame       = 0;
        std::size_t           nextRotFrame   = 0;
        std::size_t           posFrame       = 0;
        std::size_t           nextPosFrame   = 0;
        std::size_t           scaleFrame     = 0;
        std::size_t           nextScaleFrame = 0;
        DWF_Math::QuaternionF rotation;
        DWF_Math::QuaternionF nextRotation;
        DWF_Math::QuaternionF finalRotation;
        DWF_Math::Vector3F    position;
        DWF_Math::Vector3F    nextPosition;
        DWF_Math::Vector3F    finalPosition;
        DWF_Math::Vector3F    scaling;
        DWF_Math::Vector3F    nextScaling;
        DWF_Math::Vector3F    finalScaling;

        // iterate through animation keys
        for (std::size_t j = 0; j < pAnimSet->m_Animations[i]->m_Keys.size(); ++j)
        {
            std::size_t keyIndex = 0;

            // iterate through animation key items
            for (std::size_t k = 0; k < pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys.size(); ++k)
                if (frame >= pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[k]->m_Frame)
                    keyIndex = k;
                else
                    break;

            // search for keys type
            switch (pAnimSet->m_Animations[i]->m_Keys[j]->m_Type)
            {
                case Model::IEAnimKeyType::IE_KT_Rotation:
                    if (pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values.size() != 4)
                        return false;

                    // get the rotation quaternion at index
                    rotation.m_W = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[0];
                    rotation.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[1];
                    rotation.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[2];
                    rotation.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[3];
                    rotFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Frame;

                    // get the next rotation quaternion
                    if (keyIndex + 1 >= pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys.size())
                    {
                        nextRotation.m_W = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[0];
                        nextRotation.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[1];
                        nextRotation.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[2];
                        nextRotation.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[3];
                        nextRotFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Frame;
                    }
                    else
                    {
                        nextRotation.m_W = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[0];
                        nextRotation.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[1];
                        nextRotation.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[2];
                        nextRotation.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[3];
                        nextRotFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Frame;
                    }

                    continue;

                case Model::IEAnimKeyType::IE_KT_Scale:
                    if (pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values.size() != 3)
                        return false;

                    // get the scale values at index
                    scaling.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[0];
                    scaling.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[1];
                    scaling.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[2];
                    scaleFrame  = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Frame;

                    // get the next rotation quaternion
                    if (keyIndex + 1 >= pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys.size())
                    {
                        nextScaling.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[0];
                        nextScaling.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[1];
                        nextScaling.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[2];
                        nextScaleFrame  = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Frame;
                    }
                    else
                    {
                        nextScaling.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[0];
                        nextScaling.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[1];
                        nextScaling.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[2];
                        nextScaleFrame  = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Frame;
                    }

                    continue;

                case Model::IEAnimKeyType::IE_KT_Position:
                    if (pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values.size() != 3)
                        return false;

                    // get the position values at index
                    position.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[0];
                    position.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[1];
                    position.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[2];
                    posFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Frame;

                    // get the next rotation quaternion
                    if (keyIndex + 1 >= pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys.size())
                    {
                        nextPosition.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[0];
                        nextPosition.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[1];
                        nextPosition.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[2];
                        nextPosFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Frame;
                    }
                    else
                    {
                        nextPosition.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[0];
                        nextPosition.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[1];
                        nextPosition.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[2];
                        nextPosFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Frame;
                    }

                    continue;

                case Model::IEAnimKeyType::IE_KT_MatrixKeys:
                    if (pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values.size() != 16)
                        return false;

                    // get the key matrix
                    for (std::size_t k = 0; k < 16; ++k)
                        if (pAnimSet->m_Animations[i]->m_Keys[j]->m_ColOverRow)
                            matrix.m_Table[k % 4][k / 4] =
                                    pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[k];
                        else
                            matrix.m_Table[k / 4][k % 4] =
                                    pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[k];

                    return true;

                default:
                    continue;
            }
        }

        // calculate the frame delta, the frame length and the interpolation for the position
        float frameDelta    = (float)(frame - posFrame);
        float frameLength   = (float)(nextPosFrame - posFrame);
        float interpolation = frameDelta / frameLength;

        // interpolate the position
        finalPosition.m_X = position.m_X + ((nextPosition.m_X - position.m_X) * interpolation);
        finalPosition.m_Y = position.m_Y + ((nextPosition.m_Y - position.m_Y) * interpolation);
        finalPosition.m_Z = position.m_Z + ((nextPosition.m_Z - position.m_Z) * interpolation);

        // calculate the frame delta, the frame length and the interpolation for the rotation
        frameDelta    = (float)(frame - rotFrame);
        frameLength   = (float)(nextRotFrame - rotFrame);
        interpolation = frameDelta / frameLength;

        bool error = false;

        // interpolate the rotation
        finalRotation = rotation.Slerp(nextRotation, interpolation, error);

        // calculate the frame delta, the frame length and the interpolation for the scaling
        frameDelta    = (float)(frame - scaleFrame);
        frameLength   = (float)(nextScaleFrame - scaleFrame);
        interpolation = frameDelta / frameLength;

        // interpolate the scaling
        finalScaling.m_X = scaling.m_X + ((nextScaling.m_X - scaling.m_X) * interpolation);
        finalScaling.m_Y = scaling.m_Y + ((nextScaling.m_Y - scaling.m_Y) * interpolation);
        finalScaling.m_Z = scaling.m_Z + ((nextScaling.m_Z - scaling.m_Z) * interpolation);

        DWF_Math::Matrix4x4F translateMatrix = DWF_Math::Matrix4x4F::Identity();
        DWF_Math::Matrix4x4F rotateMatrix;
        DWF_Math::Matrix4x4F scaleMatrix     = DWF_Math::Matrix4x4F::Identity();

        // build the translation, rotation and scaling matrices
        translateMatrix.Translate(finalPosition);
        rotateMatrix = finalRotation.ToMatrix();
        scaleMatrix.Scale(finalScaling);

        // build the final matrix
        matrix = rotateMatrix.Multiply(translateMatrix).Multiply(scaleMatrix);

        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
#ifdef CONVERT_ENDIANNESS
    IQM::IEEndianness IQM::GetEndianness()
    {
        const int i = 1;

        return (!*((char*)&i)) ?
                IQM::IEEndianness::IE_E_BigEndian :
                IQM::IEEndianness::IE_E_LittleEndian;
    }
#endif
//---------------------------------------------------------------------------
#ifdef CONVERT_ENDIANNESS
    void IQM::SwapMemory(void* pMemory, size_t size)
    {
              unsigned char* pBytes   = static_cast<unsigned char*>(pMemory);
        const std::size_t    halfSize = size >> 1;

        // iterate through bytes to swap
        for (std::size_t i = 0; i < halfSize; ++i)
        {
            const std::size_t lastIndex = (size - 1) - i;

            // swap the byte content (XOR method)
            pBytes[i]         = pBytes[i] ^ pBytes[lastIndex];
            pBytes[lastIndex] = pBytes[i] ^ pBytes[lastIndex];
            pBytes[i]         = pBytes[i] ^ pBytes[lastIndex];
        }
    }
#endif
//---------------------------------------------------------------------------
