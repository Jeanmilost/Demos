/*****************************************************************************
 * ==> Classe E_Audio -------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet de jouer un morceau de musique.         *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_audio.h"

// Constructeur de la classe E_Audio.
E_Audio::E_Audio()
{
    p_Segment = NULL;
}

// Destructeur de la classe E_Audio.
E_Audio::~E_Audio()
{
    // On nettoie ici la ressource utilis�e par le segment de musique.
    if ( p_Segment != NULL )
    {
        p_Segment->Release();
        p_Segment = NULL;
    }
}

// Cette fonction permet de charger un morceau de musique en m�moire.
bool E_Audio::LoadFile( IDirectMusicLoader8*      theMusicLoader,
                        IDirectMusicPerformance8* theMusicPerformance,
                        LPSTR                     Filename )
{
    // On teste si la classe Music Loader a �t� correctement initialis�e, ...
    if ( theMusicLoader == NULL )
    {
        return false;
    }

    // ... ainsi que la classe Music Performance.
    if ( theMusicPerformance == NULL )
    {
        return false;
    }

    WCHAR wstrFilename[MAX_PATH];

    int DestChar = sizeof( wstrFilename );

    // Si oui, on convertit le nom du fichier � charger, ...
    if ( MultiByteToWideChar( CP_ACP,
                              0,
                              Filename,
                              -1, 
                              wstrFilename,
                              DestChar ) == 0 )
    {
        return false;
    }

    if ( DestChar > 0 )
    {
        wstrFilename[DestChar-1] = 0;
    }

    // ... puis, on tente de charger ce fichier.
    if ( FAILED( theMusicLoader->LoadObjectFromFile( CLSID_DirectMusicSegment,
                                                     IID_IDirectMusicSegment8,
                                                     wstrFilename,
                                                     (LPVOID*)&p_Segment ) ) )
    {
        return false;
    }

    p_Segment->Download( theMusicPerformance );

    long SetVolume = DMUS_VOLUME_MAX;

    // On r�gle ensuite le volume de la musique au maximum, par d�faut.
    if ( FAILED( theMusicPerformance->SetGlobalParam( GUID_PerfMasterVolume,
                                                      (void*)&SetVolume,
                                                      sizeof( long ) ) ) )
    {
        return false;
    }

    // Enfin, on retourne le message de reussite.
    return true;
}

// Cette fonction permet de jouer un morceau de musique charg� en m�moire.
bool E_Audio::Play( IDirectMusicPerformance8* theMusicPerformance )
{
    // On teste si la classe Music Performance a �t� initialis�e, ...
    if ( theMusicPerformance == NULL )
    {
        return false;
    }

    // ... et si un segment de musique existe en m�moire.
    if ( p_Segment == NULL )
    {
        return false;
    }

    // Si oui, on teste si ce segment est d�j� en cours de lecture.
    if ( theMusicPerformance->IsPlaying( p_Segment, NULL ) != S_OK )
    {
        // Si ce n'est pas le cas, on commande la lecture du segment.
        theMusicPerformance->PlaySegmentEx( p_Segment,
                                            NULL,
                                            NULL,
                                            0,
                                            0,
                                            NULL,
                                            NULL,
                                            NULL );
    }

    // Enfin, on retourne le message de reussite.
    return true;
}

// Cette fonction permet de stopper la lecture du morceau de musique.
bool E_Audio::Stop( IDirectMusicPerformance8* theMusicPerformance )
{
    // On teste si la classe Music Performance a �t� initialis�e, ...
    if ( theMusicPerformance == NULL )
    {
        return false;
    }

    // ... et si un segment de musique existe en m�moire.
    if ( p_Segment == NULL )
    {
        return false;
    }

    // Si oui, on teste si ce segment est d�j� en cours de lecture.
    if ( theMusicPerformance->IsPlaying( p_Segment, NULL ) == S_OK )
    {
        // Si c'est le cas, on commande l'arr�t de la lecture.
        theMusicPerformance->Stop( NULL,
                                   NULL,
                                   0,
                                   0 );
    }

    // Enfin, on retourne le message de reussite.
    return true;
}

// Cette fonction permet de r�gler le volume de la musique.
bool E_Audio::SetVolume( IDirectMusicPerformance8* theMusicPerformance,
                         long                      Volume )
{
    // On teste si la classe Music Performance a �t� initialis�e, ...
    if ( theMusicPerformance == NULL )
    {
        return false;
    }

    // On contr�le ensuite que l'utilisateur ne tente pas de configurer une
    // valeur trop grande, ...
    if ( Volume > DMUS_VOLUME_MAX )
    {
        Volume = DMUS_VOLUME_MAX;
    }

    // ... ou une valeur trop petite, ...
    if ( Volume < DMUS_VOLUME_MIN )
    {
        Volume = DMUS_VOLUME_MIN;
    }

    // ... puis, on param�tre DirectMusic avec la nouvelle valeur.
    if ( FAILED( theMusicPerformance->SetGlobalParam( GUID_PerfMasterVolume,
                                                      (void*)&Volume,
                                                      sizeof( long ) ) ) )
    {
        return false;
    }

    // Enfin, on retourne le message de reussite.
    return true;
}
