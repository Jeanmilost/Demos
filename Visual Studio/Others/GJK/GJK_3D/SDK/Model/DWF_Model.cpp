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

#include "DWF_Model.h"

 // std
#include <memory>

// classes
#include "DWF_Quaternion.h"

using namespace DWF_Model;

//---------------------------------------------------------------------------
// VertexFormat
//---------------------------------------------------------------------------
VertexFormat::VertexFormat() :
    m_Stride(0),
    m_Type(IEType::IE_VT_Unknown),
    m_Format(IEFormat::IE_VF_None)
{}
//---------------------------------------------------------------------------
VertexFormat::~VertexFormat()
{}
//---------------------------------------------------------------------------
void VertexFormat::CalculateStride()
{
    // by default, at least x, y and z values
    m_Stride = 3;

    // do include normals?
    if ((unsigned)m_Format & (unsigned)IEFormat::IE_VF_Normals)
        m_Stride += 3;

    // do include texture coordinates?
    if ((unsigned)m_Format & (unsigned)IEFormat::IE_VF_TexCoords)
        m_Stride += 2;

    // do include vertex color?
    if ((unsigned)m_Format & (unsigned)IEFormat::IE_VF_Colors)
        m_Stride += 4;
}
//---------------------------------------------------------------------------
bool VertexFormat::CompareFormat(const VertexFormat& other) const
{
    return (m_Stride == other.m_Stride &&
            m_Type   == other.m_Type   &&
            m_Format == other.m_Format);
}
//---------------------------------------------------------------------------
// VertexBuffer
//---------------------------------------------------------------------------
VertexCulling::VertexCulling() :
    m_Type(IECullingType::IE_CT_None),
    m_Face(IECullingFace::IE_CF_CW)
{}
//---------------------------------------------------------------------------
VertexCulling::~VertexCulling()
{}
//---------------------------------------------------------------------------
// VertexBuffer
//---------------------------------------------------------------------------
VertexBuffer::VertexBuffer()
{}
//---------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{}
//---------------------------------------------------------------------------
VertexBuffer* VertexBuffer::Clone(bool includeData) const
{
    // clone vertex
    std::unique_ptr<VertexBuffer> pClone(new VertexBuffer());
    pClone->m_Name = m_Name;

    // copy the format
    pClone->m_Format.m_Stride = m_Format.m_Stride;
    pClone->m_Format.m_Type   = m_Format.m_Type;
    pClone->m_Format.m_Format = m_Format.m_Format;

    // copy the culling
    pClone->m_Culling.m_Type = m_Culling.m_Type;
    pClone->m_Culling.m_Face = m_Culling.m_Face;

    // copy the material
    pClone->m_Material.m_Color       = m_Material.m_Color;
    pClone->m_Material.m_Transparent = m_Material.m_Transparent;
    pClone->m_Material.m_Wireframe   = m_Material.m_Wireframe;

    // do clone the vertex buffer data?
    if (includeData)
    {
        const std::size_t dataCount = m_Data.size();

        // resize the destination
        pClone->m_Data.resize(dataCount);

        // copy the data
        for (std::size_t i = 0; i < dataCount; ++i)
            pClone->m_Data[i] = m_Data[i];
    }

    return pClone.release();
}
//---------------------------------------------------------------------------
bool VertexBuffer::Add(const DWF_Math::Vector3F* pVertex,
                       const DWF_Math::Vector3F* pNormal,
                       const DWF_Math::Vector2F* pUV,
                             std::size_t         groupIndex,
                       const ITfOnGetVertexColor fOnGetVertexColor)
{
    // the stride should be already calculated
    if (!m_Format.m_Stride)
    {
        m_Format.CalculateStride();

        // still empty?
        if (!m_Format.m_Stride)
            return false;
    }

    // keep the current offset
    std::size_t offset = m_Data.size();

    // allocate memory for new vertex
    m_Data.resize(m_Data.size() + m_Format.m_Stride);

    // source vertex exists?
    if (!pVertex)
    {
        // cannot add a nonexistent vertex, fill with empty data in this case
        m_Data[offset]     = 0.0f;
        m_Data[offset + 1] = 0.0f;
        m_Data[offset + 2] = 0.0f;
    }
    else
    {
        // copy vertex from source
        m_Data[offset]     = pVertex->m_X;
        m_Data[offset + 1] = pVertex->m_Y;
        m_Data[offset + 2] = pVertex->m_Z;
    }

    offset += 3;

    // vertex has a normal?
    if ((unsigned)m_Format.m_Format & (unsigned)VertexFormat::IEFormat::IE_VF_Normals)
    {
        // source normal exists?
        if (!pNormal)
        {
            // cannot add a nonexistent normal, fill with empty data in this case
            m_Data[offset]     = 0.0f;
            m_Data[offset + 1] = 0.0f;
            m_Data[offset + 2] = 0.0f;
        }
        else
        {
            // copy normal from source
            m_Data[offset]     = pNormal->m_X;
            m_Data[offset + 1] = pNormal->m_Y;
            m_Data[offset + 2] = pNormal->m_Z;
        }

        offset += 3;
    }

    // vertex has UV texture coordinates?
    if ((unsigned)m_Format.m_Format & (unsigned)VertexFormat::IEFormat::IE_VF_TexCoords)
    {
        // source texture coordinates exists?
        if (!pUV)
        {
            // cannot add nonexistent texture coordinates, fill with empty data in this case
            m_Data[offset]     = 0.0f;
            m_Data[offset + 1] = 0.0f;
        }
        else
        {
            // copy texture coordinates from source
            m_Data[offset]     = pUV->m_X;
            m_Data[offset + 1] = pUV->m_Y;
        }

        offset += 2;
    }

    // vertex has color?
    if ((unsigned)m_Format.m_Format & (unsigned)VertexFormat::IEFormat::IE_VF_Colors)
    {
        ColorF color;

        // get the vertex color
        if (fOnGetVertexColor)
            color = fOnGetVertexColor(this, pNormal, groupIndex);
        else
            color = m_Material.m_Color;

        // set color data
        m_Data[offset]     = color.m_R;
        m_Data[offset + 1] = color.m_G;
        m_Data[offset + 2] = color.m_B;
        m_Data[offset + 3] = color.m_A;
    }

    return true;
}
//---------------------------------------------------------------------------
// Mesh
//---------------------------------------------------------------------------
Mesh::Mesh()
{}
//---------------------------------------------------------------------------
Mesh::~Mesh()
{
    const std::size_t count = m_VB.size();

    // clear vertex buffers
    for (std::size_t i = 0; i < count; ++i)
        delete m_VB[i];
}
//---------------------------------------------------------------------------
// Model::IBone
//---------------------------------------------------------------------------
Model::IBone::IBone() :
    m_Matrix(DWF_Math::Matrix4x4F::Identity()),
    m_pParent(nullptr),
    m_pCustom(nullptr)
{}
//---------------------------------------------------------------------------
Model::IBone::~IBone()
{
    const std::size_t count = m_Children.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Children[i];
}
//---------------------------------------------------------------------------
// Model::IWeightInfluence
//---------------------------------------------------------------------------
Model::IWeightInfluence::IWeightInfluence()
{}
//---------------------------------------------------------------------------
Model::IWeightInfluence::~IWeightInfluence()
{}
//---------------------------------------------------------------------------
// Model::ISkinWeights
//---------------------------------------------------------------------------
Model::ISkinWeights::ISkinWeights() :
    m_pBone(nullptr),
    m_Matrix(DWF_Math::Matrix4x4F::Identity())
{}
//---------------------------------------------------------------------------
Model::ISkinWeights::~ISkinWeights()
{
    const std::size_t count = m_WeightInfluences.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_WeightInfluences[i];
}
//---------------------------------------------------------------------------
// Model::IDeformers
//---------------------------------------------------------------------------
Model::IDeformers::IDeformers()
{}
//---------------------------------------------------------------------------
Model::IDeformers::~IDeformers()
{
    const std::size_t count = m_SkinWeights.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_SkinWeights[i];
}
//---------------------------------------------------------------------------
// Model::IAnimationKey
//---------------------------------------------------------------------------
Model::IAnimationKey::IAnimationKey() :
    m_Frame(0),
    m_TimeStamp(0L)
{}
//---------------------------------------------------------------------------
Model::IAnimationKey::~IAnimationKey()
{}
//---------------------------------------------------------------------------
// Model::IAnimationKeys
//---------------------------------------------------------------------------
Model::IAnimationKeys::IAnimationKeys() :
    m_Type(Model::IEAnimKeyType::IE_KT_Unknown),
    m_ColOverRow(false)
{}
//---------------------------------------------------------------------------
Model::IAnimationKeys::~IAnimationKeys()
{
    const std::size_t count = m_Keys.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Keys[i];
}
//---------------------------------------------------------------------------
// Model::IAnimation
//---------------------------------------------------------------------------
Model::IAnimation::IAnimation() :
    m_pBone(nullptr)
{}
//---------------------------------------------------------------------------
Model::IAnimation::~IAnimation()
{
    const std::size_t count = m_Keys.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Keys[i];
}
//---------------------------------------------------------------------------
// Model::IAnimationSet
//---------------------------------------------------------------------------
Model::IAnimationSet::IAnimationSet() :
    m_MaxValue(0L)
{}
//---------------------------------------------------------------------------
Model::IAnimationSet::~IAnimationSet()
{
    const std::size_t count = m_Animations.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Animations[i];
}
//---------------------------------------------------------------------------
// Model
//---------------------------------------------------------------------------
Model::Model() :
    m_pSkeleton(nullptr),
    m_MeshOnly(false),
    m_PoseOnly(false)
{}
//---------------------------------------------------------------------------
Model::~Model()
{
    const std::size_t meshCount = m_Mesh.size();

    for (std::size_t i = 0; i < meshCount; ++i)
        delete m_Mesh[i];
}
//---------------------------------------------------------------------------
Model::IBone* Model::FindBone(IBone* pBone, const std::string& name) const
{
    // no bone?
    if (!pBone)
        return nullptr;

    // found the bone?
    if (!pBone->m_Name.empty() && pBone->m_Name == name)
        return pBone;

    // iterate through the bone children
    for (std::size_t i = 0; i < pBone->m_Children.size(); ++i)
    {
        // search in next children bone
        IBone* pChildBone = FindBone(pBone->m_Children[i], name);

        // found the bone?
        if (pChildBone)
            return pChildBone;
    }

    return nullptr;
}
//---------------------------------------------------------------------------
void Model::GetBoneMatrix(const IBone* pBone, const DWF_Math::Matrix4x4F& initialMatrix, DWF_Math::Matrix4x4F& matrix) const
{
    // no bone?
    if (!pBone)
        return;

    // set the output matrix as identity
    matrix = DWF_Math::Matrix4x4F::Identity();

    // iterate through bones
    while (pBone)
    {
        // get the previously stacked matrix as base to calculate the new one
        const DWF_Math::Matrix4x4F localMatrix = matrix;

        // stack the previously calculated matrix with the current bone one
        matrix = localMatrix.Multiply(pBone->m_Matrix);

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
