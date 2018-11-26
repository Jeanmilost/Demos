using System;
using System.Threading;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using Shadow_of_the_Beast.Engine;

namespace Shadow_of_the_Beast.Game
{
    enum Beast_Movements
    {
        Wait = 0,
        Run  = 1,
    };

    class LoopManager
    {
        private int             p_CurrentCount     = 0;
        private int             p_TickCount        = 0;
        private int             p_StartImageLoop   = 0;
        private int             p_EndImageLoop     = 0;
        private int             p_CurrentImageLoop = 0;
        private Beast_Movements p_CurrentMovement  = Beast_Movements.Wait;

        public int TickCount
        {
            set
            {
                if ( value >= 0 )
                {
                    p_TickCount = value;
                }
                else
                {
                    p_TickCount = 0;
                }
            }
        }

        public int CurrentImageLoop
        {
            get
            {
                return p_CurrentImageLoop;
            }
        }

        public LoopManager()
        {
        }

        public void SetMovement( Beast_Movements Value )
        {
            switch ( Value )
            {
                case Beast_Movements.Wait:
                {
                    if ( p_CurrentMovement != Beast_Movements.Wait )
                    {
                        p_StartImageLoop   = 0;
                        p_EndImageLoop     = 0;
                        p_CurrentImageLoop = 0;
                        p_CurrentMovement  = Beast_Movements.Wait;
                    }

                    break;
                }

                case Beast_Movements.Run:
                {
                    if ( p_CurrentMovement != Beast_Movements.Run )
                    {
                        p_StartImageLoop   = 1;
                        p_EndImageLoop     = 11;
                        p_CurrentImageLoop = 1;
                        p_CurrentMovement  = Beast_Movements.Run;
                    }

                    break;
                }

                default:
                {
                    if ( p_CurrentMovement != Beast_Movements.Wait )
                    {
                        p_StartImageLoop   = 0;
                        p_EndImageLoop     = 0;
                        p_CurrentImageLoop = 0;
                        p_CurrentMovement  = Beast_Movements.Wait;
                    }

                    break;
                }
            }
        }

        public void CheckTimer()
        {
            p_CurrentCount++;

            if ( p_CurrentCount >= p_TickCount )
            {
                p_CurrentImageLoop++;

                if ( p_CurrentImageLoop > p_EndImageLoop )
                {
                    p_CurrentImageLoop = p_StartImageLoop;
                }

                p_CurrentCount = 0;
            }
        }
    }

    class SOTB_Beast
    {
        private const int       p_NbSprites       = 12;

        private bool            p_LookAtLeft      = false;

        private SOTB_Sprite[]   p_Sprite          = new SOTB_Sprite[p_NbSprites];

        private LoopManager     p_LoopManager     = new LoopManager();

        private Beast_Movements p_CurrentMovement = Beast_Movements.Wait;

        private string[]        p_TextureName     = new string[]
        {
            "bete_attend_centree_fmtown.png",
            "bete_marche_centree_fmtown_01.png",
            "bete_marche_centree_fmtown_02.png",
            "bete_marche_centree_fmtown_03.png",
            "bete_marche_centree_fmtown_04.png",
            "bete_marche_centree_fmtown_05.png",
            "bete_marche_centree_fmtown_06.png",
            "bete_marche_centree_fmtown_07.png",
            "bete_marche_centree_fmtown_08.png",
            "bete_marche_centree_fmtown_09.png",
            "bete_marche_centree_fmtown_10.png",
            "bete_marche_centree_fmtown_11.png",
        };

        public bool Initialize( Device theD3DDevice )
        {
            try
            {
                for ( int i = 0; i < p_NbSprites; i++ )
                {
                    p_Sprite[i] = new SOTB_Sprite();

                    p_Sprite[i].Height = 50.0f;
                    p_Sprite[i].Width = 25.0f;
                    p_Sprite[i].Filename = "..\\..\\..\\Graphics\\Sprites_bete\\" + p_TextureName[i];
                    p_Sprite[i].TranslationY = -168.0f;

                    if ( p_Sprite[i].Initialize( theD3DDevice ) == false )
                    {
                        return false;
                    }
                }

                p_LoopManager.TickCount = 2;

                return true;
            }
            catch ( Exception )
            {
                return false;
            }
        }

        public void Render( Device theD3DDevice )
        {
            p_LoopManager.SetMovement( p_CurrentMovement );

            if ( p_Sprite.GetUpperBound( 0 ) >= p_LoopManager.CurrentImageLoop )
            {
                if ( p_LookAtLeft == true )
                {
                    theD3DDevice.RenderState.CullMode = Cull.Clockwise;

                    p_Sprite[p_LoopManager.CurrentImageLoop].RotationY =
                        (float)Math.PI;
                }
                else
                {
                    p_Sprite[p_LoopManager.CurrentImageLoop].RotationY = 0.0f;
                }

                p_Sprite[p_LoopManager.CurrentImageLoop].Render( theD3DDevice,
                                                                 true );

                theD3DDevice.RenderState.CullMode = Cull.CounterClockwise;
            }
        }

        public void BeastRunRight()
        {
            p_LookAtLeft = false;
            p_CurrentMovement = Beast_Movements.Run;
        }

        public void BeastRunLeft()
        {
            p_LookAtLeft = true;
            p_CurrentMovement = Beast_Movements.Run;
        }

        public void BeastWait()
        {
            p_CurrentMovement = Beast_Movements.Wait;
        }

        public void RunTimer()
        {
            p_LoopManager.CheckTimer();
        }

        public bool Restore( Device theD3DDevice )
        {
            for ( int i = 0; i < 11; i++ )
            {
                if ( p_Sprite[i].Restore( theD3DDevice ) == false )
                {
                    return false;
                }
            }

            return true;
        }
    }
}
