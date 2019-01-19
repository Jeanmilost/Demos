// **************************************************************
// *                                                            *
// *                     Gestion des erreurs                    *
// *                                                            *
// **************************************************************

#include "StdAfx.h"
#include "IP_Calculator.h"
#include "IP_Errors.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Affiche un message d'erreur � l'�cran
void IP_SetError(LPCTSTR Error)
{
    MessageBox(NULL, Error, Title, MB_ICONERROR);
}

// Affiche un message d'avertissement � l'�cran
void IP_SetWarning(LPCTSTR Warning)
{
    MessageBox(NULL, Warning, Title, MB_ICONWARNING);
}

// Affiche un message d'information � l'�cran
void IP_SetMessage(LPCTSTR Message)
{
    MessageBox(NULL, Message, Title, MB_OK);
}