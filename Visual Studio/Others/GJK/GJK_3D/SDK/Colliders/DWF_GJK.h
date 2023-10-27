/****************************************************************************
 * ==> DWF_GJK -------------------------------------------------------------*
 ****************************************************************************
 * Description : Gilbert-Johnson-Keerthi intersection algorithm             *
 *               implementation                                             *
 * Developer   : Jean-Milost Reymond                                        *
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

// classes
#include "DWF_Vector3.h"
#include "DWF_Collider.h"

namespace DWF_Collider
{
    /**
    * Gilbert-Johnson-Keerthi intersection algorithm implementation
    *@note based on the following project: https://github.com/kevinmoran/GJK
    *@author Jean-Milost Reymond
    */
    class GJK
    {
        public:
            GJK();
            virtual ~GJK();

            static bool Resolve(const Collider& c1, const Collider& c2, DWF_Math::Vector3F& mtv);

        private:
            static void UpdateSimplex3(DWF_Math::Vector3F& a,
                                       DWF_Math::Vector3F& b,
                                       DWF_Math::Vector3F& c,
                                       DWF_Math::Vector3F& d,
                                       std::size_t&        simpDim,
                                       DWF_Math::Vector3F& searchDir);

            static bool UpdateSimplex4(DWF_Math::Vector3F& a,
                                       DWF_Math::Vector3F& b,
                                       DWF_Math::Vector3F& c,
                                       DWF_Math::Vector3F& d,
                                       std::size_t&        simpDim,
                                       DWF_Math::Vector3F& searchDir);

            /**
            * Find minimum translation vector to resolve collision (based on Expanding Polytope Algorithm)
            */
            static DWF_Math::Vector3F FindMinTranslationVec(const DWF_Math::Vector3F& a,
                                                            const DWF_Math::Vector3F& b,
                                                            const DWF_Math::Vector3F& c,
                                                            const DWF_Math::Vector3F& d,
                                                            const Collider&           c1,
                                                            const Collider&           c2);
    };
}
