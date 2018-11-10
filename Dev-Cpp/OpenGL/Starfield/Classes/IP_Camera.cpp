/*****************************************************************************
 * ==> IP_Camera ------------------------------------------------------------*
 * ***************************************************************************
 * Description : Camera or view point object                                 *
 * Developper  : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "IP_Camera.h"
#include <gl/gl.h>
#define glOrthof glOrtho

//------------------------------------------------------------------------------
// IP_Camera
//------------------------------------------------------------------------------
void IP_Camera::CreateOrtho(float width, float height, float deep)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float minX = -width / 2.0f;
    float maxX =  width / 2.0f;
    float minY = -height / 2.0f;
    float maxY =  height / 2.0f;

    glOrthof(minX, maxX, minY, maxY, 0.0f, deep);
}
//------------------------------------------------------------------------------
