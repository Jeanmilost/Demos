/****************************************************************************
 * ==> MainWindow ----------------------------------------------------------*
 ****************************************************************************
 * Description: The game main window.                                       *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Animation;
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
            GameInfoPage.UpdateLayout();
            GameSourceCodePage.UpdateLayout();
            GameArticlesPage.UpdateLayout();

            // wait for the main page to finish to initialize its ViewModel
            if (GameInfoPage.DataContext is WPFGameInfo sharedViewModel)
            {
                // extract the source code sub-model and bind the second page
                GameSourceCodePage.DataContext = sharedViewModel.SourceCode;

                // extract the articles sub-models and bind the pages
                GameArticlesPage.DataContext = sharedViewModel;
            }
        }

        /// <summary>
        /// Scrolls the main interface between the current page to the target page at index.
        /// </summary>
        /// <param name="targetIndex">Page index to scroll to.</param>
        public void ScrollToPage(int targetIndex)
        {
            // get the page visible height and calculate the final target offset
            double pageWidth         = MainScrollViewer.ActualWidth;
            double currentOffset     = MainScrollViewer.HorizontalOffset;
            double finalTargetOffset = pageWidth * targetIndex;

            // already on the page to reach?
            if (Math.Abs(currentOffset - finalTargetOffset) < 1)
                return;

            int currentIndex = (int)Math.Round(currentOffset / pageWidth);

            // aren't the start and target pages side by side?
            if (Math.Abs(targetIndex - currentIndex) > 1)
            {
                // identify the both start and target visual components
                FrameworkElement? currentControl = currentIndex == 0 ? GameInfoPage : (currentIndex == 1 ? GameSourceCodePage : GameArticlesPage);
                FrameworkElement? targetControl  = targetIndex  == 0 ? GameInfoPage : (targetIndex  == 1 ? GameSourceCodePage : GameArticlesPage);

                // found them?
                if (currentControl != null && targetControl != null)
                {
                    // create texture brushes that captures the exact rendering of the 2 pages
                    VisualBrush currentBrush = new(currentControl) { Stretch = Stretch.UniformToFill };
                    VisualBrush targetBrush  = new(targetControl)  { Stretch = Stretch.UniformToFill };

                    CurrentPageCapture.Fill = currentBrush;
                    TargetPageCapture.Fill  = targetBrush;

                    // do scroll on the left?
                    if (targetIndex > currentIndex)
                    {
                        // go forward (0 -> 2). Start photo is located at index 0, the target photo is located on its right
                        Canvas.SetLeft(CurrentPageCapture, 0);
                        Canvas.SetLeft(TargetPageCapture, pageWidth);

                        // perform the canvas animation to the left
                        AnimateCanvasTransition(0, -pageWidth, targetIndex);
                    }
                    else
                    {
                        // go backward (2 -> 0). Start photo is located at index 0, the target photo is located on its left
                        Canvas.SetLeft(CurrentPageCapture, 0);
                        Canvas.SetLeft(TargetPageCapture, -pageWidth);

                        // perform the canvas animation to the right
                        AnimateCanvasTransition(0, pageWidth, targetIndex);
                    }

                    return;
                }
            }

            // start and target page are side by side, use the classic ScrollViewer
            DoubleAnimation scrollAnimation = new()
            {
                From           = MainScrollViewer.HorizontalOffset,
                To             = finalTargetOffset,
                Duration       = TimeSpan.FromSeconds(0.4),
                EasingFunction = new CubicEase { EasingMode = EasingMode.EaseInOut }
            };

            ScrollAnimator.AnimateScroll(MainScrollViewer, scrollAnimation);
        }

        /// <summary>
        /// Animates the canvas transition between distant pages.
        /// </summary>
        /// <param name="from">From position on the X axis.</param>
        /// <param name="to">To position on the X axis.</param>
        /// <param name="finalTargetIndex">Final target index.</param>
        private void AnimateCanvasTransition(double from, double to, int finalTargetIndex)
        {
            double pageWidth = MainScrollViewer.ActualWidth;

            // calculate the overlay transition start point
            TranslateTransform canvasTransform = new(from, 0);
            TransitionCanvas.RenderTransform   = canvasTransform;

            // turn the canvas transition overlay visible above the ScrollViewer
            TransitionCanvas.Visibility = Visibility.Visible;

            DoubleAnimation canvasAnimation = new()
            {
                From           = from,
                To             = to,
                Duration       = TimeSpan.FromSeconds(0.4),
                EasingFunction = new CubicEase { EasingMode = EasingMode.EaseInOut }
            };

            // as soon as the photo slide ends, hide the overlay
            canvasAnimation.Completed += (s, e) =>
            {
                // hide the canvas
                TransitionCanvas.Visibility = Visibility.Collapsed;
                CurrentPageCapture.Fill     = null;
                TargetPageCapture.Fill      = null;

                // move instantly and silently the ScrollViever current page to the target page
                MainScrollViewer.ScrollToHorizontalOffset(pageWidth * finalTargetIndex);
                MainScrollViewer.UpdateLayout();
            };

            canvasTransform.BeginAnimation(TranslateTransform.XProperty, canvasAnimation);
        }
    }
}
