using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;

namespace SimpleGameAssetInterface.WPF
{
    /// <summary>
    /// Classe utilitaire permettant d'animer le HorizontalOffset d'un ScrollViewer en WPF.
    /// </summary>
    public static class ScrollAnimator
    {
        // Enregistrement de la propriété attachée magique pour le système d'animation de WPF
        public static readonly DependencyProperty ScrollOffsetProperty =
            DependencyProperty.RegisterAttached(
                "ScrollOffset",
                typeof(double),
                typeof(ScrollAnimator),
                new FrameworkPropertyMetadata(0.0, OnScrollOffsetChanged));

        public static double GetScrollOffset(DependencyObject obj) => (double)obj.GetValue(ScrollOffsetProperty);
        public static void SetScrollOffset(DependencyObject obj, double value) => obj.SetValue(ScrollOffsetProperty, value);

        /// <summary>
        /// Méthode déclenchée à chaque micro-étape de l'animation pour déplacer le scroll.
        /// </summary>
        private static void OnScrollOffsetChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is ScrollViewer scrollViewer)
            {
                scrollViewer.ScrollToHorizontalOffset((double)e.NewValue);
            }
        }

        /// <summary>
        /// Lance l'animation fluide sur le ScrollViewer spécifié.
        /// </summary>
        public static void AnimateScroll(ScrollViewer scrollViewer, DoubleAnimation animation)
        {
            // On cale la valeur de départ sur la position actuelle du scroll
            scrollViewer.SetValue(ScrollOffsetProperty, scrollViewer.HorizontalOffset);

            // On demande à WPF de faire varier notre propriété virtuelle de manière fluide
            scrollViewer.BeginAnimation(ScrollOffsetProperty, animation);
        }
    }
}
