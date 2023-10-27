/****************************************************************************
 * ==> DWF_Material --------------------------------------------------------*
 ****************************************************************************
 * Description : Material descriptor                                        *
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
#include "DWF_Color.h"
#include "DWF_Texture.h"

namespace DWF_Model
{
    /**
    * Material, describes the way a mesh reacts to its environment
    *@author Jean-Milost Reymond
    */
    class Material
    {
        public:
            Texture* m_pTexture;    // texture to apply to vertex buffer
            ColorF   m_Color;       // vertex color, applied to all vertices if per-vertex color is disabled
            bool     m_Transparent; // whether or not the alpha blending should be activated
            bool     m_Wireframe;   // whether or not the vertex buffer should be drawn in wireframe

            Material();
            virtual ~Material();
    };
}
