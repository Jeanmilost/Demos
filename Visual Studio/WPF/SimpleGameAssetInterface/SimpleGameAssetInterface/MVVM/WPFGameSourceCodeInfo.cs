/****************************************************************************
 * ==> WPFGameSourceCodeInfo -----------------------------------------------*
 ****************************************************************************
 * Description: The game source code info view model.                       *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using SimpleGameAssetInterface.Core;

namespace SimpleGameAssetInterface.MVVM
{
    /// <summary>
    /// Game source code info view model.
    /// </summary>
    public class WPFGameSourceCodeInfo : WPFViewModelBase
    {
        private readonly GameSourceCodeInfo m_GameSourceCodeInfo; // core model

        /// <summary>
        /// Gets or sets the source code URL.
        /// </summary>
        public string URL
        {
            get => m_GameSourceCodeInfo.URL;

            set
            {
                // no change?
                if (m_GameSourceCodeInfo.URL == value)
                    return;

                m_GameSourceCodeInfo.URL = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Gets or sets the source code path.
        /// </summary>
        public string Path
        {
            get => m_GameSourceCodeInfo.Path;

            set
            {
                // no change?
                if (m_GameSourceCodeInfo.Path == value)
                    return;

                m_GameSourceCodeInfo.Path = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Gets or sets the source code license.
        /// </summary>
        public string License
        {
            get => m_GameSourceCodeInfo.License;

            set
            {
                // no change?
                if (m_GameSourceCodeInfo.License == value)
                    return;

                m_GameSourceCodeInfo.License = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Gets the internal game source code info core model.
        /// </summary>
        public GameSourceCodeInfo Model => m_GameSourceCodeInfo;

        /// <summary>
        /// Default constructor.
        /// </summary>
        WPFGameSourceCodeInfo()
        {
            m_GameSourceCodeInfo = new GameSourceCodeInfo();
        }

        /// <summary>
        /// Constructor allowing to build a game source code info from an existing core model.
        /// </summary>
        /// <param name="gameInfo">Core model containing the game source code info data.</param>
        public WPFGameSourceCodeInfo(GameSourceCodeInfo gameSourceCodeInfo)
        {
            m_GameSourceCodeInfo = gameSourceCodeInfo;
        }

        /// <summary>
        /// Gets if the game code source info is empty.
        /// </summary>
        /// <returns>true if if the game code source info is empty, otherwise false.</returns>
        public bool Empty()
        {
            return m_GameSourceCodeInfo.URL.Length     == 0 &&
                   m_GameSourceCodeInfo.Path.Length    == 0 &&
                   m_GameSourceCodeInfo.License.Length == 0;
        }
    }
}
