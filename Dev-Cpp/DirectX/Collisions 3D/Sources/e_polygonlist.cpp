/*****************************************************************************
 * ==> Classe E_PolygonList -------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la gestion d'une liste de polygones.    *
 *               Ceci est particulièrement utile pour la détection des       *
 *               collisions, par exemple.                                    *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_polygonlist.h"

// ************************ Classe E_PolygonContainer *************************
// Cette classe, qui représente un noeud de la liste, permet de contenir un
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
    // On nettoie la ressource utilisée par le polygone.
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

// Définit le pointeur vers le polygone.
void E_PolygonContainer::SetPolygon( E_Polygon* Polygon )
{
    p_Polygon = Polygon;
}

// Obtient le pointeur vers le noeud suivant.
E_PolygonContainer* E_PolygonContainer::GetNext()
{
    return p_Next;
}

// Définit le pointeur vers le noeud suivant.
void E_PolygonContainer::SetNext( E_PolygonContainer* Next )
{
    p_Next = Next;
}

// ************************** Classe E_PolygonList ****************************
// Cette classe est une liste de polygones, utilisée par exemple pour la
// détection des collisions. Elle permet un accès simplifié aux données
// spaciales représentant un objet.

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

    // On détruit toute la liste d'objets.
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

    // Ensuite, on crée un nouveau conteneur.
    E_PolygonContainer* p_New = new E_PolygonContainer();

    // Puis, on teste si le conteneur a été correctement alloué en mémoire.
    if ( p_New == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On alloue ensuite le pointeur vers le polygone dans le conteneur.
    p_New->SetPolygon( Polygon );

    // On teste si le premier conteneur de la liste d'objets a déjà été crée.
    if ( p_First == NULL )
    {
        // Si ce n'est pas le cas, on définit le nouveau conteneur comme
        // premier conteneur de la liste d'objets, et on définit le pointeur
        // courant sur le premier conteneur de la liste.
        p_First   = p_New;
        p_Current = p_First;
    }
    else
    {
        // Si le premier conteneur est défini, on ajoute le nouveau conteneur à
        // la fin de la liste. On commence par définir le pointeur courant sur
        // le premier conteneur.
        p_Current = p_First;

        // Puis, on passe toute la liste d'objets, jusqu'à pointer sur le
        // dernier objet de la liste.
        while ( p_Current->GetNext() != NULL )
        {
            // On passe à l'objet suivant.
            p_Current = p_Current->GetNext();
        }

        // Maintenant, on sait que p_Current contient le dernier objet de la
        // liste. On définit le pointeur vers le prochain conteneur sur le
        // nouveau conteneur que l'on a crée.
        p_Current->SetNext( p_New );
    }

    // On additionne le compteur interne de polygones.
    p_PolygonCount++;

    // Pour finir, on retourne un message de réussite.
    return true;
}

// Cette fonction permet l'ajout d'un polygone dans la liste.
bool E_PolygonList::AddPolygon( D3DXVECTOR3 Vertex1,
                                D3DXVECTOR3 Vertex2,
                                D3DXVECTOR3 Vertex3 )
{
    // On crée un nouveau polygone.
    E_Polygon* Polygon = new E_Polygon( Vertex1, Vertex2, Vertex3 );

    // Puis, on teste si le polygone a bien été crée en mémoire.
    if ( Polygon == NULL )
    {
        return false;
    }

    // Pour finir, on tente d'ajouter le polygone à la liste, et on retourne
    // la réponse.
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
