/****************************************************************************
 * ==> Buffer --------------------------------------------------------------*
 ****************************************************************************
 * Description: Generic buffer                                              *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - Capsule collision demo                                     *
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

#include "Buffer.h"

//---------------------------------------------------------------------------
// Buffer
//---------------------------------------------------------------------------
Buffer::Buffer()
{}
//---------------------------------------------------------------------------
Buffer::Buffer(Buffer& other)
{
    Copy(other);
}
//---------------------------------------------------------------------------
Buffer::~Buffer()
{}
//---------------------------------------------------------------------------
Buffer& Buffer::operator = (Buffer& other)
{
    Copy(other);
    return *this;
}
//---------------------------------------------------------------------------
void Buffer::Copy(Buffer& other)
{
    Copy(&other);
}
//---------------------------------------------------------------------------
void Buffer::Copy(Buffer* pOther)
{
    // clear current buffer content
    Clear();

    // no buffer to copy from?
    if (!pOther)
        return;

    std::uint8_t* pBuffer = nullptr;

    try
    {
        // create copy buffer
        pBuffer = new std::uint8_t[pOther->GetSize()];

        // store current source offset
        const std::size_t srcOffset = pOther->GetOffset();

        // seek source to start
        pOther->Seek(0, 0);

        // get size to copy
        const std::size_t sizeToCopy = pOther->GetSize();

        // read source buffer content
        pOther->Read(pBuffer, sizeToCopy);

        // seek source to previous offset
        pOther->Seek(0, srcOffset);

        // write source data to buffer
        Write(pBuffer, sizeToCopy);

        // seek to start position
        Seek(0, 0);
    }
    catch (...)
    {
        // delete copy buffer
        if (pBuffer)
            delete[] pBuffer;

        // re-throw exception
        throw;
    }

    // delete copy buffer
    if (pBuffer)
        delete[] pBuffer;
}
//---------------------------------------------------------------------------
