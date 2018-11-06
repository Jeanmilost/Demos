/****************************************************************************
 * ==> QR_Ray --------------------------------------------------------------*
 ****************************************************************************
 * Description : 3D ray                                                     *
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

#ifndef QR_RayH
#define QR_RayH

// std
#include <limits>

// qr engine
#include "QR_Types.h"
#include "QR_Vector3D.h"

/**
* Euclidean ray
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
template <class T>
class QR_Ray
{
    public:
        /**
        * Constructor
        */
        QR_Ray();

        /**
        * Constructor
        *@param pos - ray position
        *@param dir - ray direction
        */
        QR_Ray(const QR_Vector3D<T>& pos, const QR_Vector3D<T>& dir);

        /**
        * Destructor
        */
        virtual ~QR_Ray();

        /**
        * Gets ray position
        *@return ray position
        */
        virtual inline QR_Vector3D<T> GetPos() const;

        /**
        * Sets ray position
        *@param pos - ray position
        */
        virtual inline void SetPos(const QR_Vector3D<T>& pos);

        /**
        * Gets ray direction
        *@return ray direction
        */
        virtual inline QR_Vector3D<T> GetDir() const;

        /**
        * Sets ray direction
        *@param dir - ray direction
        */
        virtual inline void SetDir(const QR_Vector3D<T>& dir);

        /**
        * Gets ray inverted direction
        *@return ray inverted direction
        */
        virtual inline QR_Vector3D<T> GetInvDir() const;

    private:
        QR_Vector3D<T> m_Pos;    // ray position
        QR_Vector3D<T> m_Dir;    // ray direction
        QR_Vector3D<T> m_InvDir; // inverted direction (i.e. 1 / dir)
};

// formatted ray using global precision
typedef QR_Ray<M_Precision> QR_RayP;

//---------------------------------------------------------------------------
// QR_Ray
//---------------------------------------------------------------------------
template <class T>
QR_Ray<T>::QR_Ray() :
    m_InvDir(QR_Vector3D<T>(std::numeric_limits<T>::infinity(),
                            std::numeric_limits<T>::infinity(),
                            std::numeric_limits<T>::infinity()))
{}
//---------------------------------------------------------------------------
template <class T>
QR_Ray<T>::QR_Ray(const QR_Vector3D<T>& pos, const QR_Vector3D<T>& dir) :
    m_Pos(pos),
    m_Dir(dir),
    m_InvDir(QR_Vector3D<T>((dir.m_X == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_X),
                            (dir.m_Y == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Y),
                            (dir.m_Z == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Z)))
{}
//---------------------------------------------------------------------------
template <class T>
QR_Ray<T>::~QR_Ray()
{}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Ray<T>::GetPos() const
{
    return m_Pos;
}
//---------------------------------------------------------------------------
template <class T>
void QR_Ray<T>::SetPos(const QR_Vector3D<T>& pos)
{
    m_Pos = pos;
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Ray<T>::GetDir() const
{
    return m_Dir;
}
//---------------------------------------------------------------------------
template <class T>
void QR_Ray<T>::SetDir(const QR_Vector3D<T>& dir)
{
    m_Dir = dir;

    // calculate the invert direction
    m_InvDir = QR_Vector3D<T>((dir.m_X == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_X),
                              (dir.m_Y == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Y),
                              (dir.m_Z == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Z));
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Ray<T>::GetInvDir() const
{
    return m_InvDir;
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
