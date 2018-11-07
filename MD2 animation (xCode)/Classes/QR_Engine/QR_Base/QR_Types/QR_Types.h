/******************************************************************************
 * ==> QR_Types --------------------------------------------------------------*
 ******************************************************************************
 * Description : QR engine basic types                                        *
 * Developer   : Jean-Milost Reymond                                          *
 ******************************************************************************/

#ifndef QR_TypesH
#define QR_TypesH

// std
#include <cstddef>
#include <stdint.h>
#include <vector>

//------------------------------------------------------------------------------
// Global macros
//------------------------------------------------------------------------------
#define M_Precision QR_Float // real numbers precision, can be e.g. float or double
#define M_Epsilon   1.0E-3   // epsilon value used for tolerance
//------------------------------------------------------------------------------

// used cross-platform types
typedef bool               QR_Bool;
typedef float              QR_Float;
typedef double             QR_Double;
typedef std::size_t        QR_SizeT;
#ifdef __CODEGEARC__
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

#endif // QR_TypesH
