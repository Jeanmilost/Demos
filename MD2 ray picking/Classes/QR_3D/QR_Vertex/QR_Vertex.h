/****************************************************************************
 * ==> QR_Vertex -----------------------------------------------------------*
 ****************************************************************************
 * Description : Provides the classes required to manage the vertices and   *
 *               meshes                                                     *
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

#ifndef QR_VertexH
#define QR_VertexH

// qr engine
#include "QR_Types.h"

/**
* Vertex descriptor, contains global enumeration and types
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Vertex
{
    public:
        /**
        * Vertex buffer type enumeration
        */
        enum IEType
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
        enum IEFormat
        {
            IE_VF_None      = 0x00,
            IE_VF_Normals   = 0x01,
            IE_VF_TexCoords = 0x02,
            IE_VF_Colors    = 0x04,
        };

        /**
        * Vertex coordinate type
        */
        enum IECoordType
        {
            IE_VC_Unknown = 0,
            IE_VC_XY,
            IE_VC_XYZ,
        };

        typedef std::vector<M_Precision> IBuffer;

        std::wstring m_Name;
        QR_SizeT     m_Stride;    // vertex stride (i.e. length between each vertex) in bytes
        IEType       m_Type;      // vertex type (i.e. how vertex is organized: triangle list, triangle fan, ...)
        IEFormat     m_Format;    // vertex format (i.e. what data vertex contains: position, normal, texture, ...)
        IECoordType  m_CoordType; // vertex coordinate type (i.e. 2D coordinates, 3D coordinates, ...)
        IBuffer      m_Buffer;    // vertex buffer

        QR_Vertex();
        virtual ~QR_Vertex();

        /**
        * Clones vertex in such manner that vertex info are copied, but not vertex buffer
        *@return cloned vertex
        *@note Cloned vertex should be deleted when useless
        */
        virtual QR_Vertex* Clone() const;

        /**
        * Calculates vertex stride
        *@return vertex stride
        */
        virtual QR_SizeT CalculateStride() const;

        /**
        * Compares vertex and determine if their format are equivalent
        *@param other - other vertex to compare with
        *@return true if both vertex header are equivalent, otherwise false
        */
        virtual bool CompareFormat(const QR_Vertex& other) const;

        /**
        * Verifies buffer alignment
        *@return true if buffer is correctly aligned, otherwise false
        */
        static bool VerifyAlignment();
};

/**
* Mesh, it's a set of vertex buffers representing a model
*@author Jean-Milost Reymond
*/
typedef std::vector<QR_Vertex*> QR_Mesh;

#endif
