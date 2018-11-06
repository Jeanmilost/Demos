/****************************************************************************
 * ==> QR_MemoryTools ------------------------------------------------------*
 * **************************************************************************
 * Description : Some memory tools                                          *
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

#ifndef QR_MemoryToolsH
#define QR_MemoryToolsH

// qr engine
#include "QR_Types.h"

//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#define M_ArraySize(array) (sizeof(array) / sizeof((array)[0]))
//---------------------------------------------------------------------------

/**
* Some memory tools
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_MemoryTools
{
    public:
        /**
        * Buffer comparison type
        */
        enum IEBufferCompType
        {
            IE_BC_FromStartToEnd,
            IE_BC_FromEndToStart,
            IE_BC_Symmetric,
        };

        /**
        * Buffer description for comparison functions
        */
        struct IBuffer
        {
            const QR_BufferDataType* m_pBuffer;
                  QR_BufferSizeType  m_Length;

            IBuffer();
            virtual ~IBuffer();
        };

        QR_MemoryTools();
        virtual ~QR_MemoryTools();

        /**
        * Checks if system on which program is executed is big endian
        *@return true if system on which program is executed is big endian, false if little endian
        */
        static inline bool IsSystemBE();

        /**
        * Binary compares buffers content
        *@param buffer1 - first buffer to compare
        *@param buffer2 - second buffer to compare
        *@param type - comparison type
        *@param blockLength - length of memory block to compare on each cycle
        *@return true if buffers are identical, otherwise false
        *@throw Exception on error
        */
        static bool CompareBuffers(const IBuffer&         buffer1,
                                   const IBuffer&         buffer2,
                                         IEBufferCompType type,
                                         QR_SizeT         blockLength);
};

//---------------------------------------------------------------------------
// QR_MemoryTools
//---------------------------------------------------------------------------
bool QR_MemoryTools::IsSystemBE()
{
    union
    {
        QR_UInt32 i;
        QR_Int8   c[4];
    } bInt = {0x01020304};

    return (bInt.c[0] == 1);
}
//---------------------------------------------------------------------------

#endif
