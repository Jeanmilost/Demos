/****************************************************************************
 * ==> MathHelper ----------------------------------------------------------*
 ****************************************************************************
 * Description: Helper class for math                                       *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - Capsule collision demo                                     *
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
#include <algorithm>

// epsilon value used for tolerance
#define M_Epsilon 1.0E-3

/**
* Helper class for math
*@author Jean-Milost Reymond
*/
class MathHelper
{
    public:
        /**
        * Clamps a value
        *@param value - value to clamp
        *@param rangeMin - range minimum value
        *@param rangeMax - range maximum value
        *@return clamped value
        */
        template <class T>
        static T Clamp(T value, T rangeMin, T rangeMax);

        /**
        * Clamps a value between 0 and 1
        *@param value - value to clamp
        *@return clamped value
        */
        template <class T>
        static T Clamp01(T value);

        /**
        * Checks if a value is between a range defined by start and end
        *@param value - value to check
        *@param start - range start value
        *@param end - range end value
        *@param tolerance - tolerance
        *@return true if value is between the range, otherwise false
        */
        template <class T>
        static bool IsBetween(T value, T start, T end, T tolerance);
};

//---------------------------------------------------------------------------
// MathHelper
//---------------------------------------------------------------------------
template <class T>
static T MathHelper::Clamp(T value, T rangeMin, T rangeMax)
{
    return std::min(std::max(value, rangeMin), rangeMax);
}
//---------------------------------------------------------------------------
template <class T>
static T MathHelper::Clamp01(T value)
{
    return Clamp(value, T(0.0), T(1.0));
}
//---------------------------------------------------------------------------
template <class T>
static bool MathHelper::IsBetween(T value, T start, T end, T tolerance)
{
    // check if each value is between start and end limits considering tolerance
    return (value >= std::min(start, end) - tolerance &&
            value <= std::max(start, end) + tolerance);
}
//---------------------------------------------------------------------------
