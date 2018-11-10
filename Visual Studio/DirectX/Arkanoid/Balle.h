// * Objet de gestion des déplacements de la balle *

class Balle
{
	int LeftBalle, TopBalle, RightBalle, BottomBalle;
	int DeltaXBalle, DeltaYBalle;
	int AddCurrentXPosition, AddCurrentYPosition;
	int XBarre, YBarre, DeltaXBarre;
	int Referency_Value;
	float XBalle, YBalle;
	float BalleXCenter, BalleYCenter;
	float OffsetX, OffsetY;
	float Rayon;
	BOOL SendFlag, NotOutFlag, Detect_OK;

	BOOL PasteFlag;

	public:

	Balle(int StartXBalle=0, int StartYBalle=0);
	void SendBalle();
	void Borders();
	void MoveBalle();
	void NewPosBarre(int NewXBarre, int NewYBarre, int NewDeltaXBarre);
	void NewOffsetX(float NewOffsetX);
	void NewOffsetY(float NewOffsetY);
	void LOCK();
	void UNLOCK();
	void Reset();
	void Paste();
	void ResetPaste();
	void ResetBonusAndMalus();
	void DesactiveSendFlag();
	BOOL BalleInScene();
	BOOL OK();
	BOOL PasteFlagStatement();
	int ReturnXBalle();
	int ReturnYBalle();
	int ReturnDimLeftBalle();
	int ReturnDimRightBalle();
	int ReturnDimTopBalle();
	int ReturnDimBottomBalle();
	float ReturnRayon();
	float ReturnOffsetX();
	float ReturnOffsetY();
};

Balle::Balle(int StartXBalle, int StartYBalle)
{
		LeftBalle=0;
		TopBalle=213;
		RightBalle=21;
		BottomBalle=234;
		
		DeltaXBalle=RightBalle-LeftBalle;
		DeltaYBalle=BottomBalle-TopBalle;
		
		Rayon=(float)DeltaYBalle/2;

		XBalle=(float)StartXBalle-(DeltaXBalle/2);
		YBalle=(float)(2+(StartYBalle-DeltaYBalle));
		Referency_Value=5;
		OffsetX=0;
		OffsetY=-(float)Referency_Value;

		NotOutFlag=TRUE;
		SendFlag=FALSE;
		Detect_OK=FALSE;
};

// Lance la balle.
void Balle::SendBalle()
{
	SendFlag=TRUE;
	NotOutFlag=TRUE;
}

// Algorythme de détéction de collision avec les
// bordures de l'écran.
void Balle::Borders()
{
	if (XBalle<5)
	{
		OffsetX=OffsetX<0 ? -OffsetX : OffsetX;
	}

	if (XBalle>635-DeltaXBalle)
	{
		OffsetX=OffsetX>0 ? -OffsetX : OffsetX;
	}

	if (YBalle<5)
	{
		OffsetY=-OffsetY;
		UNLOCK();
	}

	if (YBalle>480)
	{
		Reset();
	}
}

// Déplace la balle selon les directives du programme.
void Balle::MoveBalle()
{
	if (SendFlag && NotOutFlag)
	{
		if (OffsetY<0.1f && OffsetY>-0.1f) {OffsetY=0.1f;}
		XBalle+=OffsetX;
		YBalle+=OffsetY;
		Borders();
	}
	else
	{
		if (PasteFlag) 
		{
			XBalle=(float)XBarre+AddCurrentXPosition;
			YBalle=(float)YBarre+AddCurrentYPosition;
		}
		else 
		{
			XBalle=(float)XBarre+(DeltaXBarre/2)-(DeltaXBalle/2);
			YBalle=(float)YBarre-(float)DeltaYBalle+2;
			OffsetY=-(float)Referency_Value;
			OffsetX=0;
			NotOutFlag=TRUE;
		}
	}
}

// Copie les nouvelles coordonnées de la barre.
void Balle::NewPosBarre(int NewXBarre, int NewYBarre, int NewDeltaXBarre)
{
	XBarre=NewXBarre;
	YBarre=NewYBarre;
	DeltaXBarre=NewDeltaXBarre;
}

// Copie les nouvelles valeurs de déplacement.
void Balle::NewOffsetX(float NewOffsetX)
{
	OffsetX=NewOffsetX;
}

void Balle::NewOffsetY(float NewOffsetY)
{
	OffsetY=NewOffsetY;

}

// Pour les tests de collisions : empêche la balle
// de pouvoir redétécter à nouveau la barre.
void Balle::LOCK()
{
	Detect_OK=FALSE;
}

// Autorise à nouveau les tests de collisions avec la
// barre.
void Balle::UNLOCK()
{
	Detect_OK=TRUE;
}

// Replace la balle à sa position initiale.
void Balle::Reset()
{
	Detect_OK=FALSE;
	NotOutFlag=FALSE;
	SendFlag=FALSE;
	PasteFlag=FALSE;
	YBalle=2.0f+(YBarre-DeltaYBalle);
}

// Indique que le bonus Colle à été activé.
void Balle::Paste()
{
	PasteFlag=TRUE;
}

void Balle::ResetPaste()
{
	PasteFlag=FALSE;
	SendBalle();
}

// Remets à leurs états initiaux les bonus et 
// les malus.
void Balle::ResetBonusAndMalus()
{
	PasteFlag=FALSE;
}

// Pour le bonus Colle, si barre touchée, empêche le
// renvoi immédiat et calcule le point d'impact sur
// la barre.
void Balle::DesactiveSendFlag()
{
	SendFlag=FALSE;
	AddCurrentXPosition=(int)(XBalle-XBarre);
	AddCurrentYPosition=(int)(YBalle-YBarre);
}

// Renvoie l'indicateur de présence de la balle sur
// le terrain de jeu.
BOOL Balle::BalleInScene()
{
	return (NotOutFlag);
}

// Renvoie l'indicateur d'autorisation pour
// les algorythmes.
BOOL Balle::OK()
{
	return (Detect_OK);
}

// Renvoie l'indicateur d'activation du bonus Colle.
BOOL Balle::PasteFlagStatement()
{
	return (PasteFlag);
}

// Renvoie les coordonnées de la balle.
int Balle::ReturnXBalle()
{
	return (int)(XBalle);
}

int Balle::ReturnYBalle()
{
	return (int)(YBalle);
}

// Renvoie les dimensions de la balle.
int Balle::ReturnDimLeftBalle()
{
	return (int)(LeftBalle);
}

int Balle::ReturnDimRightBalle()
{
	return (int)(RightBalle);
}

int Balle::ReturnDimTopBalle()
{
	return (TopBalle);
}

int Balle::ReturnDimBottomBalle()
{
	return (int)(BottomBalle);
}

float Balle::ReturnRayon()
{
	return (Rayon);
}

// Renvoie les valeurs de déplacement.
float Balle::ReturnOffsetX()
{
	return (OffsetX);
}

float Balle::ReturnOffsetY()
{
	return (OffsetY);
}