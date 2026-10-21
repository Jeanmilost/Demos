/****************************************************************************
 * ==> GameInfo ------------------------------------------------------------*
 ****************************************************************************
 * Description: Container owning the information about a video game.        *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using SimpleGameAssetInterface.Core;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace SimpleGameAssetInterface.Core
{
    /// <summary>
    /// Game information.
    /// </summary>
    public class GameInfo
    {
        private string             m_Title       = string.Empty;
        private string             m_Description = string.Empty;
        private GameImageAsset     m_Artwork;
        private GameInstallInfo    m_Installation;
        private GameSourceCodeInfo m_SourceCode;

        /// <summary>
        /// The game title.
        /// </summary>
        /// <exception cref="ArgumentException">Thrown if the <paramref name="value"/> is empty or whitespace only.</exception>
        public string Title
        {
            get => m_Title;
            set => m_Title = ValidateTitle(value);
        }

        /// <summary>
        /// The game description.
        /// </summary>
        public string Description { get; set; } = string.Empty;

        /// <summary>
        /// The game editor.
        /// </summary>
        public string Editor { get; set; } = string.Empty;

        /// <summary>
        /// The game release date.
        /// </summary>
        public DateOnly ReleaseDate { get; set; } = new DateOnly();

        /// <summary>
        /// The game artwork (may be the cover, a screenshot, ...).
        /// </summary>
        [JsonInclude]
        public GameImageAsset Artwork
        {
            get         => m_Artwork;
            private set => m_Artwork = value;
        }

        /// <summary>
        /// All information relative to the game installation on the local computer.
        /// </summary>
        [JsonInclude]
        public GameInstallInfo Installation
        {
            get         => m_Installation;
            private set => m_Installation = value;
        }

        /// <summary>
        /// All information relative to the game source code.
        /// </summary>
        [JsonInclude]
        public GameSourceCodeInfo SourceCode
        {
            get         => m_SourceCode;
            private set => m_SourceCode = value;
        }

        /// <summary>
        /// The press articles related to the game.
        /// </summary>
        public List<GameArticleInfo> Articles { get; } = new List<GameArticleInfo>();

        /// <summary>
        /// Initializes a new instance of the <see cref="GameInfo"/> class.
        /// </summary>
        /// <param name="title">The game title.</param>
        /// <exception cref="ArgumentException">Thrown if the <paramref name="title"/> is empty or whitespace only.</exception>
        [SetsRequiredMembers]
        public GameInfo(string title)
        {
            Title          = title;
            m_Artwork      = new GameImageAsset();
            m_Installation = new GameInstallInfo();
            m_SourceCode   = new GameSourceCodeInfo();
        }

        /// <summary>
        /// Exports all the game info content, including the sub assets, to a JSON file.
        /// </summary>
        /// <param name="fileName">JSON file name to export to.</param>
        /// <returns>true on success, otherwise false.</returns>
        public bool ExportToJSON(string fileName)
        {
            try
            {
                // if the previous log file exists, delete it to start fresh
                if (File.Exists(fileName))
                    File.Delete(fileName);

            #if DEBUG
                var    options    = new JsonSerializerOptions { WriteIndented = true };
                string jsonString = JsonSerializer.Serialize(this, options);
            #else
                string jsonString = JsonSerializer.Serialize(this);
            #endif

                File.WriteAllText(fileName, jsonString);
            }
            catch (IOException ex)
            {
                Trace.TraceError("Exception while exporting JSON data - {0} - error - {1}", fileName, ex.Message);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Imports all the game info content, including the sub assets, from a JSON file.
        /// </summary>
        /// <param name="fileName">JSON file name to import from.</param>
        /// <returns>true on success, otherwise false.</returns>
        public static GameInfo? ImportFromJSON(string fileName)
        {
            try
            {
                if (!File.Exists(fileName))
                {
                    Trace.TraceWarning("Import from JSON - file not found - {0}", fileName);
                    return null;
                }

                string jsonString = File.ReadAllText(fileName);

                return JsonSerializer.Deserialize<GameInfo>(jsonString);
            }
            catch (Exception ex)
            {
                Trace.TraceError("Exception while importing JSON data - {0} - error - {1}", fileName, ex.Message);
                return null;
            }
        }

        /// <summary>
        /// Validates the game title.
        /// </summary>
        /// <param name="title">Title to validate.</param>
        /// <returns>Validated title.</returns>
        /// <exception cref="ArgumentException">Thrown if the <paramref name="title"/> is empty or whitespace only.</exception>
        protected static string ValidateTitle(string title)
        {
            if (string.IsNullOrWhiteSpace(title))
                throw new ArgumentException("Title cannot be empty", nameof(title));

            return title;
        }
    }
}
