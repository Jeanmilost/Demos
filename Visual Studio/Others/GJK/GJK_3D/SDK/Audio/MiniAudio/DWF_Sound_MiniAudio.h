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

#pragma once

// std
#include <memory>

// classes
#include "DWF_Buffer.h"
#include "DWF_Sound.h"

// miniaudio
#include <miniaudio.h>

namespace DWF_Audio
{
    /**
    * MiniAudio based sound player
    *@author Jean-Milost Reymond
    */
    class Sound_MiniAudio : public Sound
    {
        public:
            Sound_MiniAudio();
            virtual ~Sound_MiniAudio();

            /**
            * Opens a sound from a file
            *@param fileName - file name
            *@return true on success, otherwise false
            */
            virtual bool Open(const std::wstring& fileName);

            /**
            * Opens a sound from a buffer
            *@param buffer - buffer containing the file
            *@return true on success, otherwise false
            */
            virtual bool Open(DWF_Buffer::Buffer& buffer);

            /**
            * Plays the sound
            *@return true on success, otherwise false
            */
            virtual bool Play();

            /**
            * Pauses the sound
            *@return true on success, otherwise false
            */
            virtual bool Pause();

            /**
            * Stops the sound
            *@return true on success, otherwise false
            */
            virtual bool Stop();

            /**
            * Checks if the sound is currently playing
            *@return true on success, otherwise false
            */
            virtual bool IsPlaying();

            /**
            * Changes the sound pitch
            *@param value - new pitch value
            *@return true on success, otherwise false
            */
            virtual bool ChangePitch(float value);

            /**
            * Changes the sound volume
            *@param value - new volume value
            *@return true on success, otherwise false
            */
            virtual bool ChangeVolume(float value);

            /**
            * Changes the sound minimum volume limit
            *@param value - new minimum volume limit value
            *@return true on success, otherwise false
            */
            virtual bool ChangeVolumeMin(float value);

            /**
            * Changes the sound maximum volume limit
            *@param value - new maximum volume limit value
            *@return true on success, otherwise false
            */
            virtual bool  ChangeVolumeMax(float value);

            /**
            * Sets the sound source position in 3D environment (e.g. sound can be played on the left)
            *@param pos - sound source position
            *@return true on success, otherwise false
            */
            virtual bool ChangePosition(const DWF_Math::Vector3F& pos);

            /**
            * Loops the sound when end is reached
            *@param value - if true, sound will be looped on end reached, otherwise sound will be stopped
            *@return true on success, otherwise false
            */
            virtual bool Loop(bool value);

        private:
            ma_decoder                       m_Decoder = { 0 };
            ma_device                        m_Device  = { 0 };
            std::unique_ptr<unsigned char[]> m_pBuffer;
            bool                             m_Opened  = false;

            /**
            * Release the sound
            */
            void Release();

            /**
            * Decode data callback
            *@param pDevice - sound device
            *@param pOutput - decoded data
            *@param pInput - data to decode
            *@param frameCount - frame count
            */
            static void DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    };
}
