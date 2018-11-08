/****************************************************************************
 * ==> QR_GeometryHelper ---------------------------------------------------*
 ****************************************************************************
 * Description : Geometrical functions and algorithms                       *
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

#ifndef QR_GeometryHelperH
#define QR_GeometryHelperH

// qr engine
#include "QR_Types.h"
#include "QR_Plane.h"
#include "QR_Ray.h"
#include "QR_Box.h"
#include "QR_Sphere.h"
#include "QR_Polygon.h"

/**
* Geometrical functions and algorithms
*@author Jean-Milost Reymond
*/
class QR_GeometryHelper
{
    public:
        QR_GeometryHelper();
        virtual ~QR_GeometryHelper();

        /**
        * Checks if a box intersects a sphere
        *@param box - box to check
        *@param sphere - sphere to check
        *@return true if box intersects sphere, otherwise false
        */
        template <class T>
        static bool BoxIntersectsSphere(const QR_Box<T>& box, const QR_Sphere<T>& sphere);

        /**
        * Checks if a box intersects a plane
        *@param box - box to check
        *@param plane - plane to check
        *@return true if box intersects plane, otherwise false
        */
        template <class T>
        static bool BoxIntersectsPlane(const QR_Box<T>& box, const QR_Plane<T>& plane);

        /**
        * Checks if a sphere intersects a plane
        *@param sphere - sphere to check
        *@param plane - plane to check
        *@return true if box intersects plane, otherwise false
        */
        template <class T>
        static bool SphereIntersectsPlane(const QR_Sphere<T>& sphere, const QR_Plane<T>& plane);

        /**
        * Checks if a ray intersects a box
        *@param ray - ray to check
        *@param box - box to check
        *@return true if ray intersects box, otherwise false
        */
        template <class T>
        static bool RayIntersectsBox(const QR_Ray<T>& ray, const QR_Box<T>& box);

        /**
        * Checks if a ray intersects a polygon
        *@param ray - ray to check
        *@param polygon - polygon to check
        *@return true if ray intersects polygon, otherwise false
        */
        template <class T>
        static bool RayIntersectsPolygon(const QR_Ray<T>& ray, const QR_Polygon<T>& polygon);
};

//---------------------------------------------------------------------------
// QR_GeometryHelper
//---------------------------------------------------------------------------
template <class T>
bool QR_GeometryHelper::BoxIntersectsSphere(const QR_Box<T>& box, const QR_Sphere<T>& sphere)
{
    T d = T(0.0);

    // find the square of the distance from the sphere to the box on the x axis
    if (sphere.m_Center.m_X < box.m_Min.m_X)
        d += std::pow(sphere.m_Center.m_X - box.m_Min.m_X, T(2.0));
    else
    if (sphere.m_Center.m_X > box.m_Max.m_X)
        d += std::pow(sphere.m_Center.m_X - box.m_Max.m_X, T(2.0));

    // find the square of the distance from the sphere to the box on the y axis
    if (sphere.m_Center.m_Y < box.m_Min.m_Y)
        d += std::pow(sphere.m_Center.m_Y - box.m_Min.m_Y, T(2.0));
    else
    if (sphere.m_Center.m_Y > box.m_Max.m_Y)
        d += std::pow(sphere.m_Center.m_Y - box.m_Max.m_Y, T(2.0));

    // find the square of the distance from the sphere to the box on the z axis
    if (sphere.m_Center.m_Z < box.m_Min.m_Z)
        d += std::pow(sphere.m_Center.m_Z - box.m_Min.m_Z, T(2.0));
    else
    if (sphere.m_Center.m_Z > box.m_Max.m_Z)
        d += std::pow(sphere.m_Center.m_Z - box.m_Max.m_Z, T(2.0));

    return (d <= std::pow(sphere.m_Radius, T(2.0)));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_GeometryHelper::BoxIntersectsPlane(const QR_Box<T>& box, const QR_Plane<T>& plane)
{
    // get the 8 box vertexes
    QR_Vector3D<T> vertexes[8];
    vertexes[0] = box.m_Min;
    vertexes[1] = QR_Vector3D<T>(box.m_Min.m_X, box.m_Max.m_Y, box.m_Min.m_Z);
    vertexes[2] = QR_Vector3D<T>(box.m_Max.m_X, box.m_Min.m_Y, box.m_Min.m_Z);
    vertexes[3] = QR_Vector3D<T>(box.m_Max.m_X, box.m_Max.m_Y, box.m_Min.m_Z);
    vertexes[4] = QR_Vector3D<T>(box.m_Min.m_X, box.m_Min.m_Y, box.m_Max.m_Z);
    vertexes[5] = QR_Vector3D<T>(box.m_Min.m_X, box.m_Max.m_Y, box.m_Max.m_Z);
    vertexes[6] = QR_Vector3D<T>(box.m_Max.m_X, box.m_Min.m_Y, box.m_Max.m_Z);
    vertexes[7] = box.m_Max;

    T refLength = T(0.0);

    // iterate through box vertexes
    for (QR_UInt8 i = 0; i < 8; ++i)
    {
        // calculate distance between current box vertex and plane
        const T length = plane.DistanceTo(vertexes[i]);

        // is vertex on plane?
        if (length == T(0.0))
            return true;

        // is reference initialized?
        if (refLength == T(0.0))
        {
            refLength = length;
            continue;
        }

        // box cross plane if at least one vertex is on the other plane side
        if ((length < T(0.0) && refLength > T(0.0)) || (length > T(0.0) && refLength < T(0.0)))
            return true;
    }

    return false;
}
//---------------------------------------------------------------------------
template <class T>
bool QR_GeometryHelper::SphereIntersectsPlane(const QR_Sphere<T>& sphere, const QR_Plane<T>& plane)
{
    // calculate distance between sphere center and plane
    const T sphereToPlane = plane.DistanceTo(sphere.m_Center);

    QR_Vector3D<T> sphereNormal;

    // is sphere before or after plane?
    if (sphereToPlane < T(0.0))
        // get sphere normal
        sphereNormal = QR_Vector3D<T>(plane.m_A, plane.m_B, plane.m_C);
    else
        // get sphere normal, invert values to conserve correct direction
        sphereNormal = QR_Vector3D<T>(-plane.m_A, -plane.m_B, -plane.m_C);

    // calculate where vector starting from sphere center toward the plane
    // crosses the sphere border
    const QR_Vector3D<T> pointOnSphere(sphere.m_Center.m_X + (sphere.m_Radius * sphereNormal.m_X),
                                       sphere.m_Center.m_Y + (sphere.m_Radius * sphereNormal.m_Y),
                                       sphere.m_Center.m_Z + (sphere.m_Radius * sphereNormal.m_Z));

    // calculate where vector starting from sphere center toward the plane
    // crosses the plane
    T radiusToPlane = plane.DistanceTo(pointOnSphere);

    // sphere cross plane if both points are located on each side of the plane
    // or if one point on the plane
    return ((sphereToPlane <  T(0.0) && radiusToPlane >  T(0.0)) ||
            (sphereToPlane >  T(0.0) && radiusToPlane <  T(0.0)) ||
             sphereToPlane == T(0.0) || radiusToPlane == T(0.0));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_GeometryHelper::RayIntersectsBox(const QR_Ray<T>& ray, const QR_Box<T>& box)
{
    // get infinite value and ray pos and dir
    const T              inf       = std::numeric_limits<T>::infinity();
    const QR_Vector3D<T> rayPos    = ray.GetPos();
    const QR_Vector3D<T> rayDir    = ray.GetDir();
    const QR_Vector3D<T> rayInvDir = ray.GetInvDir();

    // calculate points where ray intersects box
    const T tx1 = (rayInvDir.m_X != inf) ? ((box.m_Min.m_X - rayPos.m_X) * rayInvDir.m_X) :
            ((box.m_Min.m_X - rayPos.m_X) < T(0.0) ? -inf : inf);
    const T tx2 = (rayInvDir.m_X != inf) ? ((box.m_Max.m_X - rayPos.m_X) * rayInvDir.m_X) :
            ((box.m_Max.m_X - rayPos.m_X) < T(0.0) ? -inf : inf);
    const T ty1 = (rayInvDir.m_Y != inf) ? ((box.m_Min.m_Y - rayPos.m_Y) * rayInvDir.m_Y) :
            ((box.m_Min.m_Y - rayPos.m_Y) < T(0.0) ? -inf : inf);
    const T ty2 = (rayInvDir.m_Y != inf) ? ((box.m_Max.m_Y - rayPos.m_Y) * rayInvDir.m_Y) :
            ((box.m_Max.m_Y - rayPos.m_Y) < T(0.0) ? -inf : inf);
    const T tz1 = (rayInvDir.m_Z != inf) ? ((box.m_Min.m_Z - rayPos.m_Z) * rayInvDir.m_Z) :
            ((box.m_Min.m_Z - rayPos.m_Z) < T(0.0) ? -inf : inf);
    const T tz2 = (rayInvDir.m_Z != inf) ? ((box.m_Max.m_Z - rayPos.m_Z) * rayInvDir.m_Z) :
            ((box.m_Max.m_Z - rayPos.m_Z) < T(0.0) ? -inf : inf);

    // calculate near/far intersection on each axis
    const T txn = std::min(tx1, tx2);
    const T txf = std::max(tx1, tx2);
    const T tyn = std::min(ty1, ty2);
    const T tyf = std::max(ty1, ty2);
    const T tzn = std::min(tz1, tz2);
    const T tzf = std::max(tz1, tz2);

    // calculate final near/far intersection point
    const T tnear = std::max(txn, std::max(tyn, tzn));
    const T tfar  = std::min(txf, std::min(tyf, tzf));

    // check if ray intersects box
    return (tfar >= tnear);
}
//---------------------------------------------------------------------------
template <class T>
bool QR_GeometryHelper::RayIntersectsPolygon(const QR_Ray<T>& ray, const QR_Polygon<T>& polygon)
{
    // create a plane using the 3 vertices of the polygon
    const QR_Plane<T> polygonPlane = polygon.GetPlane();

    QR_Vector3D<T> pointOnPlane;

    // calculate the intersection point
    if (!polygonPlane.IntersectRay(ray.GetPos(), ray.GetDir(), pointOnPlane))
        return false;

    // check if calculated point is inside the polygon
    return polygon.Inside(pointOnPlane);
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
