/****************************************************************************
 * ==> ScrollAnimator ------------------------------------------------------*
 ****************************************************************************
 * Description: Helper class to scroll between pages onto the main          *
 *              interface.                                                  *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;

namespace SimpleGameAssetInterface.WPF
{
    /// <summary>
    /// Helper class to scroll between pages onto the main interface.
    /// </summary>
    public static class ScrollAnimator
    {
        /// <summary>
        /// Scroll offset property
        /// </summary>
        /// <remarks>Registers the magic property for the WPF animation system.</remarks>
        public static readonly DependencyProperty ScrollOffsetProperty = DependencyProperty.RegisterAttached
                (
                    "ScrollOffset",
                    typeof(double),
                    typeof(ScrollAnimator),
                    new FrameworkPropertyMetadata(0.0, OnScrollOffsetChanged)
                );

        /// <summary>
        /// Gets the scroll offset
        /// </summary>
        /// <param name="obj">Dependency object.</param>
        /// <returns></returns>
        public static double GetScrollOffset(DependencyObject obj) => (double)obj.GetValue(ScrollOffsetProperty);

        /// <summary>
        /// Sets the scroll offset
        /// </summary>
        /// <param name="obj">Dependency object.</param>
        /// <param name="value">Scroll offset value.</param>
        public static void SetScrollOffset(DependencyObject obj, double value) => obj.SetValue(ScrollOffsetProperty, value);

        /// <summary>
        /// Starts the scroll animation.
        /// </summary>
        /// <param name="scrollViewer">Viewert on which the scroll will be performed.</param>
        /// <param name="animation">Scroll animation.</param>
        public static void AnimateScroll(ScrollViewer scrollViewer, DoubleAnimation animation)
        {
            // calculate the start value from the current scroll position
            scrollViewer.SetValue(ScrollOffsetProperty, scrollViewer.HorizontalOffset);

            // call WPF in order to apply the virtual property in a fluid manner
            scrollViewer.BeginAnimation(ScrollOffsetProperty, animation);
        }

        /// <summary>
        /// Called when the scroll offset changed.
        /// </summary>
        /// <param name="d">Dependency object.</param>
        /// <param name="e">Dependency object event arguments.</param>
        private static void OnScrollOffsetChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is ScrollViewer scrollViewer)
                scrollViewer.ScrollToHorizontalOffset((double)e.NewValue);
        }
    }
}
