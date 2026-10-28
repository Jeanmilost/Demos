/****************************************************************************
 * ==> GameArticleInfo -----------------------------------------------------*
 ****************************************************************************
 * Description: Container owning the information about a press article      *
 *              related to a video game.                                    *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.Diagnostics.CodeAnalysis;
using System.Text.Json.Serialization;

namespace SimpleGameAssetInterface.Core
{
    /// <summary>
    /// Game press article information.
    /// </summary>
    public class GameArticleInfo
    {
        private GameImageAsset m_Artwork;

        /// <summary>
        /// The article title.
        /// </summary>
        public string Title { get; set; } = string.Empty;

        /// <summary>
        /// The article artwork.
        /// </summary>
        [JsonInclude]
        public GameImageAsset Artwork
        {
            get         => m_Artwork;
            private set => m_Artwork = value;
        }

        /// <summary>
        /// Distant URL where the press article was released.
        /// </summary>
        public string URL { get; set; } = string.Empty;

        /// <summary>
        /// Local file name on the computer where the press article was downloaded.
        /// </summary>
        /// <remarks>If empty, the press article is not available locally.</remarks>
        public string FileName { get; set; } = string.Empty;

        /// <summary>
        /// Default constructor.
        /// </summary>
        public GameArticleInfo()
        {
            m_Artwork = new GameImageAsset();
        }
    }
}
