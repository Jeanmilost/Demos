// * Objet de gestion des déplacements de la barre. *

#include <math.h>

class Barre
{
	protected:

	int LeftBarre, TopBarre, RightBarre, BottomBarre;
	int XBarre, YBarre, ConstYBarre;
	int OffsetXBarre;
	int DeltaXBarre, DeltaYBarre;
	int LongBarreMax, ShortBarreMax;
	int RightBarreNormal, RightBarreMax, RightBarreMin;
	BOOL Fly_Flag;
	BOOL ActiveLevelFlag;
	BOOL LevelUpFlag;

	BOOL LongBarFlag;
	BOOL ShortBarFlag;

	BOOL SlowFlag;
	BOOL StopFlag;

	BOOL ClassicBarreObj;

	public:

		Barre(int XBarStrt, int YBarStrt, int LeftBarStrt, int TopBarStrt, int RightBarStrt, int BottomBarStrt); 
		// Forme mathématique de la barre.
		virtual void Forms()=0;
		// Algorythme de détéction des collisions
		// avec la balle.
		virtual BOOL Algorythm(float XBalle, float YBalle, float RayonBalle, float OffsetXBalle, float OffsetYBalle)=0;
		void ActiveLongBarre();
		void ActiveShortBarre();
		void LongBarre();
		void ShortBarre();
		void NormalBarre();
		void XBarreEntrie(int NewX);
		void AddBarre();
		void SubBarre();
		void AddFlyingBarre();
		void SubFlyingBarre();
		void MoveBarre(int NewX);
		void Fly();
		void Slow();
		void Stop();
		void ResetStop();
		void ActiveLevelUpFlag();
		void DesactiveLevelUpFlag();
		void ResetBonusAndMalus();
		int ReturnXBarre();
		int ReturnYBarre();
		int ReturnDimLeftBarre();
		int ReturnDimRightBarre();
		int ReturnConstRightBarre();
		int ReturnDimTopBarre();
		int ReturnDimBottomBarre();
		BOOL Fly_OK();
		BOOL Level_OK();
		BOOL Barre_State();
		BOOL Slow_OK();
		BOOL ReturnClasicBarreObj();
};

Barre::Barre(int XBarStrt, int YBarStrt, int LeftBarStrt, int TopBarStrt, int RightBarStrt, int BottomBarStrt)
{
	XBarre=XBarStrt;
	YBarre=ConstYBarre=YBarStrt;

	LeftBarre=LeftBarStrt;
	TopBarre=TopBarStrt;
	RightBarre=RightBarreNormal=RightBarStrt;
	RightBarreMax=RightBarreNormal+((RightBarreNormal*60)/100);
	RightBarreMin=RightBarreNormal-((RightBarreNormal*29)/100);
	BottomBarre=BottomBarStrt;

	DeltaXBarre=RightBarre-LeftBarre;
	DeltaYBarre=BottomBarre-TopBarre;

	OffsetXBarre=10;
	Fly_Flag=FALSE;
	StopFlag=FALSE;
	ActiveLevelFlag=FALSE;
	LevelUpFlag=FALSE;
	LongBarFlag=FALSE;
	ShortBarFlag=FALSE;
	ClassicBarreObj=FALSE;
};

void Barre::ActiveLongBarre()
{
	LongBarFlag=TRUE;
	ShortBarFlag=FALSE;
}

void Barre::ActiveShortBarre()
{
	ShortBarFlag=TRUE;
	LongBarFlag=FALSE;
}

void Barre::LongBarre()
{
	if (LongBarFlag)
	{
		if (RightBarre<RightBarreMax)
		{
			RightBarre++;
			DeltaXBarre=RightBarre-LeftBarre;
		}
		else RightBarre=RightBarreMax;
	}
}

void Barre::ShortBarre()
{
	if (ShortBarFlag)
	{
		if (RightBarre>RightBarreMin)
		{
			RightBarre--;
			DeltaXBarre=RightBarre-LeftBarre;
		}
		else RightBarre=RightBarreMin;
	}
}

void Barre::NormalBarre()
{
	if (!LongBarFlag && !ShortBarFlag)
	{
		if (RightBarre!=RightBarreNormal)
		{
			if (RightBarre<RightBarreNormal)
			{
				RightBarre++;
				DeltaXBarre=RightBarre-LeftBarre;
			}
			if (RightBarre>RightBarreNormal)
			{
				RightBarre--;
				DeltaXBarre=RightBarre-LeftBarre;
			}
		}
	}
}

// Gestion de l'entrée des informations souris
// relatives aux mouvements sur l'axe X.
void Barre::XBarreEntrie(int NewX)
{
	if (!StopFlag)
	{
		XBarre=NewX;
		Forms();
		if (XBarre>=640-RightBarre && ActiveLevelFlag && YBarre==ConstYBarre)
		{
			LevelUpFlag=TRUE;
			XBarre=640-RightBarre;		
			Forms();
		}
	}
}

// Algorythme de déplacement de la barre d'un pas à
// droite.
void Barre::AddBarre()
{
	if (!StopFlag)
	{
		if (XBarre<640-RightBarre)
		{
			XBarre+=OffsetXBarre;
			Forms();
		}
		else
		{
			if (ActiveLevelFlag && YBarre==ConstYBarre)
			{
				LevelUpFlag=TRUE;
			}

			XBarre=640-RightBarre;		
			Forms();
		}
	}
}

// Algorythme de déplacement de la barre d'un pas à
// gauche.
void Barre::SubBarre()
{
	if (!StopFlag)
	{
		if (XBarre>0)
		{
			XBarre-=OffsetXBarre;
			Forms();
		}
		else
		{
			XBarre=0;
			Forms();
		}
	}
}

// Algorythme de déplacement de la barre d'un pas en
// bas.
void Barre::AddFlyingBarre()
{
	if (!StopFlag)
	{
		if (YBarre<ConstYBarre)
		{
			YBarre+=2;
			Forms();
		}
		else
		{
			YBarre=ConstYBarre;
			Forms();
		}
	}
}

// Algorythme de déplacement de la barre d'un pas en
// haut.
void Barre::SubFlyingBarre()
{
	if (!StopFlag)
	{
		if (YBarre>30)
		{
			YBarre-=4;
			Forms();
		}
		else
		{
			YBarre=30;
			Forms();
		}
	}
}

void Barre::MoveBarre(int NewX)
{
	XBarre=NewX;
}

// Active le bonus barre volante.
void Barre::Fly()
{
	SlowFlag=FALSE;
	Fly_Flag=TRUE;
	OffsetXBarre=10;
}

// Active le malus barre ralentie.
void Barre::Slow()
{
	SlowFlag=TRUE;
	Fly_Flag=FALSE;
	OffsetXBarre=5;
}

void Barre::Stop()
{
	StopFlag=TRUE;
}

void Barre::ResetStop()
{
	StopFlag=FALSE;
}

void Barre::ActiveLevelUpFlag()
{
	ActiveLevelFlag=TRUE;
}

void Barre::DesactiveLevelUpFlag()
{
	ActiveLevelFlag=FALSE;
}

// Remets à leurs états initiaux les bonus et
// les malus.
void Barre::ResetBonusAndMalus()
{
	SlowFlag=FALSE;
	ShortBarFlag=FALSE;
	LongBarFlag=FALSE;
	DeltaXBarre=RightBarre-LeftBarre;
	Fly_Flag=FALSE;
	StopFlag=FALSE;
	ActiveLevelFlag=FALSE;
	LevelUpFlag=FALSE;
	OffsetXBarre=10;
}

// Désactive le malus barre ralentie.

// Retourne l'emplacement de la barre.
int Barre::ReturnXBarre()
{
	return (XBarre);
}

int Barre::ReturnYBarre()
{
	return (YBarre);
}

// Retourne les dimensions de la barre.
int Barre::ReturnDimLeftBarre()
{
	return (LeftBarre);
}

// Retourne la dimension réelle de la barre.
int Barre::ReturnDimRightBarre()
{
	return (RightBarre);
}

// Retourne la constante de RightBarre,
// pour l'affichage des sprites.
int Barre::ReturnConstRightBarre()
{
	return (RightBarreNormal);
}

int Barre::ReturnDimTopBarre()
{
	return (TopBarre);
}

int Barre::ReturnDimBottomBarre()
{
	return (BottomBarre);
}

// Retourne le drapeau d'état du bonus barre volante:
// TRUE=Actif;
// False=Inactif.
BOOL Barre::Fly_OK()
{
	return (Fly_Flag);
}

BOOL Barre::Level_OK()
{
	return (LevelUpFlag);
}

BOOL Barre::Barre_State()
{
	if (RightBarre>RightBarreNormal) return TRUE;
	return FALSE;
}

BOOL Barre::Slow_OK()
{
	return (SlowFlag);
}

BOOL Barre::ReturnClasicBarreObj()
{
	return (ClassicBarreObj);
}

// *******************************************
// * Classes des différents types de barres. *
// *******************************************

class ClassicBarre : public Barre
{
	int RayonSphere1, RayonSphere2;
	int R1XCenter, R1YCenter, R2XCenter, R2YCenter;
	int RectangleXStart, RectangleXStop;
	int	RectangleYStart, RectangleYStop;
	int Referency_Value;
	float OffsetXBalle, OffsetYBalle;
	BOOL Detect_OK;

	public:

	ClassicBarre(int XBarStrt=320, int YbarStrt=440, int LeftBarStrt=0, int TopBarStrt=0, int RightBarStrt=100, int BottomBarStrt=21);
	void Forms();
	void MathematicSphere(float X, float Y, float Rayon);
	void MathematicComposedRectangle(float BalleXCenter, float BalleYCenter, float Rayon);
	BOOL Algorythm(float XBalle, float YBalle, float RayonBalle, float OffsetXBalle, float OffsetYBalle);
	float ReturnOffsetX();
	float ReturnOffsetY();
};

ClassicBarre::ClassicBarre(int XBarStrt, int YBarStrt, int LeftBarStrt, int TopBarStrt, int RightBarStrt, int BottomBarStrt):Barre(XBarStrt, YBarStrt, LeftBarStrt, TopBarStrt, RightBarStrt, BottomBarStrt)
{
	Referency_Value=5;
	
	RayonSphere1=RayonSphere2=DeltaYBarre/2;
	Forms();
	Detect_OK=FALSE;
	ClassicBarreObj=TRUE;
}

// Crée la forme mathématique pour l'algorythme de
// collision.
void ClassicBarre::Forms()
{
	R1XCenter=XBarre+RayonSphere1;
	R2XCenter=XBarre+(DeltaXBarre-RayonSphere2);
	R1YCenter=R2YCenter=YBarre+RayonSphere1;
	RectangleXStart=(XBarre+DeltaYBarre)-4;
	RectangleXStop=((XBarre+DeltaXBarre)-DeltaYBarre)+4;
	RectangleYStart=YBarre+2;
	RectangleYStop=(YBarre+DeltaYBarre)-2;
}

// Si Collision sur partie sphérique de la barre...
void ClassicBarre::MathematicSphere(float X, float Y, float Rayon)
{
	float XSave=X;
	X=X<0 ? -X : X;
	float YSave=Y;
	Y=Y<0 ? -Y : Y;

	if ((X*X)+(Y*Y)<=((RayonSphere1+Rayon)*(RayonSphere1+Rayon)))
	{
		OffsetXBalle=OffsetYBalle=(float)Referency_Value;
		float Alpha=(360.0f*atanf(Y/X))/6.2831853f;
		Alpha<45 ? OffsetYBalle=(OffsetXBalle*Y)/X : OffsetXBalle=(OffsetYBalle*X)/Y;
			
		OffsetXBalle=XSave>0 ? -OffsetXBalle : OffsetXBalle;

		OffsetYBalle=YSave>0 ? -OffsetYBalle : OffsetYBalle;
		Detect_OK=TRUE;
	}
}

// Si collision sur partie plate de la barre...
void ClassicBarre::MathematicComposedRectangle(float BalleXCenter, float BalleYCenter, float Rayon)
{
	if (BalleYCenter+Rayon>=RectangleYStart && BalleYCenter-Rayon<=RectangleYStop && BalleXCenter-Rayon>=RectangleXStart && BalleXCenter+Rayon<=RectangleXStop)
	{
		OffsetYBalle=-OffsetYBalle;
		Detect_OK=TRUE;
	}	
}

// Algorythme de détéction des collisions avec
// la balle.
BOOL ClassicBarre::Algorythm(float XBalle, float YBalle, float RayonBalle, float OffsetX, float OffsetY)
{
	Detect_OK=FALSE;
	OffsetXBalle=OffsetX;
	OffsetYBalle=OffsetY;
	float BalleXCenter=XBalle+RayonBalle;
	float BalleYCenter=YBalle+RayonBalle;
	float CurrentX=R1XCenter-BalleXCenter;
	float CurrentY=R1YCenter-BalleYCenter;
	MathematicSphere(CurrentX, CurrentY, RayonBalle);

	CurrentX=R2XCenter-BalleXCenter;
	CurrentY=R2YCenter-BalleYCenter;
	MathematicSphere(CurrentX, CurrentY, RayonBalle);

	MathematicComposedRectangle(BalleXCenter, BalleYCenter, RayonBalle);

	return (Detect_OK);
}

// En cas de collision sur les parties sphériques,
// retourne les nouvelles valeurs de déplacement de
// la balle.
float ClassicBarre::ReturnOffsetX()
{
	return (OffsetXBalle);
}

float ClassicBarre::ReturnOffsetY()
{
	return (OffsetYBalle);
}