/****************************************************************************
 * ==> QR_FileBuffer -------------------------------------------------------*
 ****************************************************************************
 * Description : File buffer interface                                      *
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

#ifndef QR_FileBufferH
#define QR_FileBufferH

// std
#include <string>

// qr engine
#include "QR_Buffer.h"

/**
* File buffer interface
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_FileBuffer : public QR_Buffer
{
    public:
        /**
        * Constructor
        */
        QR_FileBuffer();

        /**
        * Destructor
        */
        virtual ~QR_FileBuffer();

        /**
        * Buffer read/write mode
        */
        enum IEMode
        {
            IE_M_Unknown = 0,
            IE_M_Read,
            IE_M_Write,
            IE_M_RW,
        };

        /**
        * Opens file in specified mode
        *@param fileName - file name
        *@param mode - opening mode
        *@return true on success, otherwise false
        */
        virtual bool Open(const std::string&  fileName, IEMode mode);
        virtual bool Open(const std::wstring& fileName, IEMode mode);

    protected:
        IEMode m_Mode;

        /**
        * Closes file
        */
        virtual void Close() = 0;
};

#endif
