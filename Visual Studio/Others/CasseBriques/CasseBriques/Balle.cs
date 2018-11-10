using System;
using System.IO;
using System.Drawing;

namespace CasseBriques
{
    /// <summary>
    /// Classe représentant la balle
    /// </summary>
    class Balle
    {
        private int    m_Speed;   // vitesse de déplacement de la balle
        private int    m_X;       // position X de la balle
        private int    m_Y;       // position Y de la balle
        private int    m_OffsetX; // offset X de la balle
        private int    m_OffsetY; // offset Y de la balle
        private int    m_MaxX;    // point maximum à l'écran ou la balle peut aller en X
        private int    m_MaxY;    // point maximal à l'écran ou la balle peut aller en Y
        private Bitmap m_Image;   // image représentant la balle

        /// <summary>
        /// Obtient ou définit la vitesse de déplacement de la balle
        /// </summary>
        public int Speed
        {
            get
            {
                return m_Speed;
            }

            set
            {
                m_Speed = value;
            }
        }

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
        /// Obtient ou définit le point maximum que la balle peut atteindre en X
        /// </summary>
        public int MaxX
        {
            get
            {
                return m_MaxX;
            }

            set
            {
                m_MaxX = value;
            }
        }

        /// <summary>
        /// Obtient ou définit le point maximum que la balle peut atteindre en Y
        /// </summary>
        public int MaxY
        {
            get
            {
                return m_MaxY;
            }

            set
            {
                m_MaxY = value;
            }
        }

        /// <summary>
        /// Obtient ou définit l'offset de déplacement de la balle sur l'axe des X
        /// </summary>
        public int OffsetX
        {
            get
            {
                return m_OffsetX;
            }

            set
            {
                m_OffsetX = value;
            }
        }

        /// <summary>
        /// Obtient ou définit l'offset de déplacement de la balle sur l'axe des Y
        /// </summary>
        public int OffsetY
        {
            get
            {
                return m_OffsetY;
            }

            set
            {
                m_OffsetY = value;
            }
        }

        /// <summary>
        /// Obtient le diamètre de la balle
        /// </summary>
        public int Diametre
        {
            get
            {
                if ( m_Image != null )
                    return (m_Image.Width > m_Image.Height) ? m_Image.Width : m_Image.Height;
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
        /// Construit la balle
        /// </summary>
        public Balle()
        {
            m_Speed   = 5;
            m_X       = 0;
            m_Y       = 0;
            m_OffsetX = -m_Speed;
            m_OffsetY = -m_Speed; 
            m_Image   = null;
        }

        /// <summary>
        /// Charge une image pour la balle
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

        /// <summary>
        /// Inverse le déplacement en X
        /// </summary>
        public void InvertX()
        {
            m_OffsetX = -m_OffsetX;
        }

        /// <summary>
        /// Inverse le déplacement en Y
        /// </summary>
        public void InvertY()
        {
            m_OffsetY = -m_OffsetY;
        }

        /// <summary>
        /// Déplace la balle à l'écran
        /// </summary>
        /// <param name="barre">L'objet représentant la barre</param>
        /// <returns>True si le joueur peut encore jouer, false si le joueur a perdu</returns>
        public bool Move( Barre barre )
        {
            // On teste si la limite X est atteinte
            if ( m_X <= 0 || m_X >= m_MaxX )
                InvertX();

            // on teste si la limite Y est atteinet
            if ( m_Y <= 0 )
                InvertY();

            // on teste si la balle a touché la barre
            if ( Collisions.IsCollided( new Rectangle( m_X, m_Y, Diametre, Diametre ), new Rectangle( barre.X, barre.Y, barre.Width, barre.Y + 20 ) ) )
                InvertY();

            // on teste si le joueur a perdu
            if ( m_Y >= MaxY )
                return false;

            // on calcule la nouvelle position de la balle
            m_X += m_OffsetX;
            m_Y += m_OffsetY;

            return true;
        }
    }
}
