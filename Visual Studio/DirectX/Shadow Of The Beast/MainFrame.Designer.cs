namespace Shadow_of_the_Beast
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
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager( typeof( MainFrame ) );
            this.SuspendLayout();
            // 
            // MainFrame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF( 6F, 13F );
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size( 634, 455 );
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ( (System.Drawing.Icon)( resources.GetObject( "$this.Icon" ) ) );
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainFrame";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Shadow of the Beast";
            this.Load += new System.EventHandler( this.MainFrame_Load );
            this.Paint += new System.Windows.Forms.PaintEventHandler( this.MainFrame_Paint );
            this.MouseEnter += new System.EventHandler( this.MainFrame_MouseEnter );
            this.Activated += new System.EventHandler( this.MainFrame_Activated );
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler( this.MainFrame_FormClosing );
            this.ResumeLayout( false );

        }

        #endregion

    }
}

