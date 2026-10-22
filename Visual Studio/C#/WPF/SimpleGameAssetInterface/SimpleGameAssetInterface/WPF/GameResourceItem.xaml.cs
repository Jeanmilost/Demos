/****************************************************************************
 * ==> GameResourceItem ----------------------------------------------------*
 ****************************************************************************
 * Description: The game resource item component, it may represent a group  *
 *              of press articles, a source code, ... on the game info      *
 *              page.                                                       *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.Drawing;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace SimpleGameAssetInterface
{
    /// <summary>
    /// Interaction logic for GameResourceItem.xaml
    /// </summary>
    public partial class GameResourceItem : UserControl
    {
        /// <summary>
        /// Registers a dependency property for the 'Icon' property.
        /// </summary>
        /// <remarks>This allows the 'Icon' field to support advanced WPF features like data binding, animations, and styles from external XAML views.</remarks>
        public static readonly DependencyProperty LeftIconProperty =
                DependencyProperty.Register(nameof(LeftIcon), typeof(ImageSource), typeof(GameResourceItem), new PropertyMetadata(null));

        /// <summary>
        /// Registers a dependency property for the 'Title' property.
        /// </summary>
        /// <Remarks>This allows the 'Title' field to support advanced WPF features like data binding, animations, and styles from external XAML views.</Remarks>
        public static readonly DependencyProperty TitleProperty =
                DependencyProperty.Register(nameof(Title), typeof(string), typeof(GameResourceItem), new PropertyMetadata(string.Empty));

        /// <summary>
        /// Registers a dependency property for the 'Subtitle' property.
        /// </summary>
        /// <remarks>This allows the 'Subtitle' field to support advanced WPF features like data binding, animations, and styles from external XAML views.</remarks>
        public static readonly DependencyProperty SubtitleProperty =
                DependencyProperty.Register(nameof(Subtitle), typeof(string), typeof(GameResourceItem), new PropertyMetadata(string.Empty));

        /// <summary>
        /// Raised when the user clicks on the component.
        /// </summary>
        public event EventHandler? Click;

        /// <summary>
        /// Default constructor.
        /// </summary>
        public GameResourceItem()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Gets or sets the icon image source displayed on the left side.
        /// </summary>
        public ImageSource LeftIcon
        {
            get => (ImageSource)GetValue(LeftIconProperty);
            set => SetValue(LeftIconProperty, value);
        }

        /// <summary>
        /// Gets or sets the component title.
        /// </summary>
        public string Title
        {
            get => (string)GetValue(TitleProperty);
            set => SetValue(TitleProperty, value);
        }

        /// <summary>
        /// Gets or sets the component subtitle.
        /// </summary>
        public string Subtitle
        {
            get => (string)GetValue(SubtitleProperty);
            set => SetValue(SubtitleProperty, value);
        }

        /// <summary>
        /// Called when the user clicks on the component.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Mouse button event arguments.</param>
        private void UserControl_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Click?.Invoke(this, EventArgs.Empty);
        }
    }
}
