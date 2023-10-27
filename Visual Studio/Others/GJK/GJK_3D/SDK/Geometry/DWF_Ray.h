/****************************************************************************
 * ==> DWF_Ray -------------------------------------------------------------*
 ****************************************************************************
 * Description: Ray represented by a start point and a direction            *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - DwarfStar Game Engine                                      *
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
#include <limits>

// classes
#include "DWF_Vector3.h"

namespace DWF_Geometry
{
    /**
    * Ray
    *@author Jean-Milost Reymond
    */
    template <class T>
    class Ray
    {
        public:
            /**
            * Constructor
            */
            Ray();

            /**
            * Constructor
            *@param pos - ray position
            *@param dir - ray direction
            */
            Ray(const DWF_Math::Vector3<T>& pos, const DWF_Math::Vector3<T>& dir);

            /**
            * Destructor
            */
            virtual ~Ray();

            /**
            * Gets ray position
            *@return ray position
            */
            virtual inline DWF_Math::Vector3<T> GetPos() const;

            /**
            * Sets ray position
            *@param pos - ray position
            */
            virtual inline void SetPos(const DWF_Math::Vector3<T>& pos);

            /**
            * Gets ray direction
            *@return ray direction
            */
            virtual inline DWF_Math::Vector3<T> GetDir() const;

            /**
            * Sets ray direction
            *@param dir - ray direction
            */
            virtual inline void SetDir(const DWF_Math::Vector3<T>& dir);

            /**
            * Gets ray inverted direction
            *@return ray inverted direction
            */
            virtual inline DWF_Math::Vector3<T> GetInvDir() const;

            /**
            * Gets the closest point on this ray
            *@param point - point from which the closest point should be found
            *@return the closest point on this ray
            */
            virtual inline DWF_Math::Vector3<T> ClosestPoint(const DWF_Math::Vector3<T>& point) const;

            /**
            * Gets shortest distance between a point and this ray
            *@param point - point from which the distance must be calculated
            *@return shortest distance between the point and this ray
            */
            virtual inline T GetShortestDistance(const DWF_Math::Vector3<T>& point) const;

        private:
            DWF_Math::Vector3<T> m_Pos;    // ray position
            DWF_Math::Vector3<T> m_Dir;    // ray direction
            DWF_Math::Vector3<T> m_InvDir; // inverted direction (i.e 1 / dir)
    };

    typedef Ray<float>  RayF;
    typedef Ray<double> RayD;

    //---------------------------------------------------------------------------
    // Ray
    //---------------------------------------------------------------------------
    template <class T>
    Ray<T>::Ray() :
        m_InvDir(DWF_Math::Vector3<T>(std::numeric_limits<T>::infinity(),
                                      std::numeric_limits<T>::infinity(),
                                      std::numeric_limits<T>::infinity()))
    {}
    //---------------------------------------------------------------------------
    template <class T>
    Ray<T>::Ray(const DWF_Math::Vector3<T>& pos, const DWF_Math::Vector3<T>& dir) :
        m_Pos(pos),
        m_Dir(dir),
        m_InvDir(DWF_Math::Vector3<T>((dir.m_X == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_X),
                                      (dir.m_Y == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Y),
                                      (dir.m_Z == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Z)))
    {}
    //---------------------------------------------------------------------------
    template <class T>
    Ray<T>::~Ray()
    {}
    //---------------------------------------------------------------------------
    template <class T>
    DWF_Math::Vector3<T> Ray<T>::GetPos() const
    {
        return m_Pos;
    }
    //---------------------------------------------------------------------------
    template <class T>
    void Ray<T>::SetPos(const DWF_Math::Vector3<T>& pos)
    {
        m_Pos = pos;
    }
    //---------------------------------------------------------------------------
    template <class T>
    DWF_Math::Vector3<T> Ray<T>::GetDir() const
    {
        return m_Dir;
    }
    //---------------------------------------------------------------------------
    template <class T>
    void Ray<T>::SetDir(const DWF_Math::Vector3<T>& dir)
    {
        m_Dir = dir;

        // calculate the invert direction
        m_InvDir = DWF_Math::Vector3<T>((dir.m_X == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_X),
                                        (dir.m_Y == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Y),
                                        (dir.m_Z == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Z));
    }
    //---------------------------------------------------------------------------
    template <class T>
    DWF_Math::Vector3<T> Ray<T>::GetInvDir() const
    {
        return m_InvDir;
    }
    //---------------------------------------------------------------------------
    template <class T>
    inline DWF_Math::Vector3<T> Ray<T>::ClosestPoint(const DWF_Math::Vector3<T>& point) const
    {
        const DWF_Math::Vector3<T> delta = point - m_Pos;

        // calculate the dot product between the point and the ray. NOTE if dp is negative,
        // the closest point is in the opposite direction to ray dir
        const T dp = std::max(delta.Dot(m_Dir), 0.0f);

        // calculate closest point on ray
        return m_Pos + (m_Dir * dp);
    }
    //---------------------------------------------------------------------------
    template <class T>
    T Ray<T>::GetShortestDistance(const DWF_Math::Vector3<T>& point) const
    {
        // get the closest point on line
        const DWF_Math::Vector3<T> closestPointOnLine = ClosestPoint(point);

        // compute distance between point and closest point on line
        const DWF_Math::Vector3<T> d = point - closestPointOnLine;

        // calculate distance
        return (T)sqrt(d.m_X * d.m_X + d.m_Y * d.m_Y + d.m_Z * d.m_Z);
    }
    //---------------------------------------------------------------------------
}
