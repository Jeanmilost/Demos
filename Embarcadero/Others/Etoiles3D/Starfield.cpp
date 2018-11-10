#include "Starfield.h"
#include <memory>

//------------------------------------------------------------------------------
Starfield::Starfield()
{
    // initialize structures data
    std::memset(&m_Position, 0, sizeof(m_Position));
    std::memset(&m_RasterizedPos, 0, sizeof(m_RasterizedPos));
}
//------------------------------------------------------------------------------
Starfield::~Starfield()
{}
//------------------------------------------------------------------------------
const Starfield::IVector2D& Starfield::GetPosition(const IResolution& resolution,
        float focale, bool& isVisible)
{
    // check if focale value is correct
    if (focale < 1.0f)
        throw "Focale value cannot be little than 1";

    // convert 3D pixel to 2D coordinate
    Rasterize(focale, m_Position, m_RasterizedPos);
    isVisible = ApplyCorrection(resolution, m_RasterizedPos, m_RasterizedPos);

    return m_RasterizedPos;
}
//------------------------------------------------------------------------------
const Starfield::IVector3D& Starfield::GetPosition()
{
    return m_Position;
}
//------------------------------------------------------------------------------
void Starfield::SetPosition(const IVector3D& value)
{
    m_Position = value;
}
//------------------------------------------------------------------------------
void Starfield::Rasterize(float focale, const IVector3D& position, IVector2D& result)
{
    // rasterize 3D vector to 2D vector
    result.m_X = focale * (position.m_X / (position.m_Z + focale));
    result.m_Y = focale * (position.m_Y / (position.m_Z + focale));
}
//------------------------------------------------------------------------------
bool Starfield::ApplyCorrection(const IResolution& resolution, const IVector2D& pixel,
        IVector2D& result)
{
    // check if resolution is valid
    if (resolution.m_X < 1 || resolution.m_Y < 1)
        return false;

    // calculate center of screen
    IVector2D centerScreen;
    centerScreen.m_X = (float)resolution.m_X / 2.0f;
    centerScreen.m_Y = (float)resolution.m_Y / 2.0f;

    // calculate rasterized position from center of screen
    result.m_X = centerScreen.m_X + pixel.m_X;
    result.m_Y = centerScreen.m_Y + pixel.m_Y;

    // check if pixel is not out of bounds
    if (result.m_X < 0.0f || result.m_Y < 0.0f || result.m_X > (float)resolution.m_X
            || result.m_Y > (float)resolution.m_Y)
        return false;

    return true;
}
//------------------------------------------------------------------------------
unsigned Starfield::GetLuminance(float z, float deep)
{
    const unsigned char component = ((z * 0xff) / deep);

    return (0xffffff - ((component << 16) + (component << 8) + component));
}
//------------------------------------------------------------------------------
