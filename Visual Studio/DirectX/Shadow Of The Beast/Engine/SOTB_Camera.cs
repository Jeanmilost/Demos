/*********************************************************************************
 * ==> Classe SOTB_Cam�ra -------------------------------------------------------*
 * *******************************************************************************
 * Description : Cette classe repr�sente une cam�ra, ou un point de vue, dans le *
 *               monde 3D.                                                       *
 * Version     : 1.0                                                             *
 * D�veloppeur : Jean-Milost Reymond                                             *
 *********************************************************************************/

using System;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace Shadow_of_the_Beast.Engine
{
    #region Classe SOTB_Camera

    /// <summary>
    /// Cette classe repr�sente la cam�ra, ou point de vue, dans le monde 3D.
    /// </summary>
    class SOTB_Camera
    {
        #region Variable(s) de la classe

        private float p_rX          = 0.0f;
        private float p_rY          = 0.0f;
        private float p_rZ          = 0.0f;
        private float p_tX          = 0.0f;
        private float p_tY          = 0.0f;
        private float p_tZ          = 0.0f;
        private float p_FieldOfView = (float)( Math.PI / 4 );
        private float p_AspectRatio = 1.0f;
        private float p_ClipNear    = 1.0f;
        private float p_ClipFar     = 100.0f;

        #endregion

        #region Propri�t�(s)

        /// <summary>
        /// Obtient ou d�finit l'angle de rotation de la cam�ra sur l'axe X.
        /// </summary>
        public float RotationX
        {
            get
            {
                return p_rX;
            }

            set
            {
                if ( value > -(float)Math.PI * 2 && value < (float)Math.PI * 2 )
                {
                    p_rX = value;
                }
                else
                {
                    p_rX = 0;
                }
            }
        }

        /// <summary>
        /// Obtient ou d�finit l'angle de rotation de la cam�ra sur l'axe Y.
        /// </summary>
        public float RotationY
        {
            get
            {
                return p_rY;
            }

            set
            {
                if ( value > -(float)Math.PI * 2 && value < (float)Math.PI * 2 )
                {
                    p_rY = value;
                }
                else
                {
                    p_rY = 0;
                }
            }
        }

        /// <summary>
        /// Obtient ou d�finit l'angle de rotation de la cam�ra sur l'axe Z.
        /// </summary>
        public float RotationZ
        {
            get
            {
                return p_rZ;
            }

            set
            {
                if ( value > -(float)Math.PI * 2 && value < (float)Math.PI * 2 )
                {
                    p_rZ = value;
                }
                else
                {
                    p_rZ = 0;
                }
            }
        }

        /// <summary>
        /// Obtient ou d�finit la valeur de la position de la cam�ra sur l'axe X.
        /// </summary>
        public float TranslationX
        {
            get
            {
                return p_tX;
            }

            set
            {
                p_tX = value;
            }
        }

        /// <summary>
        /// Obtient ou d�finit la valeur de la position de la cam�ra sur l'axe Y.
        /// </summary>
        public float TranslationY
        {
            get
            {
                return p_tY;
            }

            set
            {
                p_tY = value;
            }
        }

        /// <summary>
        /// Obtient ou d�finit la valeur de la position de la cam�ra sur l'axe Z.
        /// </summary>
        public float TranslationZ
        {
            get
            {
                return p_tZ;
            }

            set
            {
                p_tZ = value;
            }
        }

        /// <summary>
        /// Obtient ou d�finit l'angle du champ de vue.
        /// </summary>
        public float FieldOfView
        {
            get
            {
                return p_FieldOfView;
            }

            set
            {
                p_FieldOfView = value;
            }
        }

        /// <summary>
        /// Obtient l'aspect du ratio X/Y.
        /// </summary>
        public float AspectRatio
        {
            get
            {
                return p_AspectRatio;
            }
        }

        /// <summary>
        /// Obtient ou d�finit le point de coupe proche.
        /// </summary>
        public float ClipNear
        {
            get
            {
                return p_ClipNear;
            }

            set
            {
                p_ClipNear = value;
            }
        }

        /// <summary>
        /// Obtient ou d�finit le point de coupe lointain.
        /// </summary>
        public float ClipFar
        {
            get
            {
                return p_ClipFar;
            }

            set
            {
                p_ClipFar = value;
            }
        }

        #endregion

        #region Constructeur(s)

        /// <summary>
        /// Constructeur par d�faut. Le ratio est d�fini � 1.
        /// </summary>
        public SOTB_Camera()
        {
        }

        /// <summary>
        /// Deuxi�me constructeur. Le ratio est calcul� en fonction du Device D3D.
        /// </summary>
        /// <param name="theD3DDevice"></param>
        public SOTB_Camera( Device theD3DDevice )
        {
            CalculateAspectRatio( theD3DDevice );
        }

        #endregion

        #region Fonction(s) priv�e(s)

        /// <summary>
        /// Cette fonction calcule la valeur du ratio X/Y.
        /// </summary>
        /// <param name="theD3DDevice">Le pointeur vers le Device Direct3D.</param>
        private void CalculateAspectRatio( Device theD3DDevice )
        {
            if ( theD3DDevice != null )
            {
                p_AspectRatio =
                    (float)theD3DDevice.PresentationParameters.BackBufferWidth /
                    (float)theD3DDevice.PresentationParameters.BackBufferHeight;
            }
        }

        #endregion

        #region Fonction(s) publique(s)

        /// <summary>
        /// Place la cam�ra dans le monde 3D.
        /// </summary>
        /// <param name="theD3DDevice">Le pointeur vers le Device Direct3D.</param>
        /// <param name="Orthogonal">True pour une matrice de projection orthogonale, false pour une matrice 3D.</param>
        public void SetCamera( Device theD3DDevice, bool Orthogonal )
        {
            Matrix RX = Matrix.RotationX( p_rX );
            Matrix RY = Matrix.RotationY( p_rY );
            Matrix RZ = Matrix.RotationZ( p_rZ );

            Matrix T  = Matrix.Translation( p_tX, p_tY, p_tZ );

            Matrix Camera = Matrix.Identity;
            Camera        = Matrix.Multiply( RY, RX );
            Camera        = Matrix.Multiply( Camera, RZ );
            Camera        = Matrix.Multiply( T, Camera );

            theD3DDevice.Transform.World = Matrix.Identity;
            theD3DDevice.Transform.View  = Camera;

            if ( Orthogonal == false )
            {
                theD3DDevice.Transform.Projection =
                    Matrix.PerspectiveFovLH( p_FieldOfView,
                                             p_AspectRatio,
                                             p_ClipNear,
                                             p_ClipFar );
            }
            else
            {
                theD3DDevice.Transform.Projection =
                    Matrix.OrthoLH( (float)theD3DDevice.
                                        PresentationParameters.BackBufferWidth,
                                    (float)theD3DDevice.
                                        PresentationParameters.BackBufferHeight,
                                    p_ClipNear,
                                    p_ClipFar );
            }
        }

        /// <summary>
        /// Restaure la cam�ra apr�s la construction d'un nouveau Device Direct3D.
        /// </summary>
        /// <param name="theD3DDevice">Le pointeur vers le Device Direct3D.</param>
        public void Restore( Device theD3DDevice )
        {
            CalculateAspectRatio( theD3DDevice );
        }

        #endregion
    }

    #endregion
}
