/****************************************************************************
 * ==> QR_CollisionHelper --------------------------------------------------*
 ****************************************************************************
 * Description : Helper for collision detection                             *
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

#include "QR_CollisionHelper.h"

// std
#include <cmath>
#include <limits>

//---------------------------------------------------------------------------
// QR_CollisionHelper
//---------------------------------------------------------------------------
QR_CollisionHelper::QR_CollisionHelper()
{}
//---------------------------------------------------------------------------
QR_CollisionHelper::~QR_CollisionHelper()
{}
//---------------------------------------------------------------------------
QR_BoxP QR_CollisionHelper::CalculateBoundingBox(const QR_PolygonsP& polygons)
{
    QR_BoxP box;

    bool initialized = false;

    // iterate through polygons
    for (QR_PolygonsP::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        for (QR_UInt8 i = 0; i < 3; ++i)
        {
            // get vertex
            const QR_Vector3DP vertex = (*it)->GetVertex(i);

            // are values initialized?
            if (!initialized)
            {
                // initialize bounding box with first vertex
                box.m_Min   = QR_Vector3DP(vertex.m_X, vertex.m_Y, vertex.m_Z);
                box.m_Max   = QR_Vector3DP(vertex.m_X, vertex.m_Y, vertex.m_Z);
                initialized = true;
                continue;
            }

            // search for box min edge
            box.m_Min.m_X = std::min(box.m_Min.m_X, vertex.m_X);
            box.m_Min.m_Y = std::min(box.m_Min.m_Y, vertex.m_Y);
            box.m_Min.m_Z = std::min(box.m_Min.m_Z, vertex.m_Z);

            // search for box max edge
            box.m_Max.m_X = std::max(box.m_Max.m_X, vertex.m_X);
            box.m_Max.m_Y = std::max(box.m_Max.m_Y, vertex.m_Y);
            box.m_Max.m_Z = std::max(box.m_Max.m_Z, vertex.m_Z);
        }

    return box;
}
//---------------------------------------------------------------------------
QR_SphereP QR_CollisionHelper::CalculateBoundingSphere(const QR_PolygonsP& polygons)
{
    QR_Vector3DP vMax;
    QR_Vector3DP vMin;

    bool initialized = false;

    // iterate through polygons
    for (QR_PolygonsP::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        for (QR_UInt8 i = 0; i < 3; ++i)
        {
            // get vertex
            const QR_Vector3DP vertex = (*it)->GetVertex(i);

            // are values initialized?
            if (!initialized)
            {
                // initialize bounding box with first vertex
                vMin        = QR_Vector3DP(vertex.m_X, vertex.m_Y, vertex.m_Z);
                vMax        = QR_Vector3DP(vertex.m_X, vertex.m_Y, vertex.m_Z);
                initialized = true;
                continue;
            }

            // search for min edge
            vMin.m_X = std::min(vMin.m_X, vertex.m_X);
            vMin.m_Y = std::min(vMin.m_Y, vertex.m_Y);
            vMin.m_Z = std::min(vMin.m_Z, vertex.m_Z);

            // search for max edge
            vMax.m_X = std::max(vMax.m_X, vertex.m_X);
            vMax.m_Y = std::max(vMax.m_Y, vertex.m_Y);
            vMax.m_Z = std::max(vMax.m_Z, vertex.m_Z);
        }

    QR_SphereP sphere;

    // calculate sphere center
    sphere.m_Center = QR_Vector3DP((vMin.m_X + vMax.m_X) / M_Precision(2.0),
                                   (vMin.m_Y + vMax.m_Y) / M_Precision(2.0),
                                   (vMin.m_Z + vMax.m_Z) / M_Precision(2.0));

    // calculate sphere radius
    sphere.m_Radius = QR_Vector3DP((vMax.m_X - vMin.m_X) / M_Precision(2.0),
                                   (vMax.m_Y - vMin.m_Y) / M_Precision(2.0),
                                   (vMax.m_Z - vMin.m_Z) / M_Precision(2.0)).Length();

    return sphere;
}
//---------------------------------------------------------------------------
bool QR_CollisionHelper::GetPolygons(const QR_Vertex* pVertex, QR_PolygonsP& polygons)
{
    const QR_SizeT vertexCount = pVertex->m_Buffer.size();

    // no data to extract from?
    if (!vertexCount)
        return true;

    // search for vertex type
    switch (pVertex->m_Type)
    {
        case QR_Vertex::IE_VT_Triangles:
        {
            // calculate iteration step
            const QR_SizeT step = (pVertex->m_Stride * 3);

            // iterate through source vertexes
            for (QR_SizeT i = 0; i < vertexCount; i += step)
                // extract polygon from source buffer
                AddPolygon(pVertex->m_Buffer,
                           i,
                           i +  pVertex->m_Stride,
                           i + (pVertex->m_Stride * 2),
                           polygons);

            return true;
        }

        case QR_Vertex::IE_VT_TriangleStrip:
        {
            // calculate length to read in triangle strip buffer
            const QR_SizeT stripLength = (vertexCount - (pVertex->m_Stride * 2));
                  QR_SizeT index       = 0;

            // iterate through source vertexes
            for (QR_SizeT i = 0; i < stripLength; i += pVertex->m_Stride)
            {
                // extract polygon from source buffer, revert odd polygons
                if (!index || !(index % 2))
                    AddPolygon(pVertex->m_Buffer,
                               i,
                               i +  pVertex->m_Stride,
                               i + (pVertex->m_Stride * 2),
                               polygons);
                else
                    AddPolygon(pVertex->m_Buffer,
                               i +  pVertex->m_Stride,
                               i,
                               i + (pVertex->m_Stride * 2),
                               polygons);

                ++index;
            }

            return true;
        }

        case QR_Vertex::IE_VT_TriangleFan:
        {
            // calculate length to read in triangle fan buffer
            const QR_SizeT fanLength = (vertexCount - pVertex->m_Stride);

            // iterate through source vertexes
            for (QR_SizeT i = pVertex->m_Stride; i < fanLength; i += pVertex->m_Stride)
                // extract polygon from source buffer
                AddPolygon(pVertex->m_Buffer,
                           0,
                           i,
                           i + pVertex->m_Stride,
                           polygons);

            return true;
        }

        case QR_Vertex::IE_VT_Quads:
        {
            // calculate iteration step
            const QR_SizeT step = (pVertex->m_Stride * 4);

            // iterate through source vertexes
            for (QR_SizeT i = 0; i < vertexCount; i += step)
            {
                // calculate vertexes position
                const QR_SizeT v1 = i;
                const QR_SizeT v2 = i +  pVertex->m_Stride;
                const QR_SizeT v3 = i + (pVertex->m_Stride * 2);
                const QR_SizeT v4 = i + (pVertex->m_Stride * 3);

                // extract polygons from source buffer
                AddPolygon(pVertex->m_Buffer, v1, v2, v3, polygons);
                AddPolygon(pVertex->m_Buffer, v3, v2, v4, polygons);
            }

            return true;
        }

        case QR_Vertex::IE_VT_QuadStrip:
        {
            // calculate iteration step
            const QR_SizeT step = (pVertex->m_Stride * 2);

            // calculate length to read in triangle strip buffer
            const QR_SizeT stripLength = (vertexCount - (pVertex->m_Stride * 2));

            // iterate through source vertexes
            for (QR_SizeT i = 0; i < stripLength; i += step)
            {
                // calculate vertexes position
                const QR_SizeT v1 = i;
                const QR_SizeT v2 = i +  pVertex->m_Stride;
                const QR_SizeT v3 = i + (pVertex->m_Stride * 2);
                const QR_SizeT v4 = i + (pVertex->m_Stride * 3);

                // extract polygons from source buffer
                AddPolygon(pVertex->m_Buffer, v1, v2, v3, polygons);
                AddPolygon(pVertex->m_Buffer, v3, v2, v4, polygons);
            }

            return true;
        }

        case QR_Vertex::IE_VT_Unknown:
        default:
            return false;
    }
}
//---------------------------------------------------------------------------
