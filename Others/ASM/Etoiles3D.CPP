// * Programme un champ d'étoiles en 3D. *

// Programmateur : Jean-Milost REYMOND.

#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

// Procédures en assembleur.
extern "C" void init();
extern "C" void pixel(int posx,int posy,unsigned char couleur);
extern "C" void scroll(int y);
extern "C" void waitretrace();

// Définition de types de variables -
// Simule le type BOOL de microsoft.
#define BOOL unsigned char
#define COLOR unsigned char

// *** Définition des variables globales
const unsigned char TRUE=1;
const unsigned char FALSE=0;

// ********************************************
// *** Classe de transformation de points : ***
// ***      Conversion de points 3D en      ***
// ***             coordonnees 2D 	    	  ***
// ********************************************

class Moteur3D
 {
  private:
  // Coordonnées du point dans l'espace.
  float X, Y, Z;

  // Variables de retour de la fonction
  // membre c3Dto2D.
  float X1, Y1;

  // Variables internes à la fonction
  // membre c3Dto2D.
  float Z1, Alpha, Beta;
  float Focale;

  // Variable interne à la fonction
  // membre ChangeFocale.
  float Step;

  // Variables des limites.
  int CentreX, CentreY, MaxX, MaxY, MinXY;
  float MaxZ;

  // Coordonnées de retour des pixels à l'écran.
  int XPixel, YPixel, OldX, OldY;

  // Valeur des intensités de lumière.
  COLOR Color;
  COLOR BackGroundColor;

  // Variables des tests de valeurs positives-
  // négatives.
  BOOL XNegative, YNegative;

  // Indique si le pixel est dans le champ de
  // vision.
  BOOL PixelFlag;

  // Routines privées de Moteur3D.
  void c3Dto2D();
  void Resize2D();
  void Luminance();

  public :

  Moteur3D();
  void InitXYZ(float x, float y, float z);
  void ChangeFocale(int Result);
  void MovePixel();
  float ReturnXPixel();
  float ReturnYPixel();
  float ReturnXPixelBlack();
  float ReturnYPixelBlack();
  COLOR ReturnColor();
  COLOR ReturnBGColor();
  BOOL ReturnPixelFlag();
 };

// *** Constructeur.
Moteur3D::Moteur3D()
 {
  Focale=160;
  Step=1;
  CentreX=160;
  CentreY=100;
  MaxX=319;
  MaxY=199;
  MinXY=0;
  BackGroundColor=0;
  XNegative=FALSE;
  YNegative=FALSE;
  PixelFlag=FALSE;
 }

// *** Valeurs initiales de X, Y et Z.
void Moteur3D::InitXYZ(float x, float y, float z)
 {
  X=x;
  Y=y;
  Z=z;
  MaxZ=z;

  if (X<0)
  {
   X=-X;
   XNegative=TRUE;
  }

  if (Y<0)
  {
   Y=-Y;
   YNegative=TRUE;
  }
 }

// *** Routine de conversion des
// points 3D en valeurs 2D.
void Moteur3D::c3Dto2D()
 {
  Z1=Z+Focale;
  Alpha=X/Z1;
  X1=Focale*Alpha;

  Beta=Y/Z1;
  Y1=Focale*Beta;
  }

// *** Correction des emplacements pour
// l'affichage à l'écran.
void Moteur3D::Resize2D()
  {
   if (!XNegative) {XPixel=CentreX+X1;}
   else {XPixel=CentreX-X1;}
   if (!YNegative) {YPixel=CentreY+Y1;}
   else {YPixel=CentreY-Y1;}
   if (XPixel>MaxX || XPixel<MinXY) {PixelFlag=FALSE;}
   if (YPixel>MaxY || YPixel<MinXY) {PixelFlag=FALSE;}
  }

// *** Calcule la valeur de la lumière.
void Moteur3D::Luminance()
 {
  Color=31-(Z/(MaxZ/15));
 }

// *** Ordonne un changement de focale
// en fonction du bouton de la souris
// présentement appuyé.
void Moteur3D::ChangeFocale(int Result)
{
 if (Result==1)
  {
    Focale+=Step;
   if (Focale>=1600) Focale=1600;
  }
 if (Result==2)
  {
   Focale-=Step;
   if (Focale<=1) Focale=1;
  }
}

// *** Fait avancer d'un pas le champ
// de pixel => Impression de mouvement.
void Moteur3D::MovePixel()
 {
  OldX=XPixel;  // Sauvegarde les
  OldY=YPixel;  // anciennes valeurs.
  Z--;
  if (Z<=0)
  {
   Z=MaxZ;
  }

  PixelFlag=TRUE;
  c3Dto2D();      // Appel des routines
  Resize2D();     // internes.
  Luminance();
 }

// Renvoie la coordonnée X.
float Moteur3D::ReturnXPixel()
 {
  return XPixel;
 }

//Renvoie la coordonnée Y.
float Moteur3D::ReturnYPixel()
 {
  return YPixel;
 }

// Renvoie la coordonnée précédente
// de X.
float Moteur3D::ReturnXPixelBlack()
 {
  return OldX;
 }

// Renvoie la coordonnée précédente
// de Y.
float Moteur3D::ReturnYPixelBlack()
 {
  return OldY;
 }

// Renvoie la valeur de luminance.
COLOR Moteur3D::ReturnColor()
 {
  return Color;
 }

// Renvoie la couleur de fond.
COLOR Moteur3D::ReturnBGColor()
 {
  return BackGroundColor;
 }

// Renvoie le drapeau d'affichage
// du pixel.
// TRUE = Pixel afficheable,
// FALSE = Pixel non afficheable.
BOOL Moteur3D::ReturnPixelFlag()
 {
  return PixelFlag;
 }

// *******************************************
// *** Detection des boutons de la souris. ***
// *******************************************

int Detection_Souris()
{
 int Bottom=0;
 asm
 {
  mov al,0x03
  xor bx,bx
  int 0x33
  and bx,3
  mov Bottom,bx
 }
 return Bottom;
}

// **************************
// *** Boucle principale. ***
// **************************

int main(void)
{
 init();	  // Mode 320*200 - 256 Couleurs.
 Moteur3D Etoile[64];

 // Calcule, de façon aléatoire, les
 // emplacements de départ des étoiles.
 srand((unsigned) time(NULL));

 for (int i=0, j, k, l; i<64; i++)
  {
   j=rand()%800;
   if (j>400) {j=-(j-400);}
   k=rand()%600;
   if (k>300) {k=-(k-300);}
   l=(rand()%1500)+500;
   Etoile[i].InitXYZ(j, k, l);
  }

 int i=0;
 while (!kbhit())
 {
  i=++i&63;

  Etoile[i].ChangeFocale(Detection_Souris());

  if (!Detection_Souris())
   {
   Etoile[i].MovePixel();

   pixel (Etoile[i].ReturnXPixelBlack(),
  			 Etoile[i].ReturnYPixelBlack(),
          Etoile[i].ReturnBGColor());

   if (Etoile[i].ReturnPixelFlag())
   {
    pixel (Etoile[i].ReturnXPixel(),
  		     Etoile[i].ReturnYPixel(),
           Etoile[i].ReturnColor());
   }
  }
 }
 return 0;
}