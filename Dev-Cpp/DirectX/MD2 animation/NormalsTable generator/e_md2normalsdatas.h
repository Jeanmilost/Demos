/*****************************************************************************
 * ==> Fichier E_MD2NormalsDatas.h ------------------------------------------*
 * ***************************************************************************
 * Description : Ce fichier contient une table de normales pré-calculées     *
 *               pour les modèles MD2.                                       *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_MD2NORMALSDATAS_H
#define E_MD2NORMALSDATAS_H

#include <stdio.h>
#include <stdlib.h>

#include <d3dx9.h>
#include <windows.h>

// Nombre de normales contenues dans la table.
#define NUMVERTEXNORMALS 162

// Prototype de la variable contenant les données des normales.
extern D3DXVECTOR3 MD2PreCalculedNormalsTable[NUMVERTEXNORMALS];

#endif // E_MD2NORMALSDATAS_H
