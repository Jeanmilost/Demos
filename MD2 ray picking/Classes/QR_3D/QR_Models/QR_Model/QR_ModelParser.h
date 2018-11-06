/****************************************************************************
 * ==> QR_ModelParser ------------------------------------------------------*
 ****************************************************************************
 * Description : Basic model parser interface                               *
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

#ifndef QR_ModelParserH
#define QR_ModelParserH

// qr engine
#include "QR_Types.h"
#include "QR_Hash.h"
#include "QR_Buffer.h"

/**
* Basic model parser interface
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_ModelParser
{
    public:
        QR_ModelParser();
        virtual ~QR_ModelParser();

        /**
        * Clears model parser
        */
        virtual void Clear();

        /**
        * Copies content from another model parser
        *@param other - other model parser to copy from
        */
        virtual void Copy(const QR_ModelParser& other);

        /**
        * Loads model from file
        *@param fileName - file name
        *@return true on success, otherwise false
        */
        virtual bool Load(const std::string&  fileName);
        virtual bool Load(const std::wstring& fileName);

        /**
        * Loads model from buffer
        *@param buffer - buffer
        *@param length - length to read in buffer, in bytes
        *@return true on success, otherwise false
        *@note Read will begin from current offset
        */
        virtual bool Load(      QR_Buffer&            buffer,
                          const QR_Buffer::ISizeType& length) = 0;

        /**
        * Gets the model hash code
        *@returns hash code
        */
        virtual inline QR_UInt32 GetHashCode() const;

    protected:
        /**
        * Generates the model hash code
        *@param pBuffer - input buffer to hash
        *@param length - length of data to hash in buffer
        *@param startVal - hash initial value
        *@returns hash code
        */
        virtual inline void GenerateHashCode(      QR_Buffer&            buffer,
                                             const QR_Buffer::ISizeType& length,
                                                   QR_UInt32             startVal);

    private:
        QR_UInt32 m_HashCode;
};

//---------------------------------------------------------------------------
// QR_ModelParser
//---------------------------------------------------------------------------
QR_UInt32 QR_ModelParser::GetHashCode() const
{
    return m_HashCode;
}
//---------------------------------------------------------------------------
void QR_ModelParser::GenerateHashCode(      QR_Buffer&            buffer,
                                      const QR_Buffer::ISizeType& length,
                                            QR_UInt32             startVal)
{
    m_HashCode = QR_Hash::CRC32(buffer, length, startVal);
}
//---------------------------------------------------------------------------

#endif
