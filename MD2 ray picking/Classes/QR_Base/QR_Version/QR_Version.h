/****************************************************************************
 * ==> QR_Version ----------------------------------------------------------*
 * **************************************************************************
 * Description : Version management class, used e.g. for files, ...         *
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

#ifndef QR_VersionH
#define QR_VersionH

// std
#include <string>

// qr engine
#include "QR_Types.h"

/**
* Version management class
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Version
{
    public:
        /**
        * Constructor
        */
        QR_Version();

        /**
        * Constructor
        *@param major - major version
        *@param minor - minor version
        *@param release - release version
        *@param build - build version
        */
        QR_Version(QR_UInt32 major, QR_UInt32 minor, QR_UInt32 release, QR_UInt32 build);

        virtual ~QR_Version();

        /**
        * Gets version major component
        *@return major version
        */
        virtual QR_UInt32 GetMajor() const;

        /**
        * Sets version major component
        *@param value - major version
        */
        virtual void SetMajor(QR_UInt32 value);

        /**
        * Gets version minor component
        *@return version minor component
        */
        virtual QR_UInt32 GetMinor() const;

        /**
        * Sets version minor component
        *@param value - version minor component
        */
        virtual void SetMinor(QR_UInt32 value);

        /**
        * Gets version release component
        *@return version release component
        */
        virtual QR_UInt32 GetRelease() const;

        /**
        * Sets version release component
        *@param value - version release component
        */
        virtual void SetRelease(QR_UInt32 value);

        /**
        * Gets version build component
        *@return version build component
        */
        virtual QR_UInt32 GetBuild() const;

        /**
        * Sets version build component
        *@param value - version build component
        */
        virtual void SetBuild(QR_UInt32 value);

        /**
        * Gets version as string
        *@return version as string
        */
        virtual std::string  ToStr();
        virtual std::wstring ToWStr();

    private:
        QR_UInt32 m_Major;
        QR_UInt32 m_Minor;
        QR_UInt32 m_Release;
        QR_UInt32 m_Build;
};
#endif
