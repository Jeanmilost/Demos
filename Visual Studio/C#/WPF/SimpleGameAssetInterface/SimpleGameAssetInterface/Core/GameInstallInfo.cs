/****************************************************************************
 * ==> GameInstallInfo -----------------------------------------------------*
 ****************************************************************************
 * Description: Container owning the information about the local            *
 *              installation of a video game.                               *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;

namespace SimpleGameAssetInterface.Core
{
    /// <summary>
    /// Game installation information on local computer.
    /// </summary>
    public class GameInstallInfo
    {
        /// <summary>
        /// Distant URL where the game may be bought or get.
        /// </summary>
        public string URL { get; set; } = string.Empty;

        /// <summary>
        /// Local path on the computer where the game is installed.
        /// </summary>
        /// <remarks>If empty, the game is not installed on the computer.</remarks>
        public string Path { get; set; } = string.Empty;
    }
}
