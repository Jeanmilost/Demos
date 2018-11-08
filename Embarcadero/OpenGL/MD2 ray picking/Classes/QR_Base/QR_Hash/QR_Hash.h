/****************************************************************************
 * ==> QR_Hash -------------------------------------------------------------*
 ****************************************************************************
 * Description : Provides several common hash algorithms                    *
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

#ifndef QR_HashH
#define QR_HashH

// qr engine
#include "QR_Types.h"
#include "QR_Buffer.h"

/**
* Several common hash algorithms
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Hash
{
    public:
        /**
        * Generates a hash code based on the input buffer
        *@param pData - data on which the hash will be calculated
        *@param length - data length
        *@param startVal - hash initial value
        *@return hash code
        */
        static QR_UInt32 Hash(const QR_UInt8* pData,
                                    QR_SizeT  length,
                                    QR_UInt32 startVal);

        /**
        * Generates a hash code based on the input buffer
        *@param pBuffer - input buffer to hash
        *@param length - length of data to hash in buffer
        *@param startVal - hash initial value
        *@return hash code
        */
        static QR_UInt32 Hash(      QR_Buffer&            buffer,
                              const QR_Buffer::ISizeType& length,
                                    QR_UInt32             startVal);

        /**
        * Generates a hash code based on the Bob Jenkins One At A Time algorithm
        *@param pData - data on which the hash will be calculated
        *@param length - data length
        *@param startVal - hash initial value
        *@return hash code
        */
        static QR_UInt32 BobJenkins_OneAtATime(const QR_UInt8* pData,
                                                     QR_SizeT  length,
                                                     QR_UInt32 startVal);

        /**
        * Generates a hash code based on the Bob Jenkins One At A Time algorithm
        *@param pBuffer - input buffer to hash
        *@param length - length of data to hash in buffer
        *@param startVal - hash initial value
        *@return hash code
        */
        static QR_UInt32 BobJenkins_OneAtATime(      QR_Buffer&            buffer,
                                               const QR_Buffer::ISizeType& length,
                                                     QR_UInt32             startVal);

        /**
        * Generates a hash based on the CRC32 algorithm
        *@param pBuffer - input buffer to hash
        *@param length - length of data to hash
        *@param crc - initial CRC value
        *@return hash code
        *@note This code doesn't use lookup table so it can be slow
        */
        static QR_UInt32 CRC32_NoTable(const QR_UInt8* pBuffer, QR_SizeT length, QR_UInt32 crc);

        /**
        * Generates a hash based on the CRC32 algorithm
        *@param pBuffer - input buffer to hash
        *@param length - length of data to hash
        *@return hash code
        *@param crc - initial CRC value
        *@note This code doesn't use lookup table so it can be slow
        */
        static QR_UInt32 CRC32(const QR_UInt8* pBuffer, QR_SizeT length, QR_UInt32 crc);

        /**
        * Generates a hash based on the CRC32 algorithm
        *@param pBuffer - input buffer to hash
        *@param length - length of data to hash in buffer
        *@return hash code
        *@param crc - initial CRC value
        *@note This code doesn't use lookup table so it can be slow
        */
        static QR_UInt32 CRC32(QR_Buffer& buffer, const QR_Buffer::ISizeType& length, QR_UInt32 crc);
};

#endif
