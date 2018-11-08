/****************************************************************************
 * ==> Rasterizer ----------------------------------------------------------*
 ****************************************************************************
 * Description : This module provides a simple rasterizer                   *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************/

#include "Rasterizer.h"

// std
#include <memory>

#define PERSP_CORRECT

//---------------------------------------------------------------------------
// Raster::Vertex
//---------------------------------------------------------------------------
Raster::Vertex::Vertex() :
    m_Red(0.0f),
    m_Green(0.0f),
    m_Blue(0.0f)
{}
//---------------------------------------------------------------------------
Raster::Vertex::Vertex(const Vector3DF& position, float red, float green, float blue) :
    m_Position(position),
    m_Red(red),
    m_Green(green),
    m_Blue(blue)
{}
//---------------------------------------------------------------------------
Raster::Vertex::~Vertex()
{}
//---------------------------------------------------------------------------
// Raster::Frame
//---------------------------------------------------------------------------
Raster::Frame::Frame() :
    m_pPixels(NULL),
    m_pZBuffer(NULL),
    m_Width(0),
    m_Height(0)
{}
//---------------------------------------------------------------------------
Raster::Frame::Frame(std::size_t width, std::size_t height, bool useZBuffer, float zFar) :
    m_pPixels(NULL),
    m_pZBuffer(NULL),
    m_Width(width),
    m_Height(height)
{
    // create a new pixel bufer and clear it
    m_pPixels = new Pixel[width * height];
    std::memset(m_pPixels, 0x0, m_Width * m_Height * sizeof(Pixel));

    // do use a z buffer?
    if (useZBuffer)
    {
        // calculate the z buffer size
        const std::size_t size = width * height;

        // create it
        m_pZBuffer = new float[size];

        // initialize it with the z far value
        for (std::size_t i = 0; i < size; ++i)
            m_pZBuffer[i] = zFar;
    }
}
//---------------------------------------------------------------------------
Raster::Frame::~Frame()
{
    if (m_pPixels)
        delete[] m_pPixels;

    if (m_pZBuffer)
        delete[] m_pZBuffer;
}
//---------------------------------------------------------------------------
// Raster
//---------------------------------------------------------------------------
void Raster::Rasterize(const Vertex& vertex1,
                       const Vertex& vertex2,
                       const Vertex& vertex3,
                             Frame*  pFrame)
{
    if (!pFrame)
        return;

    if (!pFrame->m_Width || !pFrame->m_Height)
        return;

    if (!pFrame->m_pPixels)
        return;

    Vertex v1 = vertex1;
    Vertex v2 = vertex2;
    Vertex v3 = vertex3;

    // project triangle onto the screen
    v1.m_Position.m_X /= v1.m_Position.m_Z;
    v1.m_Position.m_Y /= v1.m_Position.m_Z;
    v2.m_Position.m_X /= v2.m_Position.m_Z;
    v2.m_Position.m_Y /= v2.m_Position.m_Z;
    v3.m_Position.m_X /= v3.m_Position.m_Z;
    v3.m_Position.m_Y /= v3.m_Position.m_Z;

    // convert from screen space to raster space
    v1.m_Position.m_X = (1.0f + v1.m_Position.m_X) * 0.5f * pFrame->m_Width;
    v1.m_Position.m_Y = (1.0f + v1.m_Position.m_Y) * 0.5f * pFrame->m_Height;
    v2.m_Position.m_X = (1.0f + v2.m_Position.m_X) * 0.5f * pFrame->m_Width;
    v2.m_Position.m_Y = (1.0f + v2.m_Position.m_Y) * 0.5f * pFrame->m_Height;
    v3.m_Position.m_X = (1.0f + v3.m_Position.m_X) * 0.5f * pFrame->m_Width;
    v3.m_Position.m_Y = (1.0f + v3.m_Position.m_Y) * 0.5f * pFrame->m_Height;

    #ifdef PERSP_CORRECT
        // divide each vertex attribute by the vertex z coordinate
        v1.m_Red   /= v1.m_Position.m_Z;
        v1.m_Green /= v1.m_Position.m_Z;
        v1.m_Blue  /= v1.m_Position.m_Z;
        v2.m_Red   /= v2.m_Position.m_Z;
        v2.m_Green /= v2.m_Position.m_Z;
        v2.m_Blue  /= v2.m_Position.m_Z;
        v3.m_Red   /= v3.m_Position.m_Z;
        v3.m_Green /= v3.m_Position.m_Z;
        v3.m_Blue  /= v3.m_Position.m_Z;

        // pre-compute 1 over z
        v1.m_Position.m_Z = 1.0f / v1.m_Position.m_Z;
        v2.m_Position.m_Z = 1.0f / v2.m_Position.m_Z;
        v3.m_Position.m_Z = 1.0f / v3.m_Position.m_Z;
    #endif

    const float area = FindEdge(v1.m_Position, v2.m_Position, v3.m_Position);

    // iterate through the frame pixels to draw
    for (std::size_t y = 0; y < pFrame->m_Height; ++y)
        for (std::size_t x = 0; x < pFrame->m_Width; ++x)
        {
            // calculate the next pixel physical position on the frame
            Vector3DF p(x + 0.5f, pFrame->m_Height - y + 0.5f, 0.0f);

            // check if pixel is inside the triangle limits to draw
            float w1 = FindEdge(v2.m_Position, v3.m_Position, p);
            float w2 = FindEdge(v3.m_Position, v1.m_Position, p);
            float w3 = FindEdge(v1.m_Position, v2.m_Position, p);

            // is pixel inside the triangle?
            if (w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f)
            {
                w1 /= area;
                w2 /= area;
                w3 /= area;

                // calculate the z magnitude
                const float z = 1.0f / (w1 * v1.m_Position.m_Z +
                                        w2 * v2.m_Position.m_Z +
                                        w3 * v3.m_Position.m_Z);

                // if the depth buffer is available, test the pixel against it
                if (!pFrame->m_pZBuffer || z < pFrame->m_pZBuffer[y * pFrame->m_Width + x])
                {
                    // test passed, update the depth buffer
                    if (pFrame->m_pZBuffer)
                        pFrame->m_pZBuffer[y * pFrame->m_Width + x] = z;

                    // calculate the interpolated pixel color
                    float r = w1 * v1.m_Red   + w2 * v2.m_Red   + w3 * v3.m_Red;
                    float g = w1 * v1.m_Green + w2 * v2.m_Green + w3 * v3.m_Green;
                    float b = w1 * v1.m_Blue  + w2 * v2.m_Blue  + w3 * v3.m_Blue;

                    #ifdef PERSP_CORRECT
                        // in case perspective interpolation correction is used, the result of this
                        // interpolation should be multiplied by z (the point depth on the 3d triangle
                        // that the pixel overlaps)
                        r *= z;
                        g *= z;
                        b *= z;
                    #endif

                    // set pixel inside the pixel buffer
                    pFrame->m_pPixels[y * pFrame->m_Width + x][0] = (unsigned char)(r * 255.0f);
                    pFrame->m_pPixels[y * pFrame->m_Width + x][1] = (unsigned char)(g * 255.0f);
                    pFrame->m_pPixels[y * pFrame->m_Width + x][2] = (unsigned char)(b * 255.0f);
                }
            }
        }
}
//---------------------------------------------------------------------------
float Raster::FindEdge(const Vector3DF& v1, const Vector3DF& v2, const Vector3DF& v3)
{
    return (v3.m_X - v1.m_X) * (v2.m_Y - v1.m_Y) - (v3.m_Y - v1.m_Y) * (v2.m_X - v1.m_X);
}
//---------------------------------------------------------------------------
