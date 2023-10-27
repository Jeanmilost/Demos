/****************************************************************************
 * ==> DWF_ModelFactory ----------------------------------------------------*
 ****************************************************************************
 * Description: Model factory                                               *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - DwarfStar Game Engine                                      *
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
#include "DWF_Model.h"

namespace DWF_Model
{
    /**
    * Model factory, creates simple but useful models
    *@author Jean-Milost Reymond
    */
    class Factory
    {
        public:
            /**
            * Gets a surface model
            *@param width - surface width (on the x axis)
            *@param height - surface height (on the y axis)
            *@param format - the mesh format to use
            *@param culling - the mesh culling to use
            *@param material - the mesh material to use
            *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
            *@return model, nullptr on error
            */
            static Model* GetSurface(float                             width,
                                     float                             height,
                               const VertexFormat&                     format,
                               const VertexCulling&                    culling,
                               const Material&                         material,
                               const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

            /**
            * Gets a box model
            *@param width - box width (on the x axis)
            *@param height - box height (on the y axis)
            *@param depth - box depth (on the z axis)
            *@param repeatTexOnEachFace - if true, the texture will be repeated on each face
            *@param format - the mesh format to use
            *@param culling - the mesh culling to use
            *@param material - the mesh material to use
            *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
            *@return model, nullptr on error
            */
            static Model* GetBox(float                             width,
                                 float                             height,
                                 float                             depth,
                                 bool                              repeatTexOnEachFace,
                           const VertexFormat&                     format,
                           const VertexCulling&                    culling,
                           const Material&                         material,
                           const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

            /**
            * Gets a sphere model
            *@param radius - sphere radius
            *@param slices - slices (longitude) count
            *@param stacks - stacks (latitude) count
            *@param format - the mesh format to use
            *@param culling - the mesh culling to use
            *@param material - the mesh material to use
            *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
            *@return model, nullptr on error
            */
            static Model* GetSphere(float                             radius,
                                    std::size_t                       slices,
                                    std::size_t                       stacks,
                              const VertexFormat&                     format,
                              const VertexCulling&                    culling,
                              const Material&                         material,
                              const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

            /**
            * Gets a cylinder model
            *@param minRadius - minimum cylinder radius
            *@param maxRadius - maximum cylinder radius
            *@param height - cylinder height
            *@param faces - number of faces composing the cylinder
            *@param format - the mesh format to use
            *@param culling - the mesh culling to use
            *@param material - the mesh material to use
            *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
            *@return model, nullptr on error
            */
            static Model* GetCylinder(float                             minRadius,
                                      float                             maxRadius,
                                      float                             height,
                                      std::size_t                       faces,
                                const VertexFormat&                     format,
                                const VertexCulling&                    culling,
                                const Material&                         material,
                                const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

            /**
            * Gets a capsule model
            *@param height - the capsule height
            *@param radius - the capsule radius
            *@param resolution - the capsule resolution (or slices)
            *@param format - the mesh format to use
            *@param culling - the mesh culling to use
            *@param material - the mesh material to use
            *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
            *@return model, nullptr on error
            */
            static Model* GetCapsule(float                             height,
                                     float                             radius,
                                     float                             resolution,
                               const VertexFormat&                     format,
                               const VertexCulling&                    culling,
                               const Material&                         material,
                               const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

            /**
            * Gets a disk model
            *@param centerX - the disk center on the x axis
            *@param centerY - the disk center on the y axis
            *@param radius - disk radius
            *@param slices - disk slice count
            *@param format - the mesh format to use
            *@param culling - the mesh culling to use
            *@param material - the mesh material to use
            *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
            *@return model, nullptr on error
            */
            static Model* GetDisk(float                             centerX,
                                  float                             centerY,
                                  float                             radius,
                                  std::size_t                       slices,
                            const VertexFormat&                     format,
                            const VertexCulling&                    culling,
                            const Material&                         material,
                            const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

            /**
            * Gets a ring model
            *@param centerX - the ring center on the x axis
            *@param centerY - the ring center on the y axis
            *@param minRadius - internal radius
            *@param maxRadius - external radius
            *@param slices - slice count
            *@param format - the mesh format to use
            *@param culling - the mesh culling to use
            *@param material - the mesh material to use
            *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
            *@return model, nullptr on error
            */
            static Model* GetRing(float                             centerX,
                                  float                             centerY,
                                  float                             minRadius,
                                  float                             maxRadius,
                                  std::size_t                       slices,
                            const VertexFormat&                     format,
                            const VertexCulling&                    culling,
                            const Material&                         material,
                            const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

            /**
            * Gets a torus model
            *@param centerX - the ring center on the x axis
            *@param centerY - the ring center on the y axis
            *@param innerRadius - inner radius
            *@param outerRadius - outer radius
            *@param slices - slice count
            *@param facesPerSlices - faces per slices count
            *@param format - the mesh format to use
            *@param culling - the mesh culling to use
            *@param material - the mesh material to use
            *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
            *@return model, nullptr on error
            */
            static Model* GetTorus(float                             centerX,
                                   float                             centerY,
                             const DWF_Math::Vector2F&               innerRadius,
                             const DWF_Math::Vector2F&               outerRadius,
                                   std::size_t                       slices,
                                   std::size_t                       facesPerSlices,
                             const VertexFormat&                     format,
                             const VertexCulling&                    culling,
                             const Material&                         material,
                             const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

            /**
            * Gets a spiral model
            *@param centerX - the spiral center on the x axis
            *@param centerY - the spiral center on the y axis
            *@param minRadius - internal radius
            *@param maxRadius - external radius
            *@param deltaMin - delta to apply to the min radius
            *@param deltaMax - delta to apply to the max radius
            *@param deltaZ - delta to apply to the z radius
            *@param slices - slice count
            *@param stacks - stack count
            *@param format - the mesh format to use
            *@param culling - the mesh culling to use
            *@param material - the mesh material to use
            *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
            *@return model, nullptr on error
            */
            static Model* GetSpiral(float                             centerX,
                                    float                             centerY,
                                    float                             minRadius,
                                    float                             maxRadius,
                                    float                             deltaMin,
                                    float                             deltaMax,
                                    float                             deltaZ,
                                    std::size_t                       slices,
                                    std::size_t                       stacks,
                              const VertexFormat&                     format,
                              const VertexCulling&                    culling,
                              const Material&                         material,
                              const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor = nullptr);

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
            *@return vertex
            */
            static DWF_Math::Vector3F GetCylinderVertex(const DWF_Math::Vector3F& top,
                                                        const DWF_Math::Vector3F& localX,
                                                        const DWF_Math::Vector3F& localY,
                                                        const DWF_Math::Vector3F& localZ,
                                                              float               radius,
                                                              float               length,
                                                              float               u,
                                                              float               v);

            /**
            * Gets a vertex belonging to the top half sphere part of the capsule
            *@param top - capsule top position
            *@param localX - current x position on the local axis
            *@param localY - current y position on the local axis
            *@param localZ - current z position on the local axis
            *@param radius - capsule radius
            *@param u - texture u coordinate
            *@param v - texture v coordinate
            *@return vertex
            */
            static DWF_Math::Vector3F GetSphereStartVertex(const DWF_Math::Vector3F& top,
                                                           const DWF_Math::Vector3F& localX,
                                                           const DWF_Math::Vector3F& localY,
                                                           const DWF_Math::Vector3F& localZ,
                                                                 float               radius,
                                                                 float               u,
                                                                 float               v);

            /**
            * Gets a vertex belonging to the bottom half sphere part of the capsule
            *@param top - capsule top position
            *@param localX - current x position on the local axis
            *@param localY - current y position on the local axis
            *@param localZ - current z position on the local axis
            *@param radius - capsule radius
            *@param u - texture u coordinate
            *@param v - texture v coordinate
            *@return vertex
            */
            static DWF_Math::Vector3F GetSphereEndVertex(const DWF_Math::Vector3F& top,
                                                         const DWF_Math::Vector3F& localX,
                                                         const DWF_Math::Vector3F& localY,
                                                         const DWF_Math::Vector3F& localZ,
                                                               float               radius,
                                                               float               u,
                                                               float               v);

            /**
            * Gets any perpendicular unit vector from a vector
            *@param vec - vector
            *@return perpendicular unit vector
            */
            static DWF_Math::Vector3F GetAnyPerpendicularUnitVector(const DWF_Math::Vector3F& vec);
    };
}
