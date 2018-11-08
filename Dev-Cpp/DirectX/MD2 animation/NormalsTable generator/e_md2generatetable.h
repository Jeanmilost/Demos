/*****************************************************************************
 * ==> Classe E_MD2GenerateTable --------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la g�n�ration d'un fichier binaire      *
 *               contenant les donn�es des normales pr�-calcul�es.           *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_MD2GENERATETABLE_H
#define E_MD2GENERATETABLE_H

#include <d3dx9.h>

#include "e_md2normalsdatas.h"

/*
 * Classe E_MD2GenerateTable.
 */
class E_MD2GenerateTable
{
  private:

    float p_Version;
    int   p_DatasLength;

  public:

    E_MD2GenerateTable();
    ~E_MD2GenerateTable();

  public:

    bool CreateFile();
};

#endif // E_MD2GENERATETABLE_H
