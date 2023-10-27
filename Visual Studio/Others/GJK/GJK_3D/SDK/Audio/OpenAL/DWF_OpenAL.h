/****************************************************************************
 * ==> DWF_OpenAL ----------------------------------------------------------*
 ****************************************************************************
 * Description : OpenAL manager                                             *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - DwarfStar Game Engine                                      *
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
#include <mutex>

// OpenAL
#include <al.h>
#include <alc.h>

//---------------------------------------------------------------------------
// Global defines
//---------------------------------------------------------------------------
#define M_OPENAL_ERROR_ID 0xFFFFFFFF

namespace DWF_Audio
{
    /**
    * OpenAL manager
    *@author Jean-Milost Reymond
    */
    class OpenAL
    {
        public:
            /**
            * Gets OpenAL instance
            *@return OpenAL instance, nullptr on error
            */
            static OpenAL* GetInstance();

            /**
            * Gets the OpenAL device
            *@returns the OpenAL device, nullptr on error
            */
            ALCdevice* GetOpenALDevice() const;

            /**
            * Gets the OpenAL context
            *@returns the OpenAL context, nullptr on error
            */
            ALCcontext* GetOpenALContext() const;

        private:
            /**
            * Instance class, needed to allow unique_ptr usage despite of singleton privacy and without
            * declare unique_ptr friend
            */
            struct IInstance
            {
                OpenAL* m_pInstance = nullptr;

                IInstance();
                virtual ~IInstance();
            };

            static std::unique_ptr<IInstance> m_pInstance;
            static std::mutex                 m_Mutex;
            ALCdevice*                        m_pOpenALDevice  = nullptr;
            ALCcontext*                       m_pOpenALContext = nullptr;

            OpenAL();

            /**
            * Copy constructor
            *@param other - other logger to copy from
            */
            OpenAL(const OpenAL& other);

            ~OpenAL();

            /**
            * Initializes OpenAL library
            *@param[out] pOpenALDevice - newly created OpenAL device
            *@param[out] pOpenALContext - newly created OpenAL context
            *@return true on success, otherwise false
            */
            bool Init();

            /**
            * Releases OpenAL
            *@param[in, out] pOpenALDevice - OpenAL device to release
            *@param[in, out] pOpenALContext - OpenAL context to release
            */
            void Release();

            /**
            * Copy operator
            *@param other - other logger to copy from
            */
            const OpenAL& operator = (const OpenAL& other);
    };
}
