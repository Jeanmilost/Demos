#define ExportToJSON

using SimpleGameAssetInterface.MVVM;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;

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
        {}

        /// <summary>
        /// Called when the user clicked on the source code item.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void SourceCodeItem_Click(object sender, EventArgs e)
        {
            var mainWindow = Window.GetWindow(this) as MainWindow;

            mainWindow?.ScrollToPage(1);
        }
    }
}
