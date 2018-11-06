/****************************************************************************
 * ==> QR_MathTools --------------------------------------------------------*
 ****************************************************************************
 * Description : Some c++ mathematical tools                                *
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

#ifndef QR_MathToolsH
#define QR_MathToolsH

// std
#include <cmath>

// qr engine
#include "QR_Types.h"

/**
* Some c++ mathematical tools
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_MathTools
{
    public:
        QR_MathTools();
        virtual ~QR_MathTools();

        /**
        * Compares 2 floating point values and checks if they are equal or nearest
        *@param a - first value to compare
        *@param b - second value to compare
        *@param epsilon - nearest tolerance
        *@return true if values are equal or nearest, otherwise false
        */
        template <class T>
        static inline T EqualOrNearest(T a, T b, T epsilon = M_Epsilon);

        /**
        * Gets PI value
        *@return PI value
        */
        template <class T>
        static inline T GetPI();

        /**
        * Convert angle in degrees to angle in radians
        *@param angle - angle in degrees to convert
        *@return converted angle in radians
        */
        template <class T>
        static inline T DegToRad(T angle);

        /**
        * Convert angle in radians to angle in degrees
        *@param angle - angle in radians to convert
        *@return converted angle in degrees
        */
        template <class T>
        static inline T RadToDeg(T angle);

        /**
        * Checks if a value is a power of 2
        *@param x - value to check
        *@return true if value is a power of two, otherwise false
        */
        static bool IsPowerOfTwo(std::size_t value);

        /**
        * Rounds up to the nearest power of 2
        *@param value - value to round up
        *@return rounded up power of 2
        */
        static inline std::size_t RoundUpToNearestPowerOf2(std::size_t value);

        /**
        * Gets the closest power of 2 from a value
        *@param value - value
        *@return closest power of 2
        */
        static inline std::size_t GetClosestPowerOf2(std::size_t value);
};

//---------------------------------------------------------------------------
// QR_MathTools
//---------------------------------------------------------------------------
template <class T>
T QR_MathTools::EqualOrNearest(T a, T b, T epsilon)
{
    return (std::fabs(a - b) < epsilon);
}
//---------------------------------------------------------------------------

template <class T>
T QR_MathTools::GetPI()
{
    return std::atan(T(1.0)) * T(4.0);
}
//---------------------------------------------------------------------------
template <class T>
T QR_MathTools::DegToRad(T angle)
{
    // convert degrees to radians and return value
    return ((angle * (T(2.0) * M_PI)) / T(360.0));
}
//---------------------------------------------------------------------------
template <class T>
T QR_MathTools::RadToDeg(T angle)
{
    // convert radians to degrees and return value
    return ((angle * T(360.0)) / (T(2.0) * M_PI));
}
//---------------------------------------------------------------------------

std::size_t QR_MathTools::RoundUpToNearestPowerOf2(std::size_t value)
{
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    ++value;

    return value;
}
//---------------------------------------------------------------------------
std::size_t QR_MathTools::GetClosestPowerOf2(std::size_t value)
{
    std::size_t pos = 0;

    while (value > 0)
    {
        ++pos;
        value = value >> 1;
    }

    return std::pow(2.0, double(pos));
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
