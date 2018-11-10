/*****************************************************************************
 * ==> QR_Star --------------------------------------------------------------*
 *****************************************************************************
 * Description : Class to create and manage a star                           *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_Star.h"
#include <memory>
#include <cstring>

//------------------------------------------------------------------------------
// class QR_Star - c++
//------------------------------------------------------------------------------
QR_Star::QR_Star()
{
    // initialize data
    std::memset(&m_Position, 0, sizeof(m_Position));
    std::memset(&m_RasterizedPos, 0, sizeof(m_RasterizedPos));
}
//------------------------------------------------------------------------------
QR_Star::~QR_Star()
{}
//------------------------------------------------------------------------------
const QR_Vector2D& QR_Star::GetPosition(float focal)
{
    // check if focal value is correct
    if (focal < 1.0f)
        throw "Focal value cannot be little than 1";

    // convert 3D vector to 2D vector
    Rasterize(focal, m_Position, m_RasterizedPos);

    return m_RasterizedPos;
}
//------------------------------------------------------------------------------
const QR_Vector3D& QR_Star::GetPosition()
{
    return m_Position;
}
//------------------------------------------------------------------------------
void QR_Star::SetPosition(const QR_Vector3D& value)
{
    m_Position = value;
}
//------------------------------------------------------------------------------
void QR_Star::Rasterize(float focal, const QR_Vector3D& position, QR_Vector2D& result)
{
    // convert 3D vector to 2D vector (it's a kind of raster)
    result.m_X = focal * (position.m_X / (position.m_Z + focal));
    result.m_Y = focal * (position.m_Y / (position.m_Z + focal));
}
//------------------------------------------------------------------------------
