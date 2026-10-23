/****************************************************************************
 * ==> MainWindow ----------------------------------------------------------*
 ****************************************************************************
 * Description: The game main window.                                       *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.Windows;
using SimpleGameAssetInterface.MVVM;
using SimpleGameAssetInterface.WPF;

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

            // force the initial graphics component frames loading
            InfoPage.UpdateLayout();
            SourceCodePage.UpdateLayout();

            // wait for the main page to finish to initialize its ViewModel
            if (InfoPage.DataContext is WPFGameInfo sharedViewModel)
            {
                // extract the source code sub-model and bind the second page
                SourceCodePage.DataContext = sharedViewModel.SourceCode;

                // extract the articles sub-models and bind the pages
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
