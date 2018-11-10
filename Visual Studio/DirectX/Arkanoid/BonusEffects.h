// Selon le bonus actif, Additionne l'effet au sprite.

class Effect
{
	int EffectPosition[18][6];
	int Counter;
	
	BOOL PasteFlag;
	BOOL StopFlag;

	BOOL Fly_Flag;
	BOOL SlowFlag;

	BOOL LevelUpFlag;

	BOOL UpFlag;

	BOOL AddFlag;
	BOOL SubFlag;

	BOOL CannonFlag;

	BOOL XRightFlag;
	BOOL XLeftFlag;

	public:

	Effect();
	void XRightEntrie(BOOL NewXRFlag);
	void XLeftEntrie(BOOL NewXLFlag);
	void ActivePaste();
	void ActiveStop();
	void DesactiveStop();
	void ActiveFly();
	void ActiveSlow();
	void ActiveUp();
	void DesactiveUp();
	void ActiveLevelUp();
	void DesactiveLevelUp();
	void ActiveAdd();
	void ActiveSub();
	void ActiveCannon();
	void DesactiveCannon();
	void ResetAll();
	void Tableau(int Left, int Right, int Top, int Bottom, int X, int Y);
	void CreateEffect(ClassicBarre BarreObj);
	int ReturnCounter();
	int ReturnLeft(int I);
	int ReturnRight(int I);
	int ReturnTop(int I);
	int ReturnBottom(int I);
	int ReturnXPosition(int I);
	int ReturnYPosition(int I);
};

Effect::Effect()
{
	Counter=0;
	PasteFlag=FALSE;
	StopFlag=FALSE;
	Fly_Flag=FALSE;
	SlowFlag=FALSE;
	UpFlag=FALSE;
	AddFlag=FALSE;
	SubFlag=FALSE;
	CannonFlag=FALSE;
}

void Effect::XRightEntrie(BOOL NewXRFlag)
{
	XRightFlag=NewXRFlag;
}

void Effect::XLeftEntrie(BOOL NewXLFlag)
{
	XLeftFlag=NewXLFlag;
}

void Effect::ActivePaste()
{
	PasteFlag=TRUE;
	StopFlag=FALSE;
}

void Effect::ActiveFly()
{
	Fly_Flag=TRUE;
	SlowFlag=FALSE;
}

void Effect::ActiveSlow()
{
	SlowFlag=TRUE;
	Fly_Flag=FALSE;
}

void Effect::ActiveStop()
{
	StopFlag=TRUE;
	PasteFlag=FALSE;
}

void Effect::DesactiveStop()
{
	StopFlag=FALSE;
}

void Effect::ActiveUp()
{
	UpFlag=TRUE;
}

void Effect::DesactiveUp()
{
	UpFlag=FALSE;
}

void Effect::ActiveLevelUp()
{
	LevelUpFlag=TRUE;
}

void Effect::DesactiveLevelUp()
{
	LevelUpFlag=FALSE;
}

void Effect::ActiveAdd()
{
	AddFlag=TRUE;
}

void Effect::ActiveSub()
{
	SubFlag=TRUE;
}

void Effect::ActiveCannon()
{
	CannonFlag=TRUE;
}

void Effect::DesactiveCannon()
{
	CannonFlag=FALSE;
}

void Effect::ResetAll()
{
	PasteFlag=FALSE;
	StopFlag=FALSE;
	Fly_Flag=FALSE;
	SlowFlag=FALSE;
	UpFlag=FALSE;
	LevelUpFlag=FALSE;
	AddFlag=FALSE;
	SubFlag=FALSE;
	CannonFlag=FALSE;
}

void Effect::Tableau(int Left, int Right, int Top, int Bottom, int X, int Y)
{
	EffectPosition[Counter][1]=Left;
	EffectPosition[Counter][2]=Right;
	EffectPosition[Counter][3]=Top;
	EffectPosition[Counter][4]=Bottom;
	EffectPosition[Counter][5]=X;
	EffectPosition[Counter][6]=Y;

}

void Effect::CreateEffect(ClassicBarre BarreObj)
{
	Counter=0;
	int Left, Right, Top, Bottom;

	int DeltaXBarre=BarreObj.ReturnDimRightBarre()-BarreObj.ReturnDimLeftBarre();
	int DeltaYBarre=BarreObj.ReturnDimBottomBarre()-BarreObj.ReturnDimTopBarre();

	if (LevelUpFlag)
	{
		Counter++;
		Left=176;
		Right=183;
		Top=0;
		Bottom=30;
		Tableau(Left, Right, Top, Bottom, 631-(Right-Left),435);

		Counter++;
		Tableau(BarreObj.ReturnDimLeftBarre(),
			    BarreObj.ReturnConstRightBarre()/2,
				BarreObj.ReturnDimTopBarre(),
				BarreObj.ReturnDimBottomBarre(),
				BarreObj.ReturnXBarre(),
				BarreObj.ReturnYBarre()
				);

		Counter++;
		Tableau(BarreObj.ReturnDimLeftBarre()+(BarreObj.ReturnConstRightBarre()/2),
			    BarreObj.ReturnConstRightBarre(),
				BarreObj.ReturnDimTopBarre(),
				BarreObj.ReturnDimBottomBarre(),
				BarreObj.ReturnXBarre()+(BarreObj.ReturnDimRightBarre()-BarreObj.ReturnDimLeftBarre())-(BarreObj.ReturnConstRightBarre()/2),
				BarreObj.ReturnYBarre()
				);
	}

	if (BarreObj.Barre_State())
	{
		Counter++;
		Left=187;
		Right=248;
		Top=31;
		Bottom=48;
		Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre()+((DeltaXBarre/2)-((Right-Left)/2)), BarreObj.ReturnYBarre()+2);
	}

	if (PasteFlag)
	{
		Counter++;
		Left=100;
		Right=163;
		Top=0;
		Bottom=12;
		Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre()+((DeltaXBarre/2)-((Right-Left)/2))-1, BarreObj.ReturnYBarre()+2);
	}

	if (BarreObj.ReturnClasicBarreObj())
	{
		Counter++;
		Left=129;
		Right=150;
		Top=13;
		Bottom=34;
		Tableau(Left,
				Right,
				Top,
				Bottom,
				BarreObj.ReturnXBarre(),
				BarreObj.ReturnYBarre()
				);

		Counter++;
		Tableau(Left,
				Right,
				Top,
				Bottom,
				BarreObj.ReturnXBarre()+((BarreObj.ReturnDimRightBarre()-BarreObj.ReturnDimLeftBarre())-(Right-Left)),
				BarreObj.ReturnYBarre()
				);
	}

	if (Fly_Flag)
	{
		if (UpFlag)
		{
			Counter++;
			Left=163;
			Right=175;
			Top=0;
			Bottom=23;
			Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre()+((DeltaXBarre/2)-((Right-Left)/2))-1, BarreObj.ReturnYBarre()+DeltaYBarre-2);
		}

		Counter++;
		Left=101;
		Right=114;
		Top=18;
		Bottom=20;
		Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre()+((DeltaXBarre/2)-((Right-Left)/2))-1, BarreObj.ReturnYBarre()+DeltaYBarre-2);
	}

	if (SlowFlag)
	{
		if (AddFlag || XRightFlag)
		{
			Counter++;
			Left=231;
			Right=264;
			Top=0;
			Bottom=20;
			Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre()-(Right-Left), BarreObj.ReturnYBarre());
		}
		if (SubFlag || XLeftFlag)
		{
			Counter++;
			Left=197;
			Right=231;
			Top=0;
			Bottom=20;
			Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre()+DeltaXBarre, BarreObj.ReturnYBarre());
		}
	}

	if (StopFlag)
	{
		Counter++;
		Left=129;
		Right=150;
		Top=34;
		Bottom=55;
		Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre(), BarreObj.ReturnYBarre());

		Counter++;
		Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre()+(DeltaXBarre-(Right-Left)), BarreObj.ReturnYBarre());

		Counter++;
		Left=150;
		Right=161;
		Top=28;
		Bottom=54;
		Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre()-6, BarreObj.ReturnYBarre()-2);

		Counter++;
		Left=164;
		Right=176;
		Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre()+(BarreObj.ReturnDimRightBarre()-BarreObj.ReturnDimLeftBarre())-6, BarreObj.ReturnYBarre()-2);
	}

	if (CannonFlag)
	{
		Counter++;
		Left=249;
		Right=273;
		Top=21;
		Bottom=49;
		Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre(), BarreObj.ReturnYBarre()-6);

		Counter++;
		Left=249;
		Right=273;
		Top=21;
		Bottom=49;
		Tableau(Left, Right, Top, Bottom, BarreObj.ReturnXBarre()+(DeltaXBarre-(Right-Left)), BarreObj.ReturnYBarre()-6);
	}

	if (LevelUpFlag)
	{
		
		Counter++;
		Left=183;
		Right=192;
		Top=0;
		Bottom=30;
		Tableau(Left, Right, Top, Bottom, 640-(Right-Left),435);
	}
	AddFlag=SubFlag=FALSE;
}

int Effect::ReturnCounter()
{
	return (Counter);
}

int Effect::ReturnLeft(int I)
{
	return (EffectPosition[I][1]);
}

int Effect::ReturnRight(int I)
{
	return (EffectPosition[I][2]);
}

int Effect::ReturnTop(int I)
{
	return (EffectPosition[I][3]);
}

int Effect::ReturnBottom(int I)
{
	return (EffectPosition[I][4]);
}

int Effect::ReturnXPosition(int I)
{
	return (EffectPosition[I][5]);
}

int Effect::ReturnYPosition(int I)
{
	return (EffectPosition[I][6]);
}