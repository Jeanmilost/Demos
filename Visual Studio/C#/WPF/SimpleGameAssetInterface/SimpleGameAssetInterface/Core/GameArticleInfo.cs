/****************************************************************************
 * ==> GameArticleInfo -----------------------------------------------------*
 ****************************************************************************
 * Description: Container owning the information about a press article      *
 *              related to a video game.                                    *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

namespace SimpleGameAssetInterface.Core
{
    /// <summary>
    /// Game press article information.
    /// </summary>
    public class GameArticleInfo
    {
        /// <summary>
        /// Distant URL where the press article was released.
        /// </summary>
        public string URL { get; set; } = string.Empty;

        /// <summary>
        /// Local file name on the computer where the press article was downloaded.
        /// </summary>
        /// <remarks>If empty, the press article is not available locally.</remarks>
        public string FileName { get; set; } = string.Empty;
    }
}
