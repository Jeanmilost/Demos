/****************************************************************************
 * ==> WPFGameArticleInfo --------------------------------------------------*
 ****************************************************************************
 * Description: The game article info view model.                           *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using SimpleGameAssetInterface.Core;

namespace SimpleGameAssetInterface.MVVM
{
    /// <summary>
    /// Game article info view model.
    /// </summary>
    public class WPFGameArticleInfo : WPFViewModelBase
    {
        private readonly GameArticleInfo m_GameArticleInfo; // core model

        /// <summary>
        /// Gets or sets the article URL.
        /// </summary>
        public string URL
        {
            get => m_GameArticleInfo.URL;

            set
            {
                // no change?
                if (m_GameArticleInfo.URL == value)
                    return;

                m_GameArticleInfo.URL = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Gets or sets the article file name.
        /// </summary>
        public string FileName
        {
            get => m_GameArticleInfo.FileName;

            set
            {
                // no change?
                if (m_GameArticleInfo.FileName == value)
                    return;

                m_GameArticleInfo.FileName = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Gets the internal game article info core model.
        /// </summary>
        public GameArticleInfo Model => m_GameArticleInfo;

        /// <summary>
        /// Default constructor.
        /// </summary>
        WPFGameArticleInfo()
        {
            m_GameArticleInfo = new GameArticleInfo();
        }

        /// <summary>
        /// Constructor allowing to build a game article info from an existing core model.
        /// </summary>
        /// <param name="gameInfo">Core model containing the game article info data.</param>
        public WPFGameArticleInfo(GameArticleInfo gameArticleInfo)
        {
            m_GameArticleInfo = gameArticleInfo;
        }
    }
}
