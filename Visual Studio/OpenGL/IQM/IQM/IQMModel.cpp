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

#include "IQMModel.h"

// std
#include <stdlib.h>
#include <math.h>
#include <string.h>

// classes
#include "Quaternion.h"
#include "StdFileBuffer.h"

#pragma warning(disable: 26812)

//---------------------------------------------------------------------------
// Global defines
//---------------------------------------------------------------------------
#define M_IQM_ID            "INTERQUAKEMODEL\0"
#define M_IQM_File_Version  2

//---------------------------------------------------------------------------
// IQMModel::IHeader
//---------------------------------------------------------------------------
IQMModel::IHeader::IHeader()
{}
//---------------------------------------------------------------------------
IQMModel::IHeader::~IHeader()
{}
//---------------------------------------------------------------------------
bool IQMModel::IHeader::Read(Buffer& buffer)
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
        if (GetEndianness() == IQMModel::IEEndianness::IE_E_BigEndian)
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
// IQMModel::IText
//---------------------------------------------------------------------------
IQMModel::IText::IText()
{}
//---------------------------------------------------------------------------
IQMModel::IText::~IText()
{}
//---------------------------------------------------------------------------
bool IQMModel::IText::Read(const IHeader& header, std::size_t& start, Buffer& buffer)
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
// IQMModel::IMesh
//---------------------------------------------------------------------------
IQMModel::IMesh::IMesh()
{}
//---------------------------------------------------------------------------
IQMModel::IMesh::~IMesh()
{}
//---------------------------------------------------------------------------
bool IQMModel::IMesh::Read(Buffer& buffer)
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
        if (GetEndianness() == IQMModel::IEEndianness::IE_E_BigEndian)
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
// IQMModel::IVertexArray
//---------------------------------------------------------------------------
IQMModel::IVertexArray::IVertexArray()
{}
//---------------------------------------------------------------------------
IQMModel::IVertexArray::~IVertexArray()
{}
//---------------------------------------------------------------------------
bool IQMModel::IVertexArray::Read(Buffer& buffer)
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
        if (GetEndianness() == IQMModel::IEEndianness::IE_E_BigEndian)
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
// IQMModel::ITriangle
//---------------------------------------------------------------------------
IQMModel::ITriangle::ITriangle()
{}
//---------------------------------------------------------------------------
IQMModel::ITriangle::~ITriangle()
{}
//---------------------------------------------------------------------------
bool IQMModel::ITriangle::Read(Buffer& buffer)
{
    // read the triangle content
    for (std::size_t i = 0; i < 3; ++i)
    {
        if (!buffer.Read(&m_Vertex[i], sizeof(unsigned)))
            return false;

        #ifdef CONVERT_ENDIANNESS
            // the read bytes are inverted and should be swapped if the target system is big endian
            if (GetEndianness() == IQMModel::IEEndianness::IE_E_BigEndian)
                // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
                SwapMemory(&m_Vertex[i], sizeof(unsigned));
        #endif
    }

    return true;
}
//---------------------------------------------------------------------------
// IQMModel::IAdjacency
//---------------------------------------------------------------------------
IQMModel::IAdjacency::IAdjacency()
{}
//---------------------------------------------------------------------------
IQMModel::IAdjacency::~IAdjacency()
{}
//---------------------------------------------------------------------------
bool IQMModel::IAdjacency::Read(Buffer& buffer)
{
    // read the triangle content
    for (std::size_t i = 0; i < 3; ++i)
    {
        if (!buffer.Read(&m_Triangle[i], sizeof(unsigned)))
            return false;

        #ifdef CONVERT_ENDIANNESS
            // the read bytes are inverted and should be swapped if the target system is big endian
            if (GetEndianness() == IQMModel::IEEndianness::IE_E_BigEndian)
                // swap the read values in the memory (thus 0xAABBCCDD will become 0xDDCCBBAA)
                SwapMemory(&m_Triangle[i], sizeof(unsigned));
        #endif
    }

    return true;
}
//---------------------------------------------------------------------------
// IQMModel::IJoint
//---------------------------------------------------------------------------
IQMModel::IJoint::IJoint()
{}
//---------------------------------------------------------------------------
IQMModel::IJoint::~IJoint()
{}
//---------------------------------------------------------------------------
bool IQMModel::IJoint::Read(Buffer& buffer)
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
        if (GetEndianness() == IQMModel::IEEndianness::IE_E_BigEndian)
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
// IQMModel::IPose
//---------------------------------------------------------------------------
IQMModel::IPose::IPose()
{}
//---------------------------------------------------------------------------
IQMModel::IPose::~IPose()
{}
//---------------------------------------------------------------------------
bool IQMModel::IPose::Read(Buffer& buffer)
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
        if (GetEndianness() == IQMModel::IEEndianness::IE_E_BigEndian)
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
// IQMModel::IAnim
//---------------------------------------------------------------------------
IQMModel::IAnim::IAnim()
{}
//---------------------------------------------------------------------------
IQMModel::IAnim::~IAnim()
{}
//---------------------------------------------------------------------------
bool IQMModel::IAnim::Read(Buffer& buffer)
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
        if (GetEndianness() == IQMModel::IEEndianness::IE_E_BigEndian)
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
// IQMModel::IBounds
//---------------------------------------------------------------------------
IQMModel::IBounds::IBounds()
{}
//---------------------------------------------------------------------------
IQMModel::IBounds::~IBounds()
{}
//---------------------------------------------------------------------------
bool IQMModel::IBounds::Read(Buffer& buffer)
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
        if (GetEndianness() == IQMModel::IEEndianness::IE_E_BigEndian)
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
/*
IComments()
~IComments()
bool Read(Buffer& buffer)
IExtension()
~IExtension()
bool Read(Buffer& buffer)
IExtensions()
~IExtensions()
bool Read(Buffer& buffer);
IVertex()
~IVertex()
bool Read(Buffer& buffer)
IVertices()
~IVertices()
bool Read(Buffer& buffer)
*/
//---------------------------------------------------------------------------
// IQMModel
//---------------------------------------------------------------------------
IQMModel::IQMModel(bool meshOnly, bool poseOnly) :
    m_MeshOnly(meshOnly),
    m_PoseOnly(poseOnly)
{
    // configure the default vertex format
    m_VertFormatTemplate.m_Format = (VertexFormat::IEFormat)((unsigned)VertexFormat::IEFormat::IE_VF_Colors |
                                                             (unsigned)VertexFormat::IEFormat::IE_VF_TexCoords);
    m_VertFormatTemplate.m_Type   =  VertexFormat::IEType::IE_VT_Triangles;

    // configure the default vertex culling
    m_VertCullingTemplate.m_Type = VertexCulling::IECullingType::IE_CT_Back;
    m_VertCullingTemplate.m_Face = VertexCulling::IECullingFace::IE_CF_CCW;

    // configure the default material
    m_MaterialTemplate.m_Color = ColorF(1.0f, 1.0f, 1.0f, 1.0f);
}
//---------------------------------------------------------------------------
IQMModel::~IQMModel()
{
    if (m_pModel)
        delete m_pModel;
}
//---------------------------------------------------------------------------
bool IQMModel::Open(const std::string& fileName)
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

    StdFileBuffer fileBuffer;

    // open the file
    if (!fileBuffer.Open(fileName, FileBuffer::IEMode::IE_M_Read))
        return false;

    // seek to start
    fileBuffer.Seek(0, 0);

    // read the content
    return Read(fileBuffer);
}
//---------------------------------------------------------------------------
bool IQMModel::Open(const std::wstring& fileName)
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

    StdFileBuffer fileBuffer;

    // open the file
    if (!fileBuffer.Open(fileName, FileBuffer::IEMode::IE_M_Read))
        return false;

    // seek to start
    fileBuffer.Seek(0, 0);

    // read the content
    return Read(fileBuffer);
}
//---------------------------------------------------------------------------
bool IQMModel::Open(Buffer& buffer)
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
Model* IQMModel::GetModel(int animSetIndex, int frameCount, int frameIndex) const
{
    /*FIXME
    if (!m_pModel)
        return nullptr;

    // only get the mesh and ignore all other data like bones?
    if (m_pModel->m_MeshOnly)
        return m_pModel;

    // if mesh has no skeleton, just get it
    if (!m_pModel->m_pSkeleton)
        return m_pModel;

    const std::size_t meshCount = m_pModel->m_Mesh.size();

    // iterate through the meshes to get
    for (std::size_t i = 0; i < meshCount; ++i)
    {
        // get the current model mesh to draw
        Mesh* pMesh = m_pModel->m_Mesh[i];

        // found it?
        if (!pMesh)
            continue;

        // normally each mesh should contain only one vertex buffer
        if (pMesh->m_VB.size() != 1)
            // unsupported if not (because cannot know which texture should be binded. If a such model
            // exists, a custom version of this function should also be written for it)
            continue;

        // mesh contains deformers?
        if (m_pModel->m_Deformers[i]->m_SkinWeights.size())
        {
            // clear the previous print vertices (needs to be cleared to properly apply the weights)
            for (std::size_t j = 0; j < pMesh->m_VB[0]->m_Data.size(); j += pMesh->m_VB[0]->m_Format.m_Stride)
            {
                pMesh->m_VB[0]->m_Data[j]     = 0.0f;
                pMesh->m_VB[0]->m_Data[j + 1] = 0.0f;
                pMesh->m_VB[0]->m_Data[j + 2] = 0.0f;
            }

            // iterate through mesh skin weights
            for (std::size_t j = 0; j < m_pModel->m_Deformers[i]->m_SkinWeights.size(); ++j)
            {
                Matrix4x4F boneMatrix;

                // get the bone matrix
                if (m_pModel->m_PoseOnly)
                    m_pModel->GetBoneMatrix(m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_pBone, Matrix4x4F::Identity(), boneMatrix);
                else
                    GetBoneAnimMatrix(m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_pBone,
                                      m_pModel->m_AnimationSet[animSetIndex],
                                      frameIndex,
                                      Matrix4x4F::Identity(),
                                      boneMatrix);

                // get the final matrix after bones transform
                const Matrix4x4F finalMatrix = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Matrix.Multiply(boneMatrix);

                // apply the bone and its skin weights to each vertices
                for (std::size_t k = 0; k < m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences.size(); ++k)
                    for (std::size_t l = 0; l < m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex.size(); ++l)
                    {
                        // get the next vertex to which the next skin weight should be applied
                        const std::size_t iX = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l];
                        const std::size_t iY = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l] + 1;
                        const std::size_t iZ = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l] + 2;

                        Vector3F inputVertex;

                        // get input vertex
                        inputVertex.m_X = m_SourceVB[i]->m_Data[iX];
                        inputVertex.m_Y = m_SourceVB[i]->m_Data[iY];
                        inputVertex.m_Z = m_SourceVB[i]->m_Data[iZ];

                        // apply bone transformation to vertex
                        const Vector3F outputVertex = finalMatrix.Transform(inputVertex);

                        // apply the skin weights and calculate the final output vertex
                        pMesh->m_VB[0]->m_Data[iX] += (outputVertex.m_X * m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Weights[k]);
                        pMesh->m_VB[0]->m_Data[iY] += (outputVertex.m_Y * m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Weights[k]);
                        pMesh->m_VB[0]->m_Data[iZ] += (outputVertex.m_Z * m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Weights[k]);
                    }
            }
        }
    }

    return m_pModel;
    */
    return nullptr;
}
//---------------------------------------------------------------------------
void IQMModel::GetBoneAnimMatrix(const Model::IBone* pBone,
                               const Model::IAnimationSet* pAnimSet,
                               std::size_t           frameIndex,
                               const Matrix4x4F& initialMatrix,
                               Matrix4x4F& matrix) const
{
    // no bone?
    if (!pBone)
        return;

    // set the output matrix as identity
    matrix = Matrix4x4F::Identity();

    Matrix4x4F animMatrix;

    // iterate through bones
    while (pBone)
    {
        // get the previously stacked matrix as base to calculate the new one
        const Matrix4x4F localMatrix = matrix;

        // get the animated bone matrix matching with frame. If not found use the identity one
        if (!GetAnimationMatrix(pAnimSet, pBone, frameIndex, animMatrix))
            animMatrix = Matrix4x4F::Identity();

        // stack the previously calculated matrix with the current bone one
        matrix = localMatrix.Multiply(animMatrix);

        // go to parent bone
        pBone = pBone->m_pParent;
    }

    // initial matrix provided?
    if (!initialMatrix.IsIdentity())
    {
        // get the previously stacked matrix as base to calculate the new one
        const Matrix4x4F localMatrix = matrix;

        // stack the previously calculated matrix with the initial one
        matrix = localMatrix.Multiply(initialMatrix);
    }
}
//---------------------------------------------------------------------------
void IQMModel::SetVertFormatTemplate(const VertexFormat& vertFormatTemplate)
{
    m_VertFormatTemplate = vertFormatTemplate;
}
//---------------------------------------------------------------------------
void IQMModel::SetVertCullingTemplate(const VertexCulling& vertCullingTemplate)
{
    m_VertCullingTemplate = vertCullingTemplate;
}
//---------------------------------------------------------------------------
void IQMModel::SetMaterial(const Material& materialTemplate)
{
    m_MaterialTemplate = materialTemplate;
}
//---------------------------------------------------------------------------
void IQMModel::Set_OnGetVertexColor(VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    m_fOnGetVertexColor = fOnGetVertexColor;
}
//---------------------------------------------------------------------------
void IQMModel::Set_OnLoadTexture(Texture::ITfOnLoadTexture fOnLoadTexture)
{
    m_fOnLoadTexture = fOnLoadTexture;
}
//---------------------------------------------------------------------------
bool IQMModel::Read(Buffer& buffer)
{
    IHeader header;

    if (!header.Read(buffer))
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
                std::unique_ptr<IText> pText(new IText());

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
                std::unique_ptr<IMesh> pMesh(new IMesh());

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
                std::unique_ptr<IVertexArray> pVertexArray(new IVertexArray());

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
                std::unique_ptr<ITriangle> pTriangle(new ITriangle());

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
                std::unique_ptr<IAdjacency> pAdjacency(new IAdjacency());

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
                std::unique_ptr<IJoint> pJoint(new IJoint());

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
                std::unique_ptr<IPose> pPose(new IPose());

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
                std::unique_ptr<IAnim> pAnim(new IAnim());

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
                std::unique_ptr<IBounds> pBounds(new IBounds());

                // read next bounds
                if (!pBounds->Read(buffer))
                    return false;

                boundsArray.push_back(pBounds.get());
                pBounds.release();
            }
        }
    }
    catch (...)
    {
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

        return false;
    }

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

    return true;
}
//---------------------------------------------------------------------------
bool IQMModel::GetAnimationMatrix(const Model::IAnimationSet* pAnimSet,
                                  const Model::IBone*         pBone,
                                        std::size_t           frame,
                                        Matrix4x4F&           matrix) const
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

        std::size_t rotFrame       = 0;
        std::size_t nextRotFrame   = 0;
        std::size_t posFrame       = 0;
        std::size_t nextPosFrame   = 0;
        std::size_t scaleFrame     = 0;
        std::size_t nextScaleFrame = 0;
        QuaternionF rotation;
        QuaternionF nextRotation;
        QuaternionF finalRotation;
        Vector3F    position;
        Vector3F    nextPosition;
        Vector3F    finalPosition;
        Vector3F    scaling;
        Vector3F    nextScaling;
        Vector3F    finalScaling;

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
                        matrix.m_Table[k / 4][k % 4] = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[k];

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

        Matrix4x4F translateMatrix = Matrix4x4F::Identity();
        Matrix4x4F rotateMatrix;
        Matrix4x4F scaleMatrix     = Matrix4x4F::Identity();

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
    IQMModel::IEEndianness IQMModel::GetEndianness()
    {
        const int i = 1;

        return (!*((char*)&i)) ?
                IQMModel::IEEndianness::IE_E_BigEndian :
                IQMModel::IEEndianness::IE_E_LittleEndian;
    }
#endif
//---------------------------------------------------------------------------
#ifdef CONVERT_ENDIANNESS
    void IQMModel::SwapMemory(void* pMemory, size_t size)
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
