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

#pragma once

 // std
#include <vector>
#include <string>

// classes
#include "Color.h"
#include "Texture.h"
#include "Vector2.h"
#include "Vector3.h"

/**
* Material, describes the way a mesh reacts to its environment
*/
class Material
{
    public:
        Texture* m_pTexture;    // texture to apply to vertex buffer
        ColorF   m_Color;       // vertex color, applied to all vertices if per-vertex color is disabled
        bool     m_Transparent; // whether or not the alpha blending should be activated
        bool     m_Wireframe;   // whether or not the vertex buffer should be drawn in wireframe

        Material();
        virtual ~Material();
};

/**
* Vertex format
*/
class VertexFormat
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
            IE_VF_Normals   = 0x01, // each vertex contains a normal
            IE_VF_TexCoords = 0x02, // each vertex contains an UV texture coordinate
            IE_VF_Colors    = 0x04  // each vertex contains its own color
        };

        std::size_t m_Stride; // vertex stride (i.e. length between each vertex) in bytes
        IEType      m_Type;   // vertex type (i.e. how vertex is organized: triangle list, triangle fan, ...)
        IEFormat    m_Format; // vertex format (i.e. what data vertex contains: position, normal, texture, ...)

        VertexFormat();
        virtual ~VertexFormat();

        /**
        * Calculates the vertex stride
        */
        virtual void CalculateStride();

        /**
        * Compares vertex and determine if their format are equivalent
        *@param other - other vertex to compare with
        *@return true if both vertex header are equivalent, otherwise false
        */
        virtual bool CompareFormat(const VertexFormat& other) const;
};

/**
* Vertex culling
*/
class VertexCulling
{
    public:
        /**
        * Culling type
        */
        enum IECullingType
        {
            IE_CT_None,
            IE_CT_Front,
            IE_CT_Back,
            IE_CT_Both
        };

        /**
        * Culling face
        */
        enum IECullingFace
        {
            IE_CF_CW,
            IE_CF_CCW
        };

        IECullingType m_Type;
        IECullingFace m_Face;

        VertexCulling();
        virtual ~VertexCulling();
};

/**
* Vertex descriptor, contains global enumeration and types
*@author Jean-Milost Reymond
*/
class VertexBuffer
{
    public:
        typedef std::vector<float> IData;

        std::string   m_Name;
        VertexFormat  m_Format;
        VertexCulling m_Culling;
        Material      m_Material;
        IData         m_Data;

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
        * Clones vertex in such manner that vertex info are copied, but not vertex buffer
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

/**
* Mesh
*/
class Mesh
{
    public:
        std::vector<VertexBuffer*> m_VB;

        Mesh();
        virtual ~Mesh();
};
