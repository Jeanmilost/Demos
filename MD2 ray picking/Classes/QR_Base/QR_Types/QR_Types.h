/****************************************************************************
 * ==> QR_Types ------------------------------------------------------------*
 ****************************************************************************
 * Description : QR engine basic types                                      *
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

#ifndef QR_TypesH
#define QR_TypesH

// std
#include <cstddef>
#include <stdint.h>
#include <vector>

//---------------------------------------------------------------------------
// Global macros
//---------------------------------------------------------------------------
#define M_Precision QR_Float // real numbers precision, can be e.g. float or double
#define M_Epsilon   1.0E-3   // epsilon value used for tolerance
//---------------------------------------------------------------------------

// used cross-platform types
typedef bool               QR_Bool;
typedef float              QR_Float;
typedef double             QR_Double;
typedef std::size_t        QR_SizeT;
#if defined(CP_EMBARCADERO)
    typedef std::intptr_t  QR_IntPtrT;
    typedef std::uintptr_t QR_UIntPtrT;
#else
    typedef intptr_t       QR_IntPtrT;
    typedef uintptr_t      QR_UIntPtrT;
#endif

// c++98 dependent types
typedef char               QR_Int8;
typedef unsigned char      QR_UInt8;
typedef short              QR_Int16;
typedef unsigned short     QR_UInt16;
typedef int                QR_Int32;
typedef unsigned           QR_UInt32;
typedef long long          QR_Int64;
typedef unsigned long long QR_UInt64;

// used cross-platform types for texts
typedef char    QR_Char;
typedef wchar_t QR_WChar;

// used cross-platform types for buffers
typedef QR_UIntPtrT QR_BufferSizeType;
typedef QR_IntPtrT  QR_BufferOffsetType;
typedef QR_UInt8    QR_BufferDataType;

// used cross-platform types for GUID (when based on pointer system)
typedef QR_UIntPtrT              QR_GUIDType;
typedef std::vector<QR_GUIDType> QR_GUIDList;

#endif
