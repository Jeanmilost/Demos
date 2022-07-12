/****************************************************************************
 * ==> Buffer --------------------------------------------------------------*
 ****************************************************************************
 * Description: Generic buffer                                              *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - iqm model reader                                           *
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

// std
#include <string>

/**
* Generic buffer. Buffers can contain binary data, text, ...
*@author Jean-Milost Reymond
*/
class Buffer
{
    public:
        /**
        * Constructor
        */
        Buffer();

        /**
        * Copy constructor
        *@param other - other buffer to copy from
        *@note Internal offset will be seek to 0 after copy
        */
        Buffer(Buffer& other);

        /**
        * Destructor
        */
        virtual ~Buffer();

        /**
        * Copy operator
        *@param other - other buffer to copy from
        *@return this buffer
        *@note Current buffer content will be completely replaced by new
        *@note Internal offset will be seek to 0 after copy
        */
        virtual Buffer& operator = (Buffer& other);

        /**
        * Copies buffer content from another buffer
        *@param other - other buffer to copy from
        *@note Current buffer content will be completely replaced by new
        *@note Internal offset will be seek to 0 after copy
        */
        virtual void Copy(Buffer& other);

        /**
        * Copies buffer content from another buffer
        *@param pOther - other buffer to copy from
        *@note Current buffer content will be completely replaced by new
        *@note Internal offset will be seek to 0 after copy
        */
        virtual void Copy(Buffer* pOther);

        /**
        * Clears buffer completely
        */
        virtual void Clear() = 0;

        /**
        * Checks if buffer is empty
        *@return true if buffer is empty, otherwise false
        */
        virtual bool Empty() = 0;

        /**
        * Gets current offset position, in bytes (depends on IDataType)
        *@return current offset position, in bytes
        */
        virtual std::size_t GetOffset() const = 0;

        /**
        * Gets data size, in bytes (depends on IDataType)
        *@return data size, in bytes
        */
        virtual std::size_t GetSize() const = 0;

        /**
        * Seeks offset
        *@param start - absolute start offset to seek from
        *@param delta - number of bytes to seek from start offset, must be positive
        *@return new offset position
        */
        virtual std::size_t Seek(std::size_t start, std::size_t delta) = 0;

        /**
        * Reads data from buffer
        *@param pBuffer - destination buffer that will receive the read data
        *@param length - length to read in source buffer
        *@return read data length
        *@note Data will be read from current offset
        */
        virtual std::size_t Read(void* pBuffer, std::size_t length) = 0;

        /**
        * Writes data into buffer
        *@param pBuffer - source buffer to write from
        *@param length - length to write from source buffer
        *@return written data length
        *@note Data will be written from current offset
        */
        virtual std::size_t Write(const void* pBuffer, std::size_t length) = 0;

        /**
        * Reads buffer content as string
        *@return buffer content as string
        */
        virtual std::string ToStr() = 0;
};
