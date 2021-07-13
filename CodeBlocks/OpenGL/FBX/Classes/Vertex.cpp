/****************************************************************************
 * ==> Vertex --------------------------------------------------------------*
 ****************************************************************************
 * Description : Vertex descriptor                                          *
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

#include "Vertex.h"

 // std
#include <memory>


//---------------------------------------------------------------------------
// Material
//---------------------------------------------------------------------------
Material::Material() :
    m_pTexture(nullptr),
    m_Color(ColorF(1.0f, 1.0f, 1.0f, 1.0f)),
    m_Transparent(false),
    m_Wireframe(false)
{}
//---------------------------------------------------------------------------
Material::~Material()
{
    if (m_pTexture)
        delete m_pTexture;
}
//---------------------------------------------------------------------------
// VertexFormat
//---------------------------------------------------------------------------
VertexFormat::VertexFormat() :
    m_Stride(0),
    m_Type(IE_VT_Unknown),
    m_Format(IE_VF_None)
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
    if ((unsigned)m_Format & (unsigned)IE_VF_Normals)
        m_Stride += 3;

    // do include texture coordinates?
    if ((unsigned)m_Format & (unsigned)IE_VF_TexCoords)
        m_Stride += 2;

    // do include vertex color?
    if ((unsigned)m_Format & (unsigned)IE_VF_Colors)
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
    m_Type(IE_CT_None),
    m_Face(IE_CF_CW)
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
bool VertexBuffer::Add(const Vector3F*           pVertex,
                       const Vector3F*           pNormal,
                       const Vector2F*           pUV,
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
    if ((unsigned)m_Format.m_Format & (unsigned)VertexFormat::IE_VF_Normals)
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
    if ((unsigned)m_Format.m_Format & (unsigned)VertexFormat::IE_VF_TexCoords)
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
    if ((unsigned)m_Format.m_Format & (unsigned)VertexFormat::IE_VF_Colors)
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
