/****************************************************************************
 * ==> QR_MD2Parser --------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a parser for the MD2 models                       *
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

#include "QR_MD2Parser.h"

// std
#include <memory>

#if defined (OS_WIN)
    // needed for some std::memset implementation, e.g. in CodeBlocks where
    // compiler will not found it otherwise
    #include <cstring>
#endif

// qr engine
#include "QR_Exception.h"
#include "QR_StdFileBuffer.h"

//---------------------------------------------------------------------------
// Global defines
//---------------------------------------------------------------------------
#define M_MD2_Mesh_File_Version 8
#define M_MD2_ID                (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I')
//---------------------------------------------------------------------------
// QR_MD2Parser::IHeader
//---------------------------------------------------------------------------
QR_MD2Parser::IHeader::IHeader() :
    m_ID(0),
    m_Version(0),
    m_SkinWidth(0),
    m_SkinHeight(0),
    m_FrameSize(0),
    m_SkinCount(0),
    m_VertexCount(0),
    m_TextureCoordCount(0),
    m_PolygonCount(0),
    m_GlCmdsCount(0),
    m_FrameCount(0),
    m_SkinOffset(0),
    m_TextureCoordOffset(0),
    m_PolygonOffset(0),
    m_FrameOffset(0),
    m_GlCmdsOffset(0),
    m_EndOffset(0)
{}
//---------------------------------------------------------------------------
QR_MD2Parser::IHeader::~IHeader()
{}
//---------------------------------------------------------------------------
void QR_MD2Parser::IHeader::Clear()
{
    m_ID                 = 0;
    m_Version            = 0;
    m_SkinWidth          = 0;
    m_SkinHeight         = 0;
    m_FrameSize          = 0;
    m_SkinCount          = 0;
    m_VertexCount        = 0;
    m_TextureCoordCount  = 0;
    m_PolygonCount       = 0;
    m_GlCmdsCount        = 0;
    m_FrameCount         = 0;
    m_SkinOffset         = 0;
    m_TextureCoordOffset = 0;
    m_PolygonOffset      = 0;
    m_FrameOffset        = 0;
    m_GlCmdsOffset       = 0;
    m_EndOffset          = 0;
}
//---------------------------------------------------------------------------
void QR_MD2Parser::IHeader::Copy(const IHeader& other)
{
    m_ID                 = other.m_ID;
    m_Version            = other.m_Version;
    m_SkinWidth          = other.m_SkinWidth;
    m_SkinHeight         = other.m_SkinHeight;
    m_FrameSize          = other.m_FrameSize;
    m_SkinCount          = other.m_SkinCount;
    m_VertexCount        = other.m_VertexCount;
    m_TextureCoordCount  = other.m_TextureCoordCount;
    m_PolygonCount       = other.m_PolygonCount;
    m_GlCmdsCount        = other.m_GlCmdsCount;
    m_FrameCount         = other.m_FrameCount;
    m_SkinOffset         = other.m_SkinOffset;
    m_TextureCoordOffset = other.m_TextureCoordOffset;
    m_PolygonOffset      = other.m_PolygonOffset;
    m_FrameOffset        = other.m_FrameOffset;
    m_GlCmdsOffset       = other.m_GlCmdsOffset;
    m_EndOffset          = other.m_EndOffset;
}
//---------------------------------------------------------------------------
void QR_MD2Parser::IHeader::Read(QR_Buffer& buffer)
{
    #ifdef _DEBUG
        #if defined(CP_EMBARCADERO)
            #pragma warn -8008
            #pragma warn -8066
        #endif

        // cross-platform security, check if QR_UInt32 is 4 bytes long
        M_ASSERT(sizeof(QR_UInt32) == 4);

        #if defined(CP_EMBARCADERO)
            #pragma warn .8066
            #pragma warn .8008
        #endif
    #endif

    // read header from file
    buffer.Read(&m_ID,                 sizeof(QR_UInt32));
    buffer.Read(&m_Version,            sizeof(QR_UInt32));
    buffer.Read(&m_SkinWidth,          sizeof(QR_UInt32));
    buffer.Read(&m_SkinHeight,         sizeof(QR_UInt32));
    buffer.Read(&m_FrameSize,          sizeof(QR_UInt32));
    buffer.Read(&m_SkinCount,          sizeof(QR_UInt32));
    buffer.Read(&m_VertexCount,        sizeof(QR_UInt32));
    buffer.Read(&m_TextureCoordCount,  sizeof(QR_UInt32));
    buffer.Read(&m_PolygonCount,       sizeof(QR_UInt32));
    buffer.Read(&m_GlCmdsCount,        sizeof(QR_UInt32));
    buffer.Read(&m_FrameCount,         sizeof(QR_UInt32));
    buffer.Read(&m_SkinOffset,         sizeof(QR_UInt32));
    buffer.Read(&m_TextureCoordOffset, sizeof(QR_UInt32));
    buffer.Read(&m_PolygonOffset,      sizeof(QR_UInt32));
    buffer.Read(&m_FrameOffset,        sizeof(QR_UInt32));
    buffer.Read(&m_GlCmdsOffset,       sizeof(QR_UInt32));
    buffer.Read(&m_EndOffset,          sizeof(QR_UInt32));
}
//---------------------------------------------------------------------------
// QR_MD2Parser::ISkin
//---------------------------------------------------------------------------
QR_MD2Parser::ISkin::ISkin()
{}
//---------------------------------------------------------------------------
QR_MD2Parser::ISkin::~ISkin()
{}
//---------------------------------------------------------------------------
void QR_MD2Parser::ISkin::Clear()
{
    m_Name.clear();
}
//---------------------------------------------------------------------------
void QR_MD2Parser::ISkin::Copy(const ISkin& other)
{
    m_Name = other.m_Name;
}
//---------------------------------------------------------------------------
void QR_MD2Parser::ISkin::Read(QR_Buffer& buffer)
{
    #ifdef _DEBUG
        #if defined(CP_EMBARCADERO)
            #pragma warn -8008
            #pragma warn -8066
        #endif

        // cross-platform security, check if QR_Int8 is 1 byte long
        M_ASSERT(sizeof(QR_Int8) == 1);

        #if defined(CP_EMBARCADERO)
            #pragma warn .8066
            #pragma warn .8008
        #endif
    #endif

    QR_Int8 name[64];

    // read skin name from file
    buffer.Read(&name, sizeof(name));

    // set skin name
    m_Name = name;
}
//---------------------------------------------------------------------------
// QR_MD2Parser::IVertex
//---------------------------------------------------------------------------
QR_MD2Parser::IVertex::IVertex() :
    m_NormalIndex(0)
{
    std::memset(&m_Vertex, 0, sizeof(m_Vertex));
}
//---------------------------------------------------------------------------
QR_MD2Parser::IVertex::~IVertex()
{}
//---------------------------------------------------------------------------
void QR_MD2Parser::IVertex::Clear()
{
    m_NormalIndex = 0;

    std::memset(&m_Vertex, 0, sizeof(m_Vertex));
}
//---------------------------------------------------------------------------
void QR_MD2Parser::IVertex::Copy(const IVertex& other)
{
    m_NormalIndex = other.m_NormalIndex;

    std::memcpy(&m_Vertex, &other.m_Vertex, sizeof(other.m_Vertex));
}
//---------------------------------------------------------------------------
void QR_MD2Parser::IVertex::Read(QR_Buffer& buffer)
{
    #ifdef _DEBUG
        #if defined(CP_EMBARCADERO)
            #pragma warn -8008
            #pragma warn -8066
        #endif

        // cross-platform security, check if QR_UInt8 is 1 byte long
        M_ASSERT(sizeof(QR_UInt8) == 1);

        #if defined(CP_EMBARCADERO)
            #pragma warn .8066
            #pragma warn .8008
        #endif
    #endif

    // read vertex from file
    buffer.Read(&m_Vertex,      sizeof(m_Vertex));
    buffer.Read(&m_NormalIndex, sizeof(QR_UInt8));
}
//---------------------------------------------------------------------------
// QR_MD2Parser::ITextureCoord
//---------------------------------------------------------------------------
QR_MD2Parser::ITextureCoord::ITextureCoord() :
    m_U(0),
    m_V(0)
{}
//---------------------------------------------------------------------------
QR_MD2Parser::ITextureCoord::~ITextureCoord()
{}
//---------------------------------------------------------------------------
void QR_MD2Parser::ITextureCoord::Clear()
{
    m_U = 0;
    m_V = 0;
}
//---------------------------------------------------------------------------
void QR_MD2Parser::ITextureCoord::Copy(const ITextureCoord& other)
{
    m_U = other.m_U;
    m_V = other.m_V;
}
//---------------------------------------------------------------------------
void QR_MD2Parser::ITextureCoord::Read(QR_Buffer& buffer)
{
    #ifdef _DEBUG
        #if defined(CP_EMBARCADERO)
            #pragma warn -8008
            #pragma warn -8066
        #endif

        // cross-platform security, check if QR_UInt16 is 2 bytes long
        M_ASSERT(sizeof(QR_UInt16) == 2);

        #if defined(CP_EMBARCADERO)
            #pragma warn .8066
            #pragma warn .8008
        #endif
    #endif

    // read texture coordinates from file
    buffer.Read(&m_U, sizeof(QR_UInt16));
    buffer.Read(&m_V, sizeof(QR_UInt16));
}
//---------------------------------------------------------------------------
// QR_MD2Parser::IFrame
//---------------------------------------------------------------------------
QR_MD2Parser::IFrame::IFrame() :
    m_pVertex(NULL)
{
    std::memset(&m_Scale,     0, sizeof(m_Scale));
    std::memset(&m_Translate, 0, sizeof(m_Translate));
}
//---------------------------------------------------------------------------
QR_MD2Parser::IFrame::~IFrame()
{
    // delete vertex
    if (m_pVertex)
        delete[] m_pVertex;
}
//---------------------------------------------------------------------------
void QR_MD2Parser::IFrame::Clear()
{
    m_Name.clear();

    std::memset(&m_Scale,     0, sizeof(m_Scale));
    std::memset(&m_Translate, 0, sizeof(m_Translate));

    // delete vertex
    if (m_pVertex)
    {
        delete[] m_pVertex;
        m_pVertex = NULL;
    }
}
//---------------------------------------------------------------------------
void QR_MD2Parser::IFrame::Copy(const IFrame& other, const IHeader& header)
{
    Clear();

    m_Name.clear();

    std::memcpy(&m_Scale,     &other.m_Scale,     sizeof(other.m_Scale));
    std::memcpy(&m_Translate, &other.m_Translate, sizeof(other.m_Translate));

    m_pVertex = new IVertex[header.m_VertexCount];

    for (QR_SizeT i = 0; i < header.m_VertexCount; ++i)
        m_pVertex[i].Copy(other.m_pVertex[i]);
}
//---------------------------------------------------------------------------
void QR_MD2Parser::IFrame::Read(QR_Buffer& buffer, const IHeader& header)
{
    #ifdef _DEBUG
        #if defined(CP_EMBARCADERO)
            #pragma warn -8008
            #pragma warn -8066
        #endif

        // cross-platform security, check if types sizes match with file
        M_ASSERT(sizeof(QR_Float) == 4);
        M_ASSERT(sizeof(QR_Int8)  == 1);

        #if defined(CP_EMBARCADERO)
            #pragma warn .8066
            #pragma warn .8008
        #endif
    #endif

    // previously vertex buffer exists?
    if (m_pVertex)
    {
        delete[] m_pVertex;
        m_pVertex = NULL;
    }

    // read vertex transformations
    buffer.Read(&m_Scale,     sizeof(m_Scale));
    buffer.Read(&m_Translate, sizeof(m_Translate));

    // read frame name
    QR_Int8 name[16];
    buffer.Read(&name, sizeof(name));
    m_Name = name;

    // create frame vertex buffer
    m_pVertex = new IVertex[header.m_VertexCount];

    // read frame vertices
    for (QR_UInt32 i = 0; i < header.m_VertexCount; ++i)
        m_pVertex[i].Read(buffer);
}
//---------------------------------------------------------------------------
// QR_MD2Parser::IPolygon
//---------------------------------------------------------------------------
QR_MD2Parser::IPolygon::IPolygon()
{
    std::memset(&m_VertexIndex,       0, sizeof(m_VertexIndex));
    std::memset(&m_TextureCoordIndex, 0, sizeof(m_TextureCoordIndex));
}
//---------------------------------------------------------------------------
QR_MD2Parser::IPolygon::~IPolygon()
{}
//---------------------------------------------------------------------------
void QR_MD2Parser::IPolygon::Clear()
{
    std::memset(&m_VertexIndex,       0, sizeof(m_VertexIndex));
    std::memset(&m_TextureCoordIndex, 0, sizeof(m_TextureCoordIndex));
}
//---------------------------------------------------------------------------
void QR_MD2Parser::IPolygon::Copy(const IPolygon& other)
{
    std::memcpy(&m_VertexIndex,       &other.m_VertexIndex,       sizeof(other.m_VertexIndex));
    std::memcpy(&m_TextureCoordIndex, &other.m_TextureCoordIndex, sizeof(other.m_TextureCoordIndex));
}
//---------------------------------------------------------------------------
void QR_MD2Parser::IPolygon::Read(QR_Buffer& buffer)
{
    #ifdef _DEBUG
        #if defined(CP_EMBARCADERO)
            #pragma warn -8008
            #pragma warn -8066
        #endif

        // cross-platform security, check if QR_UInt16 is 2 bytes long
        M_ASSERT(sizeof(QR_UInt16) == 2);

        #if defined(CP_EMBARCADERO)
            #pragma warn .8066
            #pragma warn .8008
        #endif
    #endif

    // read polygon from file
    buffer.Read(&m_VertexIndex,       sizeof(m_VertexIndex));
    buffer.Read(&m_TextureCoordIndex, sizeof(m_TextureCoordIndex));
}
//---------------------------------------------------------------------------
// QR_MD2Parser
//---------------------------------------------------------------------------
QR_MD2Parser::QR_MD2Parser() :
    QR_ModelParser(),
    m_pSkin(NULL),
    m_pTexCoord(NULL),
    m_pPolygon(NULL),
    m_pFrame(NULL),
    m_pGlCmds(NULL)
{}
//---------------------------------------------------------------------------
QR_MD2Parser::~QR_MD2Parser()
{
    DelAndClear();
}
//---------------------------------------------------------------------------
void QR_MD2Parser::Clear()
{
    QR_ModelParser::Clear();

    m_Header.Clear();

    DelAndClear();
}
//---------------------------------------------------------------------------
void QR_MD2Parser::Copy(const QR_ModelParser& other)
{
    Clear();

    QR_ModelParser::Copy(other);

    // try to get source model
    const QR_MD2Parser* pSource = dynamic_cast<const QR_MD2Parser*>(&other);

    // found it?
    if (!pSource)
    {
        Clear();
        return;
    }

    m_Header.Copy(pSource->m_Header);

    // copy skins
    if (pSource->m_Header.m_SkinCount)
    {
        m_pSkin = new ISkin[pSource->m_Header.m_SkinCount];

        for (QR_UInt32 i = 0; i < pSource->m_Header.m_SkinCount; ++i)
            m_pSkin[i].Copy(pSource->m_pSkin[i]);
    }

    // copy texture coordinates
    if (pSource->m_Header.m_TextureCoordCount)
    {
        m_pTexCoord = new ITextureCoord[pSource->m_Header.m_TextureCoordCount];

        for (QR_UInt32 i = 0; i < pSource->m_Header.m_TextureCoordCount; ++i)
            m_pTexCoord[i].Copy(pSource->m_pTexCoord[i]);
    }

    // copy polygons
    if (pSource->m_Header.m_PolygonCount)
    {
        m_pPolygon = new IPolygon[pSource->m_Header.m_PolygonCount];

        for (QR_UInt32 i = 0; i < pSource->m_Header.m_PolygonCount; ++i)
            m_pPolygon[i].Copy(pSource->m_pPolygon[i]);
    }

    // copy OpenGL commands
    if (pSource->m_Header.m_GlCmdsCount)
    {
        m_pGlCmds = new QR_Int32[pSource->m_Header.m_GlCmdsCount];
        std::memcpy(m_pGlCmds,
                    pSource->m_pGlCmds,
                    sizeof(QR_UInt32) * pSource->m_Header.m_GlCmdsCount);
    }

    // copy frames
    if (pSource->m_Header.m_FrameCount)
    {
        m_pFrame = new IFrame[pSource->m_Header.m_FrameCount];

        for (QR_UInt32 i = 0; i < pSource->m_Header.m_FrameCount; ++i)
            m_pFrame[i].Copy(pSource->m_pFrame[i], m_Header);
    }
}
//---------------------------------------------------------------------------
bool QR_MD2Parser::Load(const std::string& fileName)
{
    return QR_ModelParser::Load(fileName);
}
//---------------------------------------------------------------------------
bool QR_MD2Parser::Load(const std::wstring& fileName)
{
    return QR_ModelParser::Load(fileName);
}
//---------------------------------------------------------------------------
bool QR_MD2Parser::Load(QR_Buffer& buffer, const QR_Buffer::ISizeType& length)
{
    Clear();

    // generate an unique hash code based on the model buffer
    GenerateHashCode(buffer, length, 0x14F2DCA4);

    // is buffer empty?
    if (buffer.Empty())
        return false;

    // get current offset
    const QR_BufferSizeType offset = buffer.GetOffset();

    // read file header
    m_Header.Read(buffer);

    // is MD2 file and version correct?
    if ((m_Header.m_ID != M_MD2_ID) || ((QR_Float)m_Header.m_Version != M_MD2_Mesh_File_Version))
        return false;

    // do read skins?
    if (m_Header.m_SkinCount)
    {
        m_pSkin = new ISkin[m_Header.m_SkinCount];

        // go to skins offset
        buffer.Seek(offset, m_Header.m_SkinOffset);

        // read skins
        for (QR_UInt32 i = 0; i < m_Header.m_SkinCount; ++i)
            m_pSkin[i].Read(buffer);
    }

    // do read texture coordinates?
    if (m_Header.m_TextureCoordCount)
    {
        m_pTexCoord = new ITextureCoord[m_Header.m_TextureCoordCount];

        // go to texture coordinates offset
        buffer.Seek(offset, m_Header.m_TextureCoordOffset);

        // read texture coordinates
        for (QR_UInt32 i = 0; i < m_Header.m_TextureCoordCount; ++i)
            m_pTexCoord[i].Read(buffer);
    }

    // do read polygons?
    if (m_Header.m_PolygonCount)
    {
        m_pPolygon = new IPolygon[m_Header.m_PolygonCount];

        // go to polygons offset
        buffer.Seek(offset, m_Header.m_PolygonOffset);

        // read polygons
        for (QR_UInt32 i = 0; i < m_Header.m_PolygonCount; ++i)
            m_pPolygon[i].Read(buffer);
    }

    // do read OpenGL commands?
    if (m_Header.m_GlCmdsCount)
    {
        m_pGlCmds = new QR_Int32[m_Header.m_GlCmdsCount];

        // go to OpenGL commands offset
        buffer.Seek(offset, m_Header.m_GlCmdsOffset);

        // read OpenGL commands
        buffer.Read(m_pGlCmds, sizeof(QR_UInt32) * m_Header.m_GlCmdsCount);
    }

    // do read frames?
    if (m_Header.m_FrameCount)
    {
        m_pFrame = new IFrame[m_Header.m_FrameCount];

        // go to frames offset
        buffer.Seek(offset, m_Header.m_FrameOffset);

        // read frames
        for (QR_UInt32 i = 0; i < m_Header.m_FrameCount; ++i)
            m_pFrame[i].Read(buffer, m_Header);
    }

    return true;
}
//---------------------------------------------------------------------------
void QR_MD2Parser::DelAndClear()
{
    // delete skins
    if (m_pSkin)
    {
        delete[] m_pSkin;
        m_pSkin = NULL;
    }

    // delete texture coordinates
    if (m_pTexCoord)
    {
        delete[] m_pTexCoord;
        m_pTexCoord = NULL;
    }

    // delete polygons
    if (m_pPolygon)
    {
        delete[] m_pPolygon;
        m_pPolygon = NULL;
    }

    // delete OpenGL commands
    if (m_pGlCmds)
    {
        delete[] m_pGlCmds;
        m_pGlCmds = NULL;
    }

    // delete frames
    if (m_pFrame)
    {
        delete[] m_pFrame;
        m_pFrame = NULL;
    }
}
//---------------------------------------------------------------------------
