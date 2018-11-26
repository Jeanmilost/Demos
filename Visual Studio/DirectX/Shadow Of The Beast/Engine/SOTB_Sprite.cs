/*********************************************************************************
 * ==> Classe SOTB_Sprite -------------------------------------------------------*
 * *******************************************************************************
 * Description : Cette classe permet la création d'un sprite dans le monde 3D.   *
 * Version     : 1.0                                                             *
 * Développeur : Jean-Milost Reymond                                             *
 *********************************************************************************/

using System;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace Shadow_of_the_Beast.Engine
{
    #region Classe SOTB_Sprite

    class SOTB_Sprite
    {
        #region Variable(s) de la classe

        private VertexBuffer p_VertexBuffer = null;

        private Texture      p_Texture      = null;

        private float        p_Width        = 1.0f;
        private float        p_Height       = 1.0f;
        private float        p_rX           = 0.0f;
        private float        p_rY           = 0.0f;
        private float        p_rZ           = 0.0f;
        private float        p_tX           = 0.0f;
        private float        p_tY           = 0.0f;
        private float        p_tZ           = 5.0f;

        private string       p_Filename     = "";

        #endregion

        #region Propriété(s)

        /// <summary>
        /// Obtient ou définit le nom du fichier des textures.
        /// </summary>
        public string Filename
        {
            get
            {
                return p_Filename;
            }

            set
            {
                p_Filename = value;
            }
        }

        /// <summary>
        /// Définit la texture du sprite.
        /// </summary>
        public Texture TexturePtr
        {
            set
            {
                p_Texture = value;
            }
        }

        /// <summary>
        /// Obtient ou définit la largeur du sprite.
        /// </summary>
        public float Width
        {
            get
            {
                return p_Width;
            }

            set
            {
                p_Width = value;
            }
        }

        /// <summary>
        /// Obtient ou définit la hauteur du sprite.
        /// </summary>
        public float Height
        {
            get
            {
                return p_Height;
            }

            set
            {
                p_Height = value;
            }
        }

        /// <summary>
        /// Obtient ou définit l'angle de rotation du sprite sur l'axe X.
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
        /// Obtient ou définit l'angle de rotation du sprite sur l'axe Y.
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
        /// Obtient ou définit l'angle de rotation du sprite sur l'axe Z.
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
        /// Obtient ou définit la valeur de la position du sprite sur l'axe X.
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
        /// Obtient ou définit la valeur de la position du sprite sur l'axe Y.
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
        /// Obtient ou définit la valeur de la position du sprite sur l'axe Z.
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

        #endregion

        #region Fonction(s) privée(s)

        /// <summary>
        /// Cette fonction construit la matrice d'affichage locale pour le sprite.
        /// </summary>
        /// <returns>La matrice d'affichage locale.</returns>
        private Matrix GetWorldMatrix()
        {
            Matrix RX = Matrix.RotationX( p_rX );
            Matrix RY = Matrix.RotationY( p_rY );
            Matrix RZ = Matrix.RotationZ( p_rZ );

            Matrix SpriteMatrix = Matrix.Identity;

            SpriteMatrix     = Matrix.Multiply( RX, SpriteMatrix );
            SpriteMatrix     = Matrix.Multiply( RY, SpriteMatrix );
            SpriteMatrix     = Matrix.Multiply( RZ, SpriteMatrix );
            SpriteMatrix.M41 = p_tX;
            SpriteMatrix.M42 = p_tY;
            SpriteMatrix.M43 = p_tZ;

            return SpriteMatrix;
        }

        /// <summary>
        /// Cette fonction configure l'affichage pour permettre la transparence.
        /// </summary>
        /// <param name="theD3DDevice">Le pointeur vers le Device Direct3D.</param>
        private void SetAlphaBlending( Device theD3DDevice )
        {
            Caps theCaps;

            theD3DDevice.RenderState.AlphaBlendEnable = true;
            theD3DDevice.RenderState.SourceBlend      = Blend.SourceAlpha;
            theD3DDevice.RenderState.DestinationBlend = Blend.InvSourceAlpha;

            if ( theCaps.AlphaCompareCaps.SupportsGreaterEqual )
            {
                theD3DDevice.RenderState.AlphaTestEnable = true;
                theD3DDevice.RenderState.ReferenceAlpha  = 0x08;
                theD3DDevice.RenderState.AlphaFunction   = Compare.GreaterEqual;
            }
        }

        /// <summary>
        /// Cette fonction restaure les paramètres normaux d'affichage.
        /// </summary>
        /// <param name="theD3DDevice">Le pointeur vers le Device Direct3D.</param>
        private void RestoreNormalProperties( Device theD3DDevice )
        {
            theD3DDevice.RenderState.AlphaTestEnable  = false;
            theD3DDevice.RenderState.AlphaBlendEnable = false;
        }

        #endregion

        #region Fonction(s) publique(s)

        /// <summary>
        /// Cette fonction initialise le sprite.
        /// </summary>
        /// <param name="theD3DDevice">Le pointeur vers le Device Direct3D.</param>
        /// <returns>True si l'initialisation est correcte, sinon false.</returns>
        public bool Initialize( Device theD3DDevice )
        {
            try
            {
                p_VertexBuffer =
                    new VertexBuffer( typeof( CustomVertex.PositionColoredTextured ),
                                      4,
                                      theD3DDevice,
                                      0,
                                      CustomVertex.PositionColoredTextured.Format,
                                      Pool.Default );

                CustomVertex.PositionColoredTextured[] p_Vertices =
                    (CustomVertex.PositionColoredTextured[])p_VertexBuffer.Lock
                        ( 0, 0 );

                p_Vertices[0].X     = -p_Width;
                p_Vertices[0].Y     = -p_Height;
                p_Vertices[0].Z     = 0.0f;
                p_Vertices[0].Tu    = 0.0f;
                p_Vertices[0].Tv    = 1.0f;
                p_Vertices[0].Color = System.Drawing.Color.White.ToArgb();
                p_Vertices[1].X     = -p_Width;
                p_Vertices[1].Y     = p_Height;
                p_Vertices[1].Z     = 0.0f;
                p_Vertices[1].Tu    = 0.0f;
                p_Vertices[1].Tv    = 0.0f;
                p_Vertices[1].Color = System.Drawing.Color.White.ToArgb();
                p_Vertices[2].X     = p_Width;
                p_Vertices[2].Y     = -p_Height;
                p_Vertices[2].Z     = 0.0f;
                p_Vertices[2].Tu    = 1.0f;
                p_Vertices[2].Tv    = 1.0f;
                p_Vertices[2].Color = System.Drawing.Color.White.ToArgb();
                p_Vertices[3].X     = p_Width;
                p_Vertices[3].Y     = p_Height;
                p_Vertices[3].Z     = 0.0f;
                p_Vertices[3].Tu    = 1.0f;
                p_Vertices[3].Tv    = 0.0f;
                p_Vertices[3].Color = System.Drawing.Color.White.ToArgb();

                p_VertexBuffer.Unlock();

                if ( p_Filename != "" )
                {
                    p_Texture = TextureLoader.FromFile( theD3DDevice,
                                                        p_Filename,
                                                        D3DX.Default,
                                                        D3DX.Default,
                                                        D3DX.Default,
                                                        0,
                                                        Format.Unknown,
                                                        Pool.Managed,
                                                        Filter.Triangle |
                                                            Filter.Mirror,
                                                        Filter.Triangle |
                                                            Filter.Mirror,
                                                        0 );
                }

                return true;
            }
            catch ( Exception )
            {
                return false;
            }
        }

        /// <summary>
        /// Cette fonction permet le rendu du sprite.
        /// </summary>
        /// <param name="theD3DDevice">Le pointeur vers le Device Direct3D.</param>
        /// <param name="DoEnableTransparency">Enclenche ou déclenche la transparence.</param>
        public void Render( Device theD3DDevice, bool DoEnableTransparency )
        {
            if ( DoEnableTransparency == true )
            {
                SetAlphaBlending( theD3DDevice );
            }

            theD3DDevice.Transform.World = GetWorldMatrix();

            theD3DDevice.SetStreamSource( 0, p_VertexBuffer, 0 );
            theD3DDevice.VertexFormat = CustomVertex.PositionColoredTextured.Format;
            theD3DDevice.SetTexture( 0, p_Texture );
            theD3DDevice.DrawPrimitives( PrimitiveType.TriangleStrip, 0, 2 );

            RestoreNormalProperties( theD3DDevice );
        }

        /// <summary>
        /// Restaure le sprite après la construction d'un nouveau Device Direct3D.
        /// </summary>
        /// <param name="theD3DDevice">Le pointeur vers le Device Direct3D.</param>
        /// <returns>True si la restauration est correcte, sinon false.</returns>
        public bool Restore( Device theD3DDevice )
        {
            if ( p_VertexBuffer != null )
            {
                p_VertexBuffer.Dispose();
            }

            if ( p_Texture != null )
            {
                p_Texture.Dispose();
            }

            return Initialize( theD3DDevice );
        }

        #endregion
    }

    #endregion
}
