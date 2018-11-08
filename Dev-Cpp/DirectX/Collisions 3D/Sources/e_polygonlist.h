/*****************************************************************************
 * ==> Classe E_PolygonList -------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la gestion d'une liste de polygones.    *
 *               Ceci est particulièrement utile pour la détection des       *
 *               collisions, par exemple.                                    *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_POLYGONLIST_H
#define E_POLYGONLIST_H

#include "e_polygon.h"

// ************************ Classe E_PolygonContainer *************************
// Cette classe, qui représente un noeud de la liste, permet de contenir un
// polygone.

/*
 * Classe E_PolygonContainer.
 */
class E_PolygonContainer
{
  private:

    E_Polygon*          p_Polygon;
    E_PolygonContainer* p_Next;

  public:

    E_PolygonContainer();
    ~E_PolygonContainer();

  public:

    E_Polygon*          GetPolygon();
    void                SetPolygon( E_Polygon* Polygon );
    E_PolygonContainer* GetNext   ();
    void                SetNext   ( E_PolygonContainer* Next );
};

// ************************** Classe E_PolygonList ****************************
// Cette classe est une liste de polygones, utilisée par exemple pour la
// détection des collisions. Elle permet un accès simplifié aux données
// spaciales représentant un objet.

/*
 * Classe E_PolygonList.
 */
class E_PolygonList
{
  private:

    E_PolygonContainer* p_First;
    E_PolygonContainer* p_Current;

    int                 p_PolygonCount;

  public:

    E_PolygonList();
    ~E_PolygonList();

  public:

    bool AddPolygon( E_Polygon* Polygon );
    bool AddPolygon( D3DXVECTOR3 Vertex1,
                     D3DXVECTOR3 Vertex2,
                     D3DXVECTOR3 Vertex3 );

    int GetCount();

    E_PolygonContainer* GetFirst();
};

#endif // E_POLYGONLIST_H
