/*****************************************************************************
 * ==> Classe E_PolygonList -------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la gestion d'une liste de polygones.    *
 *               Ceci est particuli�rement utile pour la d�tection des       *
 *               collisions, par exemple.                                    *
 * Version     : 1.1                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_POLYGONLIST_H
#define E_POLYGONLIST_H

#include "e_maths.h"
#include "e_polygon.h"
#include "e_boundingbox.h"
#include "e_boundingsphere.h"

// ************************ Classe E_PolygonContainer *************************
// Cette classe, qui repr�sente un noeud de la liste, permet de contenir un
// polygone, ou une adresse vers un polygone si la liste est une copie d'une
// autre liste.

/*
 * Classe E_PolygonContainer.
 */
class E_PolygonContainer
{
  private:

    E_Polygon*          p_Polygon;
    E_PolygonContainer* p_Next;
    bool                p_IsCopy;

  public:

    E_PolygonContainer();
    ~E_PolygonContainer();

  public:

    E_Polygon*          GetPolygon();
    void                SetPolygon( E_Polygon* Polygon, bool IsCopy );
    E_PolygonContainer* GetNext   ();
    void                SetNext   ( E_PolygonContainer* Next );
};

// ************************** Classe E_PolygonList ****************************
// Cette classe est une liste de polygones, utilis�e par exemple pour la
// d�tection des collisions. Elle permet un acc�s simplifi� aux donn�es
// spaciales repr�sentant un objet, et mets � disposition des outils comme la
// cr�ation d'une bo�te de collisions pour l'objet.

/*
 * Classe E_PolygonList.
 */
class E_PolygonList
{
  private:

    E_PolygonContainer* p_First;
    E_PolygonContainer* p_Current;

    E_BoundingBox       p_BoundingBox;
    E_BoundingSphere    p_BoundingSphere;

    int                 p_PolygonCount;

  public:

    E_PolygonList();
    ~E_PolygonList();

  public:

    bool                CreateListFromMesh  ( LPD3DXMESH Mesh );

    bool                AddPolygon          ( E_Polygon* Polygon, bool IsCopy );
    bool                AddPolygon          ( D3DXVECTOR3 Vertex1,
                                              D3DXVECTOR3 Vertex2,
                                              D3DXVECTOR3 Vertex3 );

    bool                Exist               ( E_Polygon* Polygon );

    void                CreateBoundingBox   ();
    E_BoundingBox       GetBoundingBox      ();

    void                CreateBoundingSphere();
    E_BoundingSphere    GetBoundingSphere   ();

    int                 GetCount            ();

    E_PolygonContainer* GetFirst            ();

    void                ReleaseList         ();
};

#endif // E_POLYGONLIST_H
