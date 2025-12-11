/****************************************************************************
 * ==> Material ------------------------------------------------------------*
 ****************************************************************************
 * Description: Material                                                    *
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

// classes
#include "Color.h"
#include "Texture.h"

/**
* Material, describes the way a vertex buffer draws its content
*@author Jean-Milost Reymond
*/
class Material
{
    public:
        ColorF   m_Color       = ColorF(1.0f, 1.0f, 1.0f, 1.0f); // vertex color, applied to all vertices if per-vertex color is disabled
        Texture* m_pTexture    = nullptr;                        // texture to apply to vertex buffer
        bool     m_Transparent = false;                          // whether or not the alpha blending should be activated
        bool     m_Wireframe   = false;                          // whether or not the vertex buffer should be drawn in wire frame

        Material();
        virtual ~Material();

        /**
        * Copy operator
        *@param other - other material to copy from
        *@return this material
        */
        virtual Material& operator = (const Material& other);
};
