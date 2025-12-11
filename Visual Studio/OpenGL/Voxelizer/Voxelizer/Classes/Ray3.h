/****************************************************************************
 * ==> Ray3 ----------------------------------------------------------------*
 ****************************************************************************
 * Description: Ray represented by a start point and a direction            *
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
#include <limits>

// classes
#include "Vector3.h"

/**
* Ray
*@author Jean-Milost Reymond
*/
template <class T>
class Ray3
{
    public:
        /**
        * Constructor
        */
        Ray3();

        /**
        * Constructor
        *@param pos - ray position
        *@param dir - ray direction
        */
        Ray3(const Vector3<T>& pos, const Vector3<T>& dir);

        /**
        * Destructor
        */
        virtual ~Ray3();

        /**
        * Gets ray position
        *@return ray position
        */
        virtual inline Vector3<T> GetPos() const;

        /**
        * Sets ray position
        *@param pos - ray position
        */
        virtual inline void SetPos(const Vector3<T>& pos);

        /**
        * Gets ray direction
        *@return ray direction
        */
        virtual inline Vector3<T> GetDir() const;

        /**
        * Sets ray direction
        *@param dir - ray direction
        */
        virtual inline void SetDir(const Vector3<T>& dir);

        /**
        * Gets ray inverted direction
        *@return ray inverted direction
        */
        virtual inline Vector3<T> GetInvDir() const;

    private:
        Vector3<T> m_Pos;    // ray position
        Vector3<T> m_Dir;    // ray direction
        Vector3<T> m_InvDir; // inverted direction (i.e 1 / dir)
};

typedef Ray3<float>  Ray3F;
typedef Ray3<double> Ray3D;

//---------------------------------------------------------------------------
// Ray3
//---------------------------------------------------------------------------
template <class T>
Ray3<T>::Ray3() :
    m_InvDir(Vector3<T>(std::numeric_limits<T>::infinity(),
                        std::numeric_limits<T>::infinity(),
                        std::numeric_limits<T>::infinity()))
{}
//---------------------------------------------------------------------------
template <class T>
Ray3<T>::Ray3(const Vector3<T>& pos, const Vector3<T>& dir) :
    m_Pos(pos),
    m_Dir(dir),
    m_InvDir(Vector3<T>((dir.m_X == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_X),
                        (dir.m_Y == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Y),
                        (dir.m_Z == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Z)))
{}
//---------------------------------------------------------------------------
template <class T>
Ray3<T>::~Ray3()
{}
//---------------------------------------------------------------------------
template <class T>
Vector3<T> Ray3<T>::GetPos() const
{
    return m_Pos;
}
//---------------------------------------------------------------------------
template <class T>
void Ray3<T>::SetPos(const Vector3<T>& pos)
{
    m_Pos = pos;
}
//---------------------------------------------------------------------------
template <class T>
Vector3<T> Ray3<T>::GetDir() const
{
    return m_Dir;
}
//---------------------------------------------------------------------------
template <class T>
void Ray3<T>::SetDir(const Vector3<T>& dir)
{
    m_Dir = dir;

    // calculate the invert direction
    m_InvDir = Vector3<T>((dir.m_X == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_X),
                          (dir.m_Y == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Y),
                          (dir.m_Z == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Z));
}
//---------------------------------------------------------------------------
template <class T>
Vector3<T> Ray3<T>::GetInvDir() const
{
    return m_InvDir;
}
//---------------------------------------------------------------------------
