using System.Diagnostics;
using System.IO;
using System.Windows;

namespace SimpleGameAssetInterface
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private readonly string m_LogFileName = "SimpleGameAssetInterface_Logs.txt";

        /// <summary>
        /// Override for the OnStartup method, to execute logic when the app begins.
        /// </summary>
        /// <param name="e">Startup event arguments.</param>
        protected override void OnStartup(StartupEventArgs e)
        {
            // call the base class implementation first
            base.OnStartup(e);

            try
            {
                // if the previous log file exists, delete it to start fresh
                if (File.Exists(m_LogFileName))
                    File.Delete(m_LogFileName);
            }
            catch (IOException)
            {}

            // configure the Trace system to log to a file
            Trace.Listeners.Add(new TextWriterTraceListener(m_LogFileName));
            Trace.AutoFlush = true;

            Trace.TraceInformation("Application started successfully");
        }

        /// <summary>
        /// Override the OnExit method, to execute logic when the app ends.
        /// </summary>
        /// <param name="e">Exit event argments.</param>
        protected override void OnExit(ExitEventArgs e)
        {
            Trace.TraceInformation("Application terminated successfully");

            base.OnExit(e);
        }
    }
}
