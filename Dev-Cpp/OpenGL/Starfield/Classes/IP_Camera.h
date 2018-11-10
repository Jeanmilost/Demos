/*****************************************************************************
 * ==> IP_Camera ------------------------------------------------------------*
 * ***************************************************************************
 * Description : Camera or view point object                                 *
 * Developper  : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef IP_CameraH
#define IP_CameraH

class IP_Camera
{
    public:
        /**
        * Creates orthogonal view point
        *@param width - view point width
        *@param height - view point height
        *@param deep - view point deep
        */
        static void CreateOrtho(float width, float height, float deep);
};
#endif
