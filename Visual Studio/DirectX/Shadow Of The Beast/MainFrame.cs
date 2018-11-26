using System;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using Microsoft.DirectX.DirectInput;
using Direct3D = Microsoft.DirectX.Direct3D;
using DirectInput = Microsoft.DirectX.DirectInput;

using Shadow_of_the_Beast.Culture;
using Shadow_of_the_Beast.Engine;
using Shadow_of_the_Beast.Game;

namespace Shadow_of_the_Beast
{
    public partial class MainFrame : DirectXManager
    {
        #region Variables de la classe

        SOTB_Camera p_Camera = null;
        SOTB_Beast  p_Beast  = new SOTB_Beast();
        SOTB_Landscape p_Landscape = new SOTB_Landscape();

        CultureManager p_Culture = new CultureManager();

        #endregion

        public MainFrame()
        {
            InitializeComponent();
        }

        protected override bool OnPostInitializeD3D()
        {
            D3DDevice.RenderState.Lighting = false;

            return true;
        }

        protected override bool OnPostInitializeDInput()
        {
            try
            {
                CooperativeLevelFlags CoopFlags;

                CoopFlags = CooperativeLevelFlags.Exclusive;
                CoopFlags |= CooperativeLevelFlags.Foreground;

                DInputDevice.SetCooperativeLevel( this, CoopFlags );
            }
            catch ( InputException )
            {
                return false;
            }

            return true;
        }

        protected override void DrawScene()
        {
            p_Camera.SetCamera( D3DDevice, true );
            p_Landscape.Render( D3DDevice );
            p_Beast.Render( D3DDevice );
        }

        private void ShowError( string Message )
        {
            MessageBox.Show( this,
                             Message,
                             p_Culture.AppTitle,
                             MessageBoxButtons.OK,
                             MessageBoxIcon.Error );
        }

        private void ShowWarning( string Message )
        {
            MessageBox.Show( this,
                             Message,
                             p_Culture.AppTitle,
                             MessageBoxButtons.OK,
                             MessageBoxIcon.Warning );
        }

        private void ShowMessage( string Message )
        {
            MessageBox.Show( this,
                             Message,
                             p_Culture.AppTitle,
                             MessageBoxButtons.OK,
                             MessageBoxIcon.Information );
        }

        private void MainFrame_Load( object sender, EventArgs e )
        {
            string ErrorMessage = InitializeApplication();

            if ( ErrorMessage != "" )
            {
                ShowError( ErrorMessage );

                this.Close();
            }
        }

        private string InitializeApplication()
        {
            if ( InitializeD3D() == false )
            {
                return p_Culture.Message1;
            }

            if ( InitializeDInput() == false )
            {
                return p_Culture.Message3;
            }

            p_Camera = new SOTB_Camera( D3DDevice );

            if ( p_Beast.Initialize( D3DDevice ) == false )
            {
                return p_Culture.Message2;
            }

            if ( p_Landscape.Initialize( D3DDevice ) == false )
            {
                return p_Culture.Message2;
            }

            return "";
        }

        private void MainFrame_Paint( object sender, PaintEventArgs e )
        {
            Render();
        }

        private bool ReadKeyboard()
        {
            try
            {
                KeyboardState State = null;

                if ( DInputDevice == null )
                {
                    return true;
                }

                // Get the input's device state, and store it.
                InputException Ie = null;

                try
                {
                    State = DInputDevice.GetCurrentKeyboardState();
                }
                catch ( DirectXException )
                {
                    bool Loop = true;

                    do
                    {
                        try
                        {
                            DInputDevice.Acquire();
                        }
                        catch ( InputLostException )
                        {
                            Loop = true;
                        }
                        catch ( InputException theInputException )
                        {
                            Ie   = theInputException;
                            Loop = false;
                        }
                    } while ( Loop );

                    return true;
                }

                p_Beast.BeastWait();

                if ( State[Key.Left] == true )
                {
                    p_Beast.BeastRunLeft();
                    p_Landscape.GoLeft();
                }

                if ( State[Key.Right] == true )
                {
                    p_Beast.BeastRunRight();
                    p_Landscape.GoRight();
                }

                if ( State[Key.Escape] == true )
                {
                    this.Close();
                }

                return true;
            }
            catch ( Exception )
            {
                return false;
            }
        }

        protected override void OnResetDevice( object sender, EventArgs e )
        {
            Direct3D.Device theNewDevice = (Direct3D.Device)sender;

            theNewDevice.RenderState.Lighting = false;

            p_Camera.Restore( theNewDevice );
            p_Beast.Restore( theNewDevice );
            p_Landscape.Restore( theNewDevice );

            Render();
        }

        public void Draw()
        {
            ReadKeyboard();
            p_Beast.RunTimer();
            Render();
        }

        private void MainFrame_MouseEnter( object sender, EventArgs e )
        {
            D3DDevice.ShowCursor( false );
        }

        private void MainFrame_FormClosing( object sender, FormClosingEventArgs e )
        {
            FreeDirect3D();
            FreeDirectInput();
        }

        private void MainFrame_Activated( object sender, EventArgs e )
        {
            try
            {
                if ( DInputDevice != null )
                {
                    DInputDevice.Acquire();
                }
            }
            catch ( Exception )
            {
            }
        }
    }
}
