/****************************************************************************
 * ==> MainWindow ----------------------------------------------------------*
 ****************************************************************************
 * Description: The game main window.                                       *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using SimpleGameAssetInterface.MVVM;
using SimpleGameAssetInterface.WPF;
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

namespace SimpleGameAssetInterface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        /// <summary>
        /// Default constructor.
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();

            // Force le chargement graphique initial des composants des frames
            InfoPage.UpdateLayout();
            SourceCodePage.UpdateLayout();

            // On attend que la page principale ait fini d'initialiser son ViewModel
            if (InfoPage.DataContext is WPFGameInfo sharedViewModel)
            {
                // On extrait le sous-modèle du code source et on le transmet à la deuxième page
                SourceCodePage.DataContext = sharedViewModel.SourceCode;

                // Bonus : Si vous voulez que vos articles partagent aussi ce contexte globale :
                //ArticlesPage.DataContext = sharedViewModel;
            }
        }

        /// <summary>
        /// Scroll the main interface to a given page.
        /// </summary>
        /// <param name="targetIndex">Page index to scroll to.</param>
        public void ScrollToPage(int targetIndex)
        {
            // get the page visible height
            double pageWidth         = MainScrollViewer.ActualWidth;
            double currentOffset     = MainScrollViewer.HorizontalOffset;
            double finalTargetOffset = pageWidth * targetIndex;

            // if already on the right page, ignore the call
            if (Math.Abs(currentOffset - finalTargetOffset) < 1)
                return;

            // make the scroll to be performed only on a single page
            if (finalTargetOffset > currentOffset)
                // go forward, move the offset to the previous page before the target one
                MainScrollViewer.ScrollToHorizontalOffset(finalTargetOffset - pageWidth);
            else
                // go backward, move the offset to the previous page after the target one
                MainScrollViewer.ScrollToHorizontalOffset(finalTargetOffset + pageWidth);

            // update the graphic layout before running the animation
            MainScrollViewer.UpdateLayout();

            // scroll on the final width of a single page
            System.Windows.Media.Animation.DoubleAnimation scrollAnimation = new()
            {
                From           = MainScrollViewer.HorizontalOffset,
                To             = finalTargetOffset,
                Duration       = TimeSpan.FromSeconds(0.5),
                EasingFunction = new System.Windows.Media.Animation.CubicEase { EasingMode = System.Windows.Media.Animation.EasingMode.EaseInOut }
            };

            // start animation on the ScrollAnimator attached property
            ScrollAnimator.AnimateScroll(MainScrollViewer, scrollAnimation);
        }
    }
}
