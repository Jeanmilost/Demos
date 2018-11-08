/*****************************************************************************
 * ==> En-tête AnimationList.h ----------------------------------------------*
 * ***************************************************************************
 * Description : Cette en-tête contient les données de paramètrage des       *
 *               animations du personnage et de l'arme.                      *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "animationlist.h"

E_MD2Animation AnimValues[] =
{
    E_MD2Animation(   0,  39, "STAND"              ),
    E_MD2Animation(  40,  45, "RUN"                ),
    E_MD2Animation(  46,  53, "ATTACK"             ),
    E_MD2Animation(  54,  57, "PAIN_A"             ),
    E_MD2Animation(  58,  61, "PAIN_B"             ),
    E_MD2Animation(  62,  65, "PAIN_C"             ),
    E_MD2Animation(  66,  71, "JUMP"               ),
    E_MD2Animation(  72,  83, "FLIP"               ),
    E_MD2Animation(  84,  94, "SALUTE"             ),
    E_MD2Animation(  95, 111, "FALLBACK"           ),
    E_MD2Animation( 112, 122, "WAVE"               ),
    E_MD2Animation( 123, 134, "POINT"              ),
    E_MD2Animation( 135, 153, "CROUCH_STAND"       ),
    E_MD2Animation( 154, 159, "CROUCH_WALK"        ),
    E_MD2Animation( 160, 168, "CROUCH_ATTACK"      ),
    E_MD2Animation( 196, 172, "CROUCH_PAIN"        ),
    E_MD2Animation( 173, 177, "CROUCH_DEATH"       ),
    E_MD2Animation( 178, 183, "DEATH_FALLBACK"     ),
    E_MD2Animation( 184, 189, "DEATH_FALLFORWARD"  ),
    E_MD2Animation( 190, 197, "DEATH_FALLBACKSLOW" ),
    E_MD2Animation( 198, 198, "BOOM"               ),
};
