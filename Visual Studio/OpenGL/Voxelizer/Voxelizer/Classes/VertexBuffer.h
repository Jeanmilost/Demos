/****************************************************************************
 * ==> VertexBuffer --------------------------------------------------------*
 ****************************************************************************
 * Description: Vertex buffer and its associated types                      *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License                                                              *
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

 // std
#include <vector>
#include <string>

// classes
#include "Vector2.h"
#include "Vector3.h"
#include "Material.h"

/**
* Vertex buffer and its associated types
*@author Jean-Milost Reymond
*/
class VertexBuffer
{
    public:
        /**
        * Vertex format
        */
        class IFormat
        {
            public:
                /**
                * Vertex buffer type enumeration
                */
                enum class IEType
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
                enum class IEFormat
                {
                    IE_VF_None      = 0x00,
                    IE_VF_Normals   = 0x01, // each vertex contains a normal
                    IE_VF_TexCoords = 0x02, // each vertex contains an UV texture coordinate
                    IE_VF_Colors    = 0x04  // each vertex contains its own color
                };

                std::size_t m_Stride = 0;                     // vertex stride (i.e. length between each vertex) in bytes
                IEType      m_Type   = IEType::IE_VT_Unknown; // vertex type (i.e. how vertex is organized: triangle list, triangle fan, ...)
                IEFormat    m_Format = IEFormat::IE_VF_None;  // vertex format (i.e. what data vertex contains: position, normal, texture, ...)

                IFormat();
                virtual ~IFormat();

                /**
                * Copy operator
                *@param other - other format to copy from
                *@return this format
                */
                virtual IFormat& operator = (const IFormat& other);

                /**
                * Equality operator
                *@param other - other format to compare with
                *@return true if formats are identical, otherwise false
                */
                virtual bool operator == (const IFormat& other)  const;

                /**
                * Not equality operator
                *@param other - other format to compare with
                *@return true if values are not identical, otherwise false
                */
                virtual bool operator != (const IFormat& other)  const;

                /**
                * Calculates the vertex stride
                */
                virtual void CalculateStride();

                /**
                * Compares vertex formats
                *@param other - other format to compare with
                *@return true if both formats are equivalent, otherwise false
                */
                virtual bool Compare(const IFormat& other) const;
        };

        /**
        * Vertex culling
        */
        class ICulling
        {
            public:
                /**
                * Culling type
                */
                enum class IECullingType
                {
                    IE_CT_None,
                    IE_CT_Front,
                    IE_CT_Back,
                    IE_CT_Both
                };

                /**
                * Culling face
                */
                enum class IECullingFace
                {
                    IE_CF_CW,
                    IE_CF_CCW
                };

                IECullingType m_Type = IECullingType::IE_CT_None;
                IECullingFace m_Face = IECullingFace::IE_CF_CW;

                ICulling();
                virtual ~ICulling();
        };

        typedef std::vector<float> IData;

        std::string m_Name;
        IFormat     m_Format;
        ICulling    m_Culling;
        Material    m_Material;
        IData       m_Data;

        /**
        * Called when a vertex color should be get
        *@param pVB - vertex buffer that will contain the vertex for which the color should be get
        *@param pNormal - vertex normal
        *@param groupIndex - the vertex group index (e.g. the inner and outer vertices of a ring)
        *@return RGBA color to apply to the vertex
        *@note This callback will be called only if the per-vertex color option is activated in the vertex
        *      buffer
        */
        typedef ColorF(*ITfOnGetVertexColor)(const VertexBuffer* pVB, const Vector3F* pNormal, std::size_t groupIndex);

        VertexBuffer();
        virtual ~VertexBuffer();

        /**
        * Clones the vertex buffer
        *@param includeData - if true, vertex buffer data will also be cloned
        *@return cloned vertex
        *@note Cloned vertex should be deleted when useless
        */
        virtual VertexBuffer* Clone(bool includeData = false) const;

        /**
        * Adds a vertex to a vertex buffer
        *@param pVertex - vertex
        *@param pNormal - normal
        *@param pUV - texture coordinate
        *@param groupIndex - the vertex group index (e.g. the inner and outer vertices of a ring)
        *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
        *@return true on success, otherwise false
        */
        virtual bool Add(const Vector3F*           pVertex,
                         const Vector3F*           pNormal,
                         const Vector2F*           pUV,
                               std::size_t         groupIndex,
                         const ITfOnGetVertexColor fOnGetVertexColor);
};
