#include <ddraw.h>
#include <dinput.h>

#define SafeRelease(x) if (x) { x->Release(); x=NULL;}

//-------------------DIRECTDRAW----------------------

//-- Définit la résolution d'écran.

const int ResolutionX=640;
const int ResolutionY=480;

//-- Définit les pointeurs DirectDraw globaux.

LPDIRECTDRAW4 lpDD=NULL;
LPDIRECTDRAWSURFACE4 lpDDSPrimary=NULL;
LPDIRECTDRAWSURFACE4 lpDDSBack=NULL;

//-- Définit : - le nombre d'images.
//			   - le nombre de surfaces secondaires
//				 (BufferSurfaces.)

#define Buffer_Nbr			50

// -- Tableau à 2 dimensions définissant quel fichier
//	  à charger selon la surface secondaire.

char FileNames[Buffer_Nbr][256] = {  "SprArka21.bmp",
									   "Scene1.bmp",
									   "Scene2.bmp",
									   "Scene3.bmp",
									   "Scene4.bmp",
									   "Scene1.bmp"
};

// -- Tableau de références pointeurs vers les
//	  surfaces secondaires.

LPDIRECTDRAWSURFACE4 lp_BufferSurfaceCaller[Buffer_Nbr];


//----------------DIRECTINPUT------------------------

//-- Définit les pointeurs DirectInputs globaux.

LPDIRECTINPUT			lpDI=NULL;

//-- Définit les pointeurs clavier.

LPDIRECTINPUTDEVICE		pKeyboard=NULL;
char KeyBuffer[256];

//-- Définit les pointeurs souris.

LPDIRECTINPUTDEVICE		pMouse=NULL;
HANDLE					Event=NULL;

//-- Définit les registres d'états de la souris.

int OldiDX=0, OldiDY=0, Delta=0;
int iDX=0, iDY=0;
BOOL ButtonDown[2];

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
// *		  Libère les objets DirectDraw. 	    *
// *												*
// **************************************************

void EraseDDraw()
{
	// Libère les BufferSurfaces.

	for (int i=0;i<Buffer_Nbr;i++) 
		SafeRelease(lp_BufferSurfaceCaller[i]);

	// Libère l'interface DDRAW.

	SafeRelease(lpDDSPrimary);
	SafeRelease(lpDD);
}

// *************************************************
// *											   *
// *		Dessine le papier peint de fond.	   *
// *											   *
// *************************************************

void BackGround(int Image_Number)
{
	// Copie le BufferSurface représentant le fond
	// dans le BackBuffer.

	lpDDSBack->BltFast(0,0,lp_BufferSurfaceCaller[Image_Number],NULL,DDBLTFAST_WAIT);
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

// **************************************************
// *												*
// *		  Initialisation de DirectDraw.		    *
// *												*
// **************************************************

BOOL InitDDraw(HWND hWndDD)
{
    LPDIRECTDRAW                pDD;

	// Libère les pointeurs d'image.

	ZeroMemory(lp_BufferSurfaceCaller,sizeof(lp_BufferSurfaceCaller));

    // Crée l'objet DDRAW principal.

    if (DirectDrawCreate(NULL, &pDD, NULL)!=DD_OK)
		return FALSE;

    // Groupe l'interface DDRAW4 au projet.

    if (pDD->QueryInterface(IID_IDirectDraw4, (LPVOID *) & lpDD)!=DD_OK)
		return FALSE;

    // Définit le niveau coopératif.

    if (lpDD->SetCooperativeLevel(hWndDD, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN )!=DD_OK)
		return FALSE;

	// Définit le mode vidéo.

	if (lpDD->SetDisplayMode( 640, 480, 16, 0, 0)!=DD_OK)
		return FALSE;

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
    if (lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL )!=DD_OK)
		return FALSE;

	// Attache le BackBuffer à la surface primaire.

	ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
	if (lpDDSPrimary->GetAttachedSurface(&ddscaps,&lpDDSBack)!=DD_OK)
		return FALSE;

	// Charge l'image des modèles de sprites.

	lp_BufferSurfaceCaller[0]=bitmap_surface(FileNames[0]);
	if (!lp_BufferSurfaceCaller[0])
		return FALSE;

	DefineColorKey(lp_BufferSurfaceCaller[0]);
	
	// Charger l'image de fond.

	lp_BufferSurfaceCaller[1]=bitmap_surface(FileNames[1]);
	if (!lp_BufferSurfaceCaller[1])
		return FALSE;

	return TRUE;
}

// **************************************************
// *												*
// *		  Initialisation de DirectInput.	    *
// *												*
// **************************************************

BOOL InitDInput(HINSTANCE hInstDI, HWND hWndDI)
{
	// Ouvre l'objet DirectInput.

	if (DirectInput8Create(hInstDI, DIRECTINPUT_VERSION,
						   IID_IDirectInput8A, (VOID**)&lpDI, NULL)!=DI_OK)
		return FALSE;

	// Crée l'interface clavier.

	if (lpDI->CreateDevice(GUID_SysKeyboard, &pKeyboard, NULL)!=DI_OK)
		return FALSE;

	if (pKeyboard->SetDataFormat(&c_dfDIKeyboard)!=DI_OK)
		return FALSE;

	if (pKeyboard->SetCooperativeLevel(hWndDI, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)!=DI_OK)
		return FALSE;

	if (pKeyboard->Acquire()!=DI_OK)
		return FALSE;

	// Crée l'interface souris.

	if (lpDI->CreateDevice(GUID_SysMouse, &pMouse, NULL)!=DI_OK)
		return FALSE;

	if (pMouse->SetDataFormat(&c_dfDIMouse)!=DI_OK)
	{
		SafeRelease(pMouse);
		return FALSE;
	}

	if (pMouse->SetCooperativeLevel(hWndDI, DISCL_FOREGROUND | DISCL_EXCLUSIVE)!=DI_OK)
	{
		SafeRelease(pMouse);
		return FALSE;
	}

	Event=CreateEvent(0, 0, 0, 0);
	if ( NULL == Event )
		return FALSE;

	if (pMouse->SetEventNotification(Event)!=DI_OK)
	{
		SafeRelease(pMouse);
		return FALSE;
	}

	DIPROPDWORD dipwd=
	{
		{
			sizeof(DIPROPDWORD),
			sizeof(DIPROPHEADER),
			0,
			DIPH_DEVICE,
		},
		32
	};

	if (pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipwd.diph)!=DI_OK)
	{
		SafeRelease(pMouse);
		return FALSE;
	}

	if (pMouse->Acquire()!=DI_OK)
		return FALSE;

	return TRUE;
}

// **************************************************
// *												*
// *        	Libère les objets DINPUTS.		    *
// *												*
// **************************************************

void EraseDInput()
{
	if (pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse=NULL;
	}

	SafeRelease(lpDI);
}

// **************************************************
// *												*
// *        	Lit les données de la souris	    *
// *												*
// **************************************************

BOOL ReadMouse()
{
	OldiDX=iDX;
	OldiDY=iDY;

    BOOL                bDone;
    DIDEVICEOBJECTDATA  od;
    DWORD               dwElements;
    HRESULT             hr;

	BOOL Resultat=FALSE;

    // Attempt to read one data element.  Continue as long as
    // device data is available.
    bDone = FALSE;

    while ( !bDone ) 
    {
        dwElements = 1;
        hr = pMouse->GetDeviceData( 
                            sizeof(DIDEVICEOBJECTDATA), 
                            &od,
                            &dwElements, 
                            0 );

        if (hr == DIERR_INPUTLOST) 
        {
            pMouse->Acquire();
            break;
        }

        // Unable to read data or no data available
        if ( FAILED(hr) || dwElements == 0) 
        {
            break;
        }

        // look at the element to see what happened
        switch (od.dwOfs) 
        {     
            case DIMOFS_X:
				iDX+=od.dwData;
                Resultat=TRUE;
                break;

            case DIMOFS_Y:
				iDY+=od.dwData;
                Resultat=TRUE; 
                break;

        
            case DIMOFS_BUTTON0:
            
				if ( od.dwData & 0x80 ) 
				{  
					bDone = TRUE;
					ButtonDown[0]=TRUE;
					Resultat=TRUE;
				}
				else ButtonDown[0]=FALSE;
				break;

			case DIMOFS_BUTTON1:

				if ( od.dwData & 0x80 ) 
				{ 
					bDone = TRUE;
					ButtonDown[1]=TRUE;
					Resultat=TRUE; 
				}
				else ButtonDown[1]=FALSE;
               break;
        }
    }
	return Resultat;
}

BOOL Button0()
{
	return ButtonDown[0];
}

BOOL Button1()
{
	return ButtonDown[1];
}

void InitAxes(int InitAxeX, int InitAxeY)
{
	iDX=InitAxeX;
	iDY=InitAxeY;
}

int AxeX(int MinX, int MaxX)
{
	if (iDX>MaxX) iDX=MaxX;
	if (iDX<MinX) iDX=MinX;
	return iDX;
}

int AxeY(int MinY, int MaxY)
{

	if (iDY>MaxY) iDY=MaxY;
	if (iDY<MinY) iDY=MinY;
	return iDY;
}

// **************************************************
// *												*
// *        	Lit les données du clavier.		    *
// *												*
// **************************************************

void ReadKeyboard()
{
	if (pKeyboard->GetDeviceState(sizeof(KeyBuffer),(LPVOID)&KeyBuffer)
	== DIERR_INPUTLOST)
	{
		// Reacquiert le clavier et recommence.
		if (pKeyboard->Acquire() == DI_OK)
		pKeyboard->GetDeviceState(sizeof(KeyBuffer),(LPVOID)&KeyBuffer);
	}
}

// **************************************************
// *												*
// *          Tests de fonctions du clavier.	    *
// *												*
// **************************************************

#define KEYDOWN(buf, key) (buf[key] & 0x80)

BOOL LSHIFT()
{
	ReadKeyboard();

	if (KEYDOWN(KeyBuffer, DIK_LSHIFT))
		return TRUE;

	return FALSE;
}

BOOL RSHIFT()
{
	ReadKeyboard();

	if (KEYDOWN(KeyBuffer, DIK_RSHIFT))
		return TRUE;

	return FALSE;
}

BOOL LEFT()
{
	ReadKeyboard();

	if (KEYDOWN(KeyBuffer, DIK_LEFT))
		return TRUE;

	return FALSE;
}

BOOL RIGHT()
{
	ReadKeyboard();

	if (KEYDOWN(KeyBuffer, DIK_RIGHT))
		return TRUE;

	return FALSE;
}

BOOL UP()
{
	ReadKeyboard();

	if (KEYDOWN(KeyBuffer, DIK_UP))
		return TRUE;

	return FALSE;
}

BOOL SPACE()
{
	ReadKeyboard();

	if (KEYDOWN(KeyBuffer, DIK_SPACE))
		return TRUE;

	return FALSE;
}