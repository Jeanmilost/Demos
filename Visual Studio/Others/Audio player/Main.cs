/***********************************************************
 * Main media player class                                 *
 * @Author - Jean-Milost Reymond                           *
 ***********************************************************/

using System;
using System.Drawing;
using System.Windows.Forms;
using Player;

namespace Player
{
    public partial class Main : Form
    {
        #region Class variables

        private DirectX_Player m_Player         = new DirectX_Player(); // object containing the player
        private ArtworkReader  m_ArtworkReader  = new ArtworkReader();  // object containing MP3 artwork
        private int            m_OldX           = -1;                   // to keep last X cursor position
        private int            m_OldY           = -1;                   // to keep last Y cursor position

        #endregion

        #region Constructor / Destructor

        /// <summary>
        /// Constructor
        /// </summary>
        public Main()
        {
            InitializeComponent();
        }

        #endregion

        #region Events functions

        #region Form events

        /// <summary>
        /// Called when application initializes
        /// </summary>
        /// <param name="sender">Object that raised the event</param>
        /// <param name="e">Event arguments</param>
        private void Main_Load(object sender, EventArgs e)
        {
            // calculate starting location
            this.Left = Screen.PrimaryScreen.WorkingArea.Width - this.Width;
            this.Top = Screen.PrimaryScreen.WorkingArea.Height - this.Height;
        }

        #endregion

        #region Timer events

        /// <summary>
        /// Called when move window timer tick
        /// </summary>
        /// <param name="sender">Object that raises the event</param>
        /// <param name="e">Event arguments</param>
        private void tmMoveWindows_Tick(object sender, EventArgs e)
        {
            // calculate next window position
            this.Left += Cursor.Position.X - m_OldX;
            this.Top += Cursor.Position.Y - m_OldY;

            // backup new cursor position
            m_OldX = Cursor.Position.X;
            m_OldY = Cursor.Position.Y;
        }

        #endregion

        #region Keyboard events

        /// <summary>
        /// Called when user release key on keyboard
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Main_KeyUp(object sender, KeyEventArgs e)
        {
            // check if key pressed was Escape key
            if (e.KeyCode == Keys.Escape)
                this.Close();
        }

        #endregion

        #region Mouse events

        /// <summary>
        /// Called when user click on Close button
        /// </summary>
        /// <param name="sender">Object that raises the event</param>
        /// <param name="e">Event arguments</param>
        private void pbClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// Global OnMouseDown object called when any object has no attributed action on click
        /// </summary>
        /// <param name="sender">Object that raises the event</param>
        /// <param name="e">Event arguments</param>
        private void OnMouseDown(object sender, MouseEventArgs e)
        {
            // backup old mouse position
            m_OldX = Cursor.Position.X;
            m_OldY = Cursor.Position.Y;

            // enable move window timer
            tmMoveWindows.Enabled = true;
        }

        /// <summary>
        /// Global OnMouseUp object called when any object has no attributed action on click
        /// </summary>
        /// <param name="sender">Object that raises the event</param>
        /// <param name="e">Event arguments</param>
        private void OnMouseUp(object sender, MouseEventArgs e)
        {
            // disable move window timer
            tmMoveWindows.Enabled = false;
        }

        #endregion

        #region Drag and drop events

        /// <summary>
        /// Called when drag needs to be validated
        /// </summary>
        /// <param name="sender">Object that raises the event</param>
        /// <param name="e">Event arguments</param>
        private void Main_DragEnter(object sender, DragEventArgs e)
        {
            try
            {
                // by default, drop is denied
                e.Effect = DragDropEffects.None;

                // check kind of data user attempt to drag and drop
                if (e.Data.GetDataPresent(DataFormats.FileDrop, false))
                {
                    // if object is a file object, get data
                    string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);

                    // check if data are valid
                    if (files.Length == 0)
                        return;

                    // split file name of first drag object
                    string[] names = files[0].Split('.');

                    // check if names are valid
                    if (names.Length == 0)
                        return;

                    // get extension
                    string extension = names[names.Length - 1].ToLower();

                    // check if extension is MP3
                    if (extension != "mp3")
                        return;

                    // if yes, allow drag and drop for this object
                    e.Effect = DragDropEffects.Copy;
                }
            }
            catch (Exception)
            {
                // in case of problem ,deny drag and drop
                e.Effect = DragDropEffects.None;
            }
        }

        /// <summary>
        /// Called when drag and drop is executed
        /// </summary>
        /// <param name="sender">Object that raises the event</param>
        /// <param name="e">Event arguments</param>
        private void Main_DragDrop(object sender, DragEventArgs e)
        {
            // get object data
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);

            // attempt to get artwork with first selection file name
            if (m_ArtworkReader.Open(files[0]))
                // if success, update artwork picture
                pbArtwork.Image = m_ArtworkReader.GetArtwork();
            else
                // otherwise set default artwork picture
                pbArtwork.Image = null;

            // attempt to load and play new track
            if (m_Player.LoadTrack(files[0]))
                if (m_Player.Play())
                {
                    // if load and play success, split file name
                    string[] fileName = files[0].Split('\\');

                    // check if split is valid
                    if (fileName.Length > 0)
                    {
                        // display file name
                        lbMediaInfos.Text = "File : " + fileName[fileName.Length - 1];
                        return;
                    }
                }

            // display no name message
            lbMediaInfos.Text = "No name";
        }

        #endregion

        #endregion
    }
}
