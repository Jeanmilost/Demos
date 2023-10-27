/****************************************************************************
 * ==> DWF_MathHelper ------------------------------------------------------*
 ****************************************************************************
 * Description: Helper class for math                                       *
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
#include <algorithm>

// classes
#include "DWF_Vector3.h"

// epsilon value used for tolerance
#define M_Epsilon 1.0E-3

namespace DWF_Math
{
    /**
    * Helper class for math
    *@author Jean-Milost Reymond
    */
    class Helper
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
            static inline T Clamp(T value, T rangeMin, T rangeMax);

            /**
            * Clamps a value between 0 and 1
            *@param value - value to clamp
            *@return clamped value
            */
            template <class T>
            static inline T Clamp01(T value);

            /**
            * Checks if a vector is between start and end limits
            *@param value - value to check
            *@param start - start limit
            *@param end - end limit
            *@param tolerance - tolerance
            *@return true if value is between limits, otherwise false
            */
            template <class T>
            static bool IsBetween(const DWF_Math::Vector3<T>& value,
                                  const DWF_Math::Vector3<T>& start,
                                  const DWF_Math::Vector3<T>& end,
                                        float                 tolerance);

            /**
            * Checks if a value is between a range defined by start and end
            *@param value - value to check
            *@param start - range start value
            *@param end - range end value
            *@param tolerance - tolerance
            *@return true if value is between the range, otherwise false
            */
            template <class T>
            static inline bool IsBetween(T value, T start, T end, T tolerance);
    };

    //---------------------------------------------------------------------------
    // Helper
    //---------------------------------------------------------------------------
    template <class T>
    static inline T Helper::Clamp(T value, T rangeMin, T rangeMax)
    {
        return std::min(std::max(value, rangeMin), rangeMax);
    }
    //---------------------------------------------------------------------------
    template <class T>
    static inline T Helper::Clamp01(T value)
    {
        return Clamp(value, T(0.0), T(1.0));
    }
    //---------------------------------------------------------------------------
    template <class T>
    static bool Helper::IsBetween(const DWF_Math::Vector3<T>& value,
                                  const DWF_Math::Vector3<T>& start,
                                  const DWF_Math::Vector3<T>& end,
                                        float                 tolerance)
    {
        // check if each vector component is between start and end limits
        return (IsBetween(value.m_X, start.m_X, end.m_X, tolerance) &&
                IsBetween(value.m_Y, start.m_Y, end.m_Y, tolerance) &&
                IsBetween(value.m_Z, start.m_Z, end.m_Z, tolerance));
    }
    //---------------------------------------------------------------------------
    template <class T>
    static inline bool Helper::IsBetween(T value, T start, T end, T tolerance)
    {
        // check if each value is between start and end limits considering tolerance
        return (value >= std::min(start, end) - tolerance &&
                value <= std::max(start, end) + tolerance);
    }
    //---------------------------------------------------------------------------
}
