/****************************************************************************
 * ==> DWF_AABBTree --------------------------------------------------------*
 ****************************************************************************
 * Description: Aligned-Axis Bounding Box tree                              *
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
#include "DWF_Ray.h"
#include "DWF_Polygon.h"
#include "DWF_AABB.h"
#include "DWF_Sphere.h"
#include "DWF_Model.h"

namespace DWF_Collider
{
    /**
    * Aligned-axis bounding box tree node
    *@author Jean-Milost Reymond
    */
    struct AABB_Node
    {
        AABB_Node*             m_pParent = nullptr;
        AABB_Node*             m_pLeft   = nullptr;
        AABB_Node*             m_pRight  = nullptr;
        DWF_Geometry::AABB*    m_pBox    = nullptr;
        DWF_Geometry::Polygons m_Polygons;

        /**
        * Constructor
        *@param pParent - parent node
        */
        AABB_Node(AABB_Node* pParent);

        virtual ~AABB_Node();
    };

    /**
    * Aligned-axis bounding box tree
    *@author Jean-Milost Reymond
    */
    class AABB_Tree
    {
        public:
            AABB_Tree();
            virtual ~AABB_Tree();

            /**
            * Gets the bounding box surrounding the tree data
            *@return the bounding box, NULL if not found or on error
            */
            virtual DWF_Geometry::AABB* GetBoundingBox() const;

            /**
            * Gets the polygon count
            *@return the polygon count
            */
            virtual std::size_t GetPolygonCount() const;

            /**
            * Populates AABB tree
            *@param polygons - source polygon array
            *@return true on success, otherwise false
            */
            virtual bool Populate(const DWF_Geometry::Polygons& polygons);

            /**
            * Resolves AABB tree
            *@param pRay - ray against which tree boxes will be tested
            *@param[in, out] polygons - polygons belonging to boxes hit by ray
            *@return true on success, otherwise false
            *@note Polygon list content should be deleted when useless
            */
            virtual bool Resolve(const DWF_Geometry::RayF* pRay, DWF_Geometry::Polygons& polygons) const;

            /**
            * Gets the next position while sliding on a plane
            *@param slidingPlane - sliding plane
            *@param pos - current position
            *@param radius - radius around the current position
            *@return resulting position
            */
            virtual DWF_Math::Vector3F GetSlidingPoint(const DWF_Geometry::PlaneF& slidingPlane,
                                                       const DWF_Math::Vector3F&   pos,
                                                             float                 radius);

            /**
            * Calculates the position where a model or a point of view is placed on the ground
            *@param boundingSphere - bounding sphere surrounding the point of view or model
            *@param polygon - polygon belonging to the model showing the ground of a scene
            *@param groundDir - ground direction. If 0, a default direction of [0, -1, 0] will be used
            *@param[out] groundPos - resulting position where the bounding sphere surrounding the point of
            *                        view or model will be placed on the ground. Ignored if 0
            *@return true if the bounding sphere is above the ground polygon, otherwise false
            *@note The bounding sphere should be in the same coordinate system as the polygon. This
            *      means that any transformation should be applied to the sphere before calling this
            *      function
            */
            virtual bool GroundCollision(const DWF_Geometry::Sphere&  boundingSphere,
                                         const DWF_Geometry::Polygon& polygon,
                                         const DWF_Math::Vector3F&    groundDir,
                                               DWF_Math::Vector3F&    groundPos);

            /**
            * Calculates the y axis position where to place the point of view to stay above the ground
            *@param boundingSphere - sphere surrounding the point of view or model
            *@param groundDir - ground direction
            *@param[out] groundPolygon - polygon on which the ground was hit, ignored if 0
            *@param[out] groundY - resulting position on the y axis where to place the point of view or model
            *@return true if a ground polygon was found, otherwise false
            *@note The bounding sphere should be in the same coordinate system as the model. This means
            *      that any transformation should be applied to the sphere before calling this function
            */
            virtual bool GroundPosY(const DWF_Geometry::Sphere&  boundingSphere,
                                    const DWF_Math::Vector3F&    groundDir,
                                          DWF_Geometry::Polygon& groundPolygon,
                                          float&                 groundY);

            /**
            * Gets polygons from vertex
            *@param pVertex - source vertex descriptor, contains buffer to get from
            *@param polygonType - polygon type to generate
            *@param[out] polygons - polygon list to populate
            *@return true on success, otherwise false
            *@note Polygon list content should be deleted when useless
            */
            static bool GetPolygons(const DWF_Model::VertexBuffer* pVB, DWF_Geometry::Polygons& polygons);

        private:
            AABB_Node*  m_pRoot        = nullptr;
            std::size_t m_PolygonCount = 0;

            /**
            * Adds a polygon inside an existing bounding box
            *@param polygon - polygon to add
            *@param box - bounding box in which polygon should be added
            *@param[in, out] empty - if true, box is empty an still no contains any polygon
            */
            void AddPolygonToBoundingBox(const DWF_Geometry::Polygon& polygon,
                                               DWF_Geometry::AABB*    pBox,
                                               bool&                  empty) const;

            /**
            * Populates AABB tree
            *@param pNode - root or parent node to create from
            *@param polygons - source polygon array
            *@return true on success, otherwise false
            */
            bool Populate(AABB_Node* pNode, const DWF_Geometry::Polygons& polygons);

            /**
            * Resolves AABB tree
            *@param pRay - ray against which tree boxes will be tested
            *@param pNode - root or parent node to resolve
            *@param[in, out] polygons - polygons belonging to boxes hit by ray
            *@return true on success, otherwise false
            *@note Polygon list content should be deleted when useless
            */
            bool Resolve(const DWF_Geometry::RayF*     pRay,
                         const AABB_Node*              pNode,
                               DWF_Geometry::Polygons& polygons) const;

            /**
            * Adds polygon to array
            *@param vb - source vertex buffer
            *@param v1 - first polygon vertex index in vertex buffer
            *@param v2 - second polygon vertex index in vertex buffer
            *@param v3 - third polygon vertex index in vertex buffer
            *@param[in, out] polygons - polygon array that contains generated polygons
            *@note Generated polygons should be deleted when useless
            */
            static inline void AddPolygon(const DWF_Model::VertexBuffer& vb,
                                                std::size_t              v1,
                                                std::size_t              v2,
                                                std::size_t              v3,
                                                DWF_Geometry::Polygons&  polygons);
    };

    //---------------------------------------------------------------------------
    inline void AABB_Tree::AddPolygon(const DWF_Model::VertexBuffer& vb,
                                            std::size_t              v1,
                                            std::size_t              v2,
                                            std::size_t              v3,
                                            DWF_Geometry::Polygons&  polygons)
    {
        std::unique_ptr<DWF_Geometry::Polygon> pPolygon(new DWF_Geometry::Polygon());

        // copy polygon from source buffer
        pPolygon->m_Vertex[0] = DWF_Math::Vector3F(vb.m_Data[v1], vb.m_Data[v1 + 1], vb.m_Data[v1 + 2]);
        pPolygon->m_Vertex[1] = DWF_Math::Vector3F(vb.m_Data[v2], vb.m_Data[v2 + 1], vb.m_Data[v2 + 2]);
        pPolygon->m_Vertex[2] = DWF_Math::Vector3F(vb.m_Data[v3], vb.m_Data[v3 + 1], vb.m_Data[v3 + 2]);

        // add polygon to list
        polygons.push_back(pPolygon.get());
        pPolygon.release();
    }
    //---------------------------------------------------------------------------
}
