/****************************************************************************
 * ==> GameArticlesPage ----------------------------------------------------*
 ****************************************************************************
 * Description: The game articles page, it exposes the known press articles *
 *              related to the game.                                        *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.Windows;
using System.Windows.Controls;
using SimpleGameAssetInterface.MVVM;

namespace SimpleGameAssetInterface.WPF
{
    /// <summary>
    /// Interaction logic for GameArticlePage.xaml
    /// </summary>
    public partial class GameArticlesPage : Page
    {
        public GameArticlesPage()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Called when the user clicks on a specific article item.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        /*REM
        private void OnArticleClick(object sender, EventArgs e)
        {
            // the sender is the graphic component on which the user clicked
            if (sender is GameArticleItem clickedItem)
            {
                // the item DataContext of an game article item automatically contains the item data object
                if (clickedItem.DataContext is WPFGameArticleInfo articleVM)
                {
                    string url = articleVM.URL;

                    if (string.IsNullOrWhiteSpace(url))
                        return;

                    try
                    {
                        // open the press article link in the default browser
                        System.Diagnostics.Process.Start(new System.Diagnostics.ProcessStartInfo
                        {
                            FileName        = url,
                            UseShellExecute = true
                        });
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Trace.TraceError("Could not open article URL - {0} - error - {1}", url, ex.Message);
                    }
                }
            }
        }
        */

        /// <summary>
        /// Called when the user clicked on the back button.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void BackBtn_Click(object sender, EventArgs e)
        {
            // get main window
            var mainWindow = Window.GetWindow(this) as MainWindow;

            // open the main page
            mainWindow?.ScrollToPage(0);
        }
    }
}
