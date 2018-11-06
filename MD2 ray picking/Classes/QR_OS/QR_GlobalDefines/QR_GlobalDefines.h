/****************************************************************************
 * ==> QR_GlobalDefines ----------------------------------------------------*
 ****************************************************************************
 * Description : Defines the global switches and preprocessor macros to use *
 *               to compile the QR Engine on several systems                *
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

#ifndef QR_GlobalDefinesH
#define QR_GlobalDefinesH

// determine target system
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
    #define OS_WIN
#elif defined(__APPLE__)
    #define OS_OSX
#else
    // target system is unknown, may compile but should be defined explicitly
    #warning "Unknown target system"
#endif

// determine target compiler
#if defined(__BORLANDC__) || defined(__CODEGEARC__)
    #define CP_EMBARCADERO
#endif

//---------------------------------------------------------------------------
// global switches
//---------------------------------------------------------------------------

/**
* if enabled, indicates that OpenGL 1.x is used
*/
//#define USE_OPENGL_DIRECT_MODE

/**
* if enabled, indicates that shader are used
*/
//#define USE_SHADER

//---------------------------------------------------------------------------

#endif
