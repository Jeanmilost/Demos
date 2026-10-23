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
using SimpleGameAssetInterface.WPF;

namespace SimpleGameAssetInterface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        public void ScrollToPage(int targetIndex)
        {
            // Récupère la largeur visible d'une page
            double pageWidth         = MainScrollViewer.ActualWidth;
            double currentOffset     = MainScrollViewer.HorizontalOffset;
            double finalTargetOffset = pageWidth * targetIndex;

            // Si on est déjà calé sur la bonne page, on ignore l'appel
            if (Math.Abs(currentOffset - finalTargetOffset) < 1) return;

            // --- SHORTCUT TELEPORTATION TRICK ---
            // Pour donner l'illusion d'un scroll sur une seule page de distance :
            if (finalTargetOffset > currentOffset)
            {
                // On avance : On téléporte le scroll instantanément 1 page AVANT la page visée
                MainScrollViewer.ScrollToHorizontalOffset(finalTargetOffset - pageWidth);
            }
            else
            {
                // On recule : On téléporte le scroll instantanément 1 page APRÈS la page visée
                MainScrollViewer.ScrollToHorizontalOffset(finalTargetOffset + pageWidth);
            }

            // Force la réactualisation de la disposition graphique avant de lancer l'animation
            MainScrollViewer.UpdateLayout();

            // Animation fluide sur la largeur finale d'une unique page
            System.Windows.Media.Animation.DoubleAnimation scrollAnimation = new System.Windows.Media.Animation.DoubleAnimation
            {
                From = MainScrollViewer.HorizontalOffset,
                To = finalTargetOffset,
                Duration = TimeSpan.FromSeconds(0.5),
                EasingFunction = new System.Windows.Media.Animation.CubicEase { EasingMode = System.Windows.Media.Animation.EasingMode.EaseInOut }
            };

            // Déclenchement de l'animation sur la propriété attachée de votre ScrollAnimator
            ScrollAnimator.AnimateScroll(MainScrollViewer, scrollAnimation);
        }
    }
}
