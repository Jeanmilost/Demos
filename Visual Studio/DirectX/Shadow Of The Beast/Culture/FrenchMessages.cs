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
            "L'initialisation de Direct3D a échoué.\n",
            "Au moins un sprite n'a pas pu être crée.\n",
            "L'initialisation de DirectInput a échoué.\n",
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
