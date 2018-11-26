using System;

namespace Shadow_of_the_Beast.Culture
{
    public enum SOTB_Cultures
    {
        English = 0,
        French  = 1,
    };

    class CultureManager
    {
        string          p_AppTitle       = "Shadow of the Beast";

        FrenchMessages  p_French         = new FrenchMessages();
        EnglishMessages p_English        = new EnglishMessages();

        SOTB_Cultures   p_CurrentCulture = SOTB_Cultures.French;

        /// <summary>
        /// Obtient ou détermine la culture utilisée.
        /// </summary>
        public SOTB_Cultures CurrentCulture
        {
            get
            {
                return p_CurrentCulture;
            }

            set
            {
                p_CurrentCulture = value;
            }
        }

        /// <summary>
        /// Shadow of the Beast.
        /// </summary>
        public string AppTitle
        {
            get
            {
                return p_AppTitle;
            }
        }

        /// <summary>
        /// L'initialisation de Direct3D a échoué.
        /// </summary>
        public string Message1
        {
            get
            {
                return SelectAppropriateMessage( 1 ) + SelectAppropriateMessage( 0 );
            }
        }

        /// <summary>
        /// Au moins un sprite n'a pas pu être crée.
        /// </summary>
        public string Message2
        {
            get
            {
                return SelectAppropriateMessage( 2 ) + SelectAppropriateMessage( 0 );
            }
        }

        /// <summary>
        /// L'initialisation de DirectInput a échoué.
        /// </summary>
        public string Message3
        {
            get
            {
                return SelectAppropriateMessage( 3 ) + SelectAppropriateMessage( 0 );
            }
        }

        private string SelectAppropriateMessage( int MessageNb )
        {
            switch( p_CurrentCulture )
            {
                case SOTB_Cultures.English:
                {
                    return p_English.GetMessage( MessageNb );
                }

                case SOTB_Cultures.French:
                {
                    return p_French.GetMessage( MessageNb );
                }
            
                default:
                {
                    return p_English.GetMessage( MessageNb );
                }
            }
        }
    }
}
