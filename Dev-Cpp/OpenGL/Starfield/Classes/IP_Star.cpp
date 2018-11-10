/*****************************************************************************
 * ==> IP_Star --------------------------------------------------------------*
 * ***************************************************************************
 * Description : Star and starfield objects                                  *
 * Developper  : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "IP_Star.h"

// std
#include <memory>
#include <cstring>

//------------------------------------------------------------------------------
// IP_Star
//------------------------------------------------------------------------------
IP_Star::IP_Star()
{
    // initialize structures data
    std::memset(&m_Position, 0, sizeof(m_Position));
    std::memset(&m_RasterizedPos, 0, sizeof(m_RasterizedPos));
}
//------------------------------------------------------------------------------
IP_Star::~IP_Star()
{}
//------------------------------------------------------------------------------
const IP_Star::IVector2D& IP_Star::GetPosition(const IResolution& resolution,
        float focale)
{
    // check if focale value is correct
    if (focale < 1.0f)
        throw "Focale value cannot be little than 1";

    // convert 3D pixel to 2D coordinate
    Rasterize(focale, m_Position, m_RasterizedPos);

    return m_RasterizedPos;
}
//------------------------------------------------------------------------------
const IP_Star::IVector3D& IP_Star::GetPosition()
{
    return m_Position;
}
//------------------------------------------------------------------------------
void IP_Star::SetPosition(const IVector3D& value)
{
    m_Position = value;
}
//------------------------------------------------------------------------------
void IP_Star::Rasterize(float focale, const IVector3D& position, IVector2D& result)
{
    //         screen                             3D starfield
    //                                                  /|
    //                                                 / |
    //           /|                                   /  |
    //          / | focale                           /   | focale + z of 3D point
    //         /  | (or depht field)                /    |
    //        /___|                                /_____|
    // x in 2D coordinate                    x in 3D coordinate
    //
    // using this proportion, it's possible to resolve the z value of the 3D
    // position, and thus convert 3D point to 2D coordinate

    // rasterize 3D vector to 2D vector
    result.m_X = focale * (position.m_X / (position.m_Z + focale));
    result.m_Y = focale * (position.m_Y / (position.m_Z + focale));
}
//------------------------------------------------------------------------------
// IP_3DObject
//------------------------------------------------------------------------------
const GLfloat IP_3DObject::m_Vertices[] =
{
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f,
};
//------------------------------------------------------------------------------
GLubyte IP_3DObject::m_Colors[] =
{
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};
//------------------------------------------------------------------------------
// IP_Starfield
//------------------------------------------------------------------------------
IP_Starfield::IP_Starfield()
{
    m_pBackgroundColor = NULL;
    m_pStarsColor      = NULL;
    m_Angle            = 0.0f;
    m_SpeedVelocity    = 20.0f;
    m_RotateVelocity   = 0.0f;
    //m_pRandomGen       = [[IP_Random alloc]init];
    m_pBackgroundColor = new IP_Color();
    m_pStarsColor      = new IP_Color();
}
//------------------------------------------------------------------------------
IP_Starfield::~IP_Starfield()
{
    /*REM
	[m_pRandomGen release];
	
*/
    if (m_pBackgroundColor)
        delete m_pBackgroundColor;

    if (m_pStarsColor)
        delete m_pStarsColor;
}
//------------------------------------------------------------------------------
void IP_Starfield::SetBackgroundColor(unsigned char r, unsigned char g, unsigned char b)
{
    if (m_pBackgroundColor)
    {
        m_pBackgroundColor->m_R = r;
        m_pBackgroundColor->m_G = g;
        m_pBackgroundColor->m_B = b;
    }
}
//------------------------------------------------------------------------------
void IP_Starfield::SetStarsColor(unsigned char r, unsigned char g, unsigned char b)
{
    if (m_pStarsColor)
    {
        m_pStarsColor->m_R = r;
        m_pStarsColor->m_G = g;
        m_pStarsColor->m_B = b;
    }
}
//------------------------------------------------------------------------------
void IP_Starfield::SetSpeedVelocity(float value)
{
    m_SpeedVelocity = value;
}
//------------------------------------------------------------------------------
void IP_Starfield::SetRotateVelocity(float value)
{
    m_RotateVelocity = value;
}
//------------------------------------------------------------------------------
void IP_Starfield::InitializeStars(unsigned nbStars, unsigned screenWidth, unsigned screenHeight)
{
    // initialize starfield
    for (unsigned i = 0; i < nbStars; ++i)
    {
        IP_Star star;

        unsigned randX = IP_Random::GetNumber(screenWidth);
        unsigned randY = IP_Random::GetNumber(screenHeight);

        bool invertX = IP_Random::GetNumber(2);
        bool invertY = IP_Random::GetNumber(2);

        IP_Star::IVector3D position;
        position.m_X = invertX ? ((float)randX / 2) : -((float)randX / 2);
        position.m_Y = invertY ? ((float)randY / 2) : -((float)randY / 2);
        position.m_Z = IP_Random::GetNumber((int)M_StarDepth);
        star.SetPosition(position);

        m_Starfield.push_back(star);
    }
}
//------------------------------------------------------------------------------
void IP_Starfield::Render(unsigned screenWidth, unsigned screenHeight)
{
    // get current screen resolution
    IP_Star::IResolution resolution;
    resolution.m_X = screenWidth;
    resolution.m_Y = screenHeight;

    // iterate through all stars
    for (unsigned i = 0; i < m_Starfield.size(); ++i)
    {
        // get current 3D position
        IP_Star::IVector3D curPosition;
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
        IP_Star::IVector2D position = m_Starfield[i].GetPosition(resolution, M_StarDepth);

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
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, IP_3DObject::m_Colors);
        glEnableClientState(GL_COLOR_ARRAY);

        // set vertex buffer
        glVertexPointer(2, GL_FLOAT, 0, IP_3DObject::m_Vertices);
        glEnableClientState(GL_VERTEX_ARRAY);

        // draw object
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}
//------------------------------------------------------------------------------
void IP_Starfield::CalculateColor(float deep)
{
    int           maxR = m_pStarsColor->m_R - m_pBackgroundColor->m_R;
    unsigned char r    = (unsigned char)(m_pBackgroundColor->m_R + (maxR - ((deep * maxR) / (unsigned)M_StarDepth)));

    int           maxG = m_pStarsColor->m_G - m_pBackgroundColor->m_G;
    unsigned char g    = (unsigned char)(m_pBackgroundColor->m_G + (maxG - ((deep * maxG) / (unsigned)M_StarDepth)));

    int           maxB = m_pStarsColor->m_B - m_pBackgroundColor->m_B;
    unsigned char b    = (unsigned char)(m_pBackgroundColor->m_B + (maxB - ((deep * maxB) / (unsigned)M_StarDepth)));

    // apply value to all colors components composing the star
    for (unsigned i = 0; i < 4; ++i)
    {
        IP_3DObject::m_Colors[(i * 4)]     = r;
        IP_3DObject::m_Colors[(i * 4) + 1] = g;
        IP_3DObject::m_Colors[(i * 4) + 2] = b;
        IP_3DObject::m_Colors[(i * 4) + 3] = 255;
    }
}
//------------------------------------------------------------------------------
