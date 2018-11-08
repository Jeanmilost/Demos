/****************************************************************************
 * ==> QR_StringTools ------------------------------------------------------*
 ****************************************************************************
 * Description : Tools to work with strings                                 *
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

#include "QR_StringTools.h"

// qr engine
#include "QR_Exception.h"

// libraries
#include "ConvertUTF.h"

//---------------------------------------------------------------------------
// QR_StringTools
//---------------------------------------------------------------------------
std::wstring QR_StringTools::StrToWStr(const std::string& str)
{
    std::wstring wstr(str.length(), L' ');
    std::copy(str.begin(), str.end(), wstr.begin());
    return wstr;
}
//---------------------------------------------------------------------------
std::string QR_StringTools::WStrToStr(const std::wstring& wstr)
{
    std::string str(wstr.length(), ' ');
    std::copy(wstr.begin(), wstr.end(), str.begin());
    return str;
}
//---------------------------------------------------------------------------
std::wstring QR_StringTools::UTF8ToUTF16(const std::string& str)
{
    // source code from:
    // http://www.codeproject.com/Articles/17573/Convert-Between-std-string-and-std-wstring-UTF-8-a

    size_t wideSize = str.length();

    #if defined(CP_EMBARCADERO)
        #pragma warn -8008
        #pragma warn -8066
    #endif

    // search for system char length (can be different on Windows, Linux, OSX or other systems)
    if (sizeof(QR_WChar) == 2)
    {
        // calculate utf16 size, prepare utf16 container and calculate source and dest offsets
        QR_WChar*   pWideStringNative = new QR_WChar[wideSize + 1];
        const UTF8* pSourceStart      = reinterpret_cast<const UTF8*>(str.c_str());
        const UTF8* pSourceEnd        = pSourceStart + wideSize;
        UTF16*      pTargetStart      = reinterpret_cast<UTF16*>(pWideStringNative);
        UTF16*      pTargetEnd        = pTargetStart + wideSize + 1;

        // try to convert to utf16
        ConversionResult res = ConvertUTF8toUTF16(&pSourceStart,
                                                  pSourceEnd,
                                                  &pTargetStart,
                                                  pTargetEnd,
                                                  lenientConversion);

        // conversion failed?
        if (res != conversionOK)
        {
            delete[] pWideStringNative;
            M_THROW_EXCEPTION("Conversion failed - wchar_t length - 2");
        }

        // get and return utf16 converted string
        *pTargetStart = 0;
        std::wstring resultString(pWideStringNative);
        delete[] pWideStringNative;
        return resultString;
    }
    else
    if (sizeof(QR_WChar) == 4)
    {
        // calculate utf16 size, prepare utf16 container and calculate source and dest offsets
        QR_WChar*   pWideStringNative = new QR_WChar[wideSize + 1];
        const UTF8* pSourceStart      = reinterpret_cast<const UTF8*>(str.c_str());
        const UTF8* pSourceEnd        = pSourceStart + wideSize;
        UTF32*      pTargetStart      = reinterpret_cast<UTF32*>(pWideStringNative);
        UTF32*      pTargetEnd        = pTargetStart + wideSize;

        // try to convert to utf16
        ConversionResult res = ConvertUTF8toUTF32(&pSourceStart,
                                                  pSourceEnd,
                                                  &pTargetStart,
                                                  pTargetEnd,
                                                  lenientConversion);

        // conversion failed?
        if (res != conversionOK)
        {
            delete[] pWideStringNative;
            M_THROW_EXCEPTION("Conversion failed - wchar_t length - 4");
        }

        // get and return utf16 converted string
        *pTargetStart = 0;
        std::wstring resultString(pWideStringNative);
        delete[] pWideStringNative;
        return resultString;
    }
    else
        M_THROW_EXCEPTION("Conversion failed - unknown wchar_t length");

    #if defined(CP_EMBARCADERO)
        #pragma warn .8066
        #pragma warn .8008
    #endif
}
//---------------------------------------------------------------------------
std::string QR_StringTools::UTF16ToUTF8(const std::wstring& wstr)
{
    // source code from:
    // http://www.codeproject.com/Articles/17573/Convert-Between-std-string-and-std-wstring-UTF-8-a

    size_t wideSize = wstr.length();

    #if defined(CP_EMBARCADERO)
        #pragma warn -8008
        #pragma warn -8066
    #endif

    // search for system char length (can be different on Windows, Linux, OSX or other systems)
    if (sizeof(QR_WChar) == 2)
    {
        // calculate utf8 size, prepare utf8 container and calculate source and dest offsets
        QR_SizeT     utf8Size          = 2 * wideSize + 1;
        QR_Char*     pUtf8StringNative = new QR_Char[utf8Size];
        const UTF16* pSourceStart      = reinterpret_cast<const UTF16*>(wstr.c_str());
        const UTF16* pSourceEnd        = pSourceStart + wideSize;
        UTF8*        pTargetStart      = reinterpret_cast<UTF8*>(pUtf8StringNative);
        UTF8*        pTargetEnd        = pTargetStart + utf8Size;

        // try to convert to utf8
        ConversionResult res = ConvertUTF16toUTF8(&pSourceStart,
                                                  pSourceEnd,
                                                  &pTargetStart,
                                                  pTargetEnd,
                                                  lenientConversion);

        // conversion failed?
        if (res != conversionOK)
        {
            delete [] pUtf8StringNative;
            M_THROW_EXCEPTION("Conversion failed - wchar_t length - 2");
        }

        // get and return utf8 converted string
        *pTargetStart = 0;
        std::string resultString(pUtf8StringNative);
        delete [] pUtf8StringNative;
        return resultString;
    }
    else
    if (sizeof(QR_WChar) == 4)
    {
        // calculate utf8 size, prepare utf8 container and calculate source and dest offsets
        QR_SizeT     utf8Size          = 4 * wideSize + 1;
        QR_Char*     pUtf8StringNative = new QR_Char[utf8Size];
        const UTF32* pSourceStart      = reinterpret_cast<const UTF32*>(wstr.c_str());
        const UTF32* pSourceEnd        = pSourceStart + wideSize;
        UTF8*        pTargetStart      = reinterpret_cast<UTF8*>(pUtf8StringNative);
        UTF8*        pTargetEnd        = pTargetStart + utf8Size;

        // try to convert to utf8
        ConversionResult res = ConvertUTF32toUTF8(&pSourceStart,
                                                  pSourceEnd,
                                                  &pTargetStart,
                                                  pTargetEnd,
                                                  lenientConversion);

        // conversion failed?
        if (res != conversionOK)
        {
            delete [] pUtf8StringNative;
            M_THROW_EXCEPTION("Conversion failed - wchar_t length - 4");
        }

        // get and return utf8 converted string
        *pTargetStart = 0;
        std::string resultString(pUtf8StringNative);
        delete [] pUtf8StringNative;
        return resultString;
    }
    else
        M_THROW_EXCEPTION("Conversion failed - unknown wchar_t length");

    #if defined(CP_EMBARCADERO)
        #pragma warn .8066
        #pragma warn .8008
    #endif
}
//---------------------------------------------------------------------------
std::string QR_StringTools::BoolToStr(bool value, bool numeric)
{
    if (numeric)
        return value ? "1" : "0";

    return value ? "true" : "false";
}
//---------------------------------------------------------------------------
std::wstring QR_StringTools::BoolToWStr(bool value, bool numeric)
{
    if (numeric)
        return value ? L"1" : L"0";

    return value ? L"true" : L"false";
}
//---------------------------------------------------------------------------
bool QR_StringTools::StrToBool(const std::string& str)
{
    return (str == "1" || ToLowerCase(str) == "true");
}
//---------------------------------------------------------------------------
bool QR_StringTools::StrToBool(const std::wstring& str)
{
    return (str == L"1" || ToLowerCase(str) == L"true");
}
//---------------------------------------------------------------------------
