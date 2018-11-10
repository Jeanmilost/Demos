// ** Objet de construction des niveaux.

// Pour les briques :
// V=Violet, B=Bleu, G=Vert, Y=Or(Indestructible)
// O=Orange, R=Rouge, N=Inexistant.

struct Level
{
	int XBricks[60];
	int YBricks[60];
	char NameBricks[60];
	int ValueTable[60];

	Level();
	Level(BOOL) {};
	void DeleteValueBrick(int H);
	int ReturnXValue(int I);
	int ReturnYValue(int J);
	int ReturnValueBrick(int K);
	char ReturnName(int L);
};

// Constructeur avec tableau de briques 
// prédéfini.
Level::Level()
{
	int XCount=102, YCount=100;
	int j=0;

	for (int i=0; i<60; i++)
	{
		ValueTable[i]=1;
		XBricks[i]=XCount;
		YBricks[i]=YCount;
		XCount+=44;
		j++;
		if (j>=10)
		{
			j=0;
			XCount=102;
			YCount+=22;
		}
	}
}

// Constructeur avec tableau de briques personnalisé.
void Level::DeleteValueBrick(int H)
{
	ValueTable[H]=0;
}

// Renvoie l'emplacement d'affichage des briques.
int Level::ReturnXValue(int I)
{
	return (XBricks[I]);
}

int Level::ReturnYValue(int J)
{
	return (YBricks[J]);
}

// Renvoie un élément dans le tableau des valeurs,
// pour savoir si le niveau contient encore des
// briques à détruire.
int Level::ReturnValueBrick(int K)
{
	return (ValueTable[K]);
}

// Renvoie le type de brique à construire.
char Level::ReturnName(int L)
{
	return(NameBricks[L]);
}

// *************************************
// * Structure des différents niveaux. *
// *************************************

struct Level1 : public Level
{
	Level1();
};

Level1::Level1() : Level()
{
	char B='B', V='V', R='R', G='G', O='O', Y='Y', N='N';
	char NameLV1[]={B,V,R,O,G,B,V,R,O,G,
					G,B,V,R,O,G,B,V,R,O,
					O,G,B,V,R,O,G,B,V,R,
					R,O,G,B,V,R,O,G,B,V,
					V,R,O,G,B,V,R,O,G,B,
					B,V,R,O,G,B,V,R,O,G
	};

	for (int i=0; i<60; i++)
	{
		NameBricks[i]=NameLV1[i];
	}
}

struct Level2 : public Level
{
	Level2();
};

Level2::Level2() : Level()
{
	char B='B', V='V', R='R', G='G', O='O', Y='Y', N='N';
	char NameLV2[]={B,N,G,N,B,N,G,N,B,N,
					N,R,N,O,N,R,N,O,N,R,
					B,N,V,N,B,N,V,N,B,N,
					N,Y,N,O,N,Y,N,O,N,Y,
					N,N,Y,Y,R,O,R,Y,Y,N,
					N,N,N,N,Y,Y,Y,N,N,N
	};

	for (int i=0; i<60; i++)
	{
		NameBricks[i]=NameLV2[i];
	}
}

struct Level3 : public Level
{
	Level3(BOOL LV=TRUE);
};

Level3::Level3(BOOL LV) : Level(LV)
{
	int XBrickLV3[]={0,44,
					 0,44,88,
					 0,44,88,132,
					 0,44,88,132,176,
					 0,44,88,132,176,220,
					 0,44,88,132,176,220,264,
					 0,44,88,132,176,220,264,308,
					 0,44,88,132,176,220,264,308,352,
					 0,44,88,132,176,220,264,308,352,396,440,484,528,572,
					 0,0
	};

	int YBrickLV3[]={0,0,
					 22,22,22,
					 44,44,44,44,
					 66,66,66,66,66,
					 88,88,88,88,88,88,
					 110,110,110,110,110,110,110,
					 132,132,132,132,132,132,132,132,
					 154,154,154,154,154,154,154,154,154,
					 176,176,176,176,176,176,176,176,176,176,176,176,176,176,
					 0,0
	};

	for (int i=0; i<60; i++)
	{
		ValueTable[i]=1;
		XBricks[i]=XBrickLV3[i];
		YBricks[i]=YBrickLV3[i];
	}

	char B='B', V='V', R='R', G='G', O='O', Y='Y', N='N';
	char NameLV2[]={G,G,
					B,B,B,
					V,V,V,V,
					R,R,R,R,R,
					O,O,O,O,O,O,
					R,R,R,R,R,R,R,
					V,V,V,V,V,V,V,V,
					B,B,B,B,B,B,B,B,B,
					Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,G,
					N,N
	};

	for (int i=0; i<60; i++)
	{
		NameBricks[i]=NameLV2[i];
	}
}

struct Level4 : public Level
{
	Level4();
};

Level4::Level4() : Level()
{
	char B='B', V='V', R='R', G='G', O='O', Y='Y', N='N';
	char NameLV2[]={B,B,Y,Y,O,O,O,Y,Y,B,B,N,
					V,V,Y,N,N,O,N,N,Y,V,V,N,
					G,N,N,Y,N,N,N,Y,N,N,G,N,
					N,Y,N,R,R,R,R,R,R,N,Y,N,
					B,B,V,V,R,R,R,V,V,B,B,N,
					O,O,R,R,R,R,R,R,R,O,O,N
	};

	for (int i=0; i<60; i++)
	{
		NameBricks[i]=NameLV2[i];
	}
}

struct Level5 : public Level
{
	Level5();
};

Level5::Level5() : Level()
{
	char B='B', V='V', R='R', G='G', O='O', Y='Y', N='N';
	char NameLV2[]={Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,
					Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,
					Y,Y,Y,Y,Y,Y,N,O,G,Y,
					Y,Y,Y,Y,Y,Y,N,Y,Y,Y,
					Y,R,Y,Y,Y,Y,N,Y,Y,Y,
					Y,Y,Y,Y,Y,Y,N,Y,Y,Y,



					N,N,N,N,N,N,N,N,N,N,N
	};

	for (int i=0; i<60; i++)
	{
		NameBricks[i]=NameLV2[i];
	}
}