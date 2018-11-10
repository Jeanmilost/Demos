// *** Petite demo de sprites avec DirectDraw.

#include "stdafx.h"
#define INITGUID
#include <stdio.h>
#include <ddraw.h>
#include <mmsystem.h>

//-- Définit la résolution d'écran.

const int ResolutionX=640;
const int ResolutionY=480;

//-- Information de classes Windows.

static char szClass[] = "XmplCalcClass";
static char szCaption[] = "Calculette";

//-- Définit les pointeurs DirectDraw globaux.

LPDIRECTDRAW4 lpDD=NULL;
LPDIRECTDRAWSURFACE4 lpDDSPrimary=NULL;
LPDIRECTDRAWSURFACE4 lpDDSBack=NULL;

//-- Définit : - le nombre d'images.
//			   - le nombre de surfaces secondaires
//				 (BufferSurfaces.)

#define Buffer_Nbr			2

// -- Tableau à 2 dimensions définissant quel fichier
//	  à charger selon la surface secondaire.

char FileNames[Buffer_Nbr][256] = { "Layer3.bmp", 
									"Background.bmp",
								
};

// -- Tableau de références pointeurs vers les
//	  surfaces secondaires.

LPDIRECTDRAWSURFACE4 lp_BufferSurfaceCaller[Buffer_Nbr];

//-- Chaines des indicateurs d'erreurs.

const char *ErrStr=NULL;	

//-- Messages d'erreurs.

const char Err_Reg_Class[]			= "Error Registering Window Class";
const char Err_Create_Win[]			= "Error Creating Window";
const char Err_DirectDrawCreate[]	= "DirectDrawCreate FAILED";
const char Err_Query[]				= "QueryInterface FAILED";
const char Err_Coop[]				= "SetCooperativeLevel FAILED";
const char Err_CreateSurf[]			= "CreateSurface FAILED";
const char Err_LoadBMP[]			= "Error Loading Image";
const char Err_DispMode[]			= "Error Setting Display Mode";
const char Err_LoadImage[]			= "Error Loading Image";

int XCountFrame=0;
int YCountFrame=0;

// **************************************************
// *												*
// *  Crée les BufferSurface et renvoie un pointeur	*
// *  vers lesdites surfaces.						*
// *												*
// **************************************************

LPDIRECTDRAWSURFACE4 bitmap_surface(LPCTSTR file_name,RECT *dims=NULL)
{
	HDC hdc;
	HBITMAP bit;
	LPDIRECTDRAWSURFACE4 surf;

	// Charge l'image.

	bit=(HBITMAP) LoadImage(NULL,file_name,IMAGE_BITMAP,0,0,
								LR_DEFAULTSIZE|LR_LOADFROMFILE);
	if (!bit) 
	return NULL;

	// Definit les dimensions de l'image.

	BITMAP bitmap;
    GetObject( bit, sizeof(BITMAP), &bitmap );
	int surf_width=bitmap.bmWidth;
	int surf_height=bitmap.bmHeight;

	// Crée la BufferSurface.

	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT ;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
	ddsd.dwWidth = surf_width;
	ddsd.dwHeight = surf_height; 

	ddrval=lpDD->CreateSurface(&ddsd,&surf,NULL);
	if (ddrval!=DD_OK) 
	{
		DeleteObject(bit);
		return NULL;
	} 
	else 
	{
		surf->GetDC(&hdc);

		// Crée un DC compatible.

		HDC bit_dc=CreateCompatibleDC(hdc);

		// Blitte l'interface dans la surface.

		SelectObject(bit_dc,bit);
		BitBlt(hdc,0,0,surf_width,surf_height,bit_dc,0,0,SRCCOPY);

		// Libère le DC.

		surf->ReleaseDC(hdc);
		DeleteDC(bit_dc);

		// Sauvegarde les dimensions du rectangle.

		if (dims) 
		{
			dims->left=0;
			dims->top=0;
			dims->right=surf_width;
			dims->bottom=surf_height;
		}
	}

	// Libère le Bitmap.

	DeleteObject(bit);

	return surf;
}

// **************************************************
// *												*
// * Nettoie la mémoire - Detruit les objets DDRAW	*
// * et Windows.									*
// *												*
// **************************************************


#define SafeRelease(x) if (x) { x->Release(); x=NULL;}

void Cleanup()
{
	// Libère les BufferSurfaces.

	for (int i=0;i<Buffer_Nbr;i++) 
		SafeRelease(lp_BufferSurfaceCaller[i]);

	// Libère l'interface DDRAW.

	SafeRelease(lpDDSPrimary);
	SafeRelease(lpDD);

	// Transmets les erreurs.

	if (ErrStr) 
	{
		MessageBox(NULL, ErrStr, szCaption, MB_OK);
		ErrStr=NULL;
	}
}

// **************************************************
// *												*
// *  Fonction servant à définir la clé de couleur	*
// *  pour la copie du sprite (se trouve dans la	*
// *  première surface du tableau.)					*
// *												*
// **************************************************

void DefineColorKey(LPDIRECTDRAWSURFACE4 KeySurface)
{
	DDCOLORKEY key;
	key.dwColorSpaceHighValue=0;   // Clé RGB a zero:
	key.dwColorSpaceLowValue=0;	   // NOIR.
	KeySurface->SetColorKey(DDCKEY_SRCBLT, &key);
}

// *************************************************
// *											   *
// * Dessine un sprite à l'écran, en tenant compte *
// * du clipping.								   *
// *											   *
// *************************************************

void Sprite(int X, int Y,
			 int Left, int Right, int Top, int Bottom)
{

	int ClippingX=0, ClippingY=0;

	RECT				SpRect;
	SpRect.left=Left;
	SpRect.top=Top;
	SpRect.right=Right;
	SpRect.bottom=Bottom;

	// Vérifie et exécute si besoin est le clipping
	// du bord gauche.
	if (X<=0)
	{
		ClippingX=0-X;
		SpRect.left=Left+ClippingX;
		X=0;
	}

	// Vérifie et exécute si besoin est le clipping
	// du bord droite.
	if (X+(Right-Left)>=ResolutionX)
	{
		ClippingX=(X+(Right-Left))-ResolutionX;
		SpRect.right=Right-ClippingX;
	}

	// Vérifie et exécute si besoin est le clipping
	// du haut de l'écran.
	if (Y<=0)
	{
		ClippingY=0-Y;
		SpRect.top=Top+ClippingY;
		Y=0;
	}

	// Vérifie et exécute si besoin est le clipping
	// du bas de l'écran.
	if (Y+(Bottom-Top)>=ResolutionY)
	{
		ClippingY=(Y+(Bottom-Top))-ResolutionY;
		SpRect.bottom=Bottom-ClippingY;
	}

	if (&SpRect)
	{
		lpDDSBack->BltFast(X, Y, lp_BufferSurfaceCaller[0], &SpRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
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
	// Copie le BufferSurface représentant le fond
	// dans le BackBuffer.

	lpDDSBack->BltFast(0,0,lp_BufferSurfaceCaller[1],NULL,DDBLTFAST_WAIT);

	// Dessine les sprites.

	Sprite(150,
		   218,
		   94*XCountFrame,
		   94*(XCountFrame+1),
		   94*YCountFrame,
		   94*(YCountFrame+1));
	
	XCountFrame++;
	if (XCountFrame==6)
	{
		XCountFrame=0;
		YCountFrame++;
	}

	if (YCountFrame==5)
	{
		YCountFrame=0;
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
				case VK_SHIFT:
					break;

                case VK_LEFT: 
					break; 
 
                case VK_RIGHT: 
					break;

				case VK_UP:
					break;

				case VK_SPACE:
					break;

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
    HRESULT                     ddrval;
    LPDIRECTDRAW                pDD;

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

	// Libère les pointeurs d'image.

	ZeroMemory(lp_BufferSurfaceCaller,sizeof(lp_BufferSurfaceCaller));

    // Crée l'objet DDRAW principal.

    ddrval = DirectDrawCreate(NULL, &pDD, NULL);
    if (ddrval != DD_OK) 
	{
		ErrStr=Err_DirectDrawCreate;
		return FALSE;
	}

    // Groupe l'interface DDRAW4 au projet.

    ddrval = pDD->QueryInterface(IID_IDirectDraw4, (LPVOID *) & lpDD);
    if (ddrval != DD_OK) 
	{
        ErrStr=Err_Query;
		return FALSE;
	}

    // Définit le niveau coopératif.

    ddrval = lpDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
    if (ddrval != DD_OK) 
	{
		ErrStr=Err_Coop;
		return FALSE;
	}

	// Définit le mode vidéo.

	ddrval = lpDD->SetDisplayMode( 640, 480, 16, 0, 0);
	if (ddrval !=DD_OK) 
	{
		ErrStr=Err_DispMode;
		return FALSE;
	}


    // Crée la surface primaire avec 1 BackBuffer.

    DDSURFACEDESC2 ddsd;
	DDSCAPS2 ddscaps;
	ZeroMemory(&ddsd,sizeof(ddsd));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                          DDSCAPS_FLIP | 
                          DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
	if (ddrval!=DD_OK) 
	{
		ErrStr=Err_CreateSurf;
		return FALSE;
	}

	// Attache le BackBuffer à la surface primaire.

	ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
	ddrval=lpDDSPrimary->GetAttachedSurface(&ddscaps,&lpDDSBack);
	if (ddrval!=DD_OK) 
	{
		ErrStr=Err_CreateSurf;
		return FALSE;
	}

	// Charge l'image des modèles de sprites.

	lp_BufferSurfaceCaller[0]=bitmap_surface(FileNames[0]);
	if (!lp_BufferSurfaceCaller[0])
		return FALSE;

	DefineColorKey(lp_BufferSurfaceCaller[0]);


	// Charger l'image de fond.

	lp_BufferSurfaceCaller[1]=bitmap_surface(FileNames[1]);
	if (!lp_BufferSurfaceCaller[1])
		return FALSE;

	Draw_Surface();

	return TRUE;
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
		time_count=perf_cnt/60;
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
				
				// Emplacement pour le code.

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