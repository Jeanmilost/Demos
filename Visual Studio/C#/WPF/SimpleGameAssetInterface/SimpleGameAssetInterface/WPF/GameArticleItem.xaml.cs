/****************************************************************************
 * ==> GameArticleItem -----------------------------------------------------*
 ****************************************************************************
 * Description: The game press article item component.                      *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using SimpleGameAssetInterface.MVVM;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace SimpleGameAssetInterface.WPF
{
    /// <summary>
    /// Interaction logic for GameArticleItem.xaml
    /// </summary>
    public partial class GameArticleItem : UserControl
    {
        /// <summary>
        /// Registers a dependency property for the 'Icon' property.
        /// </summary>
        /// <remarks>This allows the 'Icon' field to support advanced WPF features like data binding, animations, and styles from external XAML views.</remarks>
        public static readonly DependencyProperty ArticleIconProperty =
                DependencyProperty.Register(nameof(ArticleIcon), typeof(ImageSource), typeof(GameArticleItem), new PropertyMetadata(null));

        /// <summary>
        /// Registers a dependency property for the 'Title' property.
        /// </summary>
        /// <Remarks>This allows the 'Title' field to support advanced WPF features like data binding, animations, and styles from external XAML views.</Remarks>
        public static readonly DependencyProperty ArticleTitleProperty =
                DependencyProperty.Register(nameof(ArticleTitle), typeof(string), typeof(GameArticleItem), new PropertyMetadata(string.Empty));

        /// <summary>
        /// Registers a dependency property for managing the visibility of the "Open Document" button.
        /// </summary>
        public static readonly DependencyProperty OpenDocVisibilityProperty =
                DependencyProperty.Register(nameof(OpenDocVisibility), typeof(Visibility), typeof(GameArticleItem), new PropertyMetadata(Visibility.Visible));

        /// <summary>
        /// Default constructor.
        /// </summary>
        public GameArticleItem()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Gets or sets the icon image source displayed on the left side.
        /// </summary>
        public ImageSource ArticleIcon
        {
            get => (ImageSource)GetValue(ArticleIconProperty);
            set => SetValue(ArticleIconProperty, value);
        }

        /// <summary>
        /// Gets or sets the component title.
        /// </summary>
        public string ArticleTitle
        {
            get => (string)GetValue(ArticleTitleProperty);
            set => SetValue(ArticleTitleProperty, value);
        }

        /// <summary>
        /// Gets or sets the open document button visibility.
        /// </summary>
        public Visibility OpenDocVisibility
        {
            get => (Visibility)GetValue(OpenDocVisibilityProperty);
            set => SetValue(OpenDocVisibilityProperty, value);
        }

        /// <summary>
        /// Called when the user clicks on the open URL button.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Mouse button event arguments.</param>
        private void OnOpenUrlClick(object sender, MouseButtonEventArgs e)
        {
            // prevent the button click to be sent to the parent item
            e.Handled = true;

            // the item DataContext of an game article item automatically contains the item data object
            if (DataContext is WPFGameArticleInfo articleVM)
            {
                string url = articleVM.URL;

                if (string.IsNullOrWhiteSpace(url))
                    return;

                try
                {
                    // open the press article link in the default browser
                    System.Diagnostics.Process.Start(new System.Diagnostics.ProcessStartInfo
                    {
                        FileName = url,
                        UseShellExecute = true
                    });
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Trace.TraceError("Could not open article URL - {0} - error - {1}", url, ex.Message);
                }
            }
        }

        /// <summary>
        /// Called when the user clicks on the open document button.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Mouse button event arguments.</param>
        private void OnOpenDocClick(object sender, MouseButtonEventArgs e)
        {
            // Prevent the button click from being bubbled up to the parent control
            e.Handled = true;

            if (DataContext is WPFGameArticleInfo articleVM)
            {
                string localFile = articleVM.FileName;

                if (string.IsNullOrWhiteSpace(localFile))
                {
                    System.Diagnostics.Trace.TraceError("Could not open article local document file - file is empty");
                    return;
                }

                try
                {
                    // convert relative path to full absolute path to avoid working directory issues
                    string absolutePath = System.IO.Path.GetFullPath(localFile);

                    if (!File.Exists(absolutePath))
                    {
                        System.Diagnostics.Trace.TraceError("Could not open article local document file - file does not exist - {0}", absolutePath);
                        return;
                    }

                    // open the local article document file
                    System.Diagnostics.Process.Start(new System.Diagnostics.ProcessStartInfo
                    {
                        FileName        = absolutePath,
                        UseShellExecute = true
                    });
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Trace.TraceError("Could not open article document file - {0} - error - {1}", localFile, ex.Message);
                }
            }
        }
    }
}
