/*****************************************************************************
 * ==> QR_Starfield ---------------------------------------------------------*
 * ***************************************************************************
 * Description : Starfield class                                             *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_Starfield.h"
#include <memory>
#include <cstring>

//------------------------------------------------------------------------------
// QR_3DObject
//------------------------------------------------------------------------------
const GLfloat QR_3DObject::m_Vertices[] =
{
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f,
};
//------------------------------------------------------------------------------
GLubyte QR_3DObject::m_Colors[] =
{
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};
//------------------------------------------------------------------------------
// QR_Starfield
//------------------------------------------------------------------------------
QR_Starfield::QR_Starfield()
{
    m_pBackgroundColor = NULL;
    m_pStarsColor      = NULL;
    m_Angle            = 0.0f;
    m_SpeedVelocity    = 20.0f;
    m_RotateVelocity   = 0.0f;
    m_pBackgroundColor = new QR_Color();
    m_pStarsColor      = new QR_Color();
}
//------------------------------------------------------------------------------
QR_Starfield::~QR_Starfield()
{
    if (m_pBackgroundColor)
        delete m_pBackgroundColor;

    if (m_pStarsColor)
        delete m_pStarsColor;
}
//------------------------------------------------------------------------------
void QR_Starfield::SetBackgroundColor(const QR_Color& color)
{
    if (m_pBackgroundColor)
        *m_pBackgroundColor = color;
}
//------------------------------------------------------------------------------
void QR_Starfield::SetStarsColor(const QR_Color& color)
{
    if (m_pStarsColor)
        *m_pStarsColor = color;
}
//------------------------------------------------------------------------------
void QR_Starfield::SetSpeedVelocity(float value)
{
    m_SpeedVelocity = value;
}
//------------------------------------------------------------------------------
void QR_Starfield::SetRotateVelocity(float value)
{
    m_RotateVelocity = value;
}
//------------------------------------------------------------------------------
void QR_Starfield::InitializeStars(unsigned nbStars, const QR_Screen& screen)
{
    // initialize starfield
    for (unsigned i = 0; i < nbStars; ++i)
    {
        QR_Star star;

        unsigned randX = QR_Random::GetNumber(screen.m_Width);
        unsigned randY = QR_Random::GetNumber(screen.m_Height);

        bool invertX = QR_Random::GetNumber(2);
        bool invertY = QR_Random::GetNumber(2);

        QR_Vector3D position;
        position.m_X = invertX ? ((float)randX / 2) : -((float)randX / 2);
        position.m_Y = invertY ? ((float)randY / 2) : -((float)randY / 2);
        position.m_Z = QR_Random::GetNumber((int)M_StarDepth);
        star.SetPosition(position);

        m_Starfield.push_back(star);
    }
}
//------------------------------------------------------------------------------
void QR_Starfield::Render()
{
    // iterate through all stars
    for (unsigned i = 0; i < m_Starfield.size(); ++i)
    {
        // get current 3D position
        QR_Vector3D curPosition;
        curPosition = m_Starfield[i].GetPosition();

        // check if z position is out of bounds
        if (curPosition.m_Z > 1.0f)
            // change star position
            curPosition.m_Z -= m_SpeedVelocity;
        else
            // reinitialize star position
            curPosition.m_Z = M_StarDepth;

        // set new star position
        m_Starfield[i].SetPosition(curPosition);

        // get new star position on screen
        QR_Vector2D position = m_Starfield[i].GetPosition(M_StarDepth);

        // calculate star color
        CalculateColor(curPosition.m_Z);

        // set object in the world
        glLoadIdentity();
        glRotatef(m_Angle, 0.0f, 0.0f, 1.0f);
        glTranslatef(position.m_X, position.m_Y, -curPosition.m_Z);

        m_Angle -= m_RotateVelocity;

        if (m_Angle > 360.0f)
            m_Angle -= 360.0f;
        else
        if (m_Angle < -360.0f)
            m_Angle += 360.0f;

        // set color buffer
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, QR_3DObject::m_Colors);
        glEnableClientState(GL_COLOR_ARRAY);

        // set vertex buffer
        glVertexPointer(2, GL_FLOAT, 0, QR_3DObject::m_Vertices);
        glEnableClientState(GL_VERTEX_ARRAY);

        // draw object
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}
//------------------------------------------------------------------------------
void QR_Starfield::CalculateColor(float deep)
{
    int           maxR = m_pStarsColor->GetRed() - m_pBackgroundColor->GetRed();
    unsigned char r    = (unsigned char)(m_pBackgroundColor->GetRed() + (maxR - ((deep * maxR) / (unsigned)M_StarDepth)));

    int           maxG = m_pStarsColor->GetGreen() - m_pBackgroundColor->GetGreen();
    unsigned char g    = (unsigned char)(m_pBackgroundColor->GetGreen() + (maxG - ((deep * maxG) / (unsigned)M_StarDepth)));

    int           maxB = m_pStarsColor->GetBlue() - m_pBackgroundColor->GetBlue();
    unsigned char b    = (unsigned char)(m_pBackgroundColor->GetBlue() + (maxB - ((deep * maxB) / (unsigned)M_StarDepth)));

    // apply value to all colors components composing the star
    for (unsigned i = 0; i < 4; ++i)
    {
        QR_3DObject::m_Colors[(i * 4)]     = r;
        QR_3DObject::m_Colors[(i * 4) + 1] = g;
        QR_3DObject::m_Colors[(i * 4) + 2] = b;
        QR_3DObject::m_Colors[(i * 4) + 3] = 255;
    }
}
//------------------------------------------------------------------------------
