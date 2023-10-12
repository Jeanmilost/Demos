/****************************************************************************
 * ==> GJK_PolygonCollider -------------------------------------------------*
 ****************************************************************************
 * Description : Polygon collider                                           *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - GJK 2D                                                     *
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

#include "GJK_PolygonCollider.h"

//------------------------------------------------------------------------------
// GJK_PolygonCollider
//------------------------------------------------------------------------------
GJK_PolygonCollider::GJK_PolygonCollider() :
    GJK_Collider()
{}
//------------------------------------------------------------------------------
GJK_PolygonCollider::~GJK_PolygonCollider()
{}
//------------------------------------------------------------------------------
GJK_Vector2F GJK_PolygonCollider::AveragePoint(bool transform) const
{
    GJK_Vector2F average;

    for (size_t i = 0; i < m_Vertices.size(); ++i)
        average += (transform ? m_Matrix.Transform(m_Vertices[i]) : m_Vertices[i]);

    return (average / m_Vertices.size());
}
//------------------------------------------------------------------------------
GJK_Vector2F GJK_PolygonCollider::Support(const GJK_Vector2F& dir) const
{
    if (!m_Vertices.size())
        return GJK_Vector2F();

    // calculate initial max product on the first vertex
    float       maxProduct = dir.Dot(m_Matrix.Transform(m_Vertices[0]));
    std::size_t index      = 0;

    // iterate through vertices
    for (std::size_t i = 1; i < m_Vertices.size(); ++i)
    {
        // calculate vertex dot product on the given direction
        const float product = dir.Dot(m_Matrix.Transform(m_Vertices[i]));

        // found a further vertex?
        if (product > maxProduct)
        {
            // update the max product and keep the index 
            maxProduct = product;
            index      = i;
        }
    }

    return m_Matrix.Transform(m_Vertices[index]);
}
//------------------------------------------------------------------------------
