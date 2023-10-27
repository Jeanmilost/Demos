/****************************************************************************
 * ==> DWF_Sound_MiniAudio -------------------------------------------------*
 ****************************************************************************
 * Description : MiniAudio based sound player                               *
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

#include "DWF_Sound_MiniAudio.h"

#pragma warning(push)
#pragma warning(disable:6078)
#pragma warning(disable:26812)

 // miniaudio
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#pragma warning(pop)

using namespace DWF_Audio;

//---------------------------------------------------------------------------
// Sound_MiniAudio
//---------------------------------------------------------------------------
Sound_MiniAudio::Sound_MiniAudio() :
    Sound()
{}
//---------------------------------------------------------------------------
Sound_MiniAudio::~Sound_MiniAudio()
{
    Release();
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::Open(const std::wstring& fileName)
{
    // release any previously opened sound
    Release();

    // create an audio decoder from file
    ma_result result = ma_decoder_init_file_w(fileName.c_str(), nullptr, &m_Decoder);

    if (result != MA_SUCCESS)
        return false;

    ma_device_config deviceConfig;

    // configure the engine to play audio
    deviceConfig                   = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = m_Decoder.outputFormat;
    deviceConfig.playback.channels = m_Decoder.outputChannels;
    deviceConfig.sampleRate        = m_Decoder.outputSampleRate;
    deviceConfig.dataCallback      = DataCallback;
    deviceConfig.pUserData         = &m_Decoder;

    // initialize the audio device
    if (ma_device_init(nullptr, &deviceConfig, &m_Device) != MA_SUCCESS)
    {
        ma_decoder_uninit(&m_Decoder);
        return false;
    }

    m_Opened = true;

    return true;
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::Open(DWF_Buffer::Buffer& buffer)
{
    // calculate length to read from buffer
    const std::size_t length = buffer.GetSize() - buffer.GetOffset();

    // initialize a memory buffer
    m_pBuffer = std::make_unique<unsigned char[]>(length);

    // copy file buffer content to memory
    buffer.Read(m_pBuffer.get(), length);

    // create an audio decoder from memory
    ma_result result = ma_decoder_init_memory(m_pBuffer.get(), length, nullptr, &m_Decoder);

    if (result != MA_SUCCESS)
        return false;

    ma_device_config deviceConfig;

    // configure the engine to play audio
    deviceConfig                   = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = m_Decoder.outputFormat;
    deviceConfig.playback.channels = m_Decoder.outputChannels;
    deviceConfig.sampleRate        = m_Decoder.outputSampleRate;
    deviceConfig.dataCallback      = DataCallback;
    deviceConfig.pUserData         = &m_Decoder;

    // initialize the audio device
    if (ma_device_init(nullptr, &deviceConfig, &m_Device) != MA_SUCCESS)
    {
        ma_decoder_uninit(&m_Decoder);
        return false;
    }

    m_Opened = true;

    return true;
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::Play()
{
    if (!m_Opened)
        return false;

    // start to play the sound
    return (ma_device_start(&m_Device) == MA_SUCCESS);
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::Pause()
{
    // todo -cFeature -oJean: not implemented
    return false;
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::Stop()
{
    if (!m_Opened)
        return false;

    // already stopped?
    if (!IsPlaying())
        return true;

    // stop the playing sound
    return (ma_device_stop(&m_Device) == MA_SUCCESS);
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::IsPlaying()
{
    if (!m_Opened)
        return false;

    // check if the sound is playing
    return ma_device_is_started(&m_Device);
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::ChangePitch(float value)
{
    // todo -cFeature -oJean: not implemented
    return false;
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::ChangeVolume(float value)
{
    if (!m_Opened)
        return false;

    return (ma_device_set_master_volume(&m_Device, value) == MA_SUCCESS);
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::ChangeVolumeMin(float value)
{
    // todo -cFeature -oJean: not implemented
    return false;
}
//---------------------------------------------------------------------------
bool  Sound_MiniAudio::ChangeVolumeMax(float value)
{
    // todo -cFeature -oJean: not implemented
    return false;
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::ChangePosition(const DWF_Math::Vector3F& pos)
{
    // todo -cFeature -oJean: not implemented
    return false;
}
//---------------------------------------------------------------------------
bool Sound_MiniAudio::Loop(bool value)
{
    if (!m_Opened)
        return false;

    // set looping state
    return (ma_data_source_set_looping(&m_Decoder, value ? MA_TRUE : MA_FALSE) == MA_SUCCESS);
}
//---------------------------------------------------------------------------
void Sound_MiniAudio::Release()
{
    if (!m_Opened)
        return;

    // release the audio device and decoder
    ma_device_uninit(&m_Device);
    ma_decoder_uninit(&m_Decoder);

    m_pBuffer.reset();

    m_Opened = false;
}
//---------------------------------------------------------------------------
void Sound_MiniAudio::DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;

    if (!pDecoder)
        return;

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, nullptr);
}
//---------------------------------------------------------------------------
