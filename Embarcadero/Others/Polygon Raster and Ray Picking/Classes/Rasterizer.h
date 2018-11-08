/****************************************************************************
 * ==> Rasterizer ----------------------------------------------------------*
 ****************************************************************************
 * Description : This module provides a simple rasterizer                   *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************/

#ifndef RasterizerH
#define RasterizerH

// std
#include <stddef.h>

// classes
#include "Geometry.h"

/**
* Raster
*@author Jean-Milost Reymond
*/
class Raster
{
    public:
        typedef unsigned char Pixel[3];

        /**
        * Vertex
        */
        struct Vertex
        {
            Vector3DF m_Position;
            float     m_Red;
            float     m_Green;
            float     m_Blue;

            Vertex();

            /**
            * Constructor
            *@param position - vertex 3D position
            *@param red - vertex color red component
            *@param green - vertex color green component
            *@param blue - vertex color blue component
            */
            Vertex(const Vector3DF& position, float red, float green, float blue);

            virtual ~Vertex();
        };

        /**
        * Rendered frame
        */
        struct Frame
        {
            Pixel*      m_pPixels;  // pixel buffer
            float*      m_pZBuffer; // z buffer
            std::size_t m_Width;    // image width
            std::size_t m_Height;   // image height

            Frame();

            /**
            * Constructor
            *@param width - image width
            *@param height - image height
            *@param useZBuffer - if true, z buffer will be used for rendering
            *@param zFar - if z buffer is used, clipping z far limit
            */
            Frame(std::size_t width, std::size_t height, bool useZBuffer, float zFar);

            virtual ~Frame();
        };

        /**
        * Rasterizes a polygon inside a frame
        *@param vertex1 - polygon vertex 1
        *@param vertex2 - polygon vertex 2
        *@param vertex3 - polygon vertex 3
        *@param[in, out] pFrame - rendered frame
        */
        static void Rasterize(const Vertex& vertex1,
                              const Vertex& vertex2,
                              const Vertex& vertex3,
                                    Frame* pFrame);

    private:
        /**
        * Finds the edge of a polygon
        *@param v1 - polygon vertex 1
        *@param v2 - polygon vertex 2
        *@param v3 - polygon vertex 3
        *@return polygon edge value
        */
        static float FindEdge(const Vector3DF& v1, const Vector3DF& v2, const Vector3DF& v3);
};
#endif
