using System;
using System.IO;
using Microsoft.DirectX.AudioVideoPlayback;

namespace CasseBriques
{
    /// <summary>
    /// Classe pour jouer de la musique
    /// </summary>
    class Music
    {
        private Audio m_Audio = null; // objet audio

        /// <summary>
        /// Obtient ou définit le volume
        /// </summary>
        public int Volume
        {
            get
            {
                if ( m_Audio != null )
                    return m_Audio.Volume;
                else
                    return 0;
            }

            set
            {
                if ( m_Audio != null )
                    m_Audio.Volume = value;
            }
        }

        /// <summary>
        /// Charge une chanson en mémoire
        /// </summary>
        /// <param name="fileName">Nom du fichier, avec le chemin, contenant la chanson</param>
        /// <returns>True si tout a réussi, sinon false</returns>
        public bool LoadTrack( string fileName )
        {
            try
            {
                // efface l'ancien gestionnaire avant d'en créer un autre
                if ( m_Audio != null )
                    m_Audio.Dispose();

                // contrôle si le fichier existe
                if ( !File.Exists( fileName ) )
                    return false;

                // initialise le module audio
                m_Audio = new Audio( fileName );

                return true;
            }
            catch ( Exception )
            {
                return false;
            }
        }

        /// <summary>
        /// Joue la chanson
        /// </summary>
        /// <returns>True si on peut jouer, sinon false</returns>
        public bool Play()
        {
            try
            {
                // teste si on joue déjà la musique
                if ( m_Audio.Playing == false )
                    // essaie de jouer
                    m_Audio.Play();

                return true;
            }
            catch ( Exception )
            {
                return false;
            }
        }

        /// <summary>
        /// Mets la chanson en pause
        /// </summary>
        /// <returns>True si on peut mettre pause, sinon false</returns>
        public bool Pause()
        {
            try
            {
                // essaie de mettre en pause
                m_Audio.Pause();
                return true;
            }
            catch ( Exception )
            {
                return false;
            }
        }

        /// <summary>
        /// Teste si on est toujours en train de jouer, sinon redémarre la musique
        /// </summary>
        /// <returns>True si réussi, sinon false</returns>
        public bool Restart()
        {
            try
            {
                // teste si on est toujours en train de jouer
                if ( m_Audio.Playing == false )
                    // rejoue la musique
                    m_Audio.Play();

                return true;
            }
            catch ( Exception )
            {
                return false;
            }
        }
    }
}
