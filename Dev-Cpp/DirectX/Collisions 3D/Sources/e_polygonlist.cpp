/*****************************************************************************
 * ==> Classe E_PolygonList -------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la gestion d'une liste de polygones.    *
 *               Ceci est particuli�rement utile pour la d�tection des       *
 *               collisions, par exemple.                                    *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_polygonlist.h"

// ************************ Classe E_PolygonContainer *************************
// Cette classe, qui repr�sente un noeud de la liste, permet de contenir un
// polygone.

// Constructeur de la classe E_PolygonContainer.
E_PolygonContainer::E_PolygonContainer()
{
    p_Polygon = NULL;
    p_Next    = NULL;
}

// Destructeur de la classe E_PolygonContainer.
E_PolygonContainer::~E_PolygonContainer()
{
    // On nettoie la ressource utilis�e par le polygone.
    if ( p_Polygon != NULL )
    {
        delete p_Polygon;
        p_Polygon = NULL;
    }
}

// Obtient le pointeur vers le polygone.
E_Polygon* E_PolygonContainer::GetPolygon()
{
    return p_Polygon;
}

// D�finit le pointeur vers le polygone.
void E_PolygonContainer::SetPolygon( E_Polygon* Polygon )
{
    p_Polygon = Polygon;
}

// Obtient le pointeur vers le noeud suivant.
E_PolygonContainer* E_PolygonContainer::GetNext()
{
    return p_Next;
}

// D�finit le pointeur vers le noeud suivant.
void E_PolygonContainer::SetNext( E_PolygonContainer* Next )
{
    p_Next = Next;
}

// ************************** Classe E_PolygonList ****************************
// Cette classe est une liste de polygones, utilis�e par exemple pour la
// d�tection des collisions. Elle permet un acc�s simplifi� aux donn�es
// spaciales repr�sentant un objet.

// Constructeur de la classe E_PolygonList.
E_PolygonList::E_PolygonList()
{
    p_First        = NULL;
    p_Current      = NULL;

    p_PolygonCount = 0;
}

// Destructeur de la classe E_PolygonList.
E_PolygonList::~E_PolygonList()
{
    p_Current = p_First;

    // On d�truit toute la liste d'objets.
    while ( p_Current != NULL )
    {
        E_PolygonContainer* p_Tmp = p_Current->GetNext();
        delete p_Current;
        p_Current = p_Tmp;
    }

    p_First   = NULL;
    p_Current = NULL;
}

// Cette fonction permet l'ajout d'un polygone dans la liste.
bool E_PolygonList::AddPolygon( E_Polygon* Polygon )
{
    // On commence par tester si l'objet est valide.
    if ( Polygon == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Ensuite, on cr�e un nouveau conteneur.
    E_PolygonContainer* p_New = new E_PolygonContainer();

    // Puis, on teste si le conteneur a �t� correctement allou� en m�moire.
    if ( p_New == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On alloue ensuite le pointeur vers le polygone dans le conteneur.
    p_New->SetPolygon( Polygon );

    // On teste si le premier conteneur de la liste d'objets a d�j� �t� cr�e.
    if ( p_First == NULL )
    {
        // Si ce n'est pas le cas, on d�finit le nouveau conteneur comme
        // premier conteneur de la liste d'objets, et on d�finit le pointeur
        // courant sur le premier conteneur de la liste.
        p_First   = p_New;
        p_Current = p_First;
    }
    else
    {
        // Si le premier conteneur est d�fini, on ajoute le nouveau conteneur �
        // la fin de la liste. On commence par d�finir le pointeur courant sur
        // le premier conteneur.
        p_Current = p_First;

        // Puis, on passe toute la liste d'objets, jusqu'� pointer sur le
        // dernier objet de la liste.
        while ( p_Current->GetNext() != NULL )
        {
            // On passe � l'objet suivant.
            p_Current = p_Current->GetNext();
        }

        // Maintenant, on sait que p_Current contient le dernier objet de la
        // liste. On d�finit le pointeur vers le prochain conteneur sur le
        // nouveau conteneur que l'on a cr�e.
        p_Current->SetNext( p_New );
    }

    // On additionne le compteur interne de polygones.
    p_PolygonCount++;

    // Pour finir, on retourne un message de r�ussite.
    return true;
}

// Cette fonction permet l'ajout d'un polygone dans la liste.
bool E_PolygonList::AddPolygon( D3DXVECTOR3 Vertex1,
                                D3DXVECTOR3 Vertex2,
                                D3DXVECTOR3 Vertex3 )
{
    // On cr�e un nouveau polygone.
    E_Polygon* Polygon = new E_Polygon( Vertex1, Vertex2, Vertex3 );

    // Puis, on teste si le polygone a bien �t� cr�e en m�moire.
    if ( Polygon == NULL )
    {
        return false;
    }

    // Pour finir, on tente d'ajouter le polygone � la liste, et on retourne
    // la r�ponse.
    return AddPolygon( Polygon );
}

// Obtient le nombre de polygones de l'objet.
int E_PolygonList::GetCount()
{
    return p_PolygonCount;
}

// Obtient le premier conteneur de polygone de la liste.
E_PolygonContainer* E_PolygonList::GetFirst()
{
    return p_First;
}
