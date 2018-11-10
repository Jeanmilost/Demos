// Classe de gestion des briques : 1 Objet par brique.

#include <math.h>

class Brick
{
	int XBrick, YBrick;
	int LeftBrick, RightBrick, TopBrick, BottomBrick;
	int DeltaXBrick, DeltaYBrick;
	float OffsetXBalle, OffsetYBalle;
	BOOL Detect_OK, StateBrick;

	protected:
	BOOL Indestructible;

	public:

	Brick() {};
	Brick(BOOL NoBrick) {StateBrick=FALSE;}
	Brick(int XBrStrt, int YBrStrt, int LeftBrStrt, int RightBrStrt, int TopBrStrt, int BottomBrStrt);
	BOOL Algorythm(float XBalle, float YBalle, float RayonBalle, float OffsetXBalle, float OffsetYBalle);
	BOOL Missile1Algorythm(Missiles MissileObj);
	BOOL Missile2Algorythm(Missiles MissileObj);
	BOOL State();
	int ReturnDimLeftBrick();
	int ReturnDimRightBrick();
	int ReturnDimTopBrick();
	int ReturnDimBottomBrick();
	int ReturnXBrick();
	int ReturnYBrick();
	float ReturnNewOffsetX();
	float ReturnNewOffsetY();
};

Brick::Brick(int XBrStrt, int YBrStrt, int LeftBrStrt, int RightBrStrt, int TopBrStrt, int BottomBrStrt)
{
	XBrick=XBrStrt;
	YBrick=YBrStrt;

	LeftBrick=LeftBrStrt;
	RightBrick=RightBrStrt;
	TopBrick=TopBrStrt;
	BottomBrick=BottomBrStrt;

	DeltaXBrick=RightBrick-LeftBrick;
	DeltaYBrick=BottomBrick-TopBrick;

	OffsetXBalle=0;
	OffsetYBalle=0;

	StateBrick=TRUE;
	Detect_OK=FALSE;
}

// Algorythme de détection des collisions.
BOOL Brick::Algorythm(float XBalle, float YBalle, float RayonBalle, float OffsetX, float OffsetY)
{
	Detect_OK=FALSE;
	if (StateBrick)
	{
		float CentreX=XBalle+RayonBalle;
		float CentreY=YBalle+RayonBalle;
		float HaloXEnd=XBrick+DeltaXBrick+RayonBalle;
		float HaloYEnd=YBrick+DeltaYBrick+RayonBalle;

		if (CentreY>=YBrick+(DeltaYBrick/2) && CentreY<=YBrick+DeltaYBrick+RayonBalle && CentreX>=XBrick && CentreX<=XBrick+DeltaXBrick)
		{
			OffsetY=OffsetY<0 ? -OffsetY : OffsetY;
			StateBrick=Indestructible;
			Detect_OK=TRUE;
		}

		if (CentreY>=YBrick-RayonBalle && CentreY<=YBrick+(DeltaYBrick/2) && CentreX>=XBrick && CentreX<=XBrick+DeltaXBrick)
		{
			OffsetY=OffsetY>0 ? -OffsetY : OffsetY;
			StateBrick=Indestructible;
			Detect_OK=TRUE;
		}

		if (CentreX>=XBrick+(DeltaXBrick/2) && CentreX<=XBrick+DeltaXBrick+RayonBalle && CentreY>=YBrick && CentreY<=YBrick+DeltaYBrick)
		{
			OffsetX=OffsetX<0 ? -OffsetX : OffsetX;
			StateBrick=Indestructible;
			Detect_OK=TRUE;
		}

		if (CentreX>=XBrick-RayonBalle && CentreX<=XBrick+(DeltaXBrick/2) && CentreY>=YBrick && CentreY<=YBrick+DeltaYBrick)
		{
			OffsetX=OffsetX>0 ? -OffsetX : OffsetX;
			StateBrick=Indestructible;
			Detect_OK=TRUE;
		}

		if (CentreX>XBrick+DeltaXBrick && CentreY>YBrick+DeltaYBrick)
		{
			float X=CentreX-(XBrick+DeltaXBrick);
			float Y=CentreY-(YBrick+DeltaYBrick);
			if ((X*X)+(Y*Y)<=(RayonBalle*RayonBalle))
			{
				if (OffsetX)
				{
					float OffsetXSave=OffsetX;
					OffsetX=-OffsetY;
					OffsetY=-OffsetXSave;
					StateBrick=Indestructible;
					Detect_OK=TRUE;
					if (Indestructible) OffsetY=OffsetY<0 ? -OffsetY : OffsetY;
				}
				else 
				{
					OffsetY=-OffsetY;
					StateBrick=Indestructible;
					Detect_OK=TRUE;
				}
			}
		}
	
		if (CentreX<XBrick && CentreY<YBrick)
		{
			float X=XBrick-CentreX;
			float Y=YBrick-CentreY;
			if ((X*X)+(Y*Y)<=(RayonBalle*RayonBalle))
			{
				if (OffsetX)
				{
					float OffsetXSave=OffsetX;
					OffsetX=-OffsetY;
					OffsetY=-OffsetXSave;
					StateBrick=Indestructible;
					if (Indestructible) OffsetY=OffsetY>0 ? -OffsetY : OffsetY;
					Detect_OK=TRUE;
				}
				else 
				{
					OffsetY=-OffsetY;
					StateBrick=Indestructible;
					Detect_OK=TRUE;
				}
			}
		}

		if (CentreX>XBrick+DeltaXBrick && CentreY<YBrick)
		{
			float X=CentreX-(XBrick+DeltaXBrick);
			float Y=YBrick-CentreY;
			if ((X*X)+(Y*Y)<=(RayonBalle*RayonBalle))
			{
				if (OffsetX)
				{
					float OffsetXSave=OffsetX;
					OffsetX=OffsetY;
					OffsetY=OffsetXSave;
					StateBrick=Indestructible;
					if (Indestructible) OffsetY=OffsetY>0 ? -OffsetY : OffsetY;
					Detect_OK=TRUE;
				}
				else 
				{
					OffsetY=-OffsetY;
					StateBrick=Indestructible;
					Detect_OK=TRUE;
				}
			}
		}

		if (CentreX<XBrick && CentreY>YBrick+DeltaYBrick)
		{
			float X=XBrick-CentreX;
			float Y=CentreY-(YBrick+DeltaYBrick);
			if ((X*X)+(Y*Y)<=(RayonBalle*RayonBalle))
			{
				if (OffsetX)
				{
					float OffsetXSave=OffsetX;
					OffsetX=OffsetY;
					OffsetY=OffsetXSave;
					StateBrick=Indestructible;
					if (Indestructible) OffsetY=OffsetY<0 ? -OffsetY : OffsetY;
					Detect_OK=TRUE;
				}
				else 
				{
					OffsetY=-OffsetY;
					StateBrick=Indestructible;
					Detect_OK=TRUE;
				}
			}
		}
		OffsetXBalle=OffsetX;
		OffsetYBalle=OffsetY;
	}
	return (Detect_OK);
}

BOOL Brick::Missile1Algorythm(Missiles MissileObj)
{
	if (MissileObj.ReturnMissileY1()<=YBrick+DeltaYBrick && MissileObj.ReturnMissileY1()>=YBrick)
	{
		if (MissileObj.ReturnMissileX1()>=XBrick && MissileObj.ReturnMissileX1()<=XBrick+DeltaXBrick)
		{
			StateBrick=Indestructible;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Brick::Missile2Algorythm(Missiles MissileObj)
{
	if (MissileObj.ReturnMissileY2()<=YBrick+DeltaYBrick && MissileObj.ReturnMissileY2()>=YBrick)
	{

		if (MissileObj.ReturnMissileX2()>=XBrick && MissileObj.ReturnMissileX2()<=XBrick+DeltaXBrick)
		{
			StateBrick=Indestructible;
			return TRUE;
		}
	}
	return FALSE;
}

// Renvoie l'état de la brique.(présente ou détruite)
BOOL Brick::State()
{
	return (StateBrick);
}

// Registres des dimensions et emplacement de 
// l'élement.
int Brick::ReturnDimLeftBrick()
{
	return (LeftBrick);
}

int Brick::ReturnDimRightBrick()
{
	return (RightBrick);
}

int Brick::ReturnDimTopBrick()
{
	return (TopBrick);
}

int Brick::ReturnDimBottomBrick()
{
	return (BottomBrick);
}

int Brick::ReturnXBrick()
{
	return (XBrick);
}

int Brick::ReturnYBrick()
{
	return (YBrick);
}

// Si angle touché, renvoie les nouvelles valeurs de
// déplacement en X et Y.
float Brick::ReturnNewOffsetX()
{
	return (OffsetXBalle);
}

float Brick::ReturnNewOffsetY()
{
	return (OffsetYBalle);
}

// ********************************************
// * Classes des différents types de briques. *
// ********************************************

class Blue : public Brick
{
	public:

	Blue(int XBrick, int YBrick, int Left=0, int Right=42, int Top=21, int Bottom=42);
};

Blue::Blue(int XBrick, int YBrick, int Left, int Right, int Top, int Bottom) : Brick(XBrick,YBrick,Left,Right,Top,Bottom)
{
	Indestructible=FALSE;
}

class Yellow : public Brick
{
	public:

	Yellow(int XBrick, int YBrick, int Left=43, int Right=86, int Top=21, int Bottom=42);
};

Yellow::Yellow(int XBrick, int YBrick, int Left, int Right, int Top, int Bottom) : Brick(XBrick,YBrick,Left,Right,Top,Bottom)
{
	Indestructible=TRUE;
}

class Violet : public Brick
{
	public:

	Violet(int XBrick, int YBrick, int Left=86, int Right=129, int Top=21, int Bottom=42);
};

Violet::Violet(int XBrick, int YBrick, int Left, int Right, int Top, int Bottom) : Brick(XBrick,YBrick,Left,Right,Top,Bottom)
{
	Indestructible=FALSE;
}

class Red : public Brick
{
	public:

	Red(int XBrick, int YBrick, int Left=0, int Right=43, int Top=42, int Bottom=63);
};

Red::Red(int XBrick, int YBrick, int Left, int Right, int Top, int Bottom) : Brick(XBrick,YBrick,Left,Right,Top,Bottom)
{
	Indestructible=FALSE;
}

class Green : public Brick
{
	public:

	Green(int XBrick, int YBrick, int Left=43, int Right=86, int Top=42, int Bottom=63);
};

Green::Green(int XBrick, int YBrick, int Left, int Right, int Top, int Bottom) : Brick(XBrick,YBrick,Left,Right,Top,Bottom)
{
	Indestructible=FALSE;
}

class Orange : public Brick
{
	public:

	Orange(int XBrick, int YBrick, int Left=86, int Right=129, int Top=42, int Bottom=63);
};

Orange::Orange(int XBrick, int YBrick, int Left, int Right, int Top, int Bottom) : Brick(XBrick,YBrick,Left,Right,Top,Bottom)
{
	Indestructible=FALSE;
}