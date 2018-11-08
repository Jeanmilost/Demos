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

#ifndef QR_MD2ParserH
#define QR_MD2ParserH

// std
#include <cstdio>
#include <string>

// qr engine
#include "QR_Types.h"
#include "QR_ModelParser.h"
#include "QR_Buffer.h"

/**
* Reads and exposes MD2 file content
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_MD2Parser : public QR_ModelParser
{
    public:
        /**
        * MD2 header
        */
        struct IHeader
        {
            QR_UInt32 m_ID;
            QR_UInt32 m_Version;
            QR_UInt32 m_SkinWidth;
            QR_UInt32 m_SkinHeight;
            QR_UInt32 m_FrameSize;
            QR_UInt32 m_SkinCount;
            QR_UInt32 m_VertexCount;
            QR_UInt32 m_TextureCoordCount;
            QR_UInt32 m_PolygonCount;
            QR_UInt32 m_GlCmdsCount;
            QR_UInt32 m_FrameCount;
            QR_UInt32 m_SkinOffset;
            QR_UInt32 m_TextureCoordOffset;
            QR_UInt32 m_PolygonOffset;
            QR_UInt32 m_FrameOffset;
            QR_UInt32 m_GlCmdsOffset;
            QR_UInt32 m_EndOffset;

            IHeader();
            ~IHeader();

            /**
            * Clears the header content
            */
            void Clear();

            /**
            * Copies content from another header
            *@param other - other header to copy from
            */
            void Copy(const IHeader& other);

            /**
            * Reads data from file
            *@param buffer - buffer to read from
            */
            void Read(QR_Buffer& buffer);
        };

        /**
        * MD2 skin
        */
        struct ISkin
        {
            std::string m_Name;

            ISkin();
            ~ISkin();

            /**
            * Clears the skin content
            */
            void Clear();

            /**
            * Copies content from another skin
            *@param other - other skin to copy from
            */
            void Copy(const ISkin& other);

            /**
            * Reads data from file
            *@param buffer - buffer to read from
            */
            void Read(QR_Buffer& buffer);
        };

        /**
        * MD2 vertex
        */
        struct IVertex
        {
            QR_UInt8 m_Vertex[3];
            QR_UInt8 m_NormalIndex;

            IVertex();
            ~IVertex();

            /**
            * Clears the vertex content
            */
            void Clear();

            /**
            * Copies content from another vertex
            *@param other - other vertex to copy from
            */
            void Copy(const IVertex& other);

            /**
            * Reads data from file
            *@param buffer - buffer to read from
            */
            void Read(QR_Buffer& buffer);
        };

        /**
        * MD2 texture coordinate
        */
        struct ITextureCoord
        {
            QR_UInt16 m_U;
            QR_UInt16 m_V;

            ITextureCoord();
            ~ITextureCoord();

            /**
            * Clears the texture coordinate content
            */
            void Clear();

            /**
            * Copies content from another texture coordinate
            *@param other - other texture coordinate to copy from
            */
            void Copy(const ITextureCoord& other);

            /**
            * Reads data from file
            *@param buffer - buffer to read from
            */
            void Read(QR_Buffer& buffer);
        };

        /**
        * MD2 frame
        */
        struct IFrame
        {
            std::string m_Name;
            QR_Float    m_Scale[3];
            QR_Float    m_Translate[3];
            IVertex*    m_pVertex;

            IFrame();
            ~IFrame();

            /**
            * Clears the frame content
            */
            void Clear();

            /**
            * Copies content from another frame
            *@param other - other frame to copy from
            *@param header - MD2 file header
            */
            void Copy(const IFrame& other, const IHeader& header);

            /**
            * Reads data from file
            *@param buffer - buffer to read from
            *@param header - MD2 file header
            */
            void Read(QR_Buffer& buffer, const IHeader& header);
        };

        /**
        * MD2 polygon
        */
        struct IPolygon
        {
            QR_UInt16 m_VertexIndex[3];
            QR_UInt16 m_TextureCoordIndex[3];

            IPolygon();
            ~IPolygon();

            /**
            * Clears the polygon content
            */
            void Clear();

            /**
            * Copies content from another polygon
            *@param other - other polygon to copy from
            */
            void Copy(const IPolygon& other);

            /**
            * Reads data from file
            *@param buffer - buffer to read from
            */
            void Read(QR_Buffer& buffer);
        };

        IHeader        m_Header;
        ISkin*         m_pSkin;
        ITextureCoord* m_pTexCoord;
        IPolygon*      m_pPolygon;
        IFrame*        m_pFrame;
        QR_Int32*      m_pGlCmds;

        QR_MD2Parser();
        virtual ~QR_MD2Parser();

        /**
        * Clears model parser
        */
        virtual void Clear();

        /**
        * Copies content from another model parser
        *@param other - other model parser to copy from
        */
        virtual void Copy(const QR_ModelParser& other);

        /**
        * Loads MD2 from file
        *@param fileName - file name
        *@return true on success, otherwise false
        */
        virtual bool Load(const std::string&  fileName);
        virtual bool Load(const std::wstring& fileName);

        /**
        * Loads MD2 from buffer
        *@param buffer - buffer
        *@param length - length to read in buffer, in bytes (not used here, can be 0)
        *@return true on success, otherwise false
        *@note Read will begin from current offset
        */
        virtual bool Load(      QR_Buffer&            buffer,
                          const QR_Buffer::ISizeType& length);

    private:
        /**
        * Deletes and clears all resources
        */
        void DelAndClear();
};

#endif
