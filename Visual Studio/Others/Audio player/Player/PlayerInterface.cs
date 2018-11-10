/***********************************************************
 * Interface to implement media player classes             *
 * @Author - Jean-Milost Reymond                           *
 ***********************************************************/

namespace Player
{
    interface PlayerInterface
    {
        #region Properties

        /// <summary>
        /// Get or set volume
        /// </summary>
        int Volume
        {
            get;
            set;
        }

        /// <summary>
        /// Get or set seek time
        /// </summary>
        double Seek
        {
            get;
            set;
        }

        #endregion

        #region Functions prototypes

        /// <summary>
        /// Play track
        /// </summary>
        /// <returns>true if success, otherwise false</returns>
        bool Play();

        /// <summary>
        /// Pause track
        /// </summary>
        /// <returns>true if success, otherwise false</returns>
        bool Pause();

        /// <summary>
        /// Stop track
        /// </summary>
        /// <returns>true if success, otherwise false</returns>
        bool Stop();

        #endregion
    }
}
