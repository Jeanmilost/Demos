/****************************************************************************
 * ==> QR_FileTools --------------------------------------------------------*
 * **************************************************************************
 * Description : Most common file and folder tools                          *
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

#ifndef QR_FileToolsH
#define QR_FileToolsH

// std
#include <string>

// qr engine
#include "QR_Types.h"
#include "QR_StringTools.h"
#include "QR_Version.h"

#if defined (OS_WIN)
    #define M_DirDelim   '\\'
    #define M_DirDelimW L'\\'
#elif defined (OS_OSX)
    #define M_DirDelim   '/'
    #define M_DirDelimW L'/'
#endif

/**
* File and folder tools
*@note This class is cross-platform, but partially
*@author Jean-Milost Reymond
*/
class QR_FileTools
{
    public:
        /**
        * Gets Windows desktop directory
        *@return desktop directory, empty string on error
        */
        static std::wstring GetDesktopDir();

        /**
        * Gets running application name and dir
        *@return running application name and dir
        */
        static std::string  GetAppName();
        static std::wstring GetAppNameW();

        /**
        * Gets running application version
        *@return running application version, empty version on error
        */
        static QR_Version GetAppVersion();

        /**
        * Gets file directory from a full path
        *@param fullPath - full path containing file dir and name
        *@param keepLastChar - if true, last delimiter will be kept
        *@param delimiter - dir delimiter
        *@return directory, empty string on error
        */
        static std::string  GetFileDir(const std::string&  fullPath, bool keepLastChar, QR_Char  delimiter = M_DirDelim);
        static std::wstring GetFileDir(const std::wstring& fullPath, bool keepLastChar, QR_WChar delimiter = M_DirDelimW);

        /**
        * Gets file name from a full path
        *@param fullPath - full path containing file dir and name
        *@param delimiter - dir delimiter
        *@return file name
        */
        static std::string  GetFileName(const std::string&  fullPath, QR_Char  delimiter = M_DirDelim);
        static std::wstring GetFileName(const std::wstring& fullPath, QR_WChar delimiter = M_DirDelimW);

        /**
        * Gets file extension from a full path
        *@param fullPath - full path containing file dir and name
        *@return file extension
        */
        static std::string  GetFileExtension(const std::string&  fullPath);
        static std::wstring GetFileExtension(const std::wstring& fullPath);

        /**
        * Remove delimiter from dir end
        *@param dirName - directory name to remove from
        *@param delimiter - delimiter used in current system
        *@return dir name without last delimiter
        *@note If delimiter not exists at end, nothing will be removed
        */
        static std::string  RemoveDelimiter(const std::string&  dirName, QR_Char  delimiter = M_DirDelim);
        static std::wstring RemoveDelimiter(const std::wstring& dirName, QR_WChar delimiter = M_DirDelimW);

        /**
        * Appends delimiter at dir end
        *@param dirName - directory name to append to
        *@param delimiter - delimiter used in current system
        *@return appended dir name
        *@note If delimiter already exists at end, it will not be appended again
        */
        static std::string  AppendDelimiter(const std::string&  dirName, QR_Char  delimiter = M_DirDelim);
        static std::wstring AppendDelimiter(const std::wstring& dirName, QR_WChar delimiter = M_DirDelimW);

        /**
        * Checks if a file exists
        *@param fileName - file name to check
        *@returns true if file exists, otherwise false
        */
        static bool FileExists(const std::string&  name);
        static bool FileExists(const std::wstring& name);

        /**
        * Checks if a directory exists
        *@param dirName - dir name to check
        *@returns true if dir exists, otherwise false
        */
        static bool DirExists(const std::string&  name);
        static bool DirExists(const std::wstring& name);

        /**
        * Checks if a path is a dir
        *@param path - path to check
        *@return true if path is a dir, otherwise false
        */
        static bool IsDir(const std::string&  path);
        static bool IsDir(const std::wstring& path);

        /**
        * Gets directory content
        *@param dir - dir name for which content should be get
        *@param[out] content - dir content
        *@param fullNames - if true, items will contain full names
        *@return true on success, otherwise false
        *@note if fullNames is set, special "." and ".." items will be skipped
        */
        static bool GetDirContent(const std::string&  dir, QR_StringTools::IStrings&  content, bool fullNames);
        static bool GetDirContent(const std::wstring& dir, QR_StringTools::IwStrings& content, bool fullNames);

        /**
        * Loads data from file
        *@param fileName - file name
        *@param[out] data - file content
        *@return true on success, otherwise false
        */
        static bool Load(const std::string& fileName, std::string& data);

        /**
        * Saves data to file
        *@param fileName - file name
        *@param data - file content
        *@param clear - if true, file will be rewritten completely
        *@return true on success, otherwise false
        */
        static bool Save(const std::string& fileName, const std::string& data, bool clear);
};
#endif
