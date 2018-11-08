/*****************************************************************************
 * ==> Classe E_Audio -------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet de jouer un morceau de musique.         *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_AUDIO_H
#define E_AUDIO_H

#include <dmusicc.h>
#include <dmusici.h>

/*
 * Cette classe permet de jouer un morceau de musique.
 */
class E_Audio
{
  private:

    IDirectMusicSegment8* p_Segment;

  public:

    E_Audio();
    ~E_Audio();

  public:

    bool LoadFile ( IDirectMusicLoader8*      theMusicLoader,
                    IDirectMusicPerformance8* theMusicPerformance,
                    LPSTR                     Filename );
    bool Play     ( IDirectMusicPerformance8* theMusicPerformance );
    bool Stop     ( IDirectMusicPerformance8* theMusicPerformance );
    bool SetVolume( IDirectMusicPerformance8* theMusicPerformance,
                    long                      Volume );
};

#endif // E_AUDIO_H
