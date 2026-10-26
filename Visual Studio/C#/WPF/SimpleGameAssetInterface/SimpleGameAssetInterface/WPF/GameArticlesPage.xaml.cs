/****************************************************************************
 * ==> GameArticlesPage ----------------------------------------------------*
 ****************************************************************************
 * Description: The game articles page, it exposes the known press articles *
 *              related to the game.                                        *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

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
