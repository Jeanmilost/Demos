#define ExportToJSON

using SimpleGameAssetInterface.MVVM;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;

namespace SimpleGameAssetInterface
{
    /// <summary>
    /// Interaction logic for GameAsset.xaml
    /// </summary>
    public partial class GameAsset : Page
    {
        /// <summary>
        /// Default constructor.
        /// </summary>
        public GameAsset()
        {
            InitializeComponent();

            bool designTime = System.ComponentModel.DesignerProperties.GetIsInDesignMode(this);

            if (designTime)
                return;

            try
            {
            #if ExportToJSON
                WPFGameInfo gameInfo = new WPFGameInfo();
                DataContext          = gameInfo;

                gameInfo.Model.ExportToJSON("Agony.json");
            #else
                DataContext = new WPFGameInfo("Agony.json");
            #endif
            }
            catch (Exception ex)
            {
                Trace.TraceError("Exception while loading the game data - error - {0}", ex.Message);
            }
        }

        /// <summary>
        /// Called when the used clicked on the articles item.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ArticlesItem_Click(object sender, EventArgs e)
        {}

        /// <summary>
        /// Called when the used clicked on the source code item.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void SourceCodeItem_Click(object sender, EventArgs e)
        {}

        /*REM FIXME
        private void ButtonArticle_Click(object sender, RoutedEventArgs e)
        {
            // Récupération de l'article lié au bouton cliqué
            var button = sender as Button;
            var articleViewModel = button?.DataContext as WPFGameArticleInfo;

            if (articleViewModel != null)
            {
                // On cherche sa position dans la liste
                var mainViewModel = this.DataContext as WPFGameInfo;
                int articleIndex = mainViewModel.Articles.IndexOf(articleViewModel);

                // L'index de la page cible sera l'index de l'article + 1
                // (car l'index 0 est occupé par le menu/la page de présentation globale)
                //ScrollToPage(articleIndex + 1);
            }
        }
        */
    }
}
