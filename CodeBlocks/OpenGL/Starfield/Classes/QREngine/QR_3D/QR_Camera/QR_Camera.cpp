/*****************************************************************************
 * ==> QR_Camera ------------------------------------------------------------*
 *****************************************************************************
 * Description : Camera (or point of view)                                   *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_Camera.h"

// std
#define _USE_MATH_DEFINES
#include <math.h>

// openGL
#include <gl/gl.h>

//------------------------------------------------------------------------------
// Global defines
//------------------------------------------------------------------------------
#define glOrthof glOrtho
//------------------------------------------------------------------------------
// QR_Camera
//------------------------------------------------------------------------------
QR_Camera::QR_Camera()
{}
//------------------------------------------------------------------------------
QR_Camera::~QR_Camera()
{}
//------------------------------------------------------------------------------
void QR_Camera::Initialize(const float& width, const float& height,
        const float& deep)
{
    m_WindowsWidth  = width;
    m_WindowsHeight = height;
    m_Deep          = deep;
}
//------------------------------------------------------------------------------
void QR_Camera::Initialize(const float& width, const float& height,
        const float& near, const float& far)
{
    m_WindowsWidth  = width;
    m_WindowsHeight = height;
    m_Near          = near;
    m_Far           = far;
}
//------------------------------------------------------------------------------
void QR_Camera::SetCamera()
{
    CreateOrtho();
}
//------------------------------------------------------------------------------
void QR_Camera::SetCamera(const QR_Vector3D& translation,
        const QR_Vector3D& rotation, const float& angle)
{
    CreateProjection();

    if (angle != 0.0f && (rotation.m_X != 0.0f || rotation.m_Y != 0.0f || rotation.m_Z != 0.0f))
        glRotatef(angle, rotation.m_X, rotation.m_Y, rotation.m_Z);

    glTranslatef(translation.m_X, translation.m_Y, translation.m_Z);
}
//------------------------------------------------------------------------------
void QR_Camera::CreateOrtho()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float minX = -m_WindowsWidth  / 2.0f;
    float maxX =  m_WindowsWidth  / 2.0f;
    float minY = -m_WindowsHeight / 2.0f;
    float maxY =  m_WindowsHeight / 2.0f;

    glOrthof(minX, maxX, minY, maxY, 0.0f, m_Deep);
}
//------------------------------------------------------------------------------
void QR_Camera::CreateProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float fov    = 45;
    float aspect = m_WindowsWidth / m_WindowsHeight;
    float top    = tanf(((fov * (2 * M_PI)) / 360.0f)) * m_Near;
    float bottom = -top;
    float left   = aspect * bottom;
    float right  = aspect * top;

    #ifdef __APPLE__
        glFrustumf(left, right, bottom, top, m_Near, m_Far);
    #else
        glFrustum(left, right, bottom, top, m_Near, m_Far);
    #endif
}
//------------------------------------------------------------------------------
