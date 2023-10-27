/****************************************************************************
 * ==> DWF_Sound_OpenAL ----------------------------------------------------*
 ****************************************************************************
 * Description : OpenAL based sound player                                  *
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

#include "DWF_Sound_OpenAL.h"

// classes
#include "DWF_StdFileBuffer.h"

using namespace DWF_Audio;

//---------------------------------------------------------------------------
// Sound_OpenAL
//---------------------------------------------------------------------------
Sound_OpenAL::Sound_OpenAL() :
    Sound()
{}
//---------------------------------------------------------------------------
Sound_OpenAL::~Sound_OpenAL()
{}
//---------------------------------------------------------------------------
bool Sound_OpenAL::OpenWav(const std::wstring& fileName)
{
    DWF_Buffer::StdFileBuffer fileBuffer;

    // open file to read
    if (!fileBuffer.Open(fileName, DWF_Buffer::FileBuffer::IEMode::IE_M_Read))
        return false;

    // create the sound from the Wav file content
    return OpenWav(fileBuffer);
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::OpenWav(DWF_Buffer::Buffer& buffer)
{
    // get buffer offset to start
    buffer.Seek(0, 0);

    // get buffer size
    const std::size_t size = buffer.GetSize();

    unsigned char signature[4];

    // read RIFF signature
    buffer.Read(&signature[0], 4);

    // check the RIFF signature
    if (signature[0] != 'R' || signature[1] != 'I' || signature[2] != 'F' || signature[3] != 'F')
        return false;

    // skip 4 next bytes (this is the entire WAV data length)
    buffer.Seek(buffer.GetOffset(), 4);

    // read WAVE signature
    buffer.Read(&signature[0], 4);

    // check the WAVE signature
    if (signature[0] != 'W' || signature[1] != 'A' || signature[2] != 'V' || signature[3] != 'E')
        return false;

    // skip next bytes until the sample rate
    buffer.Seek(buffer.GetOffset(), 12);

    unsigned sampleRate;

    // read the sample rate
    buffer.Read(&sampleRate, sizeof(unsigned));

    // skip all the following header data
    buffer.Seek(buffer.GetOffset(), 16);

    // create the sound from the file content
    return Create(buffer, sampleRate);
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::Play()
{
    if (m_ID == M_OPENAL_ERROR_ID)
        return false;

    alSourcePlay(m_ID);
    return true;
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::Pause()
{
    if (m_ID == M_OPENAL_ERROR_ID)
        return false;

    alSourcePause(m_ID);
    return true;
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::Stop()
{
    if (m_ID == M_OPENAL_ERROR_ID)
        return false;

    alSourceStop(m_ID);
    return true;
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::IsPlaying()
{
    ALenum state;

    if (m_ID == M_OPENAL_ERROR_ID)
        return false;

    alGetSourcei(m_ID, AL_SOURCE_STATE, &state);

    return (state == AL_PLAYING);
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::ChangePitch(float value)
{
    if (m_ID == M_OPENAL_ERROR_ID)
        return false;

    if (value >= 0.0f && value <= 1.0f)
    {
        alSourcef(m_ID, AL_PITCH, value);
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::ChangeVolume(float value)
{
    if (m_ID == M_OPENAL_ERROR_ID)
        return false;

    if (value >= 0.0f && value <= 1.0f)
    {
        alSourcef(m_ID, AL_GAIN, value);
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::ChangeVolumeMin(float value)
{
    if (m_ID == M_OPENAL_ERROR_ID)
        return false;

    if (value >= 0.0f && value <= 1.0f)
    {
        alSourcef(m_ID, AL_MIN_GAIN, value);
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::ChangeVolumeMax(float value)
{
    if (m_ID == M_OPENAL_ERROR_ID)
        return false;

    if (value >= 0.0f && value <= 1.0f)
    {
        alSourcef(m_ID, AL_MAX_GAIN, value);
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::ChangePosition(const DWF_Math::Vector3F& pos)
{
    if (m_ID == M_OPENAL_ERROR_ID)
        return false;

    ALfloat position[3];
    position[0] = pos.m_X;
    position[1] = pos.m_Y;
    position[2] = pos.m_Z;

    alSourcefv(m_ID, AL_POSITION, position);
    return true;
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::Loop(bool value)
{
    if (m_ID == M_OPENAL_ERROR_ID)
        return false;

    alSourcei(m_ID, AL_LOOPING, value ? AL_TRUE : AL_FALSE);
    return true;
}
//---------------------------------------------------------------------------
bool Sound_OpenAL::Create(DWF_Buffer::Buffer& buffer, unsigned sampling)
{
    // release any previous sound content
    Release();

    // grab a buffer ID from openAL
    alGenBuffers(1, &m_BufferID);

    // succeeded?
    if (alGetError() != AL_NO_ERROR)
    {
        Release();
        return false;
    }

    // get buffer size
    const std::size_t size = buffer.GetSize() - buffer.GetOffset();

    // create intermediate data buffer
    std::unique_ptr<unsigned char[]> pData = std::make_unique<unsigned char[]>(size);

    // copy file content to intermediate data buffer
    buffer.Read(pData.get(), size);

    // jam the audio data into the new buffer
    alBufferData(m_BufferID, AL_FORMAT_STEREO16, pData.get(), (ALsizei)size, sampling);

    // succeeded?
    if (alGetError() != AL_NO_ERROR)
    {
        Release();
        return false;
    }

    // grab a source ID from openAL
    alGenSources(1, &m_ID);

    // succeeded?
    if (alGetError() != AL_NO_ERROR)
    {
        Release();
        return false;
    }

    // attach the buffer to the source
    alSourcei(m_ID, AL_BUFFER, m_BufferID);

    // succeeded?
    if (alGetError() != AL_NO_ERROR)
    {
        Release();
        return false;
    }

    // set some basic source preferences
    alSourcef(m_ID, AL_GAIN,  1.0f);
    alSourcef(m_ID, AL_PITCH, 1.0f);

    return true;
}
//---------------------------------------------------------------------------
void Sound_OpenAL::Release()
{
    // delete source
    if (m_ID != M_OPENAL_ERROR_ID)
        alDeleteSources(1, &m_ID);

    // delete buffer
    if (m_BufferID != M_OPENAL_ERROR_ID)
        alDeleteBuffers(1, &m_BufferID);

    // reset the sound content
    m_BufferID = M_OPENAL_ERROR_ID;
    m_ID       = M_OPENAL_ERROR_ID;
}
//---------------------------------------------------------------------------
