/****************************************************************************
 * ==> QR_Polygon ----------------------------------------------------------*
 ****************************************************************************
 * Description : 3D Polygon                                                 *
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

#ifndef QR_PolygonH
#define QR_PolygonH

// std
#include <math.h>
#include <vector>
#include <list>
#include <cstring>
#include <limits>

#if defined (OS_WIN)
    #include <cmath>
#endif

// qr engine
#include "QR_Types.h"
#include "QR_Matrix16.h"
#include "QR_Plane.h"
#include "QR_Line.h"

/**
* Class representing a polygon
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
template <class T>
class QR_Polygon
{
    public:
        /**
        * Constructor
        */
        QR_Polygon();

        /**
        * Constructor
        *@param vertex1 - first vertex of the polygon
        *@param vertex2 - second vertex of the polygon
        *@param vertex3 - third vertex of the polygon
        */
        QR_Polygon(const QR_Vector3D<T>& vertex1,
                   const QR_Vector3D<T>& vertex2,
                   const QR_Vector3D<T>& vertex3);

        /**
        * Destructor
        */
        virtual ~QR_Polygon();

        virtual QR_Vector3D<T> operator[] (QR_UInt8 index) const;

        /**
        * Gets vertex at index
        *@param index - vertex index
        *@return corresponding vertex, empty vertex if not found
        */
        virtual QR_Vector3D<T> GetVertex(QR_UInt8 index) const;

        /**
        * Sets vertex
        *@param index - vertex index to set
        *@param vertex - vertex value
        */
        virtual void SetVertex(QR_UInt8 index, const QR_Vector3D<T>& vertex);

        /**
        * Gets first polygon vertex
        *@return first polygon vertex
        */
        virtual QR_Vector3D<T> GetVertex1() const;

        /**
        * Sets first polygon vertex
        *@param value - first polygon vertex value
        */
        virtual void SetVertex1(const QR_Vector3D<T>& value);

        /**
        * Gets second polygon vertex
        *@return second polygon vertex
        */
        virtual QR_Vector3D<T> GetVertex2() const;

        /**
        * Sets second polygon vertex
        *@param value - second polygon vertex value
        */
        virtual void SetVertex2(const QR_Vector3D<T>& value);

        /**
        * Gets third polygon vertex
        *@return third polygon vertex
        */
        virtual QR_Vector3D<T> GetVertex3() const;

        /**
        * Sets third polygon vertex
        *@param value - third polygon vertex value
        */
        virtual void SetVertex3(const QR_Vector3D<T>& value);

        /**
        * Creates and returns a clone of the polygon
        *@return a clone of the polygon
        *@note The returned polygon should be deleted when useless
        */
        virtual QR_Polygon* GetClone() const;

        /**
        * Gets the polygon plane
        *@return the polygon plane
        */
        virtual QR_Plane<T> GetPlane() const;

        /**
        * Calculates and returns the center point of the polygon
        *@return the center point of the polygon
        */
        virtual QR_Vector3D<T> GetCenter() const;

        /**
        * Checks if a point is inside polygon
        *@param x - point x coordinate
        *@param y - point y coordinate
        *@param z - point z coordinate
        *@return true if point is inside polygon, otherwise false
        */
        virtual bool Inside(T x, T y, T z) const;

        /**
        * Checks if a point is inside polygon
        *@param point - point coordinate
        *@return true if point is inside polygon, otherwise false
        */
        virtual bool Inside(const QR_Vector3D<T>& point) const;

        /**
        * Checks if polygon intersects with another polygon
        *@param pOther - other polygon to check
        *@param tolerance - tolerance
        *@return true if polygons intersect, otherwise false
        */
        virtual bool Intersect(const QR_Polygon* pOther, T tolerance = M_Epsilon) const;

        /**
        * Calculates distance to polygon
        *@param point - point from which the distance must be calculated
        *@return distance to polygon
        */
        virtual T DistanceTo(const QR_Vector3D<T>& point) const;

        /**
        * Applies the given matrix to the polygon
        *@param matrix - matrix to apply
        *@return transformed polygon
        *@note The returned polygon should be deleted when useless
        */
        virtual QR_Polygon* ApplyMatrix(const QR_Matrix16<T>& matrix) const;

    private:
        QR_Vector3D<T> m_Vertex[3];

        /**
        * Checks if a vector is between start and end limits
        *@param value - value to check
        *@param start - start limit
        *@param end - end limit
        *@param tolerance - tolerance
        *@return true if value is between limits, otherwise false
        */
        bool IsBetween(const QR_Vector3D<T>& value,
                       const QR_Vector3D<T>& start,
                       const QR_Vector3D<T>& end,
                             T               tolerance) const;

        /**
        * Checks if a value is between start and end limits
        *@param value - value to check
        *@param start - start limit
        *@param end - end limit
        *@param tolerance - tolerance
        *@return true if value is between limits, otherwise false
        */
        bool IsBetween(T value, T start, T end, T tolerance) const;
};

typedef QR_Polygon<float>       QR_PolygonF;
typedef QR_Polygon<double>      QR_PolygonD;
typedef QR_Polygon<M_Precision> QR_PolygonP;

/**
* Polygon list
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
typedef std::list<QR_PolygonP*> QR_PolygonsP;

//---------------------------------------------------------------------------
// QR_Polygon
//---------------------------------------------------------------------------
template <class T>
QR_Polygon<T>::QR_Polygon()
{}
//---------------------------------------------------------------------------
template <class T>
QR_Polygon<T>::QR_Polygon(const QR_Vector3D<T>& vertex1,
                          const QR_Vector3D<T>& vertex2,
                          const QR_Vector3D<T>& vertex3)
{
    m_Vertex[0] = vertex1;
    m_Vertex[1] = vertex2;
    m_Vertex[2] = vertex3;
}
//---------------------------------------------------------------------------
template <class T>
QR_Polygon<T>::~QR_Polygon()
{}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Polygon<T>::operator[] (QR_UInt8 index) const
{
    return GetVertex(index);
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Polygon<T>::GetVertex(QR_UInt8 index) const
{
    // search for index to get
    switch (index)
    {
        case 0:
        case 1:
        case 2:  return m_Vertex[index];
        default: return QR_Vector3D<T>();
    }
}
//---------------------------------------------------------------------------
template <class T>
void QR_Polygon<T>::SetVertex(QR_UInt8 index, const QR_Vector3D<T>& vertex)
{
    // search for index to set
    switch (index)
    {
        case 0:
        case 1:
        case 2: m_Vertex[index] = vertex; return;
    }
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Polygon<T>::GetVertex1() const
{
    return m_Vertex[0];
}
//---------------------------------------------------------------------------
template <class T>
void QR_Polygon<T>::SetVertex1(const QR_Vector3D<T>& value)
{
    m_Vertex[0] = value;
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Polygon<T>::GetVertex2() const
{
    return m_Vertex[1];
}
//---------------------------------------------------------------------------
template <class T>
void QR_Polygon<T>::SetVertex2(const QR_Vector3D<T>& value)
{
    m_Vertex[1] = value;
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Polygon<T>::GetVertex3() const
{
    return m_Vertex[2];
}
//---------------------------------------------------------------------------
template <class T>
void QR_Polygon<T>::SetVertex3(const QR_Vector3D<T>& value)
{
    m_Vertex[2] = value;
}
//---------------------------------------------------------------------------
template <class T>
QR_Polygon<T>* QR_Polygon<T>::GetClone() const
{
    // copies the polygon, then returns the copy
    return new QR_Polygon<T>(m_Vertex[0], m_Vertex[1], m_Vertex[2]);
}
//---------------------------------------------------------------------------
template <class T>
QR_Plane<T> QR_Polygon<T>::GetPlane() const
{
    // calculates the plane from the values of the 3 vertices of the polygon
    return QR_Plane<T>::FromPoints(m_Vertex[0], m_Vertex[1], m_Vertex[2]);
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Polygon<T>::GetCenter() const
{
    // calculates then returns the value of the midpoint of the polygon
    return QR_Vector3D<T>(((m_Vertex[0].m_X + m_Vertex[1].m_X + m_Vertex[2].m_X) / T(3.0)),
                          ((m_Vertex[0].m_Y + m_Vertex[1].m_Y + m_Vertex[2].m_Y) / T(3.0)),
                          ((m_Vertex[0].m_Z + m_Vertex[1].m_Z + m_Vertex[2].m_Z) / T(3.0)));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Polygon<T>::Inside(T x, T y, T z) const
{
    return Inside(QR_Vector3D<T>(x, y, z));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Polygon<T>::Inside(const QR_Vector3D<T>& point) const
{
    /*
    * check if the point p is inside the polygon in the following manner:
    *
    *                  V1                         V1
    *                  /\                         /\
    *                 /  \                       /  \
    *                / *p \                  *P /    \
    *               /      \                   /      \
    *            V2 -------- V3             V2 -------- V3
    *
    * calculate the vectors between the point p and each polygon vertex, then
    * calculate the angle formed by each of these vectors. If the sum of the
    * angles is equal to a complete circle, i.e. 2 * pi in radians, then the
    * point p is inside the polygon limits, otherwise the point is outside. It
    * is assumed that the point to check belongs to the polygon's plane
    */
    const QR_Vector3D<T> nPToV1 = (GetVertex1() - point).Normalize();
    const QR_Vector3D<T> nPToV2 = (GetVertex2() - point).Normalize();
    const QR_Vector3D<T> nPToV3 = (GetVertex3() - point).Normalize();

    // calculate the angles using the dot product of each vectors. Limit range
    // to values between -1.0f and 1.0f
    const T a1 = std::max(std::min(nPToV1.Dot(nPToV2), T(1.0)), T(-1.0));
    const T a2 = std::max(std::min(nPToV2.Dot(nPToV3), T(1.0)), T(-1.0));
    const T a3 = std::max(std::min(nPToV3.Dot(nPToV1), T(1.0)), T(-1.0));

    // calculate the sum of all angles
    #if defined(OS_OSX)
        const T angleResult = acosf(a1) + acosf(a2) + acosf(a3);
    #else
        const T angleResult = std::acos(a1) + std::acos(a2) + std::acos(a3);
    #endif

    // if sum is equal to 6.28 radians then point p is inside polygon. NOTE can
    // be higher due to precision errors in calculations
    return (angleResult >= T(6.28));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Polygon<T>::Intersect(const QR_Polygon* pOther, T tolerance) const
{
    // get planes from polygons
    const QR_Plane<T> plane1 = QR_Plane<T>::FromPoints(m_Vertex[0], m_Vertex[1], m_Vertex[2]);
    const QR_Plane<T> plane2 = QR_Plane<T>::FromPoints(pOther->m_Vertex[0],
                                                       pOther->m_Vertex[1],
                                                       pOther->m_Vertex[2]);

    // are planes merged?
    if (plane1 == plane2 || plane1 == -plane2)
    {
        // is any vertex inside other polygon?
        if (Inside(pOther->m_Vertex[0]) || Inside(pOther->m_Vertex[1]) || Inside(pOther->m_Vertex[2]) ||
            pOther->Inside(m_Vertex[0]) || pOther->Inside(m_Vertex[1]) || pOther->Inside(m_Vertex[2]))
            return true;

        // create polygon lines
        const QR_Line<T> v1v2Line  (m_Vertex[0],         m_Vertex[1]);
        const QR_Line<T> v2v3Line  (m_Vertex[1],         m_Vertex[2]);
        const QR_Line<T> v3v1Line  (m_Vertex[2],         m_Vertex[0]);
        const QR_Line<T> ov1ov2Line(pOther->m_Vertex[0], pOther->m_Vertex[1]);
        const QR_Line<T> ov2ov3Line(pOther->m_Vertex[1], pOther->m_Vertex[2]);
        const QR_Line<T> ov3ov1Line(pOther->m_Vertex[2], pOther->m_Vertex[0]);
              QR_Line<T> output;

        // is shortest distance between lines equal to 0?
        if (v1v2Line.GetShortestDistance(ov1ov2Line) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (v2v3Line.GetShortestDistance(ov1ov2Line) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (v3v1Line.GetShortestDistance(ov1ov2Line) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (v1v2Line.GetShortestDistance(ov2ov3Line) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (v2v3Line.GetShortestDistance(ov2ov3Line) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (v3v1Line.GetShortestDistance(ov2ov3Line) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (v1v2Line.GetShortestDistance(ov3ov1Line) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (v2v3Line.GetShortestDistance(ov3ov1Line) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (v3v1Line.GetShortestDistance(ov3ov1Line) < tolerance)
            return true;

        return false;
    }

    // get plane normals
    QR_Vector3D<T> normal1(plane1.m_A, plane1.m_B, plane1.m_C);
    QR_Vector3D<T> normal2(plane2.m_A, plane2.m_B, plane2.m_C);

    // calculate angle between planes
    const T planesDot = normal1.Dot(normal2);

    // are plane parallels?
    if (planesDot == T(1.0) || planesDot == T(-1.0))
        // planes are parallels but not merged, no collision is possible
        return false;

    // calculate distance from each first polygon vertex to second polygon plane
    const T dist1v1 = plane2.DistanceTo(m_Vertex[0]);
    const T dist1v2 = plane2.DistanceTo(m_Vertex[1]);
    const T dist1v3 = plane2.DistanceTo(m_Vertex[2]);

    // prepare list containing first polygon intersection points
    std::vector< QR_Vector3D<T> > p1pts;
    p1pts.reserve(2);

    // is first polygon V1 to V2 line segment intersects second polygon plane?
    if ((dist1v1 >= T(0.0) && dist1v2 < T(0.0)) || (dist1v1 < T(0.0) && dist1v2 >= T(0.0)))
    {
        QR_Vector3D<T> p;

        // calculate intersection point and add to list on success
        if (plane2.IntersectLine(m_Vertex[0], m_Vertex[1], p))
            p1pts.push_back(p);
    }

    // is first polygon V2 to V3 line segment intersects second polygon plane?
    if ((dist1v2 >= T(0.0) && dist1v3 < T(0.0)) || (dist1v2 < T(0.0) && dist1v3 >= T(0.0)))
    {
        QR_Vector3D<T> p;

        // calculate intersection point and add to list on success
        if (plane2.IntersectLine(m_Vertex[1], m_Vertex[2], p))
            p1pts.push_back(p);
    }

    // is first polygon V3 to V1 line segment intersects second polygon plane?
    if ((dist1v3 >= T(0.0) && dist1v1 < T(0.0)) || (dist1v3 < T(0.0) && dist1v1 >= T(0.0)))
    {
        QR_Vector3D<T> p;

        // calculate intersection point and add to list on success
        if (plane2.IntersectLine(m_Vertex[2], m_Vertex[0], p))
            p1pts.push_back(p);
    }

    // were the first polygon 2 intersection point found?
    if (p1pts.size() != 2)
        return false;

    // calculate distance from each second polygon vertex to first polygon plane
    const T dist2v1 = plane1.DistanceTo(pOther->m_Vertex[0]);
    const T dist2v2 = plane1.DistanceTo(pOther->m_Vertex[1]);
    const T dist2v3 = plane1.DistanceTo(pOther->m_Vertex[2]);

    // prepare list containing second polygon intersection points
    std::vector< QR_Vector3D<T> > p2pts;
    p2pts.reserve(2);

    // is second polygon V1 to V2 line segment intersects first polygon plane?
    if ((dist2v1 >= T(0.0) && dist2v2 < T(0.0)) || (dist2v1 < T(0.0) && dist2v2 >= T(0.0)))
    {
        QR_Vector3D<T> p;

        // calculate intersection point and add to list on success
        if (plane1.IntersectLine(pOther->m_Vertex[0], pOther->m_Vertex[1], p))
            p2pts.push_back(p);
    }

    // is second polygon V2 to V3 line segment intersects first polygon plane?
    if ((dist2v2 >= T(0.0) && dist2v3 < T(0.0)) || (dist2v2 < T(0.0) && dist2v3 >= T(0.0)))
    {
        QR_Vector3D<T> p;

        // calculate intersection point and add to list on success
        if (plane1.IntersectLine(pOther->m_Vertex[1], pOther->m_Vertex[2], p))
            p2pts.push_back(p);
    }

    // is second polygon V3 to V1 line segment intersects first polygon plane?
    if ((dist2v3 >= T(0.0) && dist2v1 < T(0.0)) || (dist2v3 < T(0.0) && dist2v1 >= T(0.0)))
    {
        QR_Vector3D<T> p;

        // calculate intersection point and add to list on success
        if (plane1.IntersectLine(pOther->m_Vertex[2], pOther->m_Vertex[0], p))
            p2pts.push_back(p);
    }

    // were the second polygon 2 intersection point found?
    if (p2pts.size() != 2)
        return false;

    // first and second polygon intersection points are on the same line, so
    // check if calculated first and second polygon segments intersect
    return (IsBetween(p1pts[0], p2pts[0], p2pts[1], tolerance) ||
            IsBetween(p1pts[1], p2pts[0], p2pts[1], tolerance) ||
            IsBetween(p2pts[0], p1pts[0], p1pts[1], tolerance) ||
            IsBetween(p2pts[1], p1pts[0], p1pts[1], tolerance));
}
//---------------------------------------------------------------------------
template <class T>
T QR_Polygon<T>::DistanceTo(const QR_Vector3D<T>& point) const
{
    T result = std::numeric_limits<double>::infinity();

    // check each line formed by polygon
    for (QR_UInt8 i = 0; i < 3; ++i)
    {
        // get previous line index to check with
        const QR_UInt8 previousIndex = i ? i - 1 : 2;

        // get line to check
        QR_Line<T> line(GetVertex(i), GetVertex(previousIndex));

        // calculate distance between point and polygon line
        const T dist = line.DistanceTo(point);

        // keep shortest distance
        if (dist < result)
            result = dist;
    }

    return result;
}
//---------------------------------------------------------------------------
template <class T>
QR_Polygon<T>* QR_Polygon<T>::ApplyMatrix(const QR_Matrix16<T>& matrix) const
{
    // build a new polygon transforming all vertices of the polygon using
    // given matrix, and return new built polygon
    return new QR_Polygon<T>(matrix.Transform(m_Vertex[0]),
                             matrix.Transform(m_Vertex[1]),
                             matrix.Transform(m_Vertex[2]));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Polygon<T>::IsBetween(const QR_Vector3D<T>& value,
                              const QR_Vector3D<T>& start,
                              const QR_Vector3D<T>& end,
                                    T               tolerance) const
{
    // check if each vector component is between start and end limits
    return (IsBetween(value.m_X, start.m_X, end.m_X, tolerance) &&
            IsBetween(value.m_Y, start.m_Y, end.m_Y, tolerance) &&
            IsBetween(value.m_Z, start.m_Z, end.m_Z, tolerance));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Polygon<T>::IsBetween(T value, T start, T end, T tolerance) const
{
    // check if each value is between start and end limits considering tolerance
    return (value >= std::min(start, end) - tolerance &&
            value <= std::max(start, end) + tolerance);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// RAD studio
//---------------------------------------------------------------------------
#if defined(CP_EMBARCADERO)
    // needed to avoid the W8058 error "Cannot create pre-compiled header: header incomplete"
    // warning in BCC compilers
    ;
#endif
//---------------------------------------------------------------------------

#endif
