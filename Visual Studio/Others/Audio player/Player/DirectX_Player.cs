/***********************************************************
 * Media player based on DirectX system                    *
 * @Author - Jean-Milost Reymond                           *
 ***********************************************************/

using System;
using System.IO;
using Microsoft.DirectX.AudioVideoPlayback;

namespace Player
{
    class DirectX_Player : PlayerInterface
    {
        #region Class variables
        
        private Audio m_Audio = null; // audio object containing DirectPlay functions
        
        #endregion

        #region Properties

        /// <summary>
        /// Get or set volume
        /// </summary>
        public int Volume
        {
            get
            {
                return m_Audio.Volume;
            }

            set
            {
                m_Audio.Volume = value;
            }
        }

        /// <summary>
        /// Get or set seek time
        /// </summary>
        public double Seek
        {
            get
            {
                return m_Audio.CurrentPosition;
            }

            set
            {
                m_Audio.CurrentPosition = value;
            }
        }

        #endregion

        #region Functions

        #region Public functions

        /// <summary>
        /// Load track into player
        /// </summary>
        /// <param name="fileName">File name conaining track</param>
        /// <returns>True if success, otherwise false</returns>
        public bool LoadTrack(string fileName)
        {
            try
            {
                // dispose audio manager before create another
                if (m_Audio != null)
                    m_Audio.Dispose();

                // check if file exists
                if (!File.Exists(fileName))
                    return false;

                // initialize direct audio module
                m_Audio = new Audio(fileName);

                return true;
            }
            catch(Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Get track total time in seconds
        /// </summary>
        /// <returns>Track total time in seconds</returns>
        public double GetTotalTime()
        {
            return m_Audio.Duration;
        }

        #endregion

        #region Inherited from interface

        /// <summary>
        /// Play track
        /// </summary>
        /// <returns>True if success, otherwise false</returns>
        public bool Play()
        {
            try
            {
                // attempt to execute play command
                m_Audio.Play();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Pause playback
        /// </summary>
        /// <returns>True if success, otherwise false</returns>
        public bool Pause()
        {
            try
            {
                // attempt to execue pause command
                m_Audio.Pause();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Stop playback
        /// </summary>
        /// <returns>True if success, otherwise false</returns>
        public bool Stop()
        {
            try
            {
                // attempt to execute stop command
                m_Audio.Stop();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        #endregion

        #endregion
    }
}
