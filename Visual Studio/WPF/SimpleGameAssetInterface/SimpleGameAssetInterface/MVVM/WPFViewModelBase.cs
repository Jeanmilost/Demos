/****************************************************************************
 * ==> WPFViewModelBase ----------------------------------------------------*
 ****************************************************************************
 * Description: The base class for any view model.                          *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace SimpleGameAssetInterface.MVVM
{
    /// <summary>
    /// WPF view model base class.
    /// </summary>
    public class WPFViewModelBase : INotifyPropertyChanged
    {
        /// <summary>
        /// Raises the PropertyChanged event.
        /// </summary>
        public event PropertyChangedEventHandler? PropertyChanged;

        /// <summary>
        /// Called when a property changed and the WPF interface should be refreshed.
        /// </summary>
        /// <param name="propertyName">Property name to refresh.</param>
        protected void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            var handler = PropertyChanged;

            if (handler == null)
                return;

            // get the WPF application dispatcher
            var dispatcher = System.Windows.Application.Current?.Dispatcher;

            // was property changed from same thread as WPF interface?
            if (dispatcher != null && !dispatcher.CheckAccess())
                // dispatch the event asynchronously
                dispatcher.BeginInvoke(new Action(() => handler.Invoke(this, new PropertyChangedEventArgs(propertyName))));
            else
                // dispatch the event synchronously
                handler.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
