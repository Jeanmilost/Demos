/*****************************************************************************
 * ==> Classe E_AABBTree ----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet de diminuer le nombre de polygones pour *
 *               un test de collisions, en classant les polygones dans un    *
 *               arbre. L'algorythme utilisé est appelé Aligned Axis         *
 *               Bounding Box.                                               *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_aabbtree.h"

// ************************** Classe E_AABBTreeInfo ***************************
// Cette classe contient les informations pour le comportement de l'arbre. On
// peut y définir la profondeur maximum, ou le nombre minimum de polygones par
// feuille, par exemple.

// Constructeur de la classe E_AABBTreeInfo.
E_AABBTreeInfo::E_AABBTreeInfo()
{
    p_MaxTreeDeep  = -1;
    p_CurrentDeep  = 0;
    p_MinVertices  = 0;
    p_DivisionType = E_LONGESTAXIS;
}

// Destructeur de la classe E_AABBTreeInfo.
E_AABBTreeInfo::~E_AABBTreeInfo()
{
}

// Obtient la profondeux maximum désirée.
int E_AABBTreeInfo::GetMaxTreeDeep()
{
    return p_MaxTreeDeep;
}

// Définit la profondeux maximum désirée.
void E_AABBTreeInfo::SetMaxTreeDeep( int MaxTreeDeep )
{
    p_MaxTreeDeep = MaxTreeDeep;
}

// Obtient le nombre minimum de polygones désirés par feuille.
int E_AABBTreeInfo::GetMinVertices()
{
    return p_MinVertices;
}

// Définit le nombre minimum de polygones désirés par feuille.
void E_AABBTreeInfo::SetMinVertices( int MinVertices )
{
    p_MinVertices = MinVertices;
}

// Obtient le type de division à appliquer pour le tri.
E_DivisionType E_AABBTreeInfo::GetDivisionType()
{
    return p_DivisionType;
}

// Définit le type de division à appliquer pour le tri.
void E_AABBTreeInfo::SetDivisionType( E_DivisionType DivisionType )
{
    p_DivisionType = DivisionType;
}

// Cette fonction incrémente le niveau de profondeur courant.
void E_AABBTreeInfo::AddDeep()
{
    p_CurrentDeep++;
}

// Obtient le niveau de profondeur courant.
int E_AABBTreeInfo::GetDeep()
{
    return p_CurrentDeep;
}

// Cette fonction teste si la profondeur courante a atteint le niveau maximum.
bool E_AABBTreeInfo::TestDeep()
{
    // On teste d'abord si la profondeur doit être testée. Si celle-ci vaut -1,
    // le test doit être ignoré.
    if ( p_MaxTreeDeep == -1 )
    {
        // Dans ce cas, la profondeux max ne peut pas être atteinte.
        return false;
    }

    // Puis, on teste si le niveau maximum est atteint.
    if ( p_CurrentDeep >= p_MaxTreeDeep - 1 )
    {
        // Si c'est le cas, on retourne une réponse positive.
        return true;
    }

    // sinon, on retourne une réponse négative.
    return false;
}

// Cette fonction teste si le noeud contient le nombre minimum de polygones.
bool E_AABBTreeInfo::TestMinVertices( int CurVertices )
{
    // On teste si le nombre de polygones courant est plus grand ou égal au
    // nombre minimum.
    if ( CurVertices >= p_MinVertices )
    {
        // Si c'est le cas, on retourne une réponse négative.
        return false;
    }

    // Sinon, on retourne une réponse positive.
    return true;
}

// **************************** Classe E_AABBNode *****************************
// Cette classe représente soit un noeud, soit une extrémité, aussi désignée
// par le terme feuille, dans l'arbre.

// Constructeur de la classe E_AABBNode.
E_AABBNode::E_AABBNode()
{
    p_Left        = NULL;
    p_Right       = NULL;
    p_PolygonList = NULL;
}

// Destructeur de la classe E_AABBNode.
E_AABBNode::~E_AABBNode()
{
    // On libère les ressources allouées au noeud enfant gauche.
    if ( p_Left != NULL )
    {
        delete p_Left;
        p_Left = NULL;
    }

    // On libère les ressources allouées au noeud enfant droite.
    if ( p_Right != NULL )
    {
        delete p_Right;
        p_Right = NULL;
    }

    // Puis, on détruit la liste de polygones du noeud.
    if ( p_PolygonList != NULL )
    {
        delete p_PolygonList;
        p_PolygonList = NULL;
    }
}

// Cette fonction permet d'obtenir le point central de l'objet.
D3DXVECTOR3 E_AABBNode::FindPolygonsCenter()
{
    // On assume que le point central vaut 0 par défaut.
    D3DXVECTOR3 Center = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

    // On s'assure ensuite de travailler avec une liste de polygones valide.
    if ( p_PolygonList == NULL )
    {
        // Sinon, on retourne la réponse par défaut.
        return Center;
    }

    // Puis, on obtient le premier conteneur de la liste de polygones.
    E_PolygonContainer* p_Current = p_PolygonList->GetFirst();

    // On passe ensuite toute la liste en revue.
    while ( p_Current != NULL )
    {
        // On additionne le point central de chaque polygone.
        E_Polygon Polygon = p_Current->GetPolygon()->GetClone();
        Center           += Polygon.GetCenter();

        // Puis, on passe au polygone suivant.
        p_Current = p_Current->GetNext();
    }

    // On s'assure ensuite qu'il y ait au moint 1 polygone.
    if ( p_PolygonList->GetCount() > 0 )
    {
        // Si c'est le cas, on divise le résultat par le nombre de polygones.
        Center /= p_PolygonList->GetCount();
    }

    // Pour finir, on retourne la réponse.
    return Center;
}

// Cette fonction obtient l'axe de coupe sur le côté le plus long de la boîte.
E_Axis E_AABBNode::FindLongestAxis()
{
    // On retourne l'axe défini par la boîte de collisions.
    return p_PolygonList->GetBoundingBox().GetLongestAxis();
}

// Cette fonction permet d'obtenir le meilleur axe de coupe.
E_Axis E_AABBNode::FindBestAxis()
{
    float BoxCenter [3];
    int   AxisResult[3];

    // On obtient le point central de la boîte.
    D3DXVECTOR3 vBoxCenter = p_PolygonList->GetBoundingBox().GetCenter();

    BoxCenter[0] = vBoxCenter.x;
    BoxCenter[1] = vBoxCenter.y;
    BoxCenter[2] = vBoxCenter.z;

    // Puis, on teste les 3 axes.
    for ( int Axis = 0; Axis < 3; Axis++ )
    {
        int Left  = 0;
        int Right = 0;

        E_PolygonContainer* p_Current = p_PolygonList->GetFirst();

        // Pour chaque axe, on passe en revue la liste de polygones.
        while ( p_Current != NULL )
        {
            // On obtient la copie du polygone courant, puis on calcule le
            // centre.
            E_Polygon Polygon = p_Current->GetPolygon()->GetClone();
            D3DXVECTOR3 v = Polygon.GetCenter();

            float FaceCenter[3];

            FaceCenter[0] = v.x;
            FaceCenter[1] = v.y;
            FaceCenter[2] = v.z;

            // Sur chaque axe, on recherche si le centre du polygone se trouve
            // avant ou après le point central de la boîte de collision.
            if ( FaceCenter[Axis] <= BoxCenter[Axis] )
            {
                // S'il est avant, on incrémente le compteur gauche.
                Left++;
            }
            else
            {
                // Sinon, on incrémente le compteur droite.
                Right++;
            }

            // Puis, on passe au polygone suivant.
            p_Current = p_Current->GetNext();
        }

        // Pour chaque axe, on inscrit le résultat du test.
        AxisResult[Axis] = abs( Left - Right );
    }

    int Index  = 0;
    int Result = AxisResult[0];

    // On recherche ensuite quel est le résultat le plus grand.
    for ( int i = 1; i < 3; i++ )
    {
        // On teste si le résultat de l'axe courant est plus grand que le
        // résultat final.
        if ( AxisResult[i] < Result )
        {
            // Si c'est le cas, on corrige les valeurs.
            Result = AxisResult[i];
            Index  = i;
        }
    }

    // Puis, on teste l'index obtenu.
    switch ( Index )
    {
        // Si l'index vaut 0, on choisit l'axe des X.
        case 0:
        {
            // On retourne la réponse.
            return E_X_AXIS;
        }

        // Si l'index vaut 1, on choisit l'axe des Y.
        case 1:
        {
            // On retourne la réponse.
            return E_Y_AXIS;
        }

        // Si l'index vaut 2, on choisit l'axe des Z.
        case 2:
        {
            // On retourne la réponse.
            return E_Z_AXIS;
        }

        // Le résultat est obligatoirement compris entre 0 et 2, aussi la
        // valeur indéfinie n'existe pas pour ce choix, mais on l'indique
        // tout de même pour éviter des problèmes de compilation.
        default:
        {
            // On retourne la réponse.
            return E_X_AXIS;
        }
    }
}

// Cette fonction teste de quel côté se trouve le polygone par rapport au plan.
E_Side E_AABBNode::TestPolygonWithBBoxPlane( E_Polygon* Polygon,
                                             D3DXPLANE  BBPlane )
{
    float Result[3];

    // On teste chaque sommet du polygone.
    for ( int i = 0; i < 3; i++ )
    {
        Result[i] =
            E_Maths::GetDistanceToPlane( BBPlane, Polygon->GetVertex( i ) );
    }

    // On teste si le polygone est co-planaire, après le plan, avant le plan,
    // ou s'il coupe le plan.
    if ( Result[0] == 0.0f && Result[1] == 0.0f && Result[2] == 0.0f )
    {
        // Le polygone est co-planaire, on retourne la réponse.
        return E_COPLANAR;
    }
    else if ( Result[0] <= 0.0f && Result[1] <= 0.0f && Result[2] <= 0.0f )
    {
        // Le polygone est avant le plan, on retourne la réponse.
        return E_BACK;
    }
    else if ( Result[0] >= 0.0f && Result[1] >= 0.0f && Result[2] >= 0.0f )
    {
        // Le polygone est après le plan, on retourne la réponse.
        return E_FRONT;
    }
    else
    {
        // Le polygone coupe le plan, on retourne la réponse.
        return E_SPLITTED;
    }
}

// Cette fonction ajoute un ou plusieurs noeuds enfants dans l'arbre.
bool E_AABBNode::CreateChild( E_PolygonList* PolygonList,
                              E_AABBTreeInfo Info,
                              E_AABBTree*    Root )
{
    // On teste si la liste de polygones est valide.
    if ( PolygonList == NULL )
    {
        // Si ce n'est pas le cas, on retourne une réponse négative.
        return false;
    }

    p_PolygonList = PolygonList;

    // On crée un conteneur de polygones pour le noeud enfant de gauche.
    E_PolygonList* Left = new E_PolygonList();

    // Puis, on teste si ce conteneur a été correctement crée en mémoire.
    if ( Left == NULL )
    {
        // Si ce n'est pas le cas, on retourne une réponse négative.
        return false;
    }

    // On crée un conteneur de polygones pour le noeud enfant de droite.
    E_PolygonList* Right = new E_PolygonList();

    // Puis, on teste si ce conteneur a été correctement crée en mémoire.
    if ( Right == NULL )
    {
        // Si ce n'est pas le cas, on désalloue les ressources du conteneur de
        // polygone gauche, ...
        if ( Left != NULL )
        {
            delete Left;
            Left = NULL;
        }

        // ... puis on retourne une réponse négative.
        return false;
    }

    // On obtient le point central des polygones de ce noeud.
    D3DXVECTOR3 PolysCenter = FindPolygonsCenter();

    E_Axis Axis;

    // Puis, on teste quel type d'axe de division est souhaité.
    switch ( Info.GetDivisionType() )
    {
        // On demande le calcul du meilleur axe possible.
        case E_BESTAXIS:
        {
            // On calcule l'axe.
            Axis = FindBestAxis();
            break;
        }

        // On demande le calcul de l'axe le plus long.
        case E_LONGESTAXIS:
        {
            // On calcule l'axe.
            Axis = FindLongestAxis();
            break;
        }

        // Le type de calcul est inconnu.
        case E_UNKNOWDIVISIONTYPE:
        {
            // On retourne une erreur.
            return false;
        }

        // Le type de calcul est indéfini.
        default:
        {
            // On retourne une erreur.
            return false;
        }
    }

    // Puis, on calcule le plan de coupe.
    p_SplittingPlane =
        PolygonList->GetBoundingBox().GetSplittingPlane( Axis, PolysCenter );

    E_PolygonContainer* p_Current = p_PolygonList->GetFirst();

    // On passe la liste des polygones du noeud courant en revue.
    while ( p_Current != NULL )
    {
        // On détermine ensuite de quel côté du plan se trouve le polygone.
        E_Side Side =
            TestPolygonWithBBoxPlane( p_Current->GetPolygon(),
                                      p_SplittingPlane );

        // Puis, on teste la réponse obtenue.
        switch ( Side )
        {
            // Le polygone est co-planaire.
            case E_COPLANAR:
            {
                // On ajoute le polygone dans les deux listes.
                Left->AddPolygon ( p_Current->GetPolygon(), true );
                Right->AddPolygon( p_Current->GetPolygon(), true );
                break;
            }

            // Le polygone est après le plan.
            case E_BACK:
            {
                // On ajoute le polygone dans la liste de gauche.
                Left->AddPolygon( p_Current->GetPolygon(), true );
                break;
            }

            // Le polygone est avant le plan.
            case E_FRONT:
            {
                // On ajoute le polygone dans la liste de droite.
                Right->AddPolygon( p_Current->GetPolygon(), true );
                break;
            }

            // Le polygone croise le plan.
            case E_SPLITTED:
            {
                // On ajoute le polygone dans les deux listes.
                Left->AddPolygon ( p_Current->GetPolygon(), true );
                Right->AddPolygon( p_Current->GetPolygon(), true );
                break;
            }
        }

        // On passe au polygone suivant.
        p_Current = p_Current->GetNext();
    }

    // On teste la profondeur de l'arbre, ainsi que le nombre de polygones de
    // chaque liste, pour savoir si le noeud courant peut générer d'autres
    // enfants.
    if ( Info.TestDeep()           == true             ||
         Left->GetCount()          == 0                ||
         Right->GetCount()         == 0                ||
         p_PolygonList->GetCount() == Left->GetCount() ||
         p_PolygonList->GetCount() == Right->GetCount() )
    {
        // Si ce n'est pas le cas, on libère les ressources de la liste de
        // gauche, ...
        if ( Left != NULL )
        {
            delete Left;
            Left = NULL;
        }

        // ... ainsi que celles de la liste de droite, ...
        if ( Right != NULL )
        {
            delete Right;
            Right = NULL;
        }

        // ... et on retourne une réponse positive, car le noeud courant est
        // un noeud-feuille, donc une extrémité de notre arbre.
        return true;
    }

    // On crée les boîtes de collisions des listes enfants.
    Left->CreateBoundingBox();
    Right->CreateBoundingBox();

    // Puis, on incrémente le compteur de profondeur.
    Info.AddDeep();

    // On teste si la racine de l'arbre est valide.
    if ( Root != NULL )
    {
        // Si c'est le cas, on met à jour la valeur de la profondeur.
        Root->SetDeep( E_Maths::GetMaxValue( Root->GetDeep(),
                                             Info.GetDeep() ) );
    }

    // On teste ensuite si le nombre minimum de sommet est atteint pour la
    // liste de gauche.
    if ( Info.TestMinVertices( Left->GetCount() ) == false )
    {
        // Si ce n'est pas le cas, on crée un nouveau noeud à gauche.
        p_Left = new E_AABBNode();

        // Puis, on teste si le noeud a été correctement crée en mémoire.
        if ( p_Left == NULL )
        {
            // Si ce n'est pas le cas, on libère les ressources de la liste de
            // gauche, ...
            if ( Left != NULL )
            {
                delete Left;
                Left = NULL;
            }

            // ... ainsi que celles de la liste de droite, ...
            if ( Right != NULL )
            {
                delete Right;
                Right = NULL;
            }

            // ... et on retourne une réponse négative.
            return false;
        }

        // Puis, on tente de créer les données du nouveau noeud.
        if ( p_Left->CreateChild( Left, Info, Root ) == false )
        {
            // Si la création échoue, on libère les ressources de la liste de
            // gauche, ...
            if ( Left != NULL )
            {
                delete Left;
                Left = NULL;
            }

            // ... ainsi que celles de la liste de droite, ...
            if ( Right != NULL )
            {
                delete Right;
                Right = NULL;
            }

            // ... et on retourne une réponse négative.
            return false;
        }

        // On teste si la racine de l'arbre est valide.
        if ( Root != NULL )
        {
            // Si c'est le cas, on incrémente le compteur de noeuds.
            Root->AddChildCount();
        }
    }
    else
    {
        // Si le minimum de sommets possible est atteint pour la liste de
        // gauche, on libère les ressources de celle-ci, et on ne crée pas de
        // nouveau noeud.
        if ( Left != NULL )
        {
            delete Left;
            Left = NULL;
        }
    }

    // On teste ensuite si le nombre minimum de sommet est atteint pour la
    // liste de droite.
    if ( Info.TestMinVertices( Right->GetCount() ) == false )
    {
        // Si ce n'est pas le cas, on crée un nouveau noeud à droite.
        p_Right = new E_AABBNode();

        // Puis, on teste si le noeud a été correctement crée en mémoire.
        if ( p_Right == NULL )
        {
            // Si ce n'est pas le cas, on libère les ressources de la liste de
            // gauche, ...
            if ( Left != NULL )
            {
                delete Left;
                Left = NULL;
            }

            // ... ainsi que celles de la liste de droite, ...
            if ( Right != NULL )
            {
                delete Right;
                Right = NULL;
            }

            // ... et on retourne une réponse négative.
            return false;
        }

        // Puis, on tente de créer les données du nouveau noeud.
        if ( p_Right->CreateChild( Right, Info, Root ) == false )
        {
            // Si la création échoue, on libère les ressources de la liste de
            // gauche, ...
            if ( Left != NULL )
            {
                delete Left;
                Left = NULL;
            }

            // ... ainsi que celles de la liste de droite, ...
            if ( Right != NULL )
            {
                delete Right;
                Right = NULL;
            }

            // ... et on retourne une réponse négative.
            return false;
        }

        // On teste si la racine de l'arbre est valide.
        if ( Root != NULL )
        {
            // Si c'est le cas, on incrémente le compteur de noeuds.
            Root->AddChildCount();
        }
    }
    else
    {
        // Si le minimum de sommets possible est atteint pour la liste de
        // droite, on libère les ressources de celle-ci, et on ne crée pas de
        // nouveau noeud.
        if ( Right != NULL )
        {
            delete Right;
            Right = NULL;
        }
    }

    // Ici, on retourne une réponse positive, car ce point ne peut être atteint
    // que lorsque la construction de l'arbre s'est terminée avec succès.
    return true;
}

// Cette fonction ajoute les polygones du noeud courant dans la liste réponse.
bool E_AABBNode::AddPolygons( E_PolygonList* ResultList )
{
    // On teste si la liste réponse est valide.
    if ( ResultList == NULL )
    {
        // Si ce n'est pas le cas, on retourne une réponse négative.
        return false;
    }

    E_PolygonContainer* CurPolygon = p_PolygonList->GetFirst();

    // On passe la liste de polygones du noeud courant en revue.
    while ( CurPolygon != NULL )
    {
        // On teste si le polygone existe déjà dans la liste réponse.
        if ( ResultList->Exist( CurPolygon->GetPolygon() ) == false )
        {
            // Si ce n'est pas le cas, on ajoute le polygone dans la liste.
            ResultList->AddPolygon( CurPolygon->GetPolygon(), true );
        }

        // On passe au polygone suivant.
        CurPolygon = CurPolygon->GetNext();
    }

    // Pour finir, on retourne une réponse positive.
    return true;
}

// Cette fonction retourne les polygones contenus dans la zone à tester.
bool E_AABBNode::Resolve( E_PolygonList* ResultList, E_BoundingSphere Sphere )
{
    // On teste si la liste réponse est valide.
    if ( ResultList == NULL )
    {
        // Si ce n'est pas le cas, on retourne une réponse négative.
        return false;
    }

    // On obtient la boîte de collisions du noeud courant.
    E_BoundingBox Box = p_PolygonList->GetBoundingBox();

    // On teste ensuite si la boîte de collisions croise la sphère.
    if ( E_CollisionLayout::SphereIntersectBox( Sphere.GetCenter(),
                                                Sphere.GetRadius(),
                                                Box.GetMin(),
                                                Box.GetMax() ) == false )
    {
        // Si ce n'est pas le cas, la résolution ne peut pas avoir lieu.
        return false;
    }

    // On teste s'il on a atteint un noeud enfant.
    if ( p_Left == NULL && p_Right == NULL )
    {
        // Si c'est le cas, on ajoute les polygones du noeud courant dans la
        // liste réponse.
        return AddPolygons( ResultList );
    }

    // Sinon, on teste où se trouve la sphère par rapport au plan inscrit dans
    // le noeud courant.
    if ( E_CollisionLayout::SphereIntersectPlane( Sphere.GetCenter(),
                                                  Sphere.GetRadius(),
                                                  p_SplittingPlane ) == true )
    {
        // Si la sphère croise le plan, on lance une résolution sur les deux
        // noeuds enfants.
        if ( p_Left->Resolve ( ResultList, Sphere ) == false )
        {
            // Si la résolution échoue, on retourne une réponse négative.
            return false;
        }

        if ( p_Right->Resolve( ResultList, Sphere ) == false )
        {
            // Si la résolution échoue, on retourne une réponse négative.
            return false;
        }
    }
    else if ( E_Maths::GetDistanceToPlane( p_SplittingPlane,
                                           Sphere.GetCenter() ) == 0.0f )
    {
        // Si le centre de la sphère est co-planaire, on lance une résolution
        // sur les deux noeuds enfants.
        if ( p_Left->Resolve ( ResultList, Sphere ) == false )
        {
            // Si la résolution échoue, on retourne une réponse négative.
            return false;
        }

        if ( p_Right->Resolve( ResultList, Sphere ) == false )
        {
            // Si la résolution échoue, on retourne une réponse négative.
            return false;
        }
    }
    else if ( E_Maths::GetDistanceToPlane( p_SplittingPlane,
                                           Sphere.GetCenter() ) < 0.0f )
    {
        // Si la sphère est avant le plan, on lance une résolution sur le
        // noeud de gauche.
        if ( p_Left->Resolve( ResultList, Sphere ) == false )
        {
            // Si la résolution échoue, on retourne une réponse négative.
            return false;
        }
    }
    else
    {
        // Sinon, on lance une résolution sur le noeud de droite.
        if ( p_Right->Resolve( ResultList, Sphere ) == false )
        {
            // Si la résolution échoue, on retourne une réponse négative.
            return false;
        }
    }

    // Pour finir, on retourne une réponse positive.
    return true;
}

// **************************** Classe E_AABBTree *****************************
// Cette classe représente l'arbre de tri des données. L'algorythme utilisé est
// de type Aligned Axis Bounding Box.

// Constructeur de la classe E_AABBTree.
E_AABBTree::E_AABBTree()
{
    p_Root       = NULL;

    p_Deep       = 0;
    p_ChildCount = 0;
}

// Destructeur de la classe E_AABBTree.
E_AABBTree::~E_AABBTree()
{
    // On libère les ressources du noeud racine.
    if ( p_Root != NULL )
    {
        delete p_Root;
        p_Root = NULL;
    }
}

// Cette fonction incrémente le compteur de noeuds interne.
void E_AABBTree::AddChildCount()
{
    p_ChildCount++;
}

// Obtient le nombre de noeuds de l'arbre.
int E_AABBTree::GetChildCount()
{
    return p_ChildCount + 1;
}

// Définit la profondeur de l'arbre.
void E_AABBTree::SetDeep( int Deep )
{
    p_Deep = Deep;
}

// Obtient la profondeur de l'arbre.
int E_AABBTree::GetDeep()
{
    return p_Deep;
}

// Définit les infos de comportement de l'arbre.
void E_AABBTree::SetTreeBehavior( E_AABBTreeInfo Info )
{
    p_Info = Info;
}

// Cette fonction initialise l'arbre.
bool E_AABBTree::Initialize( E_PolygonList* PolygonList )
{
    // On commence par créer le noeud racine.
    p_Root = new E_AABBNode();

    // Puis, on teste si le noeud a été correctement crée en mémoire.
    if ( p_Root == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On crée ensuite la liste de polygones pour la racine.
    E_PolygonList* p_New = new E_PolygonList();

    // Puis, on teste si la liste a été correctement crée en mémoire.
    if ( p_New == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    E_PolygonContainer* p_Current = PolygonList->GetFirst();

    // On copie les polygones passés en paramètre dans la liste racine.
    while ( p_Current != NULL )
    {
        // On copie chaque polygone dans la liste racine.
        p_New->AddPolygon( p_Current->GetPolygon(), true );

        // Puis, on passe au polygone suivant.
        p_Current = p_Current->GetNext();
    }

    // On crée ensuite une boîte de collisions pour la liste racine.
    p_New->CreateBoundingBox();

    // Puis, on tente la création de l'arbre.
    if ( p_Root->CreateChild( p_New, p_Info, this ) == false )
    {
        // Si la création échoue, on retourne une erreur.
        return false;
    }

    // Si tout s'est bien passé, on retourne une réponse positive.
    return true;
}

// Cette fonction retourne les polygones contenus dans la zone à tester.
bool E_AABBTree::Resolve( E_PolygonList* Result, E_BoundingSphere Sphere )
{
    // On commence par tester si le noeud racine existe.
    if ( p_Root == NULL )
    {
        // Si ce n'est pas le cas, on retourne une réponse négative.
        return false;
    }

    // Puis, on teste si la liste réponse est valide.
    if ( Result == NULL )
    {
        // Si ce n'est pas le cas, on retourne une réponse négative.
        return false;
    }

    // Pour finir, on tente la résolution de l'arbre.
    return p_Root->Resolve( Result, Sphere );
}
