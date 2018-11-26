/*********************************************************************************
 * ==> Classe SOTB_Texture ------------------------------------------------------*
 * *******************************************************************************
 * Description : Cette classe permet la création et la gestion d'une texture,    *
 *               utilisable pour un ou plusieurs objet(s) 3D.                    *
 * Version     : 1.0                                                             *
 * Développeur : Jean-Milost Reymond                                             *
 *********************************************************************************/

using System;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace Shadow_of_the_Beast.Engine
{
    class SOTB_Texture
    {
        #region Variable(s) de la classe

        private Texture p_Texture   = null;

        private int     p_Width     = D3DX.Default;
        private int     p_Height    = D3DX.Default;
        private int     p_MipLevel  = D3DX.Default;
        private int     p_ColorKey  = 0;

        private Usage   p_Usage     = 0;

        private Format  p_Format    = Format.Unknown;

        private Pool    p_Pool      = Pool.Managed;

        private Filter  p_Filter    = Filter.Triangle | Filter.Mirror;
        private Filter  p_MipFilter = Filter.Triangle | Filter.Mirror;

        private string  p_Filename  = "";

        #endregion

        #region Propriété(s)

        /// <summary>
        /// Obtient le pointeur vers l'objet texture.
        /// </summary>
        public Texture TexturePtr
        {
            get
            {
                return p_Texture;
            }
        }

        /// <summary>
        /// Obtient ou définit la largeur de la texture, en pixels.
        /// </summary>
        public int Width
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
        /// Obtient ou définit la hauteur de la texture, en pixels.
        /// </summary>
        public int Height
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
        /// Obtient ou définit le niveau de mip-mapping de la texture.
        /// </summary>
        public int MipLevel
        {
            get
            {
                return p_MipLevel;
            }

            set
            {
                p_MipLevel = value;
            }
        }

        /// <summary>
        /// Obtient ou définit la clé de couleur de la texture.
        /// </summary>
        public int ColorKey
        {
            get
            {
                return p_ColorKey;
            }

            set
            {
                p_ColorKey = value;
            }
        }

        /// <summary>
        /// Obtient ou définit l'usage de la texture.
        /// </summary>
        public Usage UsagePtr
        {
            get
            {
                return p_Usage;
            }

            set
            {
                p_Usage = value;
            }
        }

        /// <summary>
        /// Obtient ou définit le format de la texture.
        /// </summary>
        public Format FormatPtr
        {
            get
            {
                return p_Format;
            }

            set
            {
                p_Format = value;
            }
        }

        /// <summary>
        /// Obtient ou définit le type de mémoire de stockage de la texture.
        /// </summary>
        public Pool PoolPtr
        {
            get
            {
                return p_Pool;
            }

            set
            {
                p_Pool = value;
            }
        }

        /// <summary>
        /// Obtient ou définit le filtre d'affichage de la texture.
        /// </summary>
        public Filter FilterPtr
        {
            get
            {
                return p_Filter;
            }

            set
            {
                p_Filter = value;
            }
        }

        /// <summary>
        /// Obtient ou définit le filtre d'affichage pour le mip-mapping.
        /// </summary>
        public Filter MipFilterPtr
        {
            get
            {
                return p_MipFilter;
            }

            set
            {
                p_MipFilter = value;
            }
        }

        /// <summary>
        /// Obtient ou définit le nom du fichier de l'image de la texture.
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

        #endregion

        #region Constructeur(s)

        /// <summary>
        /// Constructeur par défaut de la classe E_Texture.
        /// </summary>
        public SOTB_Texture()
        {
        }

        /// <summary>
        /// Deuxième constructeur de la classe E_Texture.
        /// </summary>
        /// <param name="Filename">Le nom du fichier de l'image.</param>
        public SOTB_Texture( string Filename )
        {
            p_Filename = Filename;
        }

        #endregion

        #region Fonction(s) publique(s)

        /// <summary>
        /// Cette fonction initialise la texture.
        /// </summary>
        /// <param name="theD3DDevice">Le pointeur vers le device Direct3D.</param>
        /// <returns>True si l'initialisation est correcte, sinon false.</returns>
        public bool Initialize( Device theD3DDevice )
        {
            try
            {
                if ( p_Filename != "" )
                {
                    p_Texture = TextureLoader.FromFile( theD3DDevice,
                                                        p_Filename,
                                                        p_Width,
                                                        p_Height,
                                                        p_MipLevel,
                                                        p_Usage,
                                                        p_Format,
                                                        p_Pool,
                                                        p_Filter,
                                                        p_MipFilter,
                                                        p_ColorKey );

                    return true;
                }
                else
                {
                    p_Texture = null;
                    return false;
                }
            }
            catch ( Exception )
            {
                p_Texture = null;
                return false;
            }
        }

        /// <summary>
        /// Restaure la texture après la construction d'un nouveau Device Direct3D.
        /// </summary>
        /// <param name="theD3DDevice">Le pointeur vers le Device Direct3D.</param>
        /// <returns>True si la restauration est correcte, sinon false.</returns>
        public bool Restore( Device theD3DDevice )
        {
            if ( p_Texture != null )
            {
                p_Texture.Dispose();
                p_Texture = null;
            }

            return Initialize( theD3DDevice );
        }

        #endregion
    }
}
