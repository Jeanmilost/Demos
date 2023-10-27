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

#include "DWF_OpenAL.h"

using namespace DWF_Audio;

//---------------------------------------------------------------------------
// Static members
//---------------------------------------------------------------------------
std::unique_ptr<OpenAL::IInstance> OpenAL::m_pInstance;
std::mutex                         OpenAL::m_Mutex;
//---------------------------------------------------------------------------
// OpenAL::IInstance
//---------------------------------------------------------------------------
OpenAL::IInstance::IInstance()
{
    m_pInstance = new OpenAL();
}
//---------------------------------------------------------------------------
OpenAL::IInstance::~IInstance()
{
    if (m_pInstance)
        delete m_pInstance;
}
//---------------------------------------------------------------------------
// OpenAL
//---------------------------------------------------------------------------
OpenAL::OpenAL()
{
    Init();
}
//---------------------------------------------------------------------------
OpenAL::OpenAL(const OpenAL& other)
{
    throw new std::exception("Cannot create a copy of a singleton class");
}
//---------------------------------------------------------------------------
OpenAL::~OpenAL()
{
    Release();
}
//---------------------------------------------------------------------------
const OpenAL& OpenAL::operator = (const OpenAL& other)
{
    throw new std::exception("Cannot create a copy of a singleton class");
}
//---------------------------------------------------------------------------
OpenAL* OpenAL::GetInstance()
{
    // check instance out of the thread lock (double check lock)
    if (!m_pInstance)
    {
        // lock up the thread
        std::unique_lock<std::mutex> lock(m_Mutex);

        // create the instance
        if (!m_pInstance)
            m_pInstance.reset(new (std::nothrow)IInstance());
    }

    // still not created?
    if (!m_pInstance)
        throw new std::exception("Could not create the Logger unique instance");

    return m_pInstance->m_pInstance;
}
//---------------------------------------------------------------------------
ALCdevice* OpenAL::GetOpenALDevice() const
{
    return m_pOpenALDevice;
}
//---------------------------------------------------------------------------
ALCcontext* OpenAL::GetOpenALContext() const
{
    return m_pOpenALContext;
}
//---------------------------------------------------------------------------
bool OpenAL::Init()
{
    // select the "preferred device"
    m_pOpenALDevice = alcOpenDevice(0);

    // found it?
    if (!m_pOpenALDevice)
        return false;

    // use the device to make a context
    m_pOpenALContext = alcCreateContext(m_pOpenALDevice, 0);

    // found it?
    if (!m_pOpenALContext)
        return false;

    // set context to the currently active one
    alcMakeContextCurrent(m_pOpenALContext);

    return true;
}
//---------------------------------------------------------------------------
void OpenAL::Release()
{
    // destroy OpenAL context
    if (m_pOpenALContext)
        alcDestroyContext(m_pOpenALContext);

    // close the device
    if (m_pOpenALDevice)
        alcCloseDevice(m_pOpenALDevice);
}
//---------------------------------------------------------------------------
