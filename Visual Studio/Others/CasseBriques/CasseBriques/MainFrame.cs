using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace CasseBriques
{
    /// <summary>
    /// Classe principale du jeu. C'est ici que tout est coordonné.
    /// </summary>
    public partial class MainFrame : Form
    {
        private Barre  m_Barre = null;          // une barre pour le jeu
        private Balle  m_Balle = null;          // une balle pour le jeu
        private bool   m_Pause = false;         // si true, le jeu est en pause
        private Bitmap m_PauseImg = null;       // une image pour afficher "pause"
        private Music  m_Music = null;          // pour jouer le fond sonore
        private bool   m_IsInitialized = false; // si True, le programme est prêt
        private bool   m_CanPlayMusic = true;   // si True, la musique peut être jouée

        private const string m_PauseImgFile   = "..\\..\\..\\..\\Images\\Pause.jpeg";
        private const string m_BarreImgFile   = "..\\..\\..\\..\\Images\\Barre.bmp";
        private const string m_BalleImgFile   = "..\\..\\..\\..\\Images\\Balle.bmp";
        private const string m_BriquesImgFile = "..\\..\\..\\..\\Images\\Brique.bmp";
        private const string m_MusiqueFile    = "..\\..\\..\\..\\Musique\\SuperMario.mp3";

        private Briques[,] m_Briques = new Briques[10, 5]; // un mur de briques pour le jeu

        /// <summary>
        /// Construit notre application
        /// </summary>
        public MainFrame()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Est appelé lorsque l'application est en construction
        /// </summary>
        /// <param name="sender">L'objet qui a appelé</param>
        /// <param name="e">Ce que l'on peut utiliser</param>
        private void MainFrame_Load( object sender, EventArgs e )
        {
            if ( !File.Exists( m_PauseImgFile ) )
            {
                MessageBox.Show( "L'image " + m_PauseImgFile + " n'a pas pu être chargée. Le programme doit être arrêté.", "Casse briques", MessageBoxButtons.OK, MessageBoxIcon.Error );
                this.Close();
            }
            else
                m_PauseImg = new Bitmap( m_PauseImgFile );

            m_PauseImg.MakeTransparent( Color.FromArgb( 0, 255, 255 ) );

            m_Barre = new Barre();

            if ( m_Barre.LoadImage( m_BarreImgFile, Color.FromArgb( 0, 255, 255 ) ) == false )
            {
                MessageBox.Show( "L'image " + m_BarreImgFile + " n'a pas pu être chargée. Le programme doit être arrêté.", "Casse briques", MessageBoxButtons.OK, MessageBoxIcon.Error );
                this.Close();
            }

            m_Balle = new Balle();

            if ( m_Balle.LoadImage( m_BalleImgFile, Color.FromArgb( 0, 255, 255 ) ) == false )
            {
                MessageBox.Show( "L'image " + m_BalleImgFile + " n'a pas pu être chargée. Le programme doit être arrêté.", "Casse briques", MessageBoxButtons.OK, MessageBoxIcon.Error );
                this.Close();
            }

            m_Balle.X = ( pbDisplayer.Width / 2 ) - ( m_Balle.Diametre / 2 );
            m_Balle.Y = ( pbDisplayer.Height / 2 ) - ( m_Balle.Diametre / 2 );
            m_Balle.MaxX = pbDisplayer.Width - m_Balle.Diametre;
            m_Balle.MaxY = pbDisplayer.Height - m_Balle.Diametre;

            for ( int  i = 0; i < 10; ++i )
                for ( int j = 0; j < 5; ++j )
                {
                    m_Briques[i, j] = new Briques();
                    if ( m_Briques[i, j].LoadImage( m_BriquesImgFile, Color.FromArgb( 0, 255, 255 ) ) == false )
                    {
                        MessageBox.Show( "L'image " + m_BriquesImgFile + " n'a pas pu être chargée. Le programme doit être arrêté.", "Casse briques", MessageBoxButtons.OK, MessageBoxIcon.Error );
                        this.Close();
                    }

                    int wallWidth = ( ( m_Briques[0, 0].Width + 1 ) * m_Briques.GetLength( 0 ) );
                    int paddingWidth = ( pbDisplayer.Width - wallWidth ) / 2;
                    int paddingHeight = 20;

                    m_Briques[i, j].X = paddingWidth + ( ( m_Briques[i, j].Width + 1 ) * i );
                    m_Briques[i, j].Y = paddingHeight + ( m_Briques[i, j].Height + 1 ) * j;
                }

            m_Music = new Music();

            if ( m_Music.LoadTrack( m_MusiqueFile ) == false )
            {
                MessageBox.Show( "La musique " + m_MusiqueFile + " n'a pas pu être chargée. Le programme doit être arrêté.", "Casse briques", MessageBoxButtons.OK, MessageBoxIcon.Error );
                this.Close();
            }

            m_CanPlayMusic = m_Music.Play();

            this.DoubleBuffered = true;

            m_IsInitialized = true;
            tmDisplay.Enabled = true;
        }

        /// <summary>
        /// Est appelé lorsque l'application se dessine
        /// </summary>
        /// <param name="sender">L'objet qui a appelé</param>
        /// <param name="e">Ce que l'on peut utiliser</param>
        private void pbDisplayer_Paint( object sender, PaintEventArgs e )
        {
            if ( m_IsInitialized == false )
                return;

            m_Barre.X = ScreenToClient( Cursor.Position.X, Cursor.Position.Y ).X;
            m_Barre.Y = pbDisplayer.Height - 40;

            if ( m_Barre.X > pbDisplayer.Width - m_Barre.Width )
                m_Barre.X = pbDisplayer.Width - m_Barre.Width;

            e.Graphics.DrawImage( m_Balle.ImageToDraw, m_Balle.X, m_Balle.Y, m_Balle.Diametre, m_Balle.Diametre );
            e.Graphics.DrawImage( m_Barre.ImageToDraw, m_Barre.X, m_Barre.Y, m_Barre.Width, m_Barre.Height );

            for ( int  i = 0; i < 10; ++i )
                for ( int j = 0; j < 5; ++j )
                    if ( m_Briques[i, j].IsBroken == false )
                        e.Graphics.DrawImage( m_Briques[i, j].ImageToDraw,
                                              m_Briques[i, j].X,
                                              m_Briques[i, j].Y,
                                              m_Briques[i, j].Width,
                                              m_Briques[i, j].Height );

            if ( m_Pause == true )
                e.Graphics.DrawImage( m_PauseImg, ( pbDisplayer.Width / 2 ) - ( m_PauseImg.Width / 2 ), ( pbDisplayer.Height / 2 ) - ( m_PauseImg.Height / 2 ) );

        }

        /// <summary>
        /// Convertit les coordonnées de la souris sur l'écran aux coordonnées de notre application
        /// </summary>
        /// <param name="x">La position X de la souris</param>
        /// <param name="y">La position Y de la souris</param>
        /// <returns>Les coordonnées X et Y converties</returns>
        private Point ScreenToClient( int x, int y )
        {
            return new Point( ( ( x * this.ClientSize.Width ) / Screen.PrimaryScreen.WorkingArea.Width ),
                              ( ( y * this.ClientSize.Height ) / Screen.PrimaryScreen.WorkingArea.Height ) );
        }

        /// <summary>
        /// Est appelé chaque fois que le réveille sonne
        /// </summary>
        /// <param name="sender">L'objet qui a appelé</param>
        /// <param name="e">Ce que l'on peut utiliser</param>
        private void tmDisplay_Tick( object sender, EventArgs e )
        {
            if ( m_IsInitialized == false )
                return;

            pbDisplayer.Refresh();

            if ( m_Pause == true )
                return;
            else
            {
                if ( m_CanPlayMusic == true )
                    m_Music.Play();
            }

            try
            {
                tmDisplay.Enabled = false;

                if ( !m_Balle.Move( m_Barre ) )
                {
                    m_Pause = true;

                    if ( m_CanPlayMusic == true )
                        m_Music.Pause();

                    if ( MessageBox.Show( "Tu as perdu !!! Veux-tu rejouer ?", "Casse briques", MessageBoxButtons.OKCancel, MessageBoxIcon.Question ) == DialogResult.Cancel )
                        this.Close();

                    m_Balle.X = ( pbDisplayer.Width / 2 ) - ( m_Balle.Diametre / 2 );
                    m_Balle.Y = ( pbDisplayer.Height / 2 ) - ( m_Balle.Diametre / 2 );

                    m_Pause = false;

                    if ( m_CanPlayMusic == true )
                        m_Music.Play();

                    for ( int  i = 0; i < 10; ++i )
                        for ( int j = 0; j < 5; ++j )
                            m_Briques[i, j].IsBroken = false;
                }

                bool result = true;

                for ( int  i = 0; i < 10; ++i )
                    for ( int j = 0; j < 5; ++j )
                    {
                        // si la brique n' est pas cassée,et si la balle a touché la brique,on casse la brique
                        if ( m_Briques[i, j].IsBroken == false )
                            if ( Collisions.IsCollided( new Rectangle( m_Balle.X, m_Balle.Y, m_Balle.Diametre, m_Balle.Diametre ),
                                                        new Rectangle( m_Briques[i, j].X, m_Briques[i, j].Y, m_Briques[i, j].Width, m_Briques[i, j].Height ) ) )
                            {
                                m_Briques[i, j].IsBroken = true;
                                Collisions.Edge( m_Balle, m_Briques[i, j] );
                            }

                        result &= m_Briques[i, j].IsBroken;
                    }

                if ( result == true )
                {
                    m_Pause = true;

                    if ( m_CanPlayMusic == true )
                        m_Music.Pause();

                    if ( MessageBox.Show( "tu as gagné, veux-tu rejouer?", "Casse briques", MessageBoxButtons.OKCancel, MessageBoxIcon.Question ) == DialogResult.Cancel )
                        this.Close();

                    m_Balle.X = ( pbDisplayer.Width / 2 ) - ( m_Balle.Diametre / 2 );
                    m_Balle.Y = ( pbDisplayer.Height / 2 ) - ( m_Balle.Diametre / 2 );

                    m_Pause = false;

                    if ( m_CanPlayMusic == true )
                        m_Music.Play();

                    for ( int  i = 0; i < 10; ++i )
                        for ( int j = 0; j < 5; ++j )
                            m_Briques[i, j].IsBroken = false;
                }
            }
            finally
            {
                tmDisplay.Enabled = true;
            }
        }

        /// <summary>
        /// Est appelé chaque fois que l'application change de taille
        /// </summary>
        /// <param name="sender">L'objet qui a appelé</param>
        /// <param name="e">Ce que l'on peut utiliser</param>
        private void MainFrame_Resize( object sender, EventArgs e )
        {
            if ( m_IsInitialized == false )
                return;

            m_Balle.MaxX = pbDisplayer.Width - m_Balle.Diametre;
            m_Balle.MaxY = pbDisplayer.Height - m_Balle.Diametre;
        }

        /// <summary>
        /// Est appelé chaque fois qu'une touche est pressée
        /// </summary>
        /// <param name="sender">L'objet qui a appelé</param>
        /// <param name="e">Ce que l'on peut utiliser</param>
        private void MainFrame_KeyPress( object sender, KeyPressEventArgs e )
        {
            if ( m_IsInitialized == false )
                return;

            // si touche 'P' est pressée, on met en pause
            if ( e.KeyChar == 'p' || e.KeyChar == 'P' )
            {
                m_Pause = !m_Pause;

                if ( m_CanPlayMusic == true )
                    m_Music.Pause();
            }

            // si touche 'ESC' est pressée, on quitte
            if ( e.KeyChar == 0x1B )
            {
                this.Close();
            }
        }

        private void pbDisplayer_MouseEnter( object sender, EventArgs e )
        {
            Cursor.Hide();
        }

        private void pbDisplayer_MouseLeave( object sender, EventArgs e )
        {
            Cursor.Show();
        }
    }
}
