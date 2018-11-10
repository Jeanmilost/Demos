/*****************************************************************************
 * ==> QR_Camera ------------------------------------------------------------*
 *****************************************************************************
 * Description : Camera (or point of view)                                   *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_CameraH
#define QR_CameraH

// qr engine
#include "QR_Vector3D.h"

/**
* Camera (or point of view)
*@note This class is deprecated since OpenGL implements shader
*@author Jean-Milost Reymond
*/
class QR_Camera
{
    public:
        QR_Camera();
        virtual ~QR_Camera();

        /**
        * Initializes the camera (in orthogonal mode)
        *@param width - display window width
        *@param height - display windows height
        *@param deep - deep for visibility
        *@note The class must be reinitialized if the window orientation changes
        */
        virtual void Initialize(const float& width, const float& height,
                const float& deep);

        /**
        * Initializes the camera (projection mode)
        *@param width - display window width
        *@param height - display windows height
        *@param near - minimum (near) distance for visibility
        *@param far - maximum (far) distance for visibility
        *@note The class must be reinitialized if the window orientation changes
        */
        virtual void Initialize(const float& width, const float& height,
                const float& near, const float& far);

        /**
        * Sets the camera in the 3D world (orthogonal mode)
        */
        virtual void SetCamera();

        /**
        * Sets the camera in the 3D world (projection mode)
        *@param translation - translation vector
        *@param rotation - rotation vector
        *@param angle - rotation angle
        */
        virtual void SetCamera(const QR_Vector3D& translation,
                const QR_Vector3D& rotation, const float& angle);

        /**
        * Creates orthogonal view point
        *@param width - view point width
        *@param height - view point height
        *@param deep - view point deep
        */
        //REM static void CreateOrtho(float width, float height, float deep);

    private:
        float m_WindowsWidth;  // display windows width
        float m_WindowsHeight; // display windows height
        float m_Near;          // near visibility distance (projection only)
        float m_Far;           // far visibility distance (projection only)
        float m_Deep;          // display deep (ortho only)

        /**
        * Creates an orthogonal view matrix
        */
        void CreateOrtho();

        /**
        * Creates a projection view matrix
        */
        void CreateProjection();
};
#endif
