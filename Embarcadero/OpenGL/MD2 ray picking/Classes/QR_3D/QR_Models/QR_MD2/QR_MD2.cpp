/****************************************************************************
 * ==> QR_MD2 --------------------------------------------------------------*
 ****************************************************************************
 * Description : MD2 model                                                  *
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

#include "QR_MD2.h"

// std
#include <cstdio>
#include <cmath>

// qr engine
#include "QR_Exception.h"
#include "QR_StdFileBuffer.h"

//---------------------------------------------------------------------------
// Global defines
//---------------------------------------------------------------------------
#define M_MD2_Normals_Table_File_Version 1.0f
//---------------------------------------------------------------------------
// QR_MD2
//---------------------------------------------------------------------------
QR_MD2::QR_MD2() :
    QR_FramedModel()
{
    #ifdef _DEBUG
        // verify vertex buffer alignment
        if (!QR_Vertex::VerifyAlignment())
            M_THROW_EXCEPTION("Misaligned vertex buffer data type - must be 4 bytes aligned");
    #endif
}
//---------------------------------------------------------------------------
QR_MD2::~QR_MD2()
{}
//---------------------------------------------------------------------------
void QR_MD2::Clear()
{
    QR_FramedModel::Clear();

    m_Model.Clear();
    m_Normals.clear();
}
//---------------------------------------------------------------------------
void QR_MD2::Copy(const QR_Model& other)
{
    Clear();

    QR_FramedModel::Copy(other);

    // try to get source model
    const QR_MD2* pSource = dynamic_cast<const QR_MD2*>(&other);

    // found it?
    if (!pSource)
    {
        Clear();
        return;
    }

    // copy the model
    m_Model.Copy(pSource->m_Model);

    // get normal count
    const QR_SizeT normalCount = pSource->m_Normals.size();

    // resize the normal table
    m_Normals.resize(normalCount);

    // copy the normals
    for (QR_SizeT i = 0; i < normalCount; ++i)
        m_Normals[i] = pSource->m_Normals[i];
}
//---------------------------------------------------------------------------
bool QR_MD2::Load(const std::string& fileName)
{
    return m_Model.Load(fileName);
}
//---------------------------------------------------------------------------
bool QR_MD2::Load(const std::wstring& fileName)
{
    return m_Model.Load(fileName);
}
//---------------------------------------------------------------------------
bool QR_MD2::Load(QR_Buffer& buffer, const QR_Buffer::ISizeType& length)
{
    return m_Model.Load(buffer, length);
}
//---------------------------------------------------------------------------
bool QR_MD2::LoadNormals(const std::string& fileName)
{
    // create a file buffer and open it for read
    QR_StdFileBuffer buffer;
    buffer.Open(fileName, QR_FileBuffer::IE_M_Read);
    buffer.Seek(0, 0);

    // read MD2 normals content
    return LoadNormals(buffer, buffer.GetSize());
}
//---------------------------------------------------------------------------
bool QR_MD2::LoadNormals(const std::wstring& fileName)
{
    // create a file buffer and open it for read
    QR_StdFileBuffer buffer;
    buffer.Open(fileName, QR_FileBuffer::IE_M_Read);
    buffer.Seek(0, 0);

    // read MD2 normals content
    return LoadNormals(buffer, buffer.GetSize());
}
//---------------------------------------------------------------------------
bool QR_MD2::LoadNormals(QR_Buffer& buffer, const QR_Buffer::ISizeType& length)
{
    // clear previous table, if needed
    m_Normals.clear();

    // is buffer empty?
    if (buffer.Empty())
        return false;

    QR_Float fileVersion;

    // read version
    buffer.Read(&fileVersion, sizeof(QR_Float));

    // is version correct?
    if (fileVersion != M_MD2_Normals_Table_File_Version)
        return false;

    QR_UInt32 dataLength;

    // read file length
    buffer.Read(&dataLength, sizeof(QR_UInt32));

    // reserve memory for normals table
    m_Normals.reserve(dataLength);

    // iterate through normals
    for (QR_UInt32 i = 0; i < dataLength; ++i)
    {
        QR_Vector3DP normal;

        // read normal from file
        buffer.Read(&normal, sizeof(QR_Vector3DP));

        // set normal in table
        m_Normals.push_back(normal);
    }

    return true;
}
//---------------------------------------------------------------------------
const QR_MD2Parser* QR_MD2::GetParser() const
{
    return &m_Model;
}
//---------------------------------------------------------------------------
bool QR_MD2::GetMesh(QR_SizeT index, QR_Mesh& mesh, QR_AABBTree* pAABBTree) const
{
    // is frame index out of bounds?
    if (index >= m_Model.m_Header.m_FrameCount)
        return false;

    // do use normals and pre-calculated normals table wasn't populated?
    if ((m_VertexFormat & QR_Vertex::IE_VF_Normals) && !m_Normals.size())
        return false;

    // get source frame from which mesh should be extracted
    QR_MD2Parser::IFrame* pSrcFrame = &m_Model.m_pFrame[index];

    // found it?
    if (!pSrcFrame)
        return false;

    // basically stride is the coordinates values size
    QR_SizeT stride = 3;

    // do include normals?
    if (m_VertexFormat & QR_Vertex::IE_VF_Normals)
        stride += 3;

    // do include texture coordinates?
    if (m_VertexFormat & QR_Vertex::IE_VF_TexCoords)
        stride += 2;

    // do include colors?
    if (m_VertexFormat & QR_Vertex::IE_VF_Colors)
        stride += 4;

    QR_Int32* pGLCmds = m_Model.m_pGlCmds;

    // iterate through OpenGL commands (negative value is for triangle fan,
    // positive value is for triangle strip, 0 means list end)
    while (QR_Int32 i = *pGLCmds)
    {
        // the first command is the number of vertexes to process, read above,
        // so skip it
        ++pGLCmds;

        // create and populate new vertex for the current command
        std::auto_ptr<QR_Vertex> pVertex(new QR_Vertex());
        pVertex->m_Format = m_VertexFormat;
        pVertex->m_Stride = stride;

        // search for OpenGL command type
        if (i < 0)
        {
            pVertex->m_Type = QR_Vertex::IE_VT_TriangleFan;
            i = -i;
        }
        else
            pVertex->m_Type = QR_Vertex::IE_VT_TriangleStrip;

        // iterate through OpenGL commands to process
        for (; i > 0; --i, pGLCmds += 3)
        {
            // get source vertex
            QR_MD2Parser::IVertex* pSrcVertex = &pSrcFrame->m_pVertex[pGLCmds[2]];

            // found it?
            if (!pSrcVertex)
                return false;

            // uncompress vertex
            QR_Vector3DP vertex = UncompressVertex(pSrcFrame, pSrcVertex);

            // do convert right hand <-> left hand coordinate system?
            if (m_DoConvertRHLH)
                // apply conversion
                vertex.m_X = -vertex.m_X;

            // populate vertex buffer
            pVertex->m_Buffer.push_back(vertex.m_X);
            pVertex->m_Buffer.push_back(vertex.m_Y);
            pVertex->m_Buffer.push_back(vertex.m_Z);

            QR_Vector3DP normal;

            // do include normals?
            if (m_VertexFormat & QR_Vertex::IE_VF_Normals)
            {
                // get vertex normal
                normal = m_Normals[pSrcVertex->m_NormalIndex];

                // do convert right hand <-> left hand coordinate system?
                if (m_DoConvertRHLH)
                    // apply conversion
                    normal.m_X = -normal.m_X;

                pVertex->m_Buffer.push_back(normal.m_X);
                pVertex->m_Buffer.push_back(normal.m_Y);
                pVertex->m_Buffer.push_back(normal.m_Z);
            }

            // do include texture coordinates?
            if (m_VertexFormat & QR_Vertex::IE_VF_TexCoords)
            {
                // get vertex texture coordinates. Be careful, here a pointer of
                // type float should be read from memory, for that the conversion
                // cannot be done from M_Precision
                M_Precision tu = ((float*)pGLCmds)[0];
                M_Precision tv = ((float*)pGLCmds)[1];

                pVertex->m_Buffer.push_back(tu);
                pVertex->m_Buffer.push_back(tv);
            }

            // do include colors?
            if (m_VertexFormat & QR_Vertex::IE_VF_Colors)
            {
                QR_Color color;

                // get final color to apply to vertex
                if ((m_VertexFormat & QR_Vertex::IE_VF_Normals) && m_Light.m_Enabled)
                    // calculate light color
                    color = CalculateLight(normal, &m_Light);
                else
                    // use default color
                    color = m_Color;

                pVertex->m_Buffer.push_back(color.GetRedF());
                pVertex->m_Buffer.push_back(color.GetGreenF());
                pVertex->m_Buffer.push_back(color.GetBlueF());
                pVertex->m_Buffer.push_back(color.GetAlphaF());
            }
        }

        // add mesh to output
        mesh.push_back(pVertex.release());
    }

    // populate aligned-axis bounding box tree
    return PopulateAABBTree(mesh, pAABBTree);
}
//---------------------------------------------------------------------------
QR_SizeT QR_MD2::GetMeshCount() const
{
    return m_Model.m_Header.m_FrameCount;
}
//---------------------------------------------------------------------------
QR_Vector3DP QR_MD2::UncompressVertex(const QR_MD2Parser::IFrame*  pFrame,
                                      const QR_MD2Parser::IVertex* pVertex) const
{
    M_ASSERT(pFrame);
    M_ASSERT(pVertex);

    M_Precision vertex[3];

    // iterate through vertex coordinates
    for (QR_UInt8 i = 0; i < 3; ++i)
        // uncompress vertex using frame scale and translate values
        vertex[i] = (pFrame->m_Scale[i] * pVertex->m_Vertex[i]) + pFrame->m_Translate[i];

    return QR_Vector3DP(vertex[0], vertex[1], vertex[2]);
}
//---------------------------------------------------------------------------
