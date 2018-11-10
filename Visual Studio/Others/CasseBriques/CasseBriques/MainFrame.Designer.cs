namespace CasseBriques
{
    partial class MainFrame
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose( bool disposing )
        {
            if ( disposing && ( components != null ) )
            {
                components.Dispose();
            }
            base.Dispose( disposing );
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager( typeof( MainFrame ) );
            this.pbDisplayer = new System.Windows.Forms.PictureBox();
            this.tmDisplay = new System.Windows.Forms.Timer( this.components );
            ( (System.ComponentModel.ISupportInitialize)( this.pbDisplayer ) ).BeginInit();
            this.SuspendLayout();
            // 
            // pbDisplayer
            // 
            this.pbDisplayer.BackColor = System.Drawing.Color.Black;
            this.pbDisplayer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pbDisplayer.Image = ( (System.Drawing.Image)( resources.GetObject( "pbDisplayer.Image" ) ) );
            this.pbDisplayer.Location = new System.Drawing.Point( 0, 0 );
            this.pbDisplayer.Name = "pbDisplayer";
            this.pbDisplayer.Size = new System.Drawing.Size( 592, 566 );
            this.pbDisplayer.TabIndex = 0;
            this.pbDisplayer.TabStop = false;
            this.pbDisplayer.MouseLeave += new System.EventHandler( this.pbDisplayer_MouseLeave );
            this.pbDisplayer.Paint += new System.Windows.Forms.PaintEventHandler( this.pbDisplayer_Paint );
            this.pbDisplayer.MouseEnter += new System.EventHandler( this.pbDisplayer_MouseEnter );
            // 
            // tmDisplay
            // 
            this.tmDisplay.Interval = 1;
            this.tmDisplay.Tick += new System.EventHandler( this.tmDisplay_Tick );
            // 
            // MainFrame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF( 6F, 13F );
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size( 592, 566 );
            this.Controls.Add( this.pbDisplayer );
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "MainFrame";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Casse briques";
            this.TopMost = true;
            this.Load += new System.EventHandler( this.MainFrame_Load );
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler( this.MainFrame_KeyPress );
            this.Resize += new System.EventHandler( this.MainFrame_Resize );
            ( (System.ComponentModel.ISupportInitialize)( this.pbDisplayer ) ).EndInit();
            this.ResumeLayout( false );

        }

        #endregion

        private System.Windows.Forms.PictureBox pbDisplayer;
        private System.Windows.Forms.Timer tmDisplay;
    }
}

