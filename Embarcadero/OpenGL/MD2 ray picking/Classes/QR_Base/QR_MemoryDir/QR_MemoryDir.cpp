/****************************************************************************
 * ==> QR_MemoryDir --------------------------------------------------------*
 ****************************************************************************
 * Description : Memory dir, simulates a directory in memory                *
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

#include "QR_MemoryDir.h"

// qr engine
#include "QR_STDTools.h"
#include "QR_StringTools.h"
#include "QR_FileTools.h"
#include "QR_StdFileBuffer.h"

//---------------------------------------------------------------------------
// QR_MemoryDir
//---------------------------------------------------------------------------
QR_MemoryDir::QR_MemoryDir(bool deleteOnDestroy) :
    m_DeleteOnDestroy(deleteOnDestroy)
{}
//---------------------------------------------------------------------------
QR_MemoryDir::~QR_MemoryDir()
{
    // do delete file on destroy?
    if (m_DeleteOnDestroy)
        // clear memory
        QR_STDTools::DelAndClear(m_Files);
}
//---------------------------------------------------------------------------
bool QR_MemoryDir::AddFile(const std::wstring&    fileName,
                                 QR_MemoryBuffer* pBuffer,
                                 bool             overwrite)
{
    // no buffer to add?
    if (!pBuffer)
        return false;

    // search for file
    QR_MemoryFileDictionary::iterator it = m_Files.find(fileName);

    // file exists?
    if (it != m_Files.end())
    {
        // file cannot be overwritten?
        if (!overwrite)
            return false;

        // do delete file content?
        if (m_DeleteOnDestroy)
        {
            delete it->second;
            m_Files.erase(it);
        }
    }

    // add file to dir list
    m_Files[fileName] = pBuffer;
    return true;
}
//---------------------------------------------------------------------------
QR_MemoryBuffer* QR_MemoryDir::GetFile(const std::wstring& fileName) const
{
    // search for file
    QR_MemoryFileDictionary::const_iterator it = m_Files.find(fileName);

    // found it?
    if (it == m_Files.end())
        return NULL;

    return it->second;
}
//---------------------------------------------------------------------------
bool QR_MemoryDir::FileExists(const std::wstring& fileName) const
{
    return (m_Files.find(fileName) != m_Files.end());
}
//---------------------------------------------------------------------------
bool QR_MemoryDir::PopulateFromDir(const std::wstring& dir)
{
    // dir exists?
    if (!QR_FileTools::DirExists(dir))
        return false;

    QR_StringTools::IwStrings content;

    // get dir content
    if (!QR_FileTools::GetDirContent(dir, content, false))
        return false;

    const QR_SizeT     fileCount = content.size();
    const std::wstring directory = QR_FileTools::AppendDelimiter(dir);

    // iterate through dir content
    for (QR_SizeT i = 0; i < fileCount; ++i)
    {
        const std::wstring fileName = directory + content[i];

        // ignore subdirs
        if (QR_FileTools::IsDir(fileName))
            continue;

        // open file as buffer
        std::auto_ptr<QR_StdFileBuffer> pFileBuffer(new QR_StdFileBuffer());
        pFileBuffer->Open(fileName, QR_FileBuffer::IE_M_Read);
        pFileBuffer->Seek(0, 0);

        // create new memory buffer
        std::auto_ptr<QR_MemoryBuffer> pBuffer(new QR_MemoryBuffer());
        pBuffer->Copy(pFileBuffer.get());
        pBuffer->Seek(0, 0);

        // add file to memory dir
        AddFile(content[i], pBuffer.get(), false);
        pBuffer.release();
    }

    return true;
}
//---------------------------------------------------------------------------
