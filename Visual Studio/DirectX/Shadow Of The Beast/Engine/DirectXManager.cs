using System;
using System.Collections.Generic;
using System.Windows.Forms;

using System.Text;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using Microsoft.DirectX.DirectInput;
using Direct3D = Microsoft.DirectX.Direct3D;
using DirectInput = Microsoft.DirectX.DirectInput;

namespace Shadow_of_the_Beast.Engine
{
    public class DirectXManager : Form
    {
        #region Variables de la classe

        private Direct3D.Device    p_D3DDevice    = null;
        private DirectInput.Device p_DInputDevice = null;

        #endregion

        #region Propriétés

        public Direct3D.Device D3DDevice
        {
            get
            {
                return p_D3DDevice;
            }
        }

        public DirectInput.Device DInputDevice
        {
            get
            {
                return p_DInputDevice;
            }
        }

        #endregion

        public bool InitializeD3D()
        {
            try
            {
                PresentParameters PresentParams = new PresentParameters();
                PresentParams.Windowed          = true;
                PresentParams.SwapEffect        = SwapEffect.Discard;

                p_D3DDevice = new Direct3D.Device( 0,
                                                   Direct3D.DeviceType.Hardware,
                                                   this,
                                                   CreateFlags.
                                                       SoftwareVertexProcessing,
                                                   PresentParams );

                p_D3DDevice.DeviceReset += new EventHandler( this.OnResetDevice );

                if ( OnPostInitializeD3D() == false )
                {
                    throw new DirectXException( "D3D Initialisation failed." );
                }

                return true;
            }
            catch ( DirectXException )
            {
                return false;
            }
        }

        public bool InitializeDInput()
        {
            try
            {
                if ( p_DInputDevice != null )
                {
                    return true;
                }

                p_DInputDevice = new DirectInput.Device( SystemGuid.Keyboard );

                return OnPostInitializeDInput();
            }
            catch ( Exception )
            {
                FreeDirectInput();
                return false;
            }
        }

        protected void FreeDirect3D()
        {
            if ( p_D3DDevice != null )
            {
                p_D3DDevice.Dispose();
                p_D3DDevice = null;
            }
        }

        protected void FreeDirectInput()
        {
            if ( p_DInputDevice != null )
            {
                p_DInputDevice.Unacquire();
                p_DInputDevice.Dispose();
                p_DInputDevice = null;
            }
        }

        public bool Render()
        {
            if ( p_D3DDevice == null )
            {
                return false;
            }

            p_D3DDevice.Clear( ClearFlags.Target,
                               System.Drawing.Color.Black,
                               1.0f,
                               0 );

            p_D3DDevice.BeginScene();

            DrawScene();

            p_D3DDevice.EndScene();
            p_D3DDevice.Present();

            return true;
        }

        protected virtual void DrawScene()
        {
        }

        protected virtual void OnResetDevice( object sender, EventArgs e )
        {
        }

        protected virtual bool OnPostInitializeD3D()
        {
            return true;
        }

        protected virtual bool OnPostInitializeDInput()
        {
            return true;
        }
    }
}
