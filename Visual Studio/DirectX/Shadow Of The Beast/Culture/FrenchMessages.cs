using System;
using System.Collections.Generic;
using System.Text;

namespace Shadow_of_the_Beast.Culture
{
    class FrenchMessages
    {
        string[] p_Messages = new string[]
        {
            "POUR CETTE RAISON, LE JEU NE PEUT PAS CONTINUER.",
            "L'initialisation de Direct3D a �chou�.\n",
            "Au moins un sprite n'a pas pu �tre cr�e.\n",
            "L'initialisation de DirectInput a �chou�.\n",
        };

        public string GetMessage( int MessageNb )
        {
            if ( MessageNb < 0 || MessageNb > p_Messages.GetUpperBound( 0 ) )
            {
                return "";
            }
            else
            {
                return p_Messages[MessageNb];
            }
        }
    }
}
