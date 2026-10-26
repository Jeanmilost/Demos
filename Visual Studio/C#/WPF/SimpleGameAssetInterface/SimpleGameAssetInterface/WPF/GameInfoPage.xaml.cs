/****************************************************************************
 * ==> GameInfoPage --------------------------------------------------------*
 ****************************************************************************
 * Description: The game info page, it exposes the game information and the *
 *              associated resources.                                       *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

//#define ExportToJSON

using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using SimpleGameAssetInterface.MVVM;

namespace SimpleGameAssetInterface.WPF
{
    /// <summary>
    /// Interaction logic for GameInfoPage.xaml
    /// </summary>
    public partial class GameInfoPage : Page
    {
        /// <summary>
        /// Default constructor.
        /// </summary>
        public GameInfoPage()
        {
            InitializeComponent();

            bool designTime = System.ComponentModel.DesignerProperties.GetIsInDesignMode(this);

            if (designTime)
                return;

            try
            {
            #if ExportToJSON
                WPFGameInfo gameInfo = new();
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
        /// Called when the user clicked on the articles item.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ArticlesItem_Click(object sender, EventArgs e)
        {
            // get main window
            var mainWindow = Window.GetWindow(this) as MainWindow;

            // open the articles page
            mainWindow?.ScrollToPage(2);
        }

        /// <summary>
        /// Called when the user clicked on the source code item.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void SourceCodeItem_Click(object sender, EventArgs e)
        {
            // get main window
            var mainWindow = Window.GetWindow(this) as MainWindow;

            // open the source code page
            mainWindow?.ScrollToPage(1);
        }
    }
}
