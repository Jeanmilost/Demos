using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Shadow_of_the_Beast
{
    static class Program
    {
        /// <summary>
        /// Le point d'entrée de l'application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            MainFrame p_MainFrame = new MainFrame();

            p_MainFrame.Show();

            while ( p_MainFrame.Created )
            {
                p_MainFrame.Draw();
                System.Windows.Forms.Application.DoEvents();
            }
        }
    }
}
