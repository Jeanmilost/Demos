using System;
using System.IO;
using System.Drawing;

namespace CasseBriques
{
    /// <summary>
    /// Classe pour construire des briques
    /// </summary>
    class Briques
    {
        private int    m_X;        // position X de la brique
        private int    m_Y;        // position Y de la brique
        private Bitmap m_Image;    // image pour représenter la brique
        private bool   m_IsBroken; // si true, la brique est cassée 

        /// <summary>
        /// Obtient ou définit la position X de la brique
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
        /// Obtient ou définit la position Y de la brique
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
        /// Obtient la largeur de la brique
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
        /// Obtient la hauteur de la brique
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
        /// Obtient ou définit si la brique est cassée
        /// </summary>
        public bool IsBroken
        {
            get
            {
                return m_IsBroken;
            }

            set
            {
                m_IsBroken = value;
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
        /// Pour construire des briques
        /// </summary>
        public Briques()
        {
            m_X        = 0;
            m_Y        = 0;
            m_IsBroken = false;
            m_Image    = null;
        }

        /// <summary>
        /// Charge une image pour la brique
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
