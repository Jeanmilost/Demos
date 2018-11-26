using System;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using Shadow_of_the_Beast.Engine;

namespace Shadow_of_the_Beast.Game
{
    class SOTB_Landscape
    {
        const int p_NbMountainsObj = 3;
        const int p_NbCloudLevel1Obj = 5;
        const int p_NbCloudLevel2Obj = 5;
        const int p_NbCloudLevel3Obj = 5;
        const int p_NbCloudLevel4Obj = 5;
        const int p_NbCloudLevel5Obj = 5;
        const int p_NbFloorLevel1Obj = 6;
        const int p_NbFloorLevel2Obj = 6;
        const int p_NbFloorLevel3Obj = 5;
        const int p_NbFloorLevel4Obj = 5;
        const int p_NbFloorLevel5Obj = 5;

        SOTB_Background p_Sky         = new SOTB_Background();

        SOTB_Sprite     p_Moon        = new SOTB_Sprite();
        SOTB_Sprite[] p_Mountains = new SOTB_Sprite[p_NbMountainsObj];
        SOTB_Sprite[] p_CloudLevel1 = new SOTB_Sprite[p_NbCloudLevel1Obj];
        SOTB_Sprite[] p_CloudLevel2 = new SOTB_Sprite[p_NbCloudLevel2Obj];
        SOTB_Sprite[] p_CloudLevel3 = new SOTB_Sprite[p_NbCloudLevel3Obj];
        SOTB_Sprite[] p_CloudLevel4 = new SOTB_Sprite[p_NbCloudLevel4Obj];
        SOTB_Sprite[] p_CloudLevel5 = new SOTB_Sprite[p_NbCloudLevel5Obj];
        SOTB_Sprite[] p_FloorLevel1 = new SOTB_Sprite[p_NbFloorLevel1Obj];
        SOTB_Sprite[] p_FloorLevel2 = new SOTB_Sprite[p_NbFloorLevel2Obj];
        SOTB_Sprite[] p_FloorLevel3 = new SOTB_Sprite[p_NbFloorLevel3Obj];
        SOTB_Sprite[] p_FloorLevel4 = new SOTB_Sprite[p_NbFloorLevel4Obj];
        SOTB_Sprite[] p_FloorLevel5 = new SOTB_Sprite[p_NbFloorLevel5Obj];

        SOTB_Texture p_Cloud1Txtr = new SOTB_Texture();
        SOTB_Texture p_Cloud2Txtr = new SOTB_Texture();
        SOTB_Texture p_Cloud3Txtr = new SOTB_Texture();
        SOTB_Texture p_Cloud4Txtr = new SOTB_Texture();
        SOTB_Texture p_Cloud5Txtr = new SOTB_Texture();
        SOTB_Texture p_Floor1Txtr = new SOTB_Texture();
        SOTB_Texture p_Floor2Txtr = new SOTB_Texture();
        SOTB_Texture p_Floor3Txtr = new SOTB_Texture();
        SOTB_Texture p_Floor4Txtr = new SOTB_Texture();
        SOTB_Texture p_Floor5Txtr = new SOTB_Texture();
        SOTB_Texture p_MountainsTxtr = new SOTB_Texture();

        public bool Initialize( Device theD3DDevice )
        {
            try
            {
                p_Sky.Height = 210.0f;
                p_Sky.Width = 320.0f;
                p_Sky.TranslationY = 48.0f;

                if ( p_Sky.Initialize( theD3DDevice ) == false )
                {
                    return false;
                }

                p_Moon.Height = 53.0f;
                p_Moon.Width = 53.0f;
                p_Moon.TranslationX = 150.0f;
                p_Moon.TranslationY = -10.0f;
                p_Moon.Filename = "..\\..\\..\\Graphics\\Landscape\\Lune.png";

                if ( p_Moon.Initialize( theD3DDevice ) == false )
                {
                    return false;
                }

                p_Cloud1Txtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Nuages0.png";
                p_Cloud1Txtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbCloudLevel1Obj; i++ )
                {
                    p_CloudLevel1[i] = new SOTB_Sprite();

                    p_CloudLevel1[i].Height = 32.0f;
                    p_CloudLevel1[i].Width = 256.0f;
                    p_CloudLevel1[i].TranslationX = -512.0f + ( 512.0f * i ) - 128.0f;
                    p_CloudLevel1[i].TranslationY = 200.0f;

                    p_CloudLevel1[i].TexturePtr = p_Cloud1Txtr.TexturePtr;

                    if ( p_CloudLevel1[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                p_Cloud2Txtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Nuages1.png";
                p_Cloud2Txtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbCloudLevel2Obj; i++ )
                {
                    p_CloudLevel2[i] = new SOTB_Sprite();

                    p_CloudLevel2[i].Height = 32.0f;
                    p_CloudLevel2[i].Width = 256.0f;
                    p_CloudLevel2[i].TranslationX = -512.0f + ( 512.0f * i );
                    p_CloudLevel2[i].TranslationY = 140.0f;

                    p_CloudLevel2[i].TexturePtr = p_Cloud2Txtr.TexturePtr;

                    if ( p_CloudLevel2[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                p_Cloud3Txtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Nuages2.png";
                p_Cloud3Txtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbCloudLevel3Obj; i++ )
                {
                    p_CloudLevel3[i] = new SOTB_Sprite();

                    p_CloudLevel3[i].Height = 16.0f;
                    p_CloudLevel3[i].Width = 256.0f;
                    p_CloudLevel3[i].TranslationX = -512.0f + ( 512.0f * i );
                    p_CloudLevel3[i].TranslationY = 80.0f;

                    p_CloudLevel3[i].TexturePtr = p_Cloud3Txtr.TexturePtr;

                    if ( p_CloudLevel3[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                p_Cloud4Txtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Nuages3.png";
                p_Cloud4Txtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbCloudLevel4Obj; i++ )
                {
                    p_CloudLevel4[i] = new SOTB_Sprite();

                    p_CloudLevel4[i].Height = 8.0f;
                    p_CloudLevel4[i].Width = 256.0f;
                    p_CloudLevel4[i].TranslationX = -512.0f + ( 512.0f * i );
                    p_CloudLevel4[i].TranslationY = 20.0f;

                    p_CloudLevel4[i].TexturePtr = p_Cloud4Txtr.TexturePtr;

                    if ( p_CloudLevel4[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                p_Cloud5Txtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Nuages4.png";
                p_Cloud5Txtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbCloudLevel5Obj; i++ )
                {
                    p_CloudLevel5[i] = new SOTB_Sprite();

                    p_CloudLevel5[i].Height = 4.0f;
                    p_CloudLevel5[i].Width = 256.0f;
                    p_CloudLevel5[i].TranslationX = -512.0f + ( 512.0f * i );
                    p_CloudLevel5[i].TranslationY = -20.0f;

                    p_CloudLevel5[i].TexturePtr = p_Cloud5Txtr.TexturePtr;

                    if ( p_CloudLevel5[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                p_MountainsTxtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Montagnes.png";
                p_MountainsTxtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbMountainsObj; i++ )
                {
                    p_Mountains[i] = new SOTB_Sprite();

                    p_Mountains[i].Height = 73.0f;
                    p_Mountains[i].Width = 320.0f;
                    p_Mountains[i].TranslationY = -93.0f;

                    p_Mountains[i].TexturePtr = p_MountainsTxtr.TexturePtr;

                    if ( p_Mountains[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }
                p_Floor1Txtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Herbe0.bmp";
                p_Floor1Txtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbFloorLevel1Obj; i++ )
                {
                    p_FloorLevel1[i] = new SOTB_Sprite();

                    p_FloorLevel1[i].Height = 2.0f;
                    p_FloorLevel1[i].Width = 64.0f;
                    p_FloorLevel1[i].TranslationX = -256.0f + ( 128.0f * i );
                    p_FloorLevel1[i].TranslationY = -168.0f;

                    p_FloorLevel1[i].TexturePtr = p_Floor1Txtr.TexturePtr;

                    if ( p_FloorLevel1[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                p_Floor2Txtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Herbe1.bmp";
                p_Floor2Txtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbFloorLevel2Obj; i++ )
                {
                    p_FloorLevel2[i] = new SOTB_Sprite();

                    p_FloorLevel2[i].Height = 4.0f;
                    p_FloorLevel2[i].Width = 64.0f;
                    p_FloorLevel2[i].TranslationX = -256.0f + ( 128.0f * i );
                    p_FloorLevel2[i].TranslationY = -174.0f;

                    p_FloorLevel2[i].TexturePtr = p_Floor2Txtr.TexturePtr;

                    if ( p_FloorLevel2[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                p_Floor3Txtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Herbe2.bmp";
                p_Floor3Txtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbFloorLevel3Obj; i++ )
                {
                    p_FloorLevel3[i] = new SOTB_Sprite();

                    p_FloorLevel3[i].Height = 8.0f;
                    p_FloorLevel3[i].Width = 256.0f;
                    p_FloorLevel3[i].TranslationX = -512.0f + ( 512.0f * i );
                    p_FloorLevel3[i].TranslationY = -184.0f;

                    p_FloorLevel3[i].TexturePtr = p_Floor3Txtr.TexturePtr;

                    if ( p_FloorLevel3[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                p_Floor4Txtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Herbe3.bmp";
                p_Floor4Txtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbFloorLevel4Obj; i++ )
                {
                    p_FloorLevel4[i] = new SOTB_Sprite();

                    p_FloorLevel4[i].Height = 8.0f;
                    p_FloorLevel4[i].Width = 256.0f;
                    p_FloorLevel4[i].TranslationX = -512.0f + ( 512.0f * i );
                    p_FloorLevel4[i].TranslationY = -198.0f;

                    p_FloorLevel4[i].TexturePtr = p_Floor4Txtr.TexturePtr;

                    if ( p_FloorLevel4[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                p_Floor5Txtr.Filename = "..\\..\\..\\Graphics\\Landscape\\Herbe4.bmp";
                p_Floor5Txtr.Initialize( theD3DDevice );

                for ( int i = 0; i < p_NbFloorLevel5Obj; i++ )
                {
                    p_FloorLevel5[i] = new SOTB_Sprite();

                    p_FloorLevel5[i].Height = 16.0f;
                    p_FloorLevel5[i].Width = 256.0f;
                    p_FloorLevel5[i].TranslationX = -512.0f + ( 512.0f * i );
                    p_FloorLevel5[i].TranslationY = -220.0f;

                    p_FloorLevel5[i].TexturePtr = p_Floor5Txtr.TexturePtr;

                    if ( p_FloorLevel5[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                return true;
            }
            catch ( Exception )
            {
                return false;
            }
        }

        public void Render( Device theD3DDevice )
        {
            p_Sky.Render( theD3DDevice );
            p_Moon.Render( theD3DDevice, true );

            for ( int i = 0; i < p_NbCloudLevel1Obj; i++ )
            {
                p_CloudLevel1[i].Render( theD3DDevice, true );
            }

            for ( int i = 0; i < p_NbCloudLevel2Obj; i++ )
            {
                p_CloudLevel2[i].Render( theD3DDevice, true );
            }

            for ( int i = 0; i < p_NbCloudLevel3Obj; i++ )
            {
                p_CloudLevel3[i].Render( theD3DDevice, true );
            }

            for ( int i = 0; i < p_NbCloudLevel4Obj; i++ )
            {
                p_CloudLevel4[i].Render( theD3DDevice, true );
            }

            for ( int i = 0; i < p_NbCloudLevel5Obj; i++ )
            {
                p_CloudLevel5[i].Render( theD3DDevice, true );
            }

            for ( int i = 0; i < p_NbMountainsObj; i++ )
            {
                p_Mountains[i].Render( theD3DDevice, true );
            }

            for ( int i = 0; i < p_NbFloorLevel1Obj; i++ )
            {
                p_FloorLevel1[i].Render( theD3DDevice, false );
            }

            for ( int i = 0; i < p_NbFloorLevel2Obj; i++ )
            {
                p_FloorLevel2[i].Render( theD3DDevice, false );
            }

            for ( int i = 0; i < p_NbFloorLevel3Obj; i++ )
            {
                p_FloorLevel3[i].Render( theD3DDevice, false );
            }

            for ( int i = 0; i < p_NbFloorLevel4Obj; i++ )
            {
                p_FloorLevel4[i].Render( theD3DDevice, false );
            }

            for ( int i = 0; i < p_NbFloorLevel5Obj; i++ )
            {
                p_FloorLevel5[i].Render( theD3DDevice, false );
            }
        }

        public bool Restore( Device theD3DDevice )
        {
            if ( p_Sky.Restore( theD3DDevice ) == false )
            {
                return false;
            }

            if ( p_Moon.Restore( theD3DDevice ) == false )
            {
                return false;
            }

            p_Cloud1Txtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbCloudLevel1Obj; i++ )
            {
                if ( p_CloudLevel1[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_CloudLevel1[i].TexturePtr = p_Floor1Txtr.TexturePtr;
            }

            p_Cloud2Txtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbCloudLevel2Obj; i++ )
            {
                if ( p_CloudLevel2[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_CloudLevel2[i].TexturePtr = p_Floor2Txtr.TexturePtr;
            }

            p_Cloud3Txtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbCloudLevel3Obj; i++ )
            {
                if ( p_CloudLevel3[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_CloudLevel3[i].TexturePtr = p_Floor3Txtr.TexturePtr;
            }

            p_Cloud4Txtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbCloudLevel4Obj; i++ )
            {
                if ( p_CloudLevel4[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_CloudLevel4[i].TexturePtr = p_Floor4Txtr.TexturePtr;
            }

            p_Cloud5Txtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbCloudLevel5Obj; i++ )
            {
                if ( p_CloudLevel5[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_CloudLevel5[i].TexturePtr = p_Floor5Txtr.TexturePtr;
            }

            p_MountainsTxtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbMountainsObj; i++ )
            {
                if ( p_Mountains[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_Mountains[i].TexturePtr = p_MountainsTxtr.TexturePtr;
            }

            p_Floor1Txtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbFloorLevel1Obj; i++ )
            {
                if ( p_FloorLevel1[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_FloorLevel1[i].TexturePtr = p_Floor1Txtr.TexturePtr;
            }

            p_Floor2Txtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbFloorLevel2Obj; i++ )
            {
                if ( p_FloorLevel2[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_FloorLevel2[i].TexturePtr = p_Floor2Txtr.TexturePtr;
            }

            p_Floor3Txtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbFloorLevel3Obj; i++ )
            {
                if ( p_FloorLevel3[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_FloorLevel3[i].TexturePtr = p_Floor3Txtr.TexturePtr;
            }

            p_Floor4Txtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbFloorLevel4Obj; i++ )
            {
                if ( p_FloorLevel4[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_FloorLevel4[i].TexturePtr = p_Floor4Txtr.TexturePtr;
            }

            p_Floor5Txtr.Restore( theD3DDevice );

            for ( int i = 0; i < p_NbFloorLevel5Obj; i++ )
            {
                if ( p_FloorLevel5[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }

                p_FloorLevel5[i].TexturePtr = p_Floor5Txtr.TexturePtr;
            }

            return true;
        }

        public void GoLeft()
        {
            for ( int i = 0; i < p_NbCloudLevel1Obj; i++ )
            {
                p_CloudLevel1[i].TranslationX += 15.0f;

                if ( p_CloudLevel1[i].TranslationX > ( 512.0f * i ) - 1024.0f - 128.0f )
                {
                    p_CloudLevel1[i].TranslationX -= 512.0f;
                }
            }

            for ( int i = 0; i < p_NbCloudLevel2Obj; i++ )
            {
                p_CloudLevel2[i].TranslationX += 12.0f;

                if ( p_CloudLevel2[i].TranslationX > ( 512.0f * i ) - 1024.0f )
                {
                    p_CloudLevel2[i].TranslationX -= 512.0f;
                }
            }

            for ( int i = 0; i < p_NbCloudLevel3Obj; i++ )
            {
                p_CloudLevel3[i].TranslationX += 9.0f;

                if ( p_CloudLevel3[i].TranslationX > ( 512.0f * i ) - 1024.0f )
                {
                    p_CloudLevel3[i].TranslationX -= 512.0f;
                }
            }

            for ( int i = 0; i < p_NbCloudLevel4Obj; i++ )
            {
                p_CloudLevel4[i].TranslationX += 6.0f;

                if ( p_CloudLevel4[i].TranslationX > ( 512.0f * i ) - 1024.0f )
                {
                    p_CloudLevel4[i].TranslationX -= 512.0f;
                }
            }

            for ( int i = 0; i < p_NbCloudLevel5Obj; i++ )
            {
                p_CloudLevel5[i].TranslationX += 3.0f;

                if ( p_CloudLevel5[i].TranslationX > ( 512.0f * i ) - 1024.0f )
                {
                    p_CloudLevel5[i].TranslationX -= 512.0f;
                }
            }
            
            for ( int i = 0; i < p_NbMountainsObj; i++ )
            {
                p_Mountains[i].TranslationX += 1.0f;

                if ( p_Mountains[i].TranslationX > ( 640.0f * i ) )
                {
                    p_Mountains[i].TranslationX -= 640.0f;
                }
            }

            for ( int i = 0; i < p_NbFloorLevel1Obj; i++ )
            {
                p_FloorLevel1[i].TranslationX += 3.0f;

                if ( p_FloorLevel1[i].TranslationX > ( 128.0f * i ) - 256.0f )
                {
                    p_FloorLevel1[i].TranslationX -= 128.0f;
                }
            }

            for ( int i = 0; i < p_NbFloorLevel2Obj; i++ )
            {
                p_FloorLevel2[i].TranslationX += 6.0f;

                if ( p_FloorLevel2[i].TranslationX > ( 128.0f * i ) - 256.0f )
                {
                    p_FloorLevel2[i].TranslationX -= 128.0f;
                }
            }

            for ( int i = 0; i < p_NbFloorLevel3Obj; i++ )
            {
                p_FloorLevel3[i].TranslationX += 9.0f;

                if ( p_FloorLevel3[i].TranslationX > ( 512.0f * i ) - 1024.0f )
                {
                    p_FloorLevel3[i].TranslationX -= 512.0f;
                }
            }

            for ( int i = 0; i < p_NbFloorLevel4Obj; i++ )
            {
                p_FloorLevel4[i].TranslationX += 12.0f;

                if ( p_FloorLevel4[i].TranslationX > ( 512.0f * i ) - 1024.0f )
                {
                    p_FloorLevel4[i].TranslationX -= 512.0f;
                }
            }

            for ( int i = 0; i < p_NbFloorLevel5Obj; i++ )
            {
                p_FloorLevel5[i].TranslationX += 15.0f;

                if ( p_FloorLevel5[i].TranslationX > ( 512.0f * i ) - 1024.0f )
                {
                    p_FloorLevel5[i].TranslationX -= 512.0f;
                }
            }
        }

        public void GoRight()
        {
            for ( int i = 0; i < p_NbCloudLevel1Obj; i++ )
            {
                p_CloudLevel1[i].TranslationX -= 15.0f;

                if ( p_CloudLevel1[i].TranslationX < -1536.0f + ( 512.0f * i ) - 128.0f )
                {
                    p_CloudLevel1[i].TranslationX += 512.0f;
                }
            }

            for ( int i = 0; i < p_NbCloudLevel2Obj; i++ )
            {
                p_CloudLevel2[i].TranslationX -= 12.0f;

                if ( p_CloudLevel2[i].TranslationX < -1536.0f + ( 512.0f * i ) )
                {
                    p_CloudLevel2[i].TranslationX += 512.0f;
                }
            }

            for ( int i = 0; i < p_NbCloudLevel3Obj; i++ )
            {
                p_CloudLevel3[i].TranslationX -= 9.0f;

                if ( p_CloudLevel3[i].TranslationX < -1536.0f + ( 512.0f * i ) )
                {
                    p_CloudLevel3[i].TranslationX += 512.0f;
                }
            }

            for ( int i = 0; i < p_NbCloudLevel4Obj; i++ )
            {
                p_CloudLevel4[i].TranslationX -= 6.0f;

                if ( p_CloudLevel4[i].TranslationX < -1536.0f + ( 512.0f * i ) )
                {
                    p_CloudLevel4[i].TranslationX += 512.0f;
                }
            }

            for ( int i = 0; i < p_NbCloudLevel5Obj; i++ )
            {
                p_CloudLevel5[i].TranslationX -= 3.0f;

                if ( p_CloudLevel5[i].TranslationX < -1536.0f + ( 512.0f * i ) )
                {
                    p_CloudLevel5[i].TranslationX += 512.0f;
                }
            }
            
            for ( int i = 0; i < p_NbMountainsObj; i++ )
            {
                p_Mountains[i].TranslationX -= 1.0f;

                if ( p_Mountains[i].TranslationX < -1280.0f + ( 640.0f * i ) )
                {
                    p_Mountains[i].TranslationX += 640.0f;
                }
            }

            for ( int i = 0; i < p_NbFloorLevel1Obj; i++ )
            {
                p_FloorLevel1[i].TranslationX -= 3.0f;

                if ( p_FloorLevel1[i].TranslationX < -384.0f + ( 128.0f * i ) )
                {
                    p_FloorLevel1[i].TranslationX += 128.0f;
                }
            }

            for ( int i = 0; i < p_NbFloorLevel2Obj; i++ )
            {
                p_FloorLevel2[i].TranslationX -= 6.0f;

                if ( p_FloorLevel2[i].TranslationX < -384.0f + ( 128.0f * i ) )
                {
                    p_FloorLevel2[i].TranslationX += 128.0f;
                }
            }

            for ( int i = 0; i < p_NbFloorLevel3Obj; i++ )
            {
                p_FloorLevel3[i].TranslationX -= 9.0f;

                if ( p_FloorLevel3[i].TranslationX < -1536.0f + ( 512.0f * i ) )
                {
                    p_FloorLevel3[i].TranslationX += 512.0f;
                }
            }

            for ( int i = 0; i < p_NbFloorLevel4Obj; i++ )
            {
                p_FloorLevel4[i].TranslationX -= 12.0f;

                if ( p_FloorLevel4[i].TranslationX < -1536.0f + ( 512.0f * i ) )
                {
                    p_FloorLevel4[i].TranslationX += 512.0f;
                }
            }

            for ( int i = 0; i < p_NbFloorLevel5Obj; i++ )
            {
                p_FloorLevel5[i].TranslationX -= 15.0f;

                if ( p_FloorLevel5[i].TranslationX < -1536.0f + ( 512.0f * i ) )
                {
                    p_FloorLevel5[i].TranslationX += 512.0f;
                }
            }
        }
    }
}
