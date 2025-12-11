/****************************************************************************
 * ==> MeshFactory ---------------------------------------------------------*
 ****************************************************************************
 * Description: Mesh factory                                                *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License                                                              *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sub-license, and/or sell copies of the Software, and to      *
 * permit persons to whom the Software is furnished to do so, subject to    *
 * the following conditions:                                                *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     *
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     *
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        *
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   *
 ****************************************************************************/

#pragma once

// classes
#include "PixelBuffer.h"
#include "VertexBuffer.h"
#include "Mesh.h"

/**
* Mesh factory
*/
class MeshFactory
{
    public:
        typedef std::vector<Vector3F> IVertices;

        /**
        * Gets the box mesh
        *@param width - box width (on the x axis)
        *@param height - box height (on the y axis)
        *@param depth - box depth (on the z axis)
        *@param repeatTexOnEachFace - if true the texture will be repeated on each face
        *@param format - the mesh format to use
        *@param culling - the mesh culling to use
        *@param material - the mesh material to use
        *@param[out] mesh - the mesh to fill with box
        *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
        */
        static void GetBox(      float                             width,
                                 float                             height,
                                 float                             depth,
                                 bool                              repeatTexOnEachFace,
                           const VertexBuffer::IFormat&            format,
                           const VertexBuffer::ICulling&           culling,
                           const Material&                         material,
                                 Mesh&                             mesh,
                           const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

        /**
        * Gets a sphere mesh
        *@param radius - sphere radius
        *@param slices - slices (longitude) count
        *@param stacks - stacks (latitude) count
        *@param format - the mesh format to use
        *@param culling - the mesh culling to use
        *@param material - the mesh material to use
        *@param[out] mesh - the mesh to fill with sphere
        *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
        */
        static void GetSphere(      float                             radius,
                                    std::size_t                       slices,
                                    std::size_t                       stacks,
                              const VertexBuffer::IFormat&            format,
                              const VertexBuffer::ICulling&           culling,
                              const Material&                         material,
                                    Mesh&                             mesh,
                              const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

        /**
        * Gets a capsule mesh
        *@param height - the capsule height
        *@param radius - the capsule radius
        *@param resolution - the capsule resolution (or slices)
        *@param format - the mesh format to use
        *@param culling - the mesh culling to use
        *@param material - the mesh material to use
        *@param[out] mesh - the mesh to fill with capsule
        *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
        */
        static void GetCapsule(      float                             height,
                                     float                             radius,
                                     float                             resolution,
                               const VertexBuffer::IFormat&            format,
                               const VertexBuffer::ICulling&           culling,
                               const Material&                         material,
                                     Mesh&                             mesh,
                               const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

        /**
        * Creates a landscape mesh from a grayscale image
        *@param pPixelBuffer - pixel buffer containing the landscape map image
        *@param height - landscape height
        *@param scale - scale factor
        *@param pVertFormat - model vertex format, if 0 the default format will be used
        *@param pVertCulling - model vertex culling, if 0 the default culling will be used
        *@param pMaterial - mesh material, if 0 the default material will be used
        *@param fOnGetVertexColor - get vertex color callback function to use, 0 if not used
        *@return mesh containing the landscape, 0 on error
        *@note The mesh content should be released using the csrMeshRelease function when useless
        */
        static bool GetLandscape(const PixelBuffer*                      pPixelBuffer,
                                       float                             height,
                                       float                             scale,
                                 const VertexBuffer::IFormat&            format,
                                 const VertexBuffer::ICulling&           culling,
                                 const Material&                         material,
                                       Mesh&                             mesh,
                                 const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor);

    private:
        /**
        * Gets a vertex belonging to the cylinder part of the capsule
        *@param top - capsule top position
        *@param localX - current x position on the local axis
        *@param localY - current y position on the local axis
        *@param localZ - current z position on the local axis
        *@param radius - capsule radius
        *@param length - capsule length
        *@param u - texture u coordinate
        *@param v - texture v coordinate
        */
        static Vector3F GetCylinderVertex(const Vector3F& top,
                                          const Vector3F& localX,
                                          const Vector3F& localY,
                                          const Vector3F& localZ,
                                                float     radius,
                                                float     length,
                                                float     u,
                                                float     v);

        /**
        * Gets a vertex belonging to the top half sphere part of the capsule
        *@param top - capsule top position
        *@param localX - current x position on the local axis
        *@param localY - current y position on the local axis
        *@param localZ - current z position on the local axis
        *@param radius - capsule radius
        *@param u - texture u coordinate
        *@param v - texture v coordinate
        */
        static Vector3F GetSphereStartVertex(const Vector3F& top,
                                             const Vector3F& localX,
                                             const Vector3F& localY,
                                             const Vector3F& localZ,
                                                   float     radius,
                                                   float     u,
                                                   float     v);

        /**
        * Gets a vertex belonging to the bottom half sphere part of the capsule
        *@param top - capsule top position
        *@param localX - current x position on the local axis
        *@param localY - current y position on the local axis
        *@param localZ - current z position on the local axis
        *@param radius - capsule radius
        *@param u - texture u coordinate
        *@param v - texture v coordinate
        */
        static Vector3F GetSphereEndVertex(const Vector3F& top,
                                           const Vector3F& localX,
                                           const Vector3F& localY,
                                           const Vector3F& localZ,
                                                 float     radius,
                                                 float     u,
                                                 float     v);

        /**
        * Gets any perpendicular unit vector from a vector
        *@param vec - vector
        *@return perpendicular unit vector
        */
        static Vector3F GetAnyPerpendicularUnitVector(const Vector3F& vec);

        /**
        * Generates landscape vertices from a grayscale image
        *@param pPixelBuffer - pixel buffer containing the image
        *@param height - map height
        *@param scale - scale factor
        *@param pVertices - the buffer containing the vertices to read
        *@return true on success, otherwise false
        */
        static bool GenerateLandscapeVertices(const PixelBuffer* pPixelBuffer,
                                                    float        height,
                                                    float        scale,
                                                    IVertices&   vertices);
};
