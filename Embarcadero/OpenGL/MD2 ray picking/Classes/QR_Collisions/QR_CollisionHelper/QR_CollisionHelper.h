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

#ifndef QR_CollisionHelperH
#define QR_CollisionHelperH

// qr engine
#include "QR_Types.h"
#include "QR_Vertex.h"
#include "QR_Polygon.h"
#include "QR_Box.h"
#include "QR_Sphere.h"
#include "QR_Ray.h"

/**
* 3D Collisions detections toolbox
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_CollisionHelper
{
    public:
        /**
        * Polygon type enumeration
        */
        enum IEPolygonType
        {
            IE_PT_Simple,
            IE_PT_Indexed,
        };

        /**
        * Constructor
        */
        QR_CollisionHelper();

        /**
        * Destructor
        */
        virtual ~QR_CollisionHelper();

        /**
        * Checks if a ray intersects a polygon, and gets the resulting slidingplane
        *@param ray - ray to check
        *@param polygon - polygon to check
        *@return true if ray intersects polygon, otherwise false
        */
        template <class T>
        static bool SphereIntersectsPolygonAndSlide(const QR_Sphere<T>&  sphere,
                                                    const QR_Polygon<T>& polygon,
                                                          QR_Plane<T>&   slidingPlane);

        /**
        * Gets sliding point (correction of the player position using a plane)
        *@param sphere - sphere representing player
        *@param slidingPlane - sliding plane
        *@return sliding point
        */
        template <class T>
        static QR_Vector3D<T> GetSlidingPoint(const QR_Sphere<T>& sphere,
                                              const QR_Plane<T>&  slidingPlane);

        /**
        * Calculates and gets the projection of a given point on a given segment
        *@param segStart - segment start
        *@param segEnd - segment end
        *@param point - point for which projection must be calculated
        *@return calculated point
        */
        template <class T>
        static QR_Vector3D<T> ClosestPointOnLine(const QR_Vector3D<T>& segStart,
                                                 const QR_Vector3D<T>& segEnd,
                                                 const QR_Vector3D<T>& point);

        /**
        * Calculates and gets the projection of a given point on a given polygon
        *@param point - point for which projection must be calculated
        *@param polygon - polygon
        *@return the calculated point
        *@throw Exception on error
        */
        template <class T>
        static QR_Vector3D<T> ClosestPointOnTriangle(const QR_Vector3D<T>& point,
                                                     const QR_Polygon<T>&  polygon);

        /**
        * Calculates bounding box from polygon list
        *@param polygons - polygon list
        *@return bounding box
        */
        static QR_BoxP CalculateBoundingBox(const QR_PolygonsP& polygons);

        /**
        * Calculates bounding sphere from polygon list
        *@param polygons - polygon list
        *@return bounding sphere
        */
        static QR_SphereP CalculateBoundingSphere(const QR_PolygonsP& polygons);

        /**
        * Gets polygons from vertex
        *@param pVertex - source vertex descriptor, contains buffer to get from
        *@param polygonType - polygon type to generate
        *@param[out] polygons - polygon list to populate
        *@return true on success, otherwise false
        *@note Polygon list content should be deleted when useless
        */
        static bool GetPolygons(const QR_Vertex* pVertex, QR_PolygonsP& polygons);

    private:
            /**
            * Adds polygon to array
            *@param vb - source vertex buffer
            *@param v1 - first polygon vertex index in vertex buffer
            *@param v2 - second polygon vertex index in vertex buffer
            *@param v3 - third polygon vertex index in vertex buffer
            *@param[in, out] polygons - polygon array that contains generated polygons
            *@note Generated polygons should be deleted when useless
            */
            static inline void AddPolygon(const QR_Vertex::IBuffer& vb,
                                                std::size_t         v1,
                                                std::size_t         v2,
                                                std::size_t         v3,
                                                QR_PolygonsP&       polygons);
};

//---------------------------------------------------------------------------
// QR_CollisionHelper
//---------------------------------------------------------------------------
template <class T>
bool QR_CollisionHelper::SphereIntersectsPolygonAndSlide(const QR_Sphere<T>&  sphere,
                                                         const QR_Polygon<T>& polygon,
                                                               QR_Plane<T>&   slidingPlane)
{
    // create a plane using the 3 vertices of the polygon
    QR_Plane<T> polygonPlane = polygon.GetPlane();

    // calculate the distance between the center of the sphere and the plane
    T testPoint1 = polygonPlane.DistanceTo(sphere.m_Center);

    QR_Vector3D<T> sphereNormal;

    // calculate the normal of the distance sphere-plane using the positive or
    // negative value of the calculated distance between plane and position
    if (testPoint1 < T(0.0))
        sphereNormal = QR_Vector3D<T>(polygonPlane.m_A,
                                      polygonPlane.m_B,
                                      polygonPlane.m_C);
    else
        sphereNormal = QR_Vector3D<T>(-polygonPlane.m_A,
                                      -polygonPlane.m_B,
                                      -polygonPlane.m_C);

    // calculate the point who the segment from center of sphere in the
    // direction of the plane will cross the border of the sphere
    QR_Vector3D<T> pointOnSphere =
            QR_Vector3D<T>(sphere.m_Center.m_X + (sphere.m_Radius * sphereNormal.m_X),
                           sphere.m_Center.m_Y + (sphere.m_Radius * sphereNormal.m_Y),
                           sphere.m_Center.m_Z + (sphere.m_Radius * sphereNormal.m_Z));

    // calculate the distance between the border of the sphere and the plane
    T testPoint2 = polygonPlane.DistanceTo(pointOnSphere);

    // if the test points are on each side of the plane, then the sphere cross
    // the plane. We assume that the segment from the center of the sphere to
    // the direction of the plane can never be co-planar
    if ((testPoint1 <= T(0.0) && testPoint2 >= T(0.0)) ||
        (testPoint2 <= T(0.0) && testPoint1 >= T(0.0)))
    {
        QR_Vector3D<T> pointOnPlane;

        // calculate who the segment cross the plane
        if (testPoint1 == T(0.0))
            // if testPoint1 is equal to 0, the center of the sphere cross the
            // plane
            pointOnPlane = sphere.m_Center;
        else
        if (testPoint2 == T(0.0))
            // if testPoint2 is equal to 0, the border of the sphere cross the
            // plane
            pointOnPlane = pointOnSphere;
        else
            // calculate the intersection point
            polygonPlane.IntersectLine(sphere.m_Center,
                                       pointOnSphere,
                                       pointOnPlane);

        // check if calculated point is inside the polygon
        if (polygon.Inside(pointOnPlane))
        {
            // if yes, the sphere collide the polygon. In this case, we copy
            // the plane and we returns true
            slidingPlane = polygonPlane;
            return true;
        }
        else
        {
            // otherwise check if the sphere collide the border of the polygon.
            // First we calculate the test point on the border of the polygon
            QR_Vector3D<T> pointOnTriangle = ClosestPointOnTriangle(pointOnPlane, polygon);

            // check if calculated point is inside the sphere
            if (sphere.Inside(pointOnTriangle))
            {
                // if yes, the sphere collide the polygon. In this case, we
                // copy the plane and we returns true
                slidingPlane = polygonPlane;
                return true;
            }
        }
    }

    // no collision was found
    return false;
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_CollisionHelper::GetSlidingPoint(const QR_Sphere<T>& sphere,
                                                   const QR_Plane<T>&  slidingPlane)
{
    QR_Plane<T> plane = slidingPlane;

    // calculate the distance between the center of the sphere and the plane
    T distanceToPlane = plane.DistanceTo(sphere.m_Center);

    // check if value is negative
    if (distanceToPlane < T(0.0))
    {
        // invert the plane
        plane.m_A = -plane.m_A;
        plane.m_B = -plane.m_B;
        plane.m_C = -plane.m_C;
        plane.m_D = -plane.m_D;
    }

    // calculate the direction of the segment position - plane
    QR_Vector3D<T> planeRatio(sphere.m_Radius * plane.m_A,
                              sphere.m_Radius * plane.m_B,
                              sphere.m_Radius * plane.m_C);

    // calculate who the segment perpendicular to the plane, from the center
    // of the sphere, cross the collision sphere. Normally this point is beyond
    // the plane
    QR_Vector3D<T> pointBeyondPlane(sphere.m_Center.m_X - planeRatio.m_X,
                                    sphere.m_Center.m_Y - planeRatio.m_Y,
                                    sphere.m_Center.m_Z - planeRatio.m_Z);

    QR_Vector3D<T> pointOnPlane;

    // calculate the point who the segment "center of the sphere - point beyond
    // the plane" cross the collision plane
    slidingPlane.IntersectLine(sphere.m_Center, pointBeyondPlane, pointOnPlane);

    // from point calculated below, we add the radius of the sphere, and we
    // returns the value
    return QR_Vector3D<T>(pointOnPlane.m_X + planeRatio.m_X,
                          pointOnPlane.m_Y + planeRatio.m_Y,
                          pointOnPlane.m_Z + planeRatio.m_Z);
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_CollisionHelper::ClosestPointOnLine(const QR_Vector3D<T>& segStart,
                                                      const QR_Vector3D<T>& segEnd,
                                                      const QR_Vector3D<T>& point)
{
    // calculate the distance between the test point and the segment
    QR_Vector3D<T> PToStart  = point  - segStart;
    QR_Vector3D<T> length    = segEnd - segStart;
    T              segLength = length.Length();

    // calculate the direction of the segment
    QR_Vector3D<T> normalizedLength = length.Normalize();

    // calculate the projection of the point on the segment
    T angle = normalizedLength.Dot(PToStart);

    // check if projection is before the segment
    if (angle < T(0.0))
        return segStart;
    else
    // check if projection is after the segment
    if (angle > segLength)
        return segEnd;
    else
    {
        // calculate the position of the projection on the segment
        QR_Vector3D<T> p(normalizedLength.m_X * angle,
                         normalizedLength.m_Y * angle,
                         normalizedLength.m_Z * angle);

        // calculate and returns the point coordinate on the segment
        return (segStart + p);
    }
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_CollisionHelper::ClosestPointOnTriangle(const QR_Vector3D<T>& point,
                                                          const QR_Polygon<T>&  polygon)
{
    // get polygon vertices
    const QR_Vector3D<T> vertex1 = polygon.GetVertex1();
    const QR_Vector3D<T> vertex2 = polygon.GetVertex2();
    const QR_Vector3D<T> vertex3 = polygon.GetVertex3();

    // calculate the projections points on each edge of the triangle
    QR_Vector3D<T> rab = ClosestPointOnLine(vertex1, vertex2, point);
    QR_Vector3D<T> rbc = ClosestPointOnLine(vertex2, vertex3, point);
    QR_Vector3D<T> rca = ClosestPointOnLine(vertex3, vertex1, point);

    // calculate the distances between points below and test point
    QR_Vector3D<T> vAB = point - rab;
    QR_Vector3D<T> vBC = point - rbc;
    QR_Vector3D<T> vCA = point - rca;

    // calculate the length of each segments
    T dAB = vAB.Length();
    T dBC = vBC.Length();
    T dCA = vCA.Length();

    // calculate the shortest distance
    T min = dAB;

    QR_Vector3D<T> result = rab;

    // check if dBC is shortest
    if (dBC < min)
    {
        min    = dBC;
        result = rbc;
    }

    // check if dCA is shortest
    if (dCA < min)
        result = rca;

    return result;
}
//---------------------------------------------------------------------------
inline void QR_CollisionHelper::AddPolygon(const QR_Vertex::IBuffer& vb,
                                          std::size_t         v1,
                                          std::size_t         v2,
                                          std::size_t         v3,
                                          QR_PolygonsP&       polygons)
{
    std::auto_ptr<QR_PolygonP> pPolygon(new QR_PolygonP());

    // copy polygon from source buffer
    pPolygon->SetVertex(0, QR_Vector3DP(vb[v1], vb[v1 + 1], vb[v1 + 2]));
    pPolygon->SetVertex(1, QR_Vector3DP(vb[v2], vb[v2 + 1], vb[v2 + 2]));
    pPolygon->SetVertex(2, QR_Vector3DP(vb[v3], vb[v3 + 1], vb[v3 + 2]));

    // add polygon to list
    polygons.push_back(pPolygon.get());
    pPolygon.release();
}
//---------------------------------------------------------------------------

#endif
