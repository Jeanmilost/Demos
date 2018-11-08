/******************************************************************************
 * Cette classe est le moteur du jeu. C'est ici que les différentes phases du *
 * jeu seront appelées et gérées.                                             *
 ******************************************************************************/

#include "engine.h"

// Constructeur
Engine::Engine()
{
    p_Playfield   = new Playfield();

    p_NbPieces    = NB_PIECES;
    p_PositionX   = 0;
    p_PositionY   = 0;

    p_CurPiece    = NULL;
    p_NextPiece   = NULL;

    p_StartTime   = clock();
    p_EndTime     = clock();

    p_GameOver    = false;
    p_BypassClock = false;

    p_Tick        = 999;
    p_Level       = 1;

    srand( 0 );
}

// Destructeur
Engine::~Engine()
{
    if ( p_Playfield != NULL )
    {
        delete p_Playfield;
        p_Playfield = NULL;
    }

    if ( p_NextPiece != NULL )
    {
         delete p_NextPiece;
         p_NextPiece = NULL;
    }

    if ( p_CurPiece != NULL )
    {
         delete p_CurPiece;
         p_CurPiece = NULL;
    }
}

// Fonction principale du jeu, on effectue une action à chaque lecture.
void Engine::Play()
{
    // Si le jeu est fini, il n'est pas utile de lire cette fonction.
    if ( p_GameOver == true )
    {
        return;
    }

    // Sauvegarde l'ancienne hauteur de la pièce, on en aura peut-être besoin
    // plus tard.
    int OldY = p_PositionY;

    // Puis, on nettoie le terrain de jeu pour permettre un nouvel affichage.
    p_Playfield->Clean();

    // On contrôle qu'une pièce existe dans la mémoire.
    if ( p_CurPiece == NULL )
    {
        // Si aucune pièce n'existe, on en tire une au sort...
        SetNextPiece();

        // ...puis, on la place à sa position de départ dans le jeu.
        p_PositionX = ( p_Playfield->GetLenght() / 2 ) -
                          ( p_CurPiece->GetLenght() / 2 );
        p_PositionY = 0;
    }
    else
    {
        // Si une pièce existe, on commence par demander au système le temps
        // qui s'est écoulé depuis la dernière lecture, en millisecondes.
        p_EndTime = clock();

        // On teste s'il est temps de descendre la pièce d'une case, et si le
        // joueur n'a pas demandé à accélérer la vitesse du jeu.
        if ( p_EndTime - p_StartTime > p_Tick || p_BypassClock == true )
        {
            // Ici, on réinitialise la référence du temps écoulé, ainsi que la
            // valeur du drapeau qui permet d'ignorer le test temporel.
            p_StartTime   = clock();
            p_BypassClock = false;

            // On définit la vitesse du jeu, en fonction du score du joueur, et
            // à condition que le compteur de secondes p_Tick soit plus grand
            // que 0.
            if ( p_Tick > 0 )
            {
                // Obtient le score actuel.
                int Score = p_Playfield->GetScore();

                if ( Score > 100 )
                {
                    p_Level = 2;
                    p_Tick  = 800;
                }

                if ( Score > 200 )
                {
                    p_Level = 3;
                    p_Tick  = 700;
                }

                if ( Score > 300 )
                {
                    p_Level = 4;
                    p_Tick  = 600;
                }

                if ( Score > 400 )
                {
                    p_Level = 5;
                    p_Tick  = 500;
                }

                if ( Score > 500 )
                {
                    p_Level = 6;
                    p_Tick  = 400;
                }

                if ( Score > 600 )
                {
                    p_Level = 7;
                    p_Tick  = 300;
                }

                if ( Score > 700 )
                {
                    p_Level = 8;
                    p_Tick  = 200;
                }

                if ( Score > 800 )
                {
                    p_Level = 9;
                    p_Tick  = 100;
                }

                if ( Score > 900 )
                {
                    p_Level = 10;
                    p_Tick  = 0;
                }
            }

            // On augmente la position de hauteur de la pièce de 1.
            p_PositionY++;

            // Avant de permettre à la pièce de descendre, on teste si tous les
            // blocs de la prochaine position sont libres dans le terrain de
            // jeu.
            if ( TestNextPosition() == false )
            {
                // Si ce n'est pas le cas, on teste s'il est possible d'afficher
                // une nouvelle pièce. Si l'ancienne position de la pièce
                // actuelle vaut 0, alors celle-ci n'a même pas pu descendre une
                // seule fois, dans ce cas, le jeu est fini.
                if ( OldY == 0 )
                {
                    p_GameOver = true;
                }

                // Sinon, on replace la pièce à sa position précédente, et on
                // demande le verrouillage des blocs qui la composent. Ainsi,
                // ces blocs resteront présent dans le terrain de jeu jusqu'à
                // ce que la ligne sur laquelle ils se trouvent soit effacée.
                p_CurPiece->SetPiece( p_PositionX, OldY, true, p_Playfield );

                // Et, pour finir, on efface la pièce courante de la mémoire,
                // afin d'obtenir une nouvelle pièce.
                if ( p_CurPiece != NULL )
                {
                    delete p_CurPiece;
                    p_CurPiece = NULL;
                }
            }
        }
    }

    // A chaque lecture de la fonction, on teste les lignes du terrain de jeu,
    // pour s'assurer que les lignes pleines soient effacées.
    p_Playfield->TestLines();

    // Et, enfin, on place la pièce actuelle à sa nouvelle position dans le
    // terrain de jeu.
    if ( p_CurPiece != NULL )
    {
        // Notez que les blocs NE SONT PAS verrouillés (paramètre false), ainsi
        // ils seront effacés lorsque le terrain de jeu sera nettoyé. (Fonction
        // Clean, voir l'appel plus haut, au début de la fonction.)
        p_CurPiece->SetPiece( p_PositionX, p_PositionY, false, p_Playfield );
    }
}

// Cette fonction réinitialise les paramètres du jeu à leurs valeurs d'origine.
void Engine::Reset()
{
    p_NbPieces    = NB_PIECES;
    p_PositionX   = 0;
    p_PositionY   = 0;

    p_StartTime   = clock();
    p_EndTime     = clock();

    p_GameOver    = false;
    p_BypassClock = false;

    p_Tick        = 999;
    p_Level       = 1;

    srand( 0 );

    if ( p_NextPiece != NULL )
    {
         delete p_NextPiece;
         p_NextPiece = NULL;
    }

    if ( p_CurPiece != NULL )
    {
         delete p_CurPiece;
         p_CurPiece = NULL;
    }

    p_Playfield->Reset();
}

// Permet d'obtenir le pointeur vers la classe du terrain de jeu.
Playfield* Engine::GetPlayfield()
{
    return p_Playfield;
}

// Permet d'obtenir la valeur du niveau.
int Engine::GetLevel()
{
    return p_Level;
}

// Cette fonction teste la validité de la prochaine position d'une pièce.
bool Engine::TestNextPosition()
{
    // S'il n'y a pas de pièce en mémoire, la prochaine position ne peut pas
    // être déterminée, dans ce cas, le teste échoue.
    if ( p_CurPiece == NULL )
    {
        return false;
    }

    // Sinon, on fait appel à la fonction de test disponible dans la classe de
    // la pièce concernée.
    return p_CurPiece->TestNextPosition( p_PositionX,
                                         p_PositionY,
                                         p_Playfield );
}

// Cette fonction détermine la prochaine pièce à envoyer dans le terrain de jeu.
void Engine::SetNextPiece()
{
    // L'ordinateur doit connaître à l'avance la prochaine pièce à envoyer après
    // la pièce courante, ceci afin de pouvoir l'afficher sur le bord du terrain
    // de jeu. Ici, on teste si cette pièce existe dans la mémoire.
    if ( p_NextPiece == NULL )
    {
        // Si elle n'existe pas, on demande à l'ordinateur d'en tirer une au
        // sort, et de nous envoyer son pointeur, que l'on copie dans la
        // variable p_NextPiece.
        p_NextPiece = GetNewPiece();
    }

    // On teste ici si la pièce courante est encore présente dans la mémoire.
    if ( p_CurPiece != NULL )
    {
        // Si c'est le cas, on l'efface avant de continuer.
        delete p_CurPiece;
        p_CurPiece = NULL;
    }

    // Puisque l'on est sûr maintenant que la pièce courante n'est plus en
    // mémoire, on attribue le pointeur de la pièce suivante, affichée au bord
    // du terrain de jeu, à celui de la pièce courante, puis on tire au sort
    // une nouvelle pièce suivante.
    p_CurPiece  = p_NextPiece;
    p_NextPiece = GetNewPiece();
}

// Tire au sort une pièce, et renvoie son pointeur.
Piece* Engine::GetNewPiece()
{
    // On utilise une fonction de hasard, que l'on limite à une plage située
    // entre 0 et le nombre de pièces possibles dans de jeu.
    switch ( rand() % p_NbPieces )
    {
        // Si le nombre 0 sort, on construit un rubis.
        case 0:
        {
            return new Rubis();
            break;
        }

        // Si le nombre 1 sort, on construit une émeraude.
        case 1:
        {
            return new Emerald();
            break;
        }

        // Si le nombre 2 sort, on construit un diamant.
        case 2:
        {
            return new Diamond();
            break;
        }

        // Si le nombre 3 sort, on construit un saphyr.
        case 3:
        {
            return new Saphyr();
            break;
        }

        // Si le nombre 4 sort, on construit une agathe.
        case 4:
        {
            return new Agatha();
            break;
        }

        // Si le nombre 5 sort, on construit une citrine.
        case 5:
        {
            return new Citrin();
            break;
        }

        // Si le nombre 6 sort, on construit une aquamarine.
        case 6:
        {
            return new Aquamarine();
            break;
        }

        // Par défaut, on ne construit rien, il s'agit d'une erreur.
        default:
        {
            return NULL;
            break;
        }
    }
}

// Permet de tourner la pièce d'1/4 de tour vers la droite.
void Engine::RotateRight()
{
    // D'abord, on teste que la pièce existe bel et bien.
    if ( p_CurPiece != NULL )
    {
        // On essaie ici de tourner la pièce d'1/4 de tour vers la droite.
        p_CurPiece->RotateRight();

        // Puis, on teste la pièce dans sa nouvelle position.
        if ( p_CurPiece->TestNextPosition
            ( p_PositionX, p_PositionY, p_Playfield ) == false )
        {
            // Si le teste échoue, on nettoie le terrain de jeu, puis on remet
            // la pièce dans sa position d'origine.
            p_Playfield->Clean();
            p_CurPiece->RotateLeft();
        }
    }
}

// Permet de tourner la pièce d'1/4 de tour vers la gauche.
void Engine::RotateLeft()
{
    // D'abord, on teste que la pièce existe bel et bien.
    if ( p_CurPiece != NULL )
    {
        // On essaie ici de tourner la pièce d'1/4 de tour vers la gauche.
        p_CurPiece->RotateLeft();

        // Puis, on teste la pièce dans sa nouvelle position.
        if ( p_CurPiece->TestNextPosition
            ( p_PositionX, p_PositionY, p_Playfield ) == false )
        {
            // Si le teste échoue, on nettoie le terrain de jeu, puis on remet
            // la pièce dans sa position d'origine.
            p_Playfield->Clean();
            p_CurPiece->RotateRight();
        }
    }
}

// Permet de faire avancer la pièce d'une case vers la droite.
void Engine::MoveRight()
{
    // D'abord, on teste que la pièce existe bel et bien.
    if ( p_CurPiece != NULL )
    {
        // On augmente la position de la pièce d'une case vers la droite.
        p_PositionX++;

        // Puis, on teste la pièce dans sa nouvelle position.
        if ( p_CurPiece->TestNextPosition
            ( p_PositionX, p_PositionY, p_Playfield ) == false )
        {
            // Si le teste échoue, on remet la pièce à sa position d'origine.
            p_PositionX--;
        }
    }
}

// Permet de faire avancer la pièce d'une case vers la gauche.
void Engine::MoveLeft()
{
    // D'abord, on teste que la pièce existe bel et bien.
    if ( p_CurPiece != NULL )
    {
        // On augmente la position de la pièce d'une case vers la gauche.
        p_PositionX--;

        // Puis, on teste la pièce dans sa nouvelle position.
        if ( p_CurPiece->TestNextPosition
            ( p_PositionX, p_PositionY, p_Playfield ) == false )
        {
            // Si le teste échoue, on remet la pièce à sa position d'origine.
            p_PositionX++;
        }
    }
}

// Permet de court-circuiter le test temporel de la fonction Play.
void Engine::BypassClock()
{
    p_BypassClock = true;
}

// Retourne l'état du drapeau de fin du jeu.
bool Engine::IsGameOver()
{
    return p_GameOver;
}

// Retourne le pointeur de la prochaine pièce qui va apparaître dans le jeu.
Piece* Engine::GetNextPiece()
{
    return p_NextPiece;
}
