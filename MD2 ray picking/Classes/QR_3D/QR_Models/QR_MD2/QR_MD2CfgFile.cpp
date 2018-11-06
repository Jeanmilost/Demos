/****************************************************************************
 * ==> QR_MD2CfgFile -------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a parser for the MD2 configuration file           *
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

#include "QR_MD2CfgFile.h"

// qr engine
#include "QR_Exception.h"
#include "QR_STDTools.h"
#include "QR_StringTools.h"

//---------------------------------------------------------------------------
// QR_MD2CfgFile::IGesture
//---------------------------------------------------------------------------
QR_MD2CfgFile::IGesture::IGesture() :
    m_StartFrame(0),
    m_FrameCount(0),
    m_FrameLoop(0),
    m_FramesPerSecond(0)
{}
//---------------------------------------------------------------------------
QR_MD2CfgFile::IGesture::~IGesture()
{}
//---------------------------------------------------------------------------
// QR_MD2CfgFile
//---------------------------------------------------------------------------
QR_MD2CfgFile::QR_MD2CfgFile() :
    QR_Script(),
    m_Column(0),
    m_StartLine(0),
    m_CurLine(0),
    m_Gesture(0),
    m_LongComment(false)
{}
//---------------------------------------------------------------------------
QR_MD2CfgFile::~QR_MD2CfgFile()
{
    DelAndClear();
}
//---------------------------------------------------------------------------
void QR_MD2CfgFile::Clear()
{
    DelAndClear();
}
//---------------------------------------------------------------------------
void QR_MD2CfgFile::Copy(const QR_MD2CfgFile& other)
{
    Clear();

    // iterate through gestures to copy
    for (IGestureDictionary::const_iterator it = other.m_Gestures.begin();
            it != other.m_Gestures.end(); ++it)
    {
        // create and populate gesture
        std::auto_ptr<IGesture> pGesture(new IGesture());
        *pGesture.get() = *it->second;

        // add it to dictionary
        m_Gestures[it->first] = pGesture.get();
        pGesture.release();
    }
}
//---------------------------------------------------------------------------
QR_MD2CfgFile::IGesture* QR_MD2CfgFile::GetGesture(QR_SizeT index) const
{
    IGestureDictionary::const_iterator it = m_Gestures.find(index);

    if (it == m_Gestures.end())
        return NULL;

    return it->second;
}
//---------------------------------------------------------------------------
QR_SizeT QR_MD2CfgFile::GetGestureCount() const
{
    return m_Gestures.size();
}
//---------------------------------------------------------------------------
bool QR_MD2CfgFile::OnParseLine(const std::string& line, QR_SizeT lineNb)
{
    m_Column = 0;

    // no line to parse?
    if (line.empty())
        return true;

    std::string data;

    // search for comment marker
    QR_SizeT commentPos = line.find("//", 0);

    // found it?
    if (commentPos == line.npos)
        // no, parse entire line
        data = line;
    else
        // yes, parse only the uncommented line part
        data = QR_StringTools::Trim(line.substr(0, commentPos));

    std::string word;

    // iterate through line chars
    for (QR_SizeT i = 0; i < data.length(); ++i)
        // search for char
        switch (data[i])
        {
            case '/':
            case '*':
                // found a long comment (i.e. comment between /* and */) start or end mark?
                if ((i + 1) < data.length())
                    if (data[i] == '/' && data[i + 1] == '*')
                    {
                        m_LongComment = true;
                        ++i;
                    }
                    else
                    if (data[i] == '*' && data[i + 1] == '/')
                    {
                        m_LongComment = false;
                        ++i;
                    }

                continue;

            case ' ':
            case '\t':
                // skip all chars inside a long comment
                if (m_LongComment)
                    continue;

                // found word to parse?
                if (!word.empty())
                {
                    // parse it
                    if (!ParseWord(word, lineNb))
                        return false;

                    // clear parsed word to read next
                    word.clear();
                }

                // skip all remaining spaces
                while ((i + 1) < data.length() && (data[i + 1] == ' ' || data[i + 1] == '\t'))
                    ++i;

                continue;

            default:
                // skip all chars inside a long comment
                if (m_LongComment)
                    continue;

                // add char to word
                word += data[i];
                continue;
        }

    // skip all chars inside a long comment
    if (m_LongComment)
        return true;

    // last word to parse?
    if (!word.empty())
        // parse it
        return ParseWord(word, lineNb);

    return true;
}
//------------------------------------------------------------------------------
bool QR_MD2CfgFile::ParseWord(const std::string& word, QR_SizeT lineNb)
{
    // each line contains 4 numeric values, that describes the animation
    for (QR_SizeT i = 0; i < word.length(); ++i)
        if (!QR_StringTools::IsNumeric(word[i], false))
            return false;

    // first item to parse?
    if (!m_Gestures.size())
    {
        // store the start line, it will be used later to find the animation type
        m_StartLine = lineNb;
        m_CurLine   = lineNb;
    }

    // first animation value?
    if (!m_Column)
    {
        // get animation gesture index
        m_Gesture = (m_CurLine - m_StartLine);

        // gesture cannot be added twice
        M_ASSERT(m_Gestures.find(m_Gesture) == m_Gestures.end());

        // create and populate new gesture, and add it to dictionary
        std::auto_ptr<IGesture> pGesture(new IGesture());
        m_Gestures[m_Gesture] = pGesture.get();
        pGesture.release();

        ++m_CurLine;
    }

    IGestureDictionary::iterator it = m_Gestures.find(m_Gesture);

    // gesture must exist
    M_ASSERT(it != m_Gestures.end());

    // get gesture item to populate
    IGesture* pGesture = m_Gestures[m_Gesture];

    // search for gesture item value to set
    switch (m_Column)
    {
        case 0:  pGesture->m_StartFrame      = std::atoi(word.c_str()); break;
        case 1:  pGesture->m_FrameCount      = std::atoi(word.c_str()); break;
        case 2:  pGesture->m_FrameLoop       = std::atoi(word.c_str()); break;
        case 3:  pGesture->m_FramesPerSecond = std::atoi(word.c_str()); break;
        default: return false;
    }

    ++m_Column;

    return true;
}
//---------------------------------------------------------------------------
void QR_MD2CfgFile::DelAndClear()
{
    QR_STDTools::DelAndClear(m_Gestures);
}
//---------------------------------------------------------------------------
