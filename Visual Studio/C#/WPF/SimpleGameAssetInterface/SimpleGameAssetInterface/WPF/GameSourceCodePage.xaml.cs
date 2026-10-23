/****************************************************************************
 * ==> GameSourceCodePage --------------------------------------------------*
 ****************************************************************************
 * Description: The game source code page, it exposes the game source code  *
 *              resources.                                                  *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using SimpleGameAssetInterface.MVVM;

namespace SimpleGameAssetInterface.WPF
{
    /// <summary>
    /// Interaction logic for GameSourceCodePage.xaml
    /// </summary>
    public partial class GameSourceCodePage : Page
    {
        public GameSourceCodePage()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Called when the user clicked on the open URL button.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void URLBtn_Click(object sender, EventArgs e)
        {
            // extract the ViewModel binded to the page from the DataContext
            if (DataContext is WPFGameSourceCodeInfo sourceCodeVM)
            {
                // get the url to open from the model
                Uri url = new(sourceCodeVM.URL);

                // check if url is not empty
                if (string.IsNullOrWhiteSpace(url.ToString()))
                    return;

                try
                {
                    var processStartInfo = new ProcessStartInfo
                    {
                        FileName        = url.ToString(),
                        UseShellExecute = true // required in modern .NET for the URLs
                    };

                    // open the url
                    Process.Start(processStartInfo);
                }
                catch (Exception ex)
                {
                    Trace.TraceError("Could not open the URL - {0} - error - {1}", url, ex.Message);
                }
            }
        }

        /// <summary>
        /// Called when the user clicked on the open path button.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PathBtn_Click(object sender, EventArgs e)
        {
            // extract the ViewModel binded to the page from the DataContext
            if (DataContext is WPFGameSourceCodeInfo sourceCodeVM)
            {
                // get the folder to open from the model
                string folder = sourceCodeVM.Path;

                // check if path is not empty
                if (string.IsNullOrWhiteSpace(folder))
                {
                    Trace.TraceError("Could not open the folder - path is empty");
                    return;
                }

                // is an existing folder?
                if (!Directory.Exists(folder))
                {
                    // not a folder, maybe a file?
                    string? extractedFolder = System.IO.Path.GetDirectoryName(folder);

                    // failed to extract folder?
                    if (extractedFolder == null)
                    {
                        Trace.TraceError("Could not open the folder - failed to extract the path");
                        return;
                    }

                    // could extract folder, but still not exists?
                    if (!Directory.Exists(extractedFolder))
                    {
                        Trace.TraceError("Could not open the folder - path does not exist - {0}", extractedFolder);
                        return;
                    }

                    folder = extractedFolder;
                }

                try
                {
                    var processStartInfo = new ProcessStartInfo
                    {
                        FileName        = folder,
                        UseShellExecute = true // required in modern .NET for the folders
                    };

                    // open the folder
                    Process.Start(processStartInfo);
                }
                catch (Exception ex)
                {
                    Trace.TraceError("Could not open the URL - {0} - error - {1}", folder, ex.Message);
                }
            }
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
