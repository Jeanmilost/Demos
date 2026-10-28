/****************************************************************************
 * ==> WPFGameArticleInfo --------------------------------------------------*
 ****************************************************************************
 * Description: The game article info view model.                           *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.Windows;
using System.Windows.Media.Imaging;
using SimpleGameAssetInterface.Core;

namespace SimpleGameAssetInterface.MVVM
{
    /// <summary>
    /// Game article info view model.
    /// </summary>
    public class WPFGameArticleInfo : WPFViewModelBase
    {
        private readonly GameArticleInfo m_GameArticleInfo;      // core model
        private          BitmapImage?    m_ArticleArtworkSource; // artwork cache for WPF interface

        /// <summary>
        /// Gets or sets the article title.
        /// </summary>
        public string Title
        {
            get => m_GameArticleInfo.Title;

            set
            {
                // no change?
                if (m_GameArticleInfo.Title == value)
                    return;

                m_GameArticleInfo.Title = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Gets the article artwork source.
        /// </summary>
        /// <remarks>This property is read-only. Use the Artwork property to update the artwork.</remarks>
        public BitmapImage? ArticleArtworkSource
        {
            get => m_ArticleArtworkSource;

            private set
            {
                m_ArticleArtworkSource = value;
                OnPropertyChanged();
            }
        }

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
        /// Gets if local article is available.
        /// </summary>
        public Visibility HasLocalArticle
        {
            get
            {
                return m_GameArticleInfo.FileName.Length == 0 ? Visibility.Collapsed : Visibility.Visible;
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
            m_GameArticleInfo    = gameArticleInfo;
            ArticleArtworkSource = m_GameArticleInfo.Artwork.GetBitmap();
        }
    }
}
