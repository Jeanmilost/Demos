// Gestion du bonus Missile - Malus tête chercheuse.

class Missiles
{
	int XMissile1, XMissile2, YMissile1, YMissile2;
	int LeftMissile, RightMissile, TopMissile, BottomMissile;

	BOOL SendMissileFlag1;
	BOOL SendMissileFlag2;
	BOOL MissileFlag;

	public:

	Missiles();
	void SendMissile(ClassicBarre BarreObj);
	void StopMissile1();
	void StopMissile2();
	void ActiveMissile();
	void DesactiveMissile(ClassicBarre BarreObj);
	void MoveMissile(ClassicBarre BarreObj);
	int ReturnMissileX1();
	int ReturnMissileX2();
	int ReturnMissileY1();
	int ReturnMissileY2();
	int ReturnLeft();
	int ReturnRight();
	int ReturnTop();
	int ReturnBottom();
	BOOL ReturnMissileFlag1();
	BOOL ReturnMissileFlag2();
	BOOL Missile_OK();
};

Missiles::Missiles()
{
	MissileFlag=FALSE;
	SendMissileFlag1=SendMissileFlag2=FALSE;
	LeftMissile=192;
	RightMissile=197;
	TopMissile=0;
	BottomMissile=13;
}

void Missiles::SendMissile(ClassicBarre BarreObj)
{
	if (!SendMissileFlag1 && !SendMissileFlag2 && MissileFlag)
	{
		SendMissileFlag1=SendMissileFlag2=TRUE;
		XMissile1=BarreObj.ReturnXBarre()+10;
		XMissile2=(BarreObj.ReturnXBarre()+(BarreObj.ReturnDimRightBarre()-BarreObj.ReturnDimLeftBarre()))-14;
		YMissile1=YMissile2=BarreObj.ReturnYBarre()-(BottomMissile-TopMissile);
	}
}

void Missiles::StopMissile1()
{
	SendMissileFlag1=FALSE;
}

void Missiles::StopMissile2()
{
	SendMissileFlag2=FALSE;
}

void Missiles::ActiveMissile()
{
	MissileFlag=TRUE;
}

void Missiles::DesactiveMissile(ClassicBarre BarreObj)
{
	XMissile1=BarreObj.ReturnXBarre()+10;
	XMissile2=(BarreObj.ReturnXBarre()+(BarreObj.ReturnDimRightBarre()-BarreObj.ReturnDimLeftBarre()))-15;
	YMissile1=YMissile2=BarreObj.ReturnYBarre()-(BottomMissile-TopMissile);

	SendMissileFlag1=FALSE;
	SendMissileFlag2=FALSE;
	MissileFlag=FALSE;
}

void Missiles::MoveMissile(ClassicBarre BarreObj)
{
	if (MissileFlag && SendMissileFlag1)
	{
		YMissile1-=10;
	}

	if (MissileFlag && SendMissileFlag2)
	{
		YMissile2-=10;
	}

	if (YMissile1<=0)
	{
		SendMissileFlag1=FALSE;
	}

	if (YMissile2<=0)
	{
		SendMissileFlag2=FALSE;
	}
}

int Missiles::ReturnMissileX1()
{
	return (XMissile1);
}

int Missiles::ReturnMissileX2()
{
	return (XMissile2);
}

int Missiles::ReturnMissileY1()
{
	return (YMissile1);
}

int Missiles::ReturnMissileY2()
{
	return (YMissile2);
}

int Missiles::ReturnLeft()
{
	return (LeftMissile);
}

int Missiles::ReturnRight()
{
	return (RightMissile);
}

int Missiles::ReturnTop()
{
	return (TopMissile);
}

int Missiles::ReturnBottom()
{
	return (BottomMissile);
}

BOOL Missiles::ReturnMissileFlag1()
{
	return (SendMissileFlag1);
}

BOOL Missiles::ReturnMissileFlag2()
{
	return (SendMissileFlag2);
}

BOOL Missiles::Missile_OK()
{
	return (MissileFlag);
}