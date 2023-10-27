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

#include "DWF_AABBTree.h"

// std
#include <algorithm>

// classes
#include "DWF_MathHelper.h"
#include "DWF_Line.h"

using namespace DWF_Collider;

//---------------------------------------------------------------------------
// AABB_Node
//---------------------------------------------------------------------------
AABB_Node::AABB_Node(AABB_Node* pParent) :
    m_pParent(pParent)
{}
//---------------------------------------------------------------------------
AABB_Node::~AABB_Node()
{
    // delete left node
    if (m_pLeft)
        delete m_pLeft;

    // delete right node
    if (m_pRight)
        delete m_pRight;

    // delete bounding box
    if (m_pBox)
        delete m_pBox;

    // delete and clear the polygon list
    for (DWF_Geometry::Polygons::iterator it = m_Polygons.begin(); it != m_Polygons.end(); ++it)
        delete (*it);
}
//---------------------------------------------------------------------------
// AABB_Tree
//---------------------------------------------------------------------------
AABB_Tree::AABB_Tree()
{}
//---------------------------------------------------------------------------
AABB_Tree::~AABB_Tree()
{
    // delete entire tree hierarchy
    if (m_pRoot)
        delete m_pRoot;
}
//---------------------------------------------------------------------------
DWF_Geometry::AABB* AABB_Tree::GetBoundingBox() const
{
    if (!m_pRoot)
        return nullptr;

    return m_pRoot->m_pBox;
}
//---------------------------------------------------------------------------
std::size_t AABB_Tree::GetPolygonCount() const
{
    return m_PolygonCount;
}
//---------------------------------------------------------------------------
bool AABB_Tree::Populate(const DWF_Geometry::Polygons& polygons)
{
    m_PolygonCount = polygons.size();

    // tree was already populated?
    if (m_pRoot)
        // clear it first
        delete m_pRoot;

    // create root node
    m_pRoot = new AABB_Node(nullptr);

    // populate tree
    return Populate(m_pRoot, polygons);
}
//---------------------------------------------------------------------------
bool AABB_Tree::Resolve(const DWF_Geometry::RayF* pRay, DWF_Geometry::Polygons& polygons) const
{
    return Resolve(pRay, m_pRoot, polygons);
}
//---------------------------------------------------------------------------
DWF_Math::Vector3F AABB_Tree::GetSlidingPoint(const DWF_Geometry::PlaneF& slidingPlane,
                                              const DWF_Math::Vector3F&   pos,
                                                    float                 radius)
{
    DWF_Geometry::PlaneF plane = slidingPlane;

    // calculate the distance between the center of the sphere and the plane
    const float distanceToPlane = plane.DistanceTo(pos);

    // check if value is negative
    if (distanceToPlane < 0.0f)
        // invert the plane
        plane = -plane;

    // calculate the direction of the line segment position - plane
    const DWF_Math::Vector3F planeRatio(radius * plane.m_A,
                                        radius * plane.m_B,
                                        radius * plane.m_C);

    // calculate who the line segment perpendicular to the plane, from the center
    // of the sphere, cross the collision sphere. Normally this point is beyond
    // the plane
    const DWF_Math::Vector3F pointBeyondPlane(pos.m_X - planeRatio.m_X,
                                              pos.m_Y - planeRatio.m_Y,
                                              pos.m_Z - planeRatio.m_Z);

    // configure the line segment to test
    const DWF_Geometry::LineF line(pos, pointBeyondPlane);

    DWF_Math::Vector3F pointOnPlane;

    if (!plane.Intersect(line, pointOnPlane))
        return DWF_Math::Vector3F();

    // from point calculated above, add the sphere radius and return the value
    return DWF_Math::Vector3F(pointOnPlane.m_X + planeRatio.m_X,
                              pointOnPlane.m_Y + planeRatio.m_Y,
                              pointOnPlane.m_Z + planeRatio.m_Z);
}
//---------------------------------------------------------------------------
bool AABB_Tree::GroundCollision(const DWF_Geometry::Sphere&  boundingSphere,
                                const DWF_Geometry::Polygon& polygon,
                                const DWF_Math::Vector3F&    groundDir,
                                      DWF_Math::Vector3F&    groundPos)
{
    // create the ground ray
    DWF_Geometry::RayF ray(boundingSphere.m_Center, groundDir);

    // calculate the point where the ground ray hit the polygon
    if (!polygon.Intersect(ray, groundPos))
        return false;

    // consider the sphere radius in the result
    groundPos.m_X += (boundingSphere.m_Radius * -groundDir.m_X);
    groundPos.m_Y += (boundingSphere.m_Radius * -groundDir.m_Y);
    groundPos.m_Z += (boundingSphere.m_Radius * -groundDir.m_Z);

    return true;
}
//---------------------------------------------------------------------------
bool AABB_Tree::GroundPosY(const DWF_Geometry::Sphere&  boundingSphere,
                           const DWF_Math::Vector3F&    groundDir,
                                 DWF_Geometry::Polygon& groundPolygon,
                                 float&                 groundY)
{
    bool result = false;

    // create the ground ray
    DWF_Geometry::RayF     groundRay(boundingSphere.m_Center, groundDir);
    DWF_Geometry::Polygons polygons;

    // using the ground ray, resolve aligned-axis bounding box tree
    Resolve(&groundRay, polygons);

    // initialize the ground position from the bounding sphere center
    DWF_Math::Vector3F groundPos = boundingSphere.m_Center;

    // iterate through polygons to check
    for (DWF_Geometry::Polygons::iterator it = polygons.begin(); it != polygons.end(); ++it)
        // check if a ground polygon was found, calculate the ground position if yes
        if (GroundCollision(boundingSphere, *(*it), groundDir, groundPos))
        {
            // copy the ground polygon
            groundPolygon = *(*it);

            result = true;
            break;
        }

    // delete and clear the found polygons (no longer required since now)
    for (DWF_Geometry::Polygons::iterator it = polygons.begin(); it != polygons.end(); ++it)
        delete (*it);

    // copy the resulting y value
    groundY = groundPos.m_Y;

    return result;
}
//---------------------------------------------------------------------------
bool AABB_Tree::GetPolygons(const DWF_Model::VertexBuffer* pVB, DWF_Geometry::Polygons& polygons)
{
    const std::size_t vertexCount = pVB->m_Data.size();

    // no data to extract from?
    if (!vertexCount)
        return true;

    // search for vertex type
    switch (pVB->m_Format.m_Type)
    {
        case DWF_Model::VertexFormat::IEType::IE_VT_Triangles:
        {
            // calculate iteration step
            const std::size_t step = (pVB->m_Format.m_Stride * 3);

            // iterate through source vertexes
            for (std::size_t i = 0; i < vertexCount; i += step)
                // extract polygon from source buffer
                AddPolygon(*pVB,
                           i,
                           i +  pVB->m_Format.m_Stride,
                           i + (pVB->m_Format.m_Stride * 2),
                           polygons);

            return true;
        }

        case DWF_Model::VertexFormat::IEType::IE_VT_TriangleStrip:
        {
            // calculate length to read in triangle strip buffer
            const std::size_t stripLength = (vertexCount - (pVB->m_Format.m_Stride * 2));
                  std::size_t index       = 0;

            // iterate through source vertexes
            for (std::size_t i = 0; i < stripLength; i += pVB->m_Format.m_Stride)
            {
                // extract polygon from source buffer, revert odd polygons
                if (!index || !(index % 2))
                    AddPolygon(*pVB,
                               i,
                               i +  pVB->m_Format.m_Stride,
                               i + (pVB->m_Format.m_Stride * 2),
                               polygons);
                else
                    AddPolygon(*pVB,
                               i +  pVB->m_Format.m_Stride,
                               i,
                               i + (pVB->m_Format.m_Stride * 2),
                               polygons);

                ++index;
            }

            return true;
        }

        case DWF_Model::VertexFormat::IEType::IE_VT_TriangleFan:
        {
            // calculate length to read in triangle fan buffer
            const std::size_t fanLength = (vertexCount - pVB->m_Format.m_Stride);

            // iterate through source vertexes
            for (std::size_t i = pVB->m_Format.m_Stride; i < fanLength; i += pVB->m_Format.m_Stride)
                // extract polygon from source buffer
                AddPolygon(*pVB,
                           0,
                           i,
                           i + pVB->m_Format.m_Stride,
                           polygons);

            return true;
        }

        case DWF_Model::VertexFormat::IEType::IE_VT_Quads:
        {
            // calculate iteration step
            const std::size_t step = (pVB->m_Format.m_Stride * 4);

            // iterate through source vertexes
            for (std::size_t i = 0; i < vertexCount; i += step)
            {
                // calculate vertexes position
                const std::size_t v1 = i;
                const std::size_t v2 = i +  pVB->m_Format.m_Stride;
                const std::size_t v3 = i + (pVB->m_Format.m_Stride * 2);
                const std::size_t v4 = i + (pVB->m_Format.m_Stride * 3);

                // extract polygons from source buffer
                AddPolygon(*pVB, v1, v2, v3, polygons);
                AddPolygon(*pVB, v3, v2, v4, polygons);
            }

            return true;
        }

        case DWF_Model::VertexFormat::IEType::IE_VT_QuadStrip:
        {
            // calculate iteration step
            const std::size_t step = (pVB->m_Format.m_Stride * 2);

            // calculate length to read in triangle strip buffer
            const std::size_t stripLength = (vertexCount - (pVB->m_Format.m_Stride * 2));

            // iterate through source vertexes
            for (std::size_t i = 0; i < stripLength; i += step)
            {
                // calculate vertexes position
                const std::size_t v1 = i;
                const std::size_t v2 = i +  pVB->m_Format.m_Stride;
                const std::size_t v3 = i + (pVB->m_Format.m_Stride * 2);
                const std::size_t v4 = i + (pVB->m_Format.m_Stride * 3);

                // extract polygons from source buffer
                AddPolygon(*pVB, v1, v2, v3, polygons);
                AddPolygon(*pVB, v3, v2, v4, polygons);
            }

            return true;
        }

        default:
            return false;
    }
}
//---------------------------------------------------------------------------
void AABB_Tree::AddPolygonToBoundingBox(const DWF_Geometry::Polygon& polygon,
                                              DWF_Geometry::AABB*    pBox,
                                              bool&                  empty) const
{
    // no box to add to?
    if (!pBox)
        return;

    // iterate through polygon vertices
    for (std::uint8_t i = 0; i < 3; ++i)
    {
        // is box empty?
        if (empty)
        {
            // initialize bounding box with first vertex
            pBox->m_Min = polygon.m_Vertex[i];
            pBox->m_Max = polygon.m_Vertex[i];
            empty       = false;
            continue;
        }

        // search for box min edge
        pBox->m_Min = DWF_Math::Vector3F(std::min(pBox->m_Min.m_X, polygon.m_Vertex[i].m_X),
                                         std::min(pBox->m_Min.m_Y, polygon.m_Vertex[i].m_Y),
                                         std::min(pBox->m_Min.m_Z, polygon.m_Vertex[i].m_Z));

        // search for box max edge
        pBox->m_Max = DWF_Math::Vector3F(std::max(pBox->m_Max.m_X, polygon.m_Vertex[i].m_X),
                                         std::max(pBox->m_Max.m_Y, polygon.m_Vertex[i].m_Y),
                                         std::max(pBox->m_Max.m_Z, polygon.m_Vertex[i].m_Z));
    }
}
//---------------------------------------------------------------------------
bool AABB_Tree::Populate(AABB_Node* pNode, const DWF_Geometry::Polygons& polygons)
{
    bool boxEmpty = true;

    // create a collision box
    pNode->m_pBox = new DWF_Geometry::AABB();

    // iterate through polygons to divide
    for (DWF_Geometry::Polygons::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        // calculate bounding box
        AddPolygonToBoundingBox(*(*it), pNode->m_pBox, boxEmpty);

    // create left and right boxes
    std::unique_ptr<DWF_Geometry::AABB> pLeftBox (new DWF_Geometry::AABB());
    std::unique_ptr<DWF_Geometry::AABB> pRightBox(new DWF_Geometry::AABB());

    // divide box in 2 sub-boxes
    pNode->m_pBox->Cut(*pLeftBox.get(), *pRightBox.get());

    DWF_Geometry::Polygons leftPolygons, rightPolygons;

    // iterate again through polygons to divide
    for (DWF_Geometry::Polygons::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        for (std::uint8_t i = 0; i < 3; ++i)
            // check if first polygon vertices belongs to left or right sub-box
            if (DWF_Math::Helper::IsBetween((*it)->m_Vertex[i],
                                            pLeftBox->m_Min,
                                            pLeftBox->m_Max,
                                            (float)M_Epsilon))
            {
                // create left polygon
                std::unique_ptr<DWF_Geometry::Polygon> pPolygon(new DWF_Geometry::Polygon());

                // copy polygon
                for (std::uint8_t j = 0; j < 3; ++j)
                    pPolygon->m_Vertex[j] = (*it)->m_Vertex[j];

                // add polygon to left list
                leftPolygons.push_back(pPolygon.get());
                pPolygon.release();

                break;
            }
            else
            if (DWF_Math::Helper::IsBetween((*it)->m_Vertex[i],
                                            pRightBox->m_Min,
                                            pRightBox->m_Max,
                                            (float)M_Epsilon))
            {
                // create left polygon
                std::unique_ptr<DWF_Geometry::Polygon> pPolygon(new DWF_Geometry::Polygon());

                // copy polygon
                for (std::uint8_t j = 0; j < 3; ++j)
                    pPolygon->m_Vertex[j] = (*it)->m_Vertex[j];

                // add polygon to left list
                rightPolygons.push_back(pPolygon.get());
                pPolygon.release();

                break;
            }

    const std::size_t polygonCount      = polygons.size();
    const std::size_t leftPolygonCount  = leftPolygons.size();
    const std::size_t rightPolygonCount = rightPolygons.size();
    const bool        canResolveLeft    = ((leftPolygonCount  > 0) && (leftPolygonCount  < polygonCount));
    const bool        canResolveRight   = ((rightPolygonCount > 0) && (rightPolygonCount < polygonCount));

    // leaf reached?
    if (!canResolveLeft && !canResolveRight)
    {
        // iterate through polygons to copy
        for (DWF_Geometry::Polygons::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        {
            // create left polygon
            std::unique_ptr<DWF_Geometry::Polygon> pPolygon(new DWF_Geometry::Polygon());

            // copy polygon
            for (std::uint8_t i = 0; i < 3; ++i)
                pPolygon->m_Vertex[i] = (*it)->m_Vertex[i];

            // add polygon to left list
            pNode->m_Polygons.push_back(pPolygon.get());
            pPolygon.release();
        }

        // delete left list, as it will no longer be used
        for (DWF_Geometry::Polygons::iterator it = leftPolygons.begin(); it != leftPolygons.end(); ++it)
            delete (*it);

        // delete right list, as it will no longer be used
        for (DWF_Geometry::Polygons::iterator it = rightPolygons.begin(); it != rightPolygons.end(); ++it)
            delete (*it);

        return true;
    }

    bool result = false;

    // do create left node?
    if (canResolveLeft)
    {
        // create and populate left node
        pNode->m_pLeft = new AABB_Node(pNode);

        try
        {
            result = Populate(pNode->m_pLeft, leftPolygons) || result;
        }
        catch (...)
        {
            // delete current list, as it will no more be used
            for (DWF_Geometry::Polygons::iterator it = leftPolygons.begin(); it != leftPolygons.end(); ++it)
                delete (*it);

            throw;
        }

        // delete current list, as it will no more be used
        for (DWF_Geometry::Polygons::iterator it = leftPolygons.begin(); it != leftPolygons.end(); ++it)
            delete (*it);
    }

    // do create right node?
    if (canResolveRight)
    {
        // create and populate right node
        pNode->m_pRight = new AABB_Node(pNode);

        try
        {
            result = Populate(pNode->m_pRight, rightPolygons) || result;
        }
        catch (...)
        {
            // delete current list, as it will no more be used
            for (DWF_Geometry::Polygons::iterator it = rightPolygons.begin(); it != rightPolygons.end(); ++it)
                delete (*it);

            throw;
        }

        // delete current list, as it will no more be used
        for (DWF_Geometry::Polygons::iterator it = rightPolygons.begin(); it != rightPolygons.end(); ++it)
            delete (*it);
    }

    return result;
}
//---------------------------------------------------------------------------
bool AABB_Tree::Resolve(const DWF_Geometry::RayF* pRay, const AABB_Node* pNode, DWF_Geometry::Polygons& polygons) const
{
    // no node to resolve? (this should never happen, but...)
    if (!pNode)
        return false;

    bool leftResolved  = false;
    bool rightResolved = false;

    // is leaf?
    if (!pNode->m_pLeft && !pNode->m_pRight)
    {
        // iterate through polygons
        for (DWF_Geometry::Polygons::const_iterator it = pNode->m_Polygons.begin();
             it != pNode->m_Polygons.end(); ++it)
        {
            // create polygon
            std::unique_ptr<DWF_Geometry::Polygon> pPolygon(new DWF_Geometry::Polygon());

            // copy polygon
            for (std::uint8_t i = 0; i < 3; ++i)
                pPolygon->m_Vertex[i] = (*it)->m_Vertex[i];

            // add polygon to result
            polygons.push_back(pPolygon.get());
            pPolygon.release();
        }

        return true;
    }

    // check if ray intersects the left box
    if (pNode->m_pLeft && pNode->m_pLeft->m_pBox->Intersect(*pRay))
        // resolve left node
        leftResolved = Resolve(pRay, pNode->m_pLeft, polygons);

    // check if ray intersects the right box
    if (pNode->m_pRight && pNode->m_pRight->m_pBox->Intersect(*pRay))
        // resolve right node
        rightResolved = Resolve(pRay, pNode->m_pRight, polygons);

    return (leftResolved || rightResolved);
}
//---------------------------------------------------------------------------
