namespace Player
{
    partial class Main
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.pbArtwork = new System.Windows.Forms.PictureBox();
            this.pbClose = new System.Windows.Forms.PictureBox();
            this.tmMoveWindows = new System.Windows.Forms.Timer(this.components);
            this.lbMediaInfos = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pbArtwork)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbClose)).BeginInit();
            this.SuspendLayout();
            // 
            // pbArtwork
            // 
            this.pbArtwork.BackColor = System.Drawing.Color.Black;
            this.pbArtwork.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pbArtwork.BackgroundImage")));
            this.pbArtwork.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.pbArtwork.Location = new System.Drawing.Point(70, 20);
            this.pbArtwork.Name = "pbArtwork";
            this.pbArtwork.Size = new System.Drawing.Size(180, 180);
            this.pbArtwork.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbArtwork.TabIndex = 0;
            this.pbArtwork.TabStop = false;
            this.pbArtwork.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
            this.pbArtwork.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
            // 
            // pbClose
            // 
            this.pbClose.BackColor = System.Drawing.Color.Black;
            this.pbClose.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.pbClose.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pbClose.Image = ((System.Drawing.Image)(resources.GetObject("pbClose.Image")));
            this.pbClose.Location = new System.Drawing.Point(292, 12);
            this.pbClose.Name = "pbClose";
            this.pbClose.Size = new System.Drawing.Size(16, 16);
            this.pbClose.TabIndex = 1;
            this.pbClose.TabStop = false;
            this.pbClose.Click += new System.EventHandler(this.pbClose_Click);
            // 
            // tmMoveWindows
            // 
            this.tmMoveWindows.Tick += new System.EventHandler(this.tmMoveWindows_Tick);
            // 
            // lbMediaInfos
            // 
            this.lbMediaInfos.BackColor = System.Drawing.Color.Black;
            this.lbMediaInfos.Font = new System.Drawing.Font("Verdana", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbMediaInfos.ForeColor = System.Drawing.Color.White;
            this.lbMediaInfos.Location = new System.Drawing.Point(12, 212);
            this.lbMediaInfos.Name = "lbMediaInfos";
            this.lbMediaInfos.Size = new System.Drawing.Size(296, 19);
            this.lbMediaInfos.TabIndex = 2;
            this.lbMediaInfos.Text = "Welcome to MP3 player";
            this.lbMediaInfos.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lbMediaInfos.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
            this.lbMediaInfos.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
            // 
            // Main
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Fuchsia;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(320, 240);
            this.Controls.Add(this.lbMediaInfos);
            this.Controls.Add(this.pbClose);
            this.Controls.Add(this.pbArtwork);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Main";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "MP3 player";
            this.TopMost = true;
            this.TransparencyKey = System.Drawing.Color.Fuchsia;
            this.Load += new System.EventHandler(this.Main_Load);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.Main_DragDrop);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.Main_DragEnter);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Main_KeyUp);
            ((System.ComponentModel.ISupportInitialize)(this.pbArtwork)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbClose)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pbArtwork;
        private System.Windows.Forms.PictureBox pbClose;
        private System.Windows.Forms.Timer tmMoveWindows;
        private System.Windows.Forms.Label lbMediaInfos;
    }
}

