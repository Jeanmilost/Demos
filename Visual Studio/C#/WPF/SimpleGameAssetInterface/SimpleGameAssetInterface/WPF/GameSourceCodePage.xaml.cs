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
        { }

        /// <summary>
        /// Called when the user clicked on the open path button.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PathBtn_Click(object sender, EventArgs e)
        { }
    }
}
