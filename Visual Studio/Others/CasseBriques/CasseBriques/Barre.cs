using System;
using System.IO;
using System.Drawing;

namespace CasseBriques
{
    /// <summary>
    /// Classe représentant la barre
    /// </summary>
    class Barre
    {
        private int m_X;     // position X de la barre
        private int m_Y;     // position Y de la barre
        Bitmap      m_Image; // image pour dessiner la barre

        /// <summary>
        /// Obtient ou définit la position X
        /// </summary>
        public int X
        {
            get
            {
                return m_X;
            }

            set
            {
                m_X = value;
            }
        }

        /// <summary>
        /// Obtient ou définit la position Y
        /// </summary>
        public int Y
        {
            get
            {
                return m_Y;
            }

            set
            {
                m_Y = value;
            }
        }

        /// <summary>
        /// Obtient la largeur
        /// </summary>
        public int Width
        {
            get
            {
                if ( m_Image != null )
                    return m_Image.Width;
                else
                    return 0;
            }
        }

        /// <summary>
        /// Obtient la hauteur
        /// </summary>
        public int Height
        {
            get
            {
                if ( m_Image != null )
                    return m_Image.Height;
                else
                    return 0;
            }
        }

        /// <summary>
        /// Obtient l'image à dessiner
        /// </summary>
        public Bitmap ImageToDraw
        {
            get
            {
                return m_Image;
            }
        }

        /// <summary>
        /// Construit la barre
        /// </summary>
        public Barre()
        {
            m_X     = 0;
            m_Y     = 25;
            m_Image = null;
        }

        /// <summary>
        /// Charge une image pour la barre
        /// </summary>
        /// <param name="filename">Nom du fichier, avec le chemin, de l'image</param>
        /// <param name="colorKey">Valeur de la couleur qui sera remplacée par l'image de fond</param>
        /// <returns>True si on a pu charger le fichier, sinon false</returns>
        public bool LoadImage( string filename, Color colorKey )
        {
            if ( File.Exists( filename ) )
            {
                m_Image = new Bitmap( filename );

                m_Image.MakeTransparent( colorKey );

                return true;
            }
            else
                return false;
        }
    }
}
