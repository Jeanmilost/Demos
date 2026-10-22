/****************************************************************************
 * ==> GameSourceCodeInfo --------------------------------------------------*
 ****************************************************************************
 * Description: Container owning the information about a source code        *
 *              related to a video game.                                    *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

namespace SimpleGameAssetInterface.Core
{
    /// <summary>
    /// Game source code information.
    /// </summary>
    public class GameSourceCodeInfo
    {
        /// <summary>
        /// Distant URL of the repository or resource containing the source code.
        /// </summary>
        public string URL { get; set; } = string.Empty;

        /// <summary>
        /// Local path on the computer where the source code is available.
        /// </summary>
        /// <remarks>If empty, the source code is not available locally.</remarks>
        public string Path { get; set; } = string.Empty;

        /// <summary>
        /// The code source license
        /// </summary>
        public string License { get; set; } = string.Empty;
    }
}
