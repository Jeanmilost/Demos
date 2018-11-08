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

#ifndef QR_MemoryDirH
#define QR_MemoryDirH

// std
#include <map>

// qr engine
#include "QR_MemoryBuffer.h"

typedef std::map<std::wstring, QR_MemoryBuffer*> QR_MemoryFileDictionary;

/**
* Memory directory, allows to create a structure closest to a dir that contains all files as
* memory buffers
*@author Jean-Milost Reymond
*/
class QR_MemoryDir
{
    public:
        /**
        * Constructor
        *@param deleteOnDestroy - if true, added items will be deleted while dir will be destroyed
        */
        QR_MemoryDir(bool deleteOnDestroy = false);

        virtual ~QR_MemoryDir();

        /**
        * Adds memory file
        *@param fileName - memory file name
        *@param pBuffer - memory buffer containing file data
        *@param overwrite - if true, existing file will be overwritten
        *@return true on success, otherwise false
        */
        virtual bool AddFile(const std::wstring&    fileName,
                                   QR_MemoryBuffer* pBuffer,
                                   bool             overwrite);

        /**
        * Gets file
        *@param fileName - memory file name to get
        *@return memory buffer containing file data, NULL if not found or on error
        */
        virtual QR_MemoryBuffer* GetFile(const std::wstring& fileName) const;

        /**
        * Checks if file exists
        *@param fileName - file name to check
        *@return true if file exists, otherwise false
        */
        virtual bool FileExists(const std::wstring& fileName) const;

        /**
        * Populates a memory dir from a dir
        *@param dir - source dir
        *@return true on success, otherwise false
        */
        virtual bool PopulateFromDir(const std::wstring& dir);

    protected:
        QR_MemoryFileDictionary m_Files;
        bool                    m_DeleteOnDestroy;
};

#endif
