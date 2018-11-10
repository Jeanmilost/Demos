// Arkanoid ** Version Windows avec DirectX.

#include "stdafx.h"
#include "Barre.h"
#include "Balle.h"
#include "Missiles.h"
#include "Brick.h"
#include "Levels.h"
#include "Bonus.h"
#include "BonusEffects.h"
#define INITGUID
#include <stdio.h>
#include "DirectXTools.h"
#include <mmsystem.h>

//-- Information de classes Windows.

static char szClass[] = "XmplArkaClass";
static char szCaption[] = "Arkanoid";

//-- Chaines des indicateurs d'erreurs.
const char *ErrStr=NULL;

//-- Messages d'erreurs.
const char Err_Reg_Class[]			= "Error Registering Window Class";
const char Err_Create_Win[]			= "Error Creating Window";
const char Err_DD[]					= "Impossible d'acceder a DirectDraw.";
const char Err_DI[]					= "Impossible d'acceder a DirectInput.";

// - Données de jeu.

const char B='B', V='V', R='R', G='G', O='O', Y='Y', N='N';
ClassicBarre BarreObj;
int Nbr_Brick=60;
Brick BrickObj[60];
Bonus BonusObj[60];
Effect EffectObj;
Missiles MissileObj;
Level LevelObj[]={Level1(), Level2(), Level3(), Level4(), Level5()};
int Lvl=0;
int Loop=640-BarreObj.ReturnDimRightBarre();
BOOL LevelFlag=FALSE;
BOOL NewLevelFlag=FALSE;
BOOL ConstructorFlag=FALSE;
Balle BalleObj(BarreObj.ReturnXBarre(), BarreObj.ReturnYBarre());
BOOL KeyRight=FALSE, KeyLeft=FALSE, KeyUp=FALSE;
int OldMoveMouse=0, OldMoveCount=0;
BOOL AutorisationRedimBarre=TRUE;

// **************************************************
// *												*
// * Nettoie la mémoire - Detruit les objets DirectX*
// * et Windows.									*
// *												*
// **************************************************

void Cleanup()
{
	EraseDDraw();

	EraseDInput();

	// Transmets les erreurs.

	if (ErrStr) 
	{
		MessageBox(NULL, ErrStr, szCaption, MB_OK);
		ErrStr=NULL;
	}
}

// **************************************************
// *												*
// *  Fonction de blitting des surfaces secondaires	*
// *  dans la surface primaire.						*
// *												*
// **************************************************

void Draw_Surface()
{	

	BackGround(Lvl+1);

	// Dessine les sprites.

	for (int j=0; j<Nbr_Brick; j++)
	{
		if (BonusObj[j].Existence() && BonusObj[j].Autorisation())
		{
			Sprite(BonusObj[j].ReturnXBonus(),
					BonusObj[j].ReturnYBonus(),
					BonusObj[j].ReturnLeftBonus(),
					BonusObj[j].ReturnRightBonus(),
					BonusObj[j].ReturnTopBonus(),
					BonusObj[j].ReturnBottomBonus()
					);
		}
	}

	for (int i=0; i<Nbr_Brick; i++)
	{
		if (BrickObj[i].State())
		{
			Sprite(BrickObj[i].ReturnXBrick(),
					BrickObj[i].ReturnYBrick(),
					BrickObj[i].ReturnDimLeftBrick(),
					BrickObj[i].ReturnDimRightBrick(),
					BrickObj[i].ReturnDimTopBrick(),
					BrickObj[i].ReturnDimBottomBrick()
					);
		}
	}

	if (BalleObj.BalleInScene())
	{
		Sprite(BalleObj.ReturnXBalle(),
				BalleObj.ReturnYBalle(),
				BalleObj.ReturnDimLeftBalle(),
				BalleObj.ReturnDimRightBalle(),
				BalleObj.ReturnDimTopBalle(),
				BalleObj.ReturnDimBottomBalle()
				);
	}

	Sprite(BarreObj.ReturnXBarre(),
			BarreObj.ReturnYBarre(),
			BarreObj.ReturnDimLeftBarre(),
			BarreObj.ReturnConstRightBarre()/2,
			BarreObj.ReturnDimTopBarre(),
			BarreObj.ReturnDimBottomBarre()
			);

	Sprite(BarreObj.ReturnXBarre()+(BarreObj.ReturnDimRightBarre()-BarreObj.ReturnDimLeftBarre())-(BarreObj.ReturnConstRightBarre()/2),
			BarreObj.ReturnYBarre(),
			BarreObj.ReturnDimLeftBarre()+(BarreObj.ReturnConstRightBarre()/2),
			BarreObj.ReturnConstRightBarre(),
			BarreObj.ReturnDimTopBarre(),
			BarreObj.ReturnDimBottomBarre()
			);

	EffectObj.CreateEffect(BarreObj);
	for (int i=0; i<EffectObj.ReturnCounter()+1; i++)
	{
		Sprite(EffectObj.ReturnXPosition(i),
				EffectObj.ReturnYPosition(i),
				EffectObj.ReturnLeft(i),
				EffectObj.ReturnRight(i),
				EffectObj.ReturnTop(i),
				EffectObj.ReturnBottom(i)
				);
	}

	if (MissileObj.Missile_OK())
	{
		if (MissileObj.ReturnMissileFlag1())
		{
			Sprite(MissileObj.ReturnMissileX1(),
				   MissileObj.ReturnMissileY1(),
				   MissileObj.ReturnLeft(),
				   MissileObj.ReturnRight(),
				   MissileObj.ReturnTop(),
				   MissileObj.ReturnBottom()
				   );
		}

		if (MissileObj.ReturnMissileFlag2())
		{
			Sprite(MissileObj.ReturnMissileX2(),
				   MissileObj.ReturnMissileY2(),
				   MissileObj.ReturnLeft(),
				   MissileObj.ReturnRight(),
				   MissileObj.ReturnTop(),
				   MissileObj.ReturnBottom()
				   );
		}
	}

	// Permute le BackBuffer et la surface primaire.

	lpDDSPrimary->Flip(0,DDFLIP_WAIT); 

}

// **************************************************
// *												*
// *         Boucle des messages de Windows.     	*
// *												*
// **************************************************


LRESULT CALLBACK 
WindowProc(HWND hWnd, unsigned uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

        case WM_DESTROY:
			Cleanup();
            PostQuitMessage(0);
            break;

		case WM_SETCURSOR:
			SetCursor(NULL);
			return TRUE;

        case WM_KEYDOWN: 
            switch (wParam) 
            {
				case VK_ESCAPE:

					DestroyWindow(hWnd);
					break;

                default:
                    break; 
            }

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0L;
}

// **************************************************
// *												*
// *    Fonction d'initialisation de l'interface	*
// *	Windows et de l'objet DirectDraw.			*
// *												*
// **************************************************

static BOOL Init(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASS                    wc;

    // Registre de données de la classe fenêtre.

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(DWORD);
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szClass;
    if (!RegisterClass(&wc)) 
	{
		ErrStr=Err_Reg_Class;
        return FALSE;
	}

    // Get dimensions of display

    int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Crée une fenêtre et l'affiche.

	HWND hWnd;

    hWnd = CreateWindow(szClass,							// class
                        szCaption,							// caption
						WS_VISIBLE|WS_POPUP,				// style 
						0,									// left
						0,									// top
						ScreenWidth,						// width
						ScreenHeight,						// height
                        NULL,								// parent window
                        NULL,								// menu 
                        hInstance,							// instance
                        NULL);								// parms
    if (!hWnd) 
	{
		ErrStr=Err_Create_Win;
        return FALSE;
	}
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


	if (!InitDDraw(hWnd))
	{
		ErrStr=Err_DD;
		return FALSE;
	}

	if (!InitDInput(hInstance, hWnd))
	{
		ErrStr=Err_DI;
		return FALSE;
	}

	Draw_Surface();

	return TRUE;
}

// **************************************************
// *												*
// *		     Constructeur de niveau.            *
// *												*
// **************************************************

// Les bonus sont tirés au hasard, et correspondent
// à la couleur de la brique.

// Cette boucle teste également si le niveau est
// passé, et installe le niveau suivant.

BOOL InitLevel()
{
	int Value=0;
	for (int b=0; b<Nbr_Brick; b++)
	{
		Value+=LevelObj[Lvl].ReturnValueBrick(b);
	}

	if (!Value || NewLevelFlag)
	{
		LevelFlag=FALSE;
		NewLevelFlag=FALSE;
		Lvl++;
		for (int i=0; i<Nbr_Brick; i++)
		{
			BonusObj[i].Reset();
		}
		BalleObj.Reset();
		BalleObj.ResetBonusAndMalus();
		BarreObj.ResetBonusAndMalus();
		EffectObj.ResetAll();
		MissileObj.DesactiveMissile(BarreObj);
	}

	if (!LevelFlag || ConstructorFlag)
	{
		InitAxes(BarreObj.ReturnXBarre(), BarreObj.ReturnYBarre());
		ConstructorFlag=FALSE;
		srand((unsigned)time(NULL));
		LevelFlag=TRUE;
		for (int b=0; b<Nbr_Brick; b++)
		{
			int Bonus_Malus=1+rand()%2;
			int Create=rand()%10;
			if (LevelObj[Lvl].ReturnName(b)==B) 
			{
				BrickObj[b]=Blue(LevelObj[Lvl].ReturnXValue(b),LevelObj[Lvl].ReturnYValue(b));
				BonusObj[b]=BlueBonus();
				if (Create>5) BonusObj[b].Autorise();
			}

			if (LevelObj[Lvl].ReturnName(b)==V) 
			{
				BrickObj[b]=Violet(LevelObj[Lvl].ReturnXValue(b),LevelObj[Lvl].ReturnYValue(b));
				BonusObj[b]=VioletBonus();
				if (Create>5) BonusObj[b].Autorise();

			}
			if (LevelObj[Lvl].ReturnName(b)==G) 
			{
				BrickObj[b]=Green(LevelObj[Lvl].ReturnXValue(b),LevelObj[Lvl].ReturnYValue(b));
				BonusObj[b]=GreenBonus();
				if (Create>5) BonusObj[b].Autorise();
			}
			if (LevelObj[Lvl].ReturnName(b)==R) 
			{
				BrickObj[b]=Red(LevelObj[Lvl].ReturnXValue(b),LevelObj[Lvl].ReturnYValue(b));
				BonusObj[b]=RedBonus();
				if (Create>5) BonusObj[b].Autorise();
			}
			if (LevelObj[Lvl].ReturnName(b)==O) 
			{
				BrickObj[b]=Orange(LevelObj[Lvl].ReturnXValue(b),LevelObj[Lvl].ReturnYValue(b));
				BonusObj[b]=OrangeBonus();
				if (Create>5) BonusObj[b].Autorise();
			}
			if (LevelObj[Lvl].ReturnName(b)==Y)
			{
				BrickObj[b]=Yellow(LevelObj[Lvl].ReturnXValue(b),LevelObj[Lvl].ReturnYValue(b));
				LevelObj[Lvl].DeleteValueBrick(b);
			}
			if (LevelObj[Lvl].ReturnName(b)==N)
			{
				BrickObj[b]=Brick(FALSE);
				LevelObj[Lvl].DeleteValueBrick(b);
			}
		}
		lp_BufferSurfaceCaller[Lvl+1]=bitmap_surface(FileNames[Lvl+1]);
		if (!lp_BufferSurfaceCaller[Lvl+1])
		return FALSE;
	}
	return TRUE;
}

// **************************************************
// *												*
// *		   Boucle des messages du jeu.          *
// *												*
// **************************************************

// Factor : Facteur de réactivité de la souris.
//			Plus le facteur est élevé, et plus la
//			souris réagit lentement.

void GameMessages()
{
	int Factor=2;
	if (BarreObj.Slow_OK()) Factor=10;

	InitAxes(BarreObj.ReturnXBarre()*Factor, BarreObj.ReturnYBarre());

	KeyRight=KeyLeft=KeyUp=FALSE;

	if (LEFT()) 
		KeyLeft=TRUE; 
 
	if (RIGHT()) 
		KeyRight=TRUE;

	if (UP())
		KeyUp=TRUE;

	if (SPACE())
	{
		BalleObj.SendBalle();
		MissileObj.SendMissile(BarreObj);
	}

	// Gestion de la souris.

	BOOL MouseMoving=ReadMouse();

	// Les axes...

	int Move=AxeX(0, (640*Factor)-((BarreObj.ReturnDimRightBarre()*Factor)-(BarreObj.ReturnDimLeftBarre()*Factor)))/Factor;
	BarreObj.XBarreEntrie(Move);

	if (BarreObj.Slow_OK())
	{
		int DeltaMove=Move-OldMoveMouse;

		if (DeltaMove>0)
		{
			EffectObj.XRightEntrie(TRUE);
			EffectObj.XLeftEntrie(FALSE);
			OldMoveCount=0;
		}

		if (DeltaMove<0)
		{
			EffectObj.XLeftEntrie(TRUE);
			EffectObj.XRightEntrie(FALSE);
			OldMoveCount=0;
		}

		if (!MouseMoving)
		{
			OldMoveCount++;
			if (OldMoveCount>3)
			{
				EffectObj.XRightEntrie(FALSE);
				EffectObj.XLeftEntrie(FALSE);
				OldMoveCount=0;
			}
		}
	}

	// Lancement de la balle + Missiles...

	if (Button0())
	{
		BalleObj.SendBalle();
		MissileObj.SendMissile(BarreObj);
	}

	// Et la barre volante.

	if (BarreObj.Fly_OK()) 
	{
		if (Button1())
		{
			KeyUp=TRUE;
		}
	}

	OldMoveMouse=Move;

	// Codes de triche...

	if (RSHIFT() && LSHIFT() && SPACE() && UP())
		NewLevelFlag=TRUE;

	if (LSHIFT() && LEFT() && RIGHT())
		BalleObj.Reset();
}

// **************************************************
// *												*
// *					Le jeu.			            *
// *												*
// **************************************************

void Game()
{
	BarreObj.LongBarre();
	BarreObj.ShortBarre();
	BarreObj.NormalBarre();
	if (BarreObj.Level_OK())
	{
		if (AutorisationRedimBarre)
		{
			Loop=640-BarreObj.ReturnDimRightBarre();
			AutorisationRedimBarre=FALSE;
		}
		Loop++;
		BarreObj.MoveBarre(Loop);

		if (BalleObj.BalleInScene())
		{
			BalleObj.SendBalle();
			BalleObj.MoveBalle();
		}

		MissileObj.MoveMissile(BarreObj);

		for (int i=0; i<Nbr_Brick; i++)
		{
			BonusObj[i].MoveBonus();
			if (BrickObj[i].State() && BrickObj[i].Algorythm((float)BalleObj.ReturnXBalle(), (float)BalleObj.ReturnYBalle(), BalleObj.ReturnRayon(), BalleObj.ReturnOffsetX(), BalleObj.ReturnOffsetY()))
			{
				BalleObj.NewOffsetX(BrickObj[i].ReturnNewOffsetX());
				BalleObj.NewOffsetY(BrickObj[i].ReturnNewOffsetY());
				BalleObj.UNLOCK();
				LevelObj[Lvl].DeleteValueBrick(i);
				if (LevelObj[Lvl].ReturnName(i)==R || LevelObj[Lvl].ReturnName(i)==O || LevelObj[Lvl].ReturnName(i)==G || LevelObj[Lvl].ReturnName(i)==B || LevelObj[Lvl].ReturnName(i)==V)
				{
					BonusObj[i].Start(BrickObj[i].ReturnXBrick(),BrickObj[i].ReturnYBrick(), (BrickObj[i].ReturnDimRightBrick()-BrickObj[i].ReturnDimLeftBrick()), (BrickObj[i].ReturnDimBottomBrick()-BrickObj[i].ReturnDimTopBrick()));
				}
			}

			if (BrickObj[i].State() && BrickObj[i].Missile1Algorythm(MissileObj))
			{
				MissileObj.StopMissile1();
				LevelObj[Lvl].DeleteValueBrick(i);
				if (LevelObj[Lvl].ReturnName(i)==R || LevelObj[Lvl].ReturnName(i)==O || LevelObj[Lvl].ReturnName(i)==G || LevelObj[Lvl].ReturnName(i)==B || LevelObj[Lvl].ReturnName(i)==V)
				{
					BonusObj[i].Start(BrickObj[i].ReturnXBrick(),BrickObj[i].ReturnYBrick(), (BrickObj[i].ReturnDimRightBrick()-BrickObj[i].ReturnDimLeftBrick()), (BrickObj[i].ReturnDimBottomBrick()-BrickObj[i].ReturnDimTopBrick()));
				}
			}

			if (BrickObj[i].State() && BrickObj[i].Missile2Algorythm(MissileObj))
			{
				MissileObj.StopMissile2();
				LevelObj[Lvl].DeleteValueBrick(i);
				if (LevelObj[Lvl].ReturnName(i)==R || LevelObj[Lvl].ReturnName(i)==O || LevelObj[Lvl].ReturnName(i)==G || LevelObj[Lvl].ReturnName(i)==B || LevelObj[Lvl].ReturnName(i)==V)
				{
					BonusObj[i].Start(BrickObj[i].ReturnXBrick(),BrickObj[i].ReturnYBrick(), (BrickObj[i].ReturnDimRightBrick()-BrickObj[i].ReturnDimLeftBrick()), (BrickObj[i].ReturnDimBottomBrick()-BrickObj[i].ReturnDimTopBrick()));
				}
			}
		}

		if (Loop>=640) 
		{
			AutorisationRedimBarre=TRUE;
			Loop=640-BarreObj.ReturnDimRightBarre();
			NewLevelFlag=TRUE;
			BarreObj.MoveBarre((ResolutionX/2)-((BarreObj.ReturnDimRightBarre()-BarreObj.ReturnDimLeftBarre())/2));
		}
		return;
	}

	if (KeyRight) 
	{
		BarreObj.AddBarre();
		EffectObj.ActiveAdd();
	}
	if (KeyLeft) 
	{
		BarreObj.SubBarre();
		EffectObj.ActiveSub();
	}

	if (KeyUp && BarreObj.Fly_OK()) 
	{
		EffectObj.ActiveUp();
		BarreObj.SubFlyingBarre();
	}
	else
	{
		EffectObj.DesactiveUp();
		BarreObj.AddFlyingBarre();
	}

	BalleObj.NewPosBarre(BarreObj.ReturnXBarre(), BarreObj.ReturnYBarre(), BarreObj.ReturnDimRightBarre()-BarreObj.ReturnDimLeftBarre());
	BalleObj.MoveBalle();
	MissileObj.MoveMissile(BarreObj);
	if (!BalleObj.BalleInScene()) 
	{
		BarreObj.ResetBonusAndMalus();
		BalleObj.ResetBonusAndMalus();
		EffectObj.ResetAll();
		MissileObj.DesactiveMissile(BarreObj);
	}

	if (BarreObj.Algorythm((float)BalleObj.ReturnXBalle(), (float)BalleObj.ReturnYBalle(), BalleObj.ReturnRayon(), BalleObj.ReturnOffsetX(), BalleObj.ReturnOffsetY()) && BalleObj.OK())
	{
		BalleObj.LOCK();
		BalleObj.NewOffsetX(BarreObj.ReturnOffsetX());
		BalleObj.NewOffsetY(BarreObj.ReturnOffsetY());
		if (BalleObj.PasteFlagStatement()) BalleObj.DesactiveSendFlag();
	}

	if (BalleObj.OK() && BalleObj.ReturnYBalle()>=BarreObj.ReturnYBarre()-50)
	{
		BarreObj.ResetStop();
		EffectObj.DesactiveStop();
	}

	for (int i=0; i<Nbr_Brick; i++)
	{
		if (BrickObj[i].State() && BrickObj[i].Algorythm((float)BalleObj.ReturnXBalle(), (float)BalleObj.ReturnYBalle(), BalleObj.ReturnRayon(), BalleObj.ReturnOffsetX(), BalleObj.ReturnOffsetY()))
		{
			BalleObj.NewOffsetX(BrickObj[i].ReturnNewOffsetX());
			BalleObj.NewOffsetY(BrickObj[i].ReturnNewOffsetY());
			BalleObj.UNLOCK();
			LevelObj[Lvl].DeleteValueBrick(i);
			if (LevelObj[Lvl].ReturnName(i)==R || LevelObj[Lvl].ReturnName(i)==O || LevelObj[Lvl].ReturnName(i)==G || LevelObj[Lvl].ReturnName(i)==B || LevelObj[Lvl].ReturnName(i)==V)
			{
				BonusObj[i].Start(BrickObj[i].ReturnXBrick(),BrickObj[i].ReturnYBrick(), (BrickObj[i].ReturnDimRightBrick()-BrickObj[i].ReturnDimLeftBrick()), (BrickObj[i].ReturnDimBottomBrick()-BrickObj[i].ReturnDimTopBrick()));
			}
			break;
		}

		if (BrickObj[i].State() && BrickObj[i].Missile1Algorythm(MissileObj))
		{
			MissileObj.StopMissile1();
			LevelObj[Lvl].DeleteValueBrick(i);
			if (LevelObj[Lvl].ReturnName(i)==R || LevelObj[Lvl].ReturnName(i)==O || LevelObj[Lvl].ReturnName(i)==G || LevelObj[Lvl].ReturnName(i)==B || LevelObj[Lvl].ReturnName(i)==V)
			{
				BonusObj[i].Start(BrickObj[i].ReturnXBrick(),BrickObj[i].ReturnYBrick(), (BrickObj[i].ReturnDimRightBrick()-BrickObj[i].ReturnDimLeftBrick()), (BrickObj[i].ReturnDimBottomBrick()-BrickObj[i].ReturnDimTopBrick()));
			}
		}

		if (BrickObj[i].State() && BrickObj[i].Missile2Algorythm(MissileObj))
		{
			MissileObj.StopMissile2();
			LevelObj[Lvl].DeleteValueBrick(i);
			if (LevelObj[Lvl].ReturnName(i)==R || LevelObj[Lvl].ReturnName(i)==O || LevelObj[Lvl].ReturnName(i)==G || LevelObj[Lvl].ReturnName(i)==B || LevelObj[Lvl].ReturnName(i)==V)
			{
				BonusObj[i].Start(BrickObj[i].ReturnXBrick(),BrickObj[i].ReturnYBrick(), (BrickObj[i].ReturnDimRightBrick()-BrickObj[i].ReturnDimLeftBrick()), (BrickObj[i].ReturnDimBottomBrick()-BrickObj[i].ReturnDimTopBrick()));
			}
		}

		BonusObj[i].MoveBonus();
		
		if (BonusObj[i].Algorythm(
								  (float)BarreObj.ReturnXBarre(),
								  (float)BarreObj.ReturnYBarre(),
								  (float)BarreObj.ReturnDimRightBarre()-(float)BarreObj.ReturnDimLeftBarre()
								  ))
		{
			if (BonusObj[i].Type()=='G')
			{
				if (BonusObj[i].ReturnBonusFlag()) 
				{
					BalleObj.Paste();
					BarreObj.ResetStop();
					EffectObj.ActivePaste();
				}
				if (BonusObj[i].ReturnMalusFlag())
				{
					BalleObj.ResetPaste();
					BarreObj.Stop();
					EffectObj.ActiveStop();
				}
			}
			if (BonusObj[i].Type()=='O')
			{
				if (BonusObj[i].ReturnBonusFlag())
				{
					BarreObj.Fly();
					EffectObj.ActiveFly();
				}
				if (BonusObj[i].ReturnMalusFlag())
				{
					BarreObj.Slow();
					EffectObj.ActiveSlow();
				}
			}
			if (BonusObj[i].Type()=='R')
			{
				if (BonusObj[i].ReturnBonusFlag())
				{
					MissileObj.ActiveMissile();
					EffectObj.ActiveCannon();
				}
				if (BonusObj[i].ReturnMalusFlag())
				{
					MissileObj.DesactiveMissile(BarreObj);
					EffectObj.DesactiveCannon();
				}
			}
			if (BonusObj[i].Type()=='B')
			{
				if (BonusObj[i].ReturnBonusFlag())
				{
					BarreObj.ActiveLongBarre();
				}
				if (BonusObj[i].ReturnMalusFlag())
				{
					BarreObj.ActiveShortBarre();
				}
			}
			if (BonusObj[i].Type()=='V')
			{
				if (BonusObj[i].ReturnBonusFlag())
				{
					EffectObj.ActiveLevelUp();
					BarreObj.ActiveLevelUpFlag();
				}
				if (BonusObj[i].ReturnMalusFlag())
				{
					EffectObj.DesactiveLevelUp();
					BarreObj.DesactiveLevelUpFlag();
					ConstructorFlag=TRUE;
				}
			}
		}
	}
}

// **************************************************
// *												*
// *		       Boucle principale.               *
// *												*
// **************************************************


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;                 // message from queue
	LONGLONG cur_time;       // current time
	BOOL notDone=TRUE;       // flag for thread completion
	DWORD  time_count=16;    // ms per frame, default if no performance counter
	LONGLONG perf_cnt;       // performance timer frequency
	BOOL perf_flag=FALSE;    // flag determining which timer to use
	LONGLONG next_time=0;    // time to render next frame
	LONGLONG last_time=0;	 // time of previous frame
	double time_elapsed;	 // time since previous frame
	double time_scale;		 // scaling factor for time

	// initialize the application, exit on failure

    if (!Init(hInstance, nCmdShow)) {
		Cleanup();
        return FALSE;
	}

	// is there a performance counter available? 

	if (QueryPerformanceFrequency((LARGE_INTEGER *) &perf_cnt)) { 

		// yes, set time_count and timer choice flag 

		perf_flag=TRUE;
		time_count=(unsigned long)perf_cnt/60;
		QueryPerformanceCounter((LARGE_INTEGER *) &next_time); 
		time_scale=1.0/perf_cnt;

	} else { 

		// no performance counter, read in using timeGetTime 

		next_time=timeGetTime(); 
		time_scale=0.001;
	} 

	// save time of last frame

	last_time=next_time;

	// run till completed 

	while (notDone) { 

		// is there a message to process? 

		if (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) { 

			// yes, is it a quit message? 

			if (msg.message==WM_QUIT) 

				notDone=FALSE; 

			// dispatch the message 

			TranslateMessage(&msg);
			DispatchMessage(&msg); 

		} else {

			// use the appropriate method to get time 
			// and calculate elapsed time since last frame

			if (perf_flag) 
				QueryPerformanceCounter((LARGE_INTEGER *) &cur_time); 
			else 
				cur_time=timeGetTime(); 

			// is it time to render the frame? 

			if (cur_time>next_time) { 

				// calculate elapsed time

				time_elapsed=(cur_time-last_time)*time_scale;

				// save frame time

				last_time=cur_time;
				
				// move the screen position
				if (!InitLevel()) 
				{
					Cleanup();
					return FALSE;
				}

				GameMessages();
				Game();

				// yes, render the frame 

				Draw_Surface();

				// set time for next frame 

				next_time = cur_time + time_count; 
			}
		}
	}

	// exit returning final message

    return (msg.wParam);
}