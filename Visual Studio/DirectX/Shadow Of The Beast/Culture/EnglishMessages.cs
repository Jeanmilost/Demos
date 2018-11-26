using System;
using System.Collections.Generic;
using System.Text;

namespace Shadow_of_the_Beast.Culture
{
    class EnglishMessages
    {
        string[] p_Messages = new string[]
        {
            "FOR THIS REASON, THE GAME CANNOT RUN.",
            "Direct3D initialisation failed.\n",
            "Not all the sprites can be created.\n",
            "DirectInput initialisation failed.\n"
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
