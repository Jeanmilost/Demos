/****************************************************************************
 * ==> QR_Script -----------------------------------------------------------*
 ****************************************************************************
 * Description : Provides tools to read and write generic scripts           *
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

#ifndef QR_ScriptH
#define QR_ScriptH

// std
#include <vector>
#include <map>

// qr engine
#include "QR_Buffer.h"

/**
* Base class that provides tools to read and parse generic scripts
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Script
{
    public:
        QR_Script();
        virtual ~QR_Script();

        /**
        * Clears script
        */
        virtual void Clear() = 0;

        /**
        * Loads script from file
        *@param fileName - script file name
        *@return true on success, otherwise false
        */
        virtual bool Load(const std::string&  fileName);
        virtual bool Load(const std::wstring& fileName);

        /**
        * Loads script from buffer
        *@param buffer - buffer containing script file to read
        *@param dataLength - buffer length to read
        *@return true on success, otherwise false
        */
        virtual bool Load(QR_Buffer& buffer, QR_Buffer::ISizeType length);

    protected:
        /**
        * Parses an UTF8 or ASCII encoded script
        *@param buffer - buffer containing the script to parse
        *@param length - length to read in the buffer
        *@return true on success, otherwise false
        */
        virtual bool Parse(QR_Buffer& buffer, QR_Buffer::ISizeType length);

        /**
        * Parses an UTF16 encoded script
        *@param buffer - buffer containing the script to parse
        *@param length - length to read in the buffer
        *@param bigEndian - if true, script is written in big endian
        *@return true on success, otherwise false
        */
        virtual bool ParseW(QR_Buffer& buffer, QR_Buffer::ISizeType length, bool bigEndian);

        /**
        * Parses a script line
        *@param line - line to parse
        *@param linbeNb - line number
        *@return ture on success, otherwise false
        */
        virtual bool ParseLine(const std::string& line, QR_SizeT lineNb);

        /**
        * Parses a script line
        *@param line - line to parse
        *@param linbeNb - line number
        *@return ture on success, otherwise false
        */
        virtual bool ParseLine(const std::wstring& line, QR_SizeT lineNb);

        /**
        * Reads next wide char from buffer
        *@param buffer - buffer to read from
        *@param isTextBE - if true, text is written in big endian, otherwise little endian
        *@param isSystemBE - if true, system endianess is big endian, otherwise little endian
        *@return next read char
        */
        virtual wchar_t ReadWChar(QR_Buffer& buffer, bool isTextBE, bool isSystemBE) const;

        /**
        * Checks if script end is reached
        *@param buffer - buffer containing script to read
        *@aram start - script start offset in buffer
        *@param length - script length
        *@return true if script end is reached, otherwise false
        */
        virtual bool EndReached(QR_Buffer&           buffer,
                                QR_Buffer::ISizeType start,
                                QR_Buffer::ISizeType length) const;

        /**
        * Called when script line should be parsed
        *@param line - line to parse
        *@param linbeNb - line number
        *@return ture on success, otherwise false
        */
        virtual bool OnParseLine(const std::string& line, QR_SizeT lineNb) = 0;
};

#endif
