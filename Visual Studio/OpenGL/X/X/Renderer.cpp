/****************************************************************************
 * ==> Renderer ------------------------------------------------------------*
 ****************************************************************************
 * Description: Generic renderer                                            *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - x model reader                                             *
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

#include "Renderer.h"

//---------------------------------------------------------------------------
// Renderer
//---------------------------------------------------------------------------
Renderer::Renderer()
{}
//---------------------------------------------------------------------------
Renderer::~Renderer()
{}
//---------------------------------------------------------------------------
bool Renderer::SetPixelFormat(HDC hDC) const
{
    // no device context?
    if (!hDC)
        return false;

    ::PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(::PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,
        0,
    };

    // get best available pixel format
    const int pixelFormat = ::ChoosePixelFormat(hDC, &pfd);

    // set pixel format to use
    return ::SetPixelFormat(hDC, pixelFormat, &pfd);
}
//---------------------------------------------------------------------------
