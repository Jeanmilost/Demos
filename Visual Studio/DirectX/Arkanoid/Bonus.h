// ** Objet de gestion des bonus et des malus. 

#include <time.h>
#include <stdlib.h>

class Bonus
{
	protected:

	char TypeBonus;
	int LeftBonus, RightBonus, TopBonus, BottomBonus;
	int SaveLeftBonus, SaveRightBonus, Add;
	int XBonus, YBonus;
	int DeltaXBonus, DeltaYBonus;
	BOOL BonusExistence, AutorisationCreate;

	BOOL ActiveBonus;
	BOOL BonusFlag, MalusFlag;

	public:

	Bonus();
	Bonus(int Left, int Right, int Top, int Bottom, char Type);
	void MoveBonus();
	void Start(int X, int Y, int DimXBrick, int DimYBrick);
	void Autorise();
	void Reset();
	char Type();
	BOOL Algorythm(float XBarre, float YBarre, float DeltaXBarre);
	BOOL Existence();
	BOOL Autorisation();
	BOOL ReturnBonusFlag();
	BOOL ReturnMalusFlag();
	int ReturnLeftBonus();
	int ReturnRightBonus();
	int ReturnTopBonus();
	int ReturnBottomBonus();
	int ReturnXBonus();
	int ReturnYBonus();
};

Bonus::Bonus()
{
	LeftBonus=RightBonus=TopBonus=BottomBonus=0;
	TypeBonus='N';
	XBonus=YBonus=0;
	AutorisationCreate=FALSE;
	BonusExistence=FALSE;
	BonusFlag=FALSE;
	MalusFlag=FALSE;
}

Bonus::Bonus(int Left, int Right, int Top, int Bottom, char Type)
{
	LeftBonus=Left;
	RightBonus=Right;
	TopBonus=Top;
	BottomBonus=Bottom;
	TypeBonus=Type;

	Add=-15;
	SaveLeftBonus=LeftBonus;
	SaveRightBonus=RightBonus;
	XBonus=YBonus=0;
	DeltaXBonus=RightBonus-LeftBonus;
	DeltaYBonus=BottomBonus-TopBonus;

	AutorisationCreate=FALSE;
	BonusExistence=FALSE;

	BonusFlag=TRUE;
	MalusFlag=FALSE;
}

// Algorythme de détection des collisions Bonus-Barre.
BOOL Bonus::Algorythm(float XBarre, float YBarre, float DeltaXBarre)
{
	if (BonusExistence)
	{
		if (YBonus+DeltaYBonus>=YBarre && XBonus+DeltaXBonus>=XBarre && XBonus<=XBarre+DeltaXBarre)
		{
			ActiveBonus=TRUE;
			BonusExistence=FALSE;
		}
		else ActiveBonus=FALSE;

		return (ActiveBonus);
	}
	else return (FALSE);
}

// Retourne le type (couleur) du bonus.
char Bonus::Type()
{
	return (TypeBonus);
}

// Algorythme de déplacement du bonus à l'écran.
void Bonus::MoveBonus()
{
	// Ligne suivante à supprimer: force l'affichage
	// d'un bonus sur chaque brique.
//	AutorisationCreate=TRUE;
	if (BonusExistence && AutorisationCreate)
	{
		YBonus+=2;
		LeftBonus+=25;
		RightBonus+=25;
		if (LeftBonus>=25*10)
		{
			BOOL Buffer=BonusFlag;
			BonusFlag=MalusFlag;
			MalusFlag=Buffer;
			Add=-Add;
			LeftBonus=SaveLeftBonus;
			RightBonus=SaveRightBonus;
			TopBonus+=Add;
			BottomBonus+=Add;
		}

		if (YBonus>=480)
		{
			BonusExistence=FALSE;
		}
	}
}

// A l'activation, détermine de quelle brique part
// le bonus, et active son droit d'existence.
void Bonus::Start(int X, int Y, int DimXBrick, int DimYBrick)
{
	BonusExistence=TRUE;
	XBonus=X+((DimXBrick/2)-(DeltaXBonus/2));
	YBonus=Y+((DimYBrick/2)-(DeltaYBonus/2));
}

// A la création, autorise le bonus à être crée.
void Bonus::Autorise()
{
	AutorisationCreate=TRUE;
}

void Bonus::Reset()
{
	BonusExistence=FALSE;
}

// Retourne le facteur d'existence du bonus :
// TRUE=Bonus existe à l'écran.
// FALSE=Bonus n'existe pas encore ou n'existe plus.
BOOL Bonus::Existence()
{
	return (BonusExistence);
}

// Retourne le facteur de création du bonus :
// TRUE=Bonus peut être crée.
// FALSE=Bonus ne peut être crée.
BOOL Bonus::Autorisation()
{
	return (AutorisationCreate);
}

// Indique si la gelude est un bonus.
BOOL Bonus::ReturnBonusFlag()
{
	return (BonusFlag);
}

// Indique si la gelude est un malus.
BOOL Bonus::ReturnMalusFlag()
{
	return (MalusFlag);
}

// Retourne les dimensions de l'élément.
int Bonus::ReturnLeftBonus()
{
	return (LeftBonus);
}

int Bonus::ReturnRightBonus()
{
	return (RightBonus);
}

int Bonus::ReturnTopBonus()
{
	return (TopBonus);
}

int Bonus::ReturnBottomBonus()
{
	return (BottomBonus);
}

// Retourne l'emplacement de l'élément.
int Bonus::ReturnXBonus()
{
	return (XBonus);
}

int Bonus::ReturnYBonus()
{
	return (YBonus);
}

// ******************************************
// * Classes des différents types de bonus. *
// ******************************************

class RedBonus : public Bonus
{
	public:

	RedBonus(int Left=0, int Right=23, int Top=63, int Bottom=78, char Type='R');
};

RedBonus::RedBonus(int Left, int Right, int Top, int Bottom, char Type) : Bonus(Left, Right, Top, Bottom, Type)
{
}

class OrangeBonus : public Bonus
{
	public:

	OrangeBonus(int Left=0, int Right=23, int Top=94, int Bottom=108, char Type='O');
};

OrangeBonus::OrangeBonus(int Left, int Right, int Top, int Bottom, char Type) : Bonus(Left, Right, Top, Bottom, Type)
{
}

class GreenBonus : public Bonus
{
	public:

	GreenBonus(int Left=0, int Right=23, int Top=123, int Bottom=137, char Type='G');
};

GreenBonus::GreenBonus(int Left, int Right, int Top, int Bottom, char Type) : Bonus(Left, Right, Top, Bottom, Type)
{
}

class BlueBonus : public Bonus
{
	public:

	BlueBonus(int Left=0, int Right=23, int Top=153, int Bottom=167, char Type='B');
};

BlueBonus::BlueBonus(int Left, int Right, int Top, int Bottom, char Type) : Bonus(Left, Right, Top, Bottom, Type)
{
}

class VioletBonus : public Bonus
{
	public:

	VioletBonus(int Left=0, int Right=23, int Top=183, int Bottom=197, char Type='V');
};

VioletBonus::VioletBonus(int Left, int Right, int Top, int Bottom, char Type) : Bonus(Left, Right, Top, Bottom, Type)
{
}