/****************************************************************************
 * ==> Polygon -------------------------------------------------------------*
 ****************************************************************************
 * Description : This module provides a generic polygon                     *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************/

#ifndef Polygon3DH
#define Polygon3DH

// std
#include <vector>
#include <limits>

// classes
#include "Geometry.h"

/**
* Polygon3D
*@author Jean-Milost Reymond
*/
template <class T>
class Polygon3D
{
    public:
        /**
        * Constructor
        */
        Polygon3D();

        /**
        * Constructor
        *@param vertex1 - first vertex of the polygon
        *@param vertex2 - second vertex of the polygon
        *@param vertex3 - third vertex of the polygon
        */
        Polygon3D(const Vector3D<T>& vertex1,
                  const Vector3D<T>& vertex2,
                  const Vector3D<T>& vertex3);

        /**
        * Destructor
        */
        virtual ~Polygon3D();

        virtual inline Vector3D<T> operator[] (unsigned char index) const;

        /**
        * Gets vertex at index
        *@param index - vertex index
        *@return corresponding vertex, empty vertex if not found
        */
        virtual inline Vector3D<T> GetVertex(unsigned char index) const;

        /**
        * Sets vertex
        *@param index - vertex index to set
        *@param vertex - vertex value
        */
        virtual inline void SetVertex(unsigned char index, const Vector3D<T>& vertex);

        /**
        * Gets first polygon vertex
        *@return first polygon vertex
        */
        virtual inline Vector3D<T> GetVertex1() const;

        /**
        * Sets first polygon vertex
        *@param value - first polygon vertex value
        */
        virtual inline void SetVertex1(const Vector3D<T>& value);

        /**
        * Gets second polygon vertex
        *@return second polygon vertex
        */
        virtual inline Vector3D<T> GetVertex2() const;

        /**
        * Sets second polygon vertex
        *@param value - second polygon vertex value
        */
        virtual inline void SetVertex2(const Vector3D<T>& value);

        /**
        * Gets third polygon vertex
        *@return third polygon vertex
        */
        virtual inline Vector3D<T> GetVertex3() const;

        /**
        * Sets third polygon vertex
        *@param value - third polygon vertex value
        */
        virtual inline void SetVertex3(const Vector3D<T>& value);

        /**
        * Creates and returns a clone of the polygon
        *@return a clone of the polygon
        *@note The returned polygon should be deleted when useless
        */
        virtual inline Polygon3D* GetClone() const;

        /**
        * Gets the polygon plane
        *@return the polygon plane
        */
        virtual inline Plane<T> GetPlane() const;

        /**
        * Calculates and returns the center point of the polygon
        *@return the center point of the polygon
        */
        virtual inline Vector3D<T> GetCenter() const;

        /**
        * Checks if a point is inside the polygon
        *@param point - point coordinate
        *@param tolerance - tolerance for calculation
        *@return true if point is inside the polygon, otherwise false
        */
        virtual inline bool Inside(const Vector3D<T>& point, T tolerance) const;

        /**
        * Checks if a ray intersects the polygon
        *@param ray - ray to check
        *@return true if the ray intersects the polygon, otherwise false
        */
        virtual inline bool Intersect(const Ray<T>& ray) const;

        /**
        * Applies the given matrix to the polygon
        *@param matrix - matrix to apply
        *@return transformed polygon
        *@note The returned polygon should be deleted when useless
        */
        virtual inline Polygon3D* ApplyMatrix(const Matrix4x4<T>& matrix) const;

    private:
        Vector3D<T> m_Vertex[3];
};

typedef Polygon3D<float>  Polygon3DF;
typedef Polygon3D<double> Polygon3DD;

/**
* Polygon3D list
*@author Jean-Milost Reymond
*/
typedef std::vector<Polygon3DF*> Polygon3DsF;
typedef std::vector<Polygon3DD*> Polygon3DsD;

//---------------------------------------------------------------------------
// Polygon3D
//---------------------------------------------------------------------------
template <class T>
Polygon3D<T>::Polygon3D()
{}
//---------------------------------------------------------------------------
template <class T>
Polygon3D<T>::Polygon3D(const Vector3D<T>& vertex1,
                        const Vector3D<T>& vertex2,
                        const Vector3D<T>& vertex3)
{
    m_Vertex[0] = vertex1;
    m_Vertex[1] = vertex2;
    m_Vertex[2] = vertex3;
}
//---------------------------------------------------------------------------
template <class T>
Polygon3D<T>::~Polygon3D()
{}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Polygon3D<T>::operator[] (unsigned char index) const
{
    return GetVertex(index);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Polygon3D<T>::GetVertex(unsigned char index) const
{
    // search for index to get
    switch (index)
    {
        case 0:
        case 1:
        case 2:  return m_Vertex[index];
        default: return Vector3D<T>();
    }
}
//---------------------------------------------------------------------------
template <class T>
void Polygon3D<T>::SetVertex(unsigned char index, const Vector3D<T>& vertex)
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
Vector3D<T> Polygon3D<T>::GetVertex1() const
{
    return m_Vertex[0];
}
//---------------------------------------------------------------------------
template <class T>
void Polygon3D<T>::SetVertex1(const Vector3D<T>& value)
{
    m_Vertex[0] = value;
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Polygon3D<T>::GetVertex2() const
{
    return m_Vertex[1];
}
//---------------------------------------------------------------------------
template <class T>
void Polygon3D<T>::SetVertex2(const Vector3D<T>& value)
{
    m_Vertex[1] = value;
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Polygon3D<T>::GetVertex3() const
{
    return m_Vertex[2];
}
//---------------------------------------------------------------------------
template <class T>
void Polygon3D<T>::SetVertex3(const Vector3D<T>& value)
{
    m_Vertex[2] = value;
}
//---------------------------------------------------------------------------
template <class T>
Polygon3D<T>* Polygon3D<T>::GetClone() const
{
    // copy the polygon, then return the copy
    return new Polygon3D<T>(m_Vertex[0], m_Vertex[1], m_Vertex[2]);
}
//---------------------------------------------------------------------------
template <class T>
Plane<T> Polygon3D<T>::GetPlane() const
{
    // calculate the plane from the values of the 3 vertices of the polygon
    return Plane<T>::FromPoints(m_Vertex[0], m_Vertex[1], m_Vertex[2]);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Polygon3D<T>::GetCenter() const
{
    // calculate then return the value of the midpoint of the polygon
    return Vector3D<T>(((m_Vertex[0].m_X + m_Vertex[1].m_X + m_Vertex[2].m_X) / T(3.0)),
                       ((m_Vertex[0].m_Y + m_Vertex[1].m_Y + m_Vertex[2].m_Y) / T(3.0)),
                       ((m_Vertex[0].m_Z + m_Vertex[1].m_Z + m_Vertex[2].m_Z) / T(3.0)));
}
//---------------------------------------------------------------------------
template <class T>
bool Polygon3D<T>::Inside(const Vector3D<T>& point, T tolerance) const
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
    const Vector3D<T> nPToV1 = (GetVertex1() - point).Normalize();
    const Vector3D<T> nPToV2 = (GetVertex2() - point).Normalize();
    const Vector3D<T> nPToV3 = (GetVertex3() - point).Normalize();

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
    return (angleResult >= T(6.28) - tolerance);
}
//---------------------------------------------------------------------------
template <class T>
bool Polygon3D<T>::Intersect(const Ray<T>& ray) const
{
    // create a plane using the 3 vertices of the polygon
    const Plane<T> polygonPlane = GetPlane();

    Vector3D<T> pointOnPlane;

    // check if the ray intersects the plane and calculate the intersection point
    if (!polygonPlane.Intersect(ray, pointOnPlane))
        return false;

    // check if calculated point on plane is inside the polygon
    return Inside(pointOnPlane, T(0.0));
}
//---------------------------------------------------------------------------
template <class T>
Polygon3D<T>* Polygon3D<T>::ApplyMatrix(const Matrix4x4<T>& matrix) const
{
    // build a new polygon transforming all vertices of the polygon using
    // given matrix, and return new built polygon
    return new Polygon3D<T>(matrix.Transform(m_Vertex[0]),
                            matrix.Transform(m_Vertex[1]),
                            matrix.Transform(m_Vertex[2]));
}
//---------------------------------------------------------------------------

#endif
