/****************************************************************************
 * ==> WPFGameInfo ---------------------------------------------------------*
 ****************************************************************************
 * Description: The game info view model.                                   *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.Collections.ObjectModel;
using System.Windows.Media.Imaging;
using SimpleGameAssetInterface.Core;

namespace SimpleGameAssetInterface.MVVM
{
    /// <summary>
    /// Game info view model.
    /// </summary>
    public class WPFGameInfo : WPFViewModelBase
    {
        private readonly GameInfo              m_GameInfo;          // core model
        private          BitmapImage?          m_GameArtworkSource; // artwork cache for WPF interface
        private readonly WPFGameSourceCodeInfo m_SourceCodeInfo;

        /// <summary>
        /// Gets or sets the game title.
        /// </summary>
        public string Title
        {
            get => m_GameInfo.Title;

            set
            {
                // no change?
                if (m_GameInfo.Title == value)
                    return;

                m_GameInfo.Title = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Gets or sets the game description.
        /// </summary>
        public string Description
        {
            get => m_GameInfo.Description;

            set
            {
                // no change?
                if (m_GameInfo.Description == value)
                    return;

                m_GameInfo.Description = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Gets the game release info (composed by the editor and the year.
        /// </summary>
        public string ReleaseInfo { get => $"{m_GameInfo.Editor} • {m_GameInfo.ReleaseDate.Year.ToString()}"; }

        /// <summary>
        /// Gets the game artwork source.
        /// </summary>
        /// <remarks>This property is read-only. Use the Artwork property to update the artwork.</remarks>
        public BitmapImage? GameArtworkSource
        {
            get => m_GameArtworkSource;

            private set
            {
                m_GameArtworkSource = value;
                OnPropertyChanged();
            }
        }

        public bool HasArticles => Articles != null && Articles.Count > 0;

        public string ArticlesSummaryTitle => $"Press articles ({Articles?.Count ?? 0})";

        public string ArticlesSummarySubtitle
        {
            get
            {
                if (Articles == null || Articles.Count == 0)
                    return "No references - No local documents";

                // Compte les URLs valides/renseignées
                int referencesCount = Articles.Count(a => !string.IsNullOrWhiteSpace(a.URL));

                // Compte les fichiers locaux renseignés
                int localDocsCount = Articles.Count(a => !string.IsNullOrWhiteSpace(a.FileName));

                // Gestion du pluriel pour une finition professionnelle
                string refPlural = referencesCount > 1 ? "references"      : "reference";
                string docPlural = localDocsCount  > 1 ? "local documents" : "local document";

                return $"{referencesCount} {refPlural} • {localDocsCount} {docPlural}";
            }
        }

        public ObservableCollection<WPFGameArticleInfo> Articles { get; set; } = new ObservableCollection<WPFGameArticleInfo>();

        public bool HasSourceCode => Articles != null && Articles.Count > 0;

        public string SourceCodeSummaryTitle => $"Source code";

        public string SourceCodeSummarySubtitle
        {
            get
            {
                if (SourceCode == null || SourceCode.Empty())
                    return "No source code";

                string repository = SourceCode.Path.Length    == 0 ? (SourceCode.URL.Length == 0 ? "No repository" : "Distant repository") : "Local repository";
                string license    = SourceCode.License.Length == 0 ? "License to be verified" : SourceCode.License;

                return $"{repository} • {license}";
            }
        }

        public WPFGameSourceCodeInfo SourceCode => m_SourceCodeInfo;

        /// <summary>
        /// Gets the internal game info core model.
        /// </summary>
        public GameInfo Model => m_GameInfo;

        /// <summary>
        /// Default constructor, build a fake game info which may be used e.g. from design time (or used to create a debug json).
        /// </summary>
        /// <exception cref="System.IO.InvalidDataException">Thrown if the artwork could not be loaded.</exception>
        public WPFGameInfo()
        {
            GameInfo gameInfo    = new GameInfo("Agony");
            gameInfo.Description = "Acanthropsis discovers Cosmic Power after years of research, realizing the fatal cost is his life and deciding to pass his knowledge to a worthy successor. He devises a test for his two skilled apprentices, Alestes and Mentor, hoping one will fail.\r\n\r\nYou control Alestes transformed into a mystical owl, battling through six dangerous levels to reach the secret of Cosmic Power while fighting Mentor's creatures and collecting helpful spells and potions.";
            gameInfo.Editor      = "Psygnosys";
            gameInfo.ReleaseDate = new DateOnly(1992, 1, 1);

            // is code executed in design time or in runtime?
            string processName = System.Diagnostics.Process.GetCurrentProcess().ProcessName.ToLower();
            bool   designTime  = processName.Contains("wpfsurface") ||
                                 processName.Contains("devenv")     ||
                                 processName.Contains("xdproc");

            // load the image only in runtime
            if (!designTime)
            {
                string artworkFileName = "..\\..\\..\\..\\Common\\Assets\\Images\\Agony_Coverart.jpg";

                if (!gameInfo.Artwork.Load(artworkFileName))
                    throw new System.IO.InvalidDataException($"Failed to import game data.");

                GameArtworkSource = gameInfo.Artwork.GetBitmap();
            }

            m_GameInfo = gameInfo;

            // agony game dev article
            var agonyDevArticle = new GameArticleInfo { URL = "https://francksauer.com/index.php/games?view=article&id=10:agony&catid=15:published-games" };
            Articles.Add(new WPFGameArticleInfo(agonyDevArticle));
            m_GameInfo.Articles.Add(agonyDevArticle);

            // agony game review article
            var agonyReviewArticle = new GameArticleInfo { URL = "https://www.lemonamiga.com/game/agony" };
            Articles.Add(new WPFGameArticleInfo(agonyReviewArticle));
            m_GameInfo.Articles.Add(agonyReviewArticle);

            // agony source code
            m_GameInfo.SourceCode.URL     = "https://aminet.net/package/game/shoot/YvesGrolet-sources";
            m_GameInfo.SourceCode.Path    = "..\\..\\..\\..\\Common\\Source codes\\YvesGrolet-sources.zip";
            m_GameInfo.SourceCode.License = "Public domain";
            m_SourceCodeInfo              = new WPFGameSourceCodeInfo(m_GameInfo.SourceCode);
        }

        /// <summary>
        /// Constructor allowing to build a game info from an existing core model.
        /// </summary>
        /// <param name="gameInfo">Core model containing the game info data.</param>
        public WPFGameInfo(GameInfo gameInfo)
        {
            m_GameInfo        = gameInfo;
            GameArtworkSource = m_GameInfo.Artwork.GetBitmap();
            m_SourceCodeInfo  = new WPFGameSourceCodeInfo(m_GameInfo.SourceCode);
        }

        /// <summary>
        /// Constructor allowing to load a game info from a json file.
        /// </summary>
        /// <param name="fileName">Json file name to load.</param>
        /// <exception cref="ArgumentException">Thrown if the file name is empty or invalid.</exception>
        /// <exception cref="System.IO.InvalidDataException">Thrown if the data failed to load from the json file.</exception>
        public WPFGameInfo(string fileName)
        {
            if (string.IsNullOrWhiteSpace(fileName))
                throw new ArgumentException("File name cannot be empty", nameof(fileName));

            GameInfo? gameInfo = GameInfo.ImportFromJSON(fileName);

            if (gameInfo == null)
                throw new System.IO.InvalidDataException($"Failed to import game data - file name - {fileName}");

            m_GameInfo        = gameInfo;
            GameArtworkSource = m_GameInfo.Artwork.GetBitmap();
            m_SourceCodeInfo  = new WPFGameSourceCodeInfo(m_GameInfo.SourceCode);
        }
    }
}
