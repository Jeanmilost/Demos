/****************************************************************************
 * ==> QR_AABBTree ---------------------------------------------------------*
 ****************************************************************************
 * Description : Aligned-axis bounding box tree                             *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - QR Engine                                                  *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sublicense, and/or sell copies of the Software, and to       *
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

#include "QR_AABBTree.h"

// std
#include <stddef.h>
#include <cmath>
#include <memory>

// qr engine
#include "QR_STDTools.h"
#include "QR_GeometryHelper.h"

//---------------------------------------------------------------------------
// QR_AABBNode
//---------------------------------------------------------------------------
QR_AABBNode::QR_AABBNode(QR_AABBNode* pParent) :
    m_pParent(pParent),
    m_pLeft(NULL),
    m_pRight(NULL),
    m_pBox(NULL)
{}
//---------------------------------------------------------------------------
QR_AABBNode::~QR_AABBNode()
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
    QR_STDTools::DelAndClear(m_Polygons);
}
//---------------------------------------------------------------------------
// QR_AABBTree
//---------------------------------------------------------------------------
QR_AABBTree::QR_AABBTree() :
    m_pRoot(NULL)
{}
//---------------------------------------------------------------------------
QR_AABBTree::~QR_AABBTree()
{
    // delete entire tree hierarchy
    if (m_pRoot)
        delete m_pRoot;
}
//---------------------------------------------------------------------------
QR_BoxP* QR_AABBTree::GetBoundingBox() const
{
    if (!m_pRoot)
        return NULL;

    return m_pRoot->m_pBox;
}
//---------------------------------------------------------------------------
bool QR_AABBTree::Populate(const QR_PolygonsP& polygons)
{
    // tree was already populated?
    if (m_pRoot)
        // clear it first
        delete m_pRoot;

    // create root node
    m_pRoot = new QR_AABBNode(NULL);

    // populate tree
    return Populate(m_pRoot, polygons);
}
//---------------------------------------------------------------------------
bool QR_AABBTree::Resolve(const QR_RayP* pRay, QR_PolygonsP& polygons) const
{
    return Resolve(pRay, m_pRoot, polygons);
}
//---------------------------------------------------------------------------
bool QR_AABBTree::ValueIsBetween(M_Precision value,
                                 M_Precision valueStart,
                                 M_Precision valueEnd,
                                 M_Precision epsilon) const
{
    const M_Precision minVal = std::min(valueStart, valueEnd);
    const M_Precision maxVal = std::max(valueStart, valueEnd);

    // check if each value is between start and end limits considering tolerance
    return ((value >= (minVal - epsilon)) && (value <= (maxVal + epsilon)));
}
//---------------------------------------------------------------------------
bool QR_AABBTree::VectorIsBetween(const QR_Vector3DP& point,
                                  const QR_Vector3DP& pointStart,
                                  const QR_Vector3DP& pointEnd,
                                        M_Precision   epsilon) const
{
    // check if each vector component is between start and end limits
    return (ValueIsBetween(point.m_X, pointStart.m_X, pointEnd.m_X, epsilon) &&
            ValueIsBetween(point.m_Y, pointStart.m_Y, pointEnd.m_Y, epsilon) &&
            ValueIsBetween(point.m_Z, pointStart.m_Z, pointEnd.m_Z, epsilon));
}
//---------------------------------------------------------------------------
void QR_AABBTree::AddPolygonToBoundingBox(const QR_PolygonP& polygon,
                                                QR_BoxP*     pBox,
                                                bool&        empty) const
{
    // no box to add to?
    if (!pBox)
        return;

    // iterate through polygon vertices
    for (QR_UInt8 i = 0; i < 3; ++i)
    {
        // is box empty?
        if (empty)
        {
            // initialize bounding box with first vertex
            pBox->m_Min = polygon.GetVertex(i);
            pBox->m_Max = polygon.GetVertex(i);
            empty       = false;
            continue;
        }

        // search for box min edge
        pBox->m_Min = QR_Vector3DP(std::min(pBox->m_Min.m_X, polygon.GetVertex(i).m_X),
                                   std::min(pBox->m_Min.m_Y, polygon.GetVertex(i).m_Y),
                                   std::min(pBox->m_Min.m_Z, polygon.GetVertex(i).m_Z));

        // search for box max edge
        pBox->m_Max = QR_Vector3DP(std::max(pBox->m_Max.m_X, polygon.GetVertex(i).m_X),
                                   std::max(pBox->m_Max.m_Y, polygon.GetVertex(i).m_Y),
                                   std::max(pBox->m_Max.m_Z, polygon.GetVertex(i).m_Z));
    }
}
//---------------------------------------------------------------------------
bool QR_AABBTree::Populate(QR_AABBNode* pNode, const QR_PolygonsP& polygons)
{
    bool boxEmpty = true;

    // create a collision box
    pNode->m_pBox = new QR_BoxP();

    // iterate through polygons to divide
    for (QR_PolygonsP::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        // calculate bounding box
        AddPolygonToBoundingBox(*(*it), pNode->m_pBox, boxEmpty);

    // create left and right boxes
    std::auto_ptr<QR_BoxP> pLeftBox(new QR_BoxP());
    std::auto_ptr<QR_BoxP> pRightBox(new QR_BoxP());

    // divide box in 2 sub-boxes
    pNode->m_pBox->Cut(*pLeftBox.get(), *pRightBox.get());

    QR_PolygonsP leftPolygons, rightPolygons;

    // iterate again through polygons to divide
    for (QR_PolygonsP::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        for (QR_UInt8 i = 0; i < 3; ++i)
        {
            // check if first polygon vertice belongs to left or right sub-box
            if (VectorIsBetween((*it)->GetVertex(i),
                                pLeftBox->m_Min,
                                pLeftBox->m_Max,
                                M_Epsilon))
            {
                // create left polygon
                std::auto_ptr<QR_PolygonP> pPolygon(new QR_PolygonP());

                // copy polygon
                for (QR_UInt8 j = 0; j < 3; ++j)
                    pPolygon->SetVertex(j, (*it)->GetVertex(j));

                // add polygon to left list
                leftPolygons.push_back(pPolygon.get());
                pPolygon.release();

                break;
            }
            else
            if (VectorIsBetween((*it)->GetVertex(i),
                                pRightBox->m_Min,
                                pRightBox->m_Max,
                                M_Epsilon))
            {
                // create left polygon
                std::auto_ptr<QR_PolygonP> pPolygon(new QR_PolygonP());

                // copy polygon
                for (QR_UInt8 j = 0; j < 3; ++j)
                    pPolygon->SetVertex(j, (*it)->GetVertex(j));

                // add polygon to left list
                rightPolygons.push_back(pPolygon.get());
                pPolygon.release();

                break;
            }
        }

    const QR_SizeT polygonCount      = polygons.size();
    const QR_SizeT leftPolygonCount  = leftPolygons.size();
    const QR_SizeT rightPolygonCount = rightPolygons.size();
    const bool     canResolveLeft    = ((leftPolygonCount  > 0) && (leftPolygonCount  < polygonCount));
    const bool     canResolveRight   = ((rightPolygonCount > 0) && (rightPolygonCount < polygonCount));

    // leaf reached?
    if (!canResolveLeft && !canResolveRight)
    {
        // iterate through polygons to copy
        for (QR_PolygonsP::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        {
            // create left polygon
            std::auto_ptr<QR_PolygonP> pPolygon(new QR_PolygonP());

            // copy polygon
            for (QR_UInt8 i = 0; i < 3; ++i)
                pPolygon->SetVertex(i, (*it)->GetVertex(i));

            // add polygon to left list
            pNode->m_Polygons.push_back(pPolygon.get());
            pPolygon.release();
        }

        // delete left and right lists, as they will no more be used
        QR_STDTools::DelAndClear(leftPolygons);
        QR_STDTools::DelAndClear(rightPolygons);

        return true;
    }

    bool result = false;

    // do create left node?
    if (canResolveLeft)
    {
        // create and populate left node
        pNode->m_pLeft = new QR_AABBNode(pNode);

        try
        {
            result = Populate(pNode->m_pLeft, leftPolygons) || result;
        }
        catch (...)
        {
            // delete current list, as it will no more be used
            QR_STDTools::DelAndClear(leftPolygons);
            throw;
        }

        // delete current list, as it will no more be used
        QR_STDTools::DelAndClear(leftPolygons);
    }

    // do create right node?
    if (canResolveRight)
    {
        // create and populate right node
        pNode->m_pRight = new QR_AABBNode(pNode);

        try
        {
            result = Populate(pNode->m_pRight, rightPolygons) || result;
        }
        catch (...)
        {
            // delete current list, as it will no more be used
            QR_STDTools::DelAndClear(rightPolygons);
            throw;
        }

        // delete current list, as it will no more be used
        QR_STDTools::DelAndClear(rightPolygons);
    }

    return result;
}
//---------------------------------------------------------------------------
bool QR_AABBTree::Resolve(const QR_RayP* pRay, const QR_AABBNode* pNode, QR_PolygonsP& polygons) const
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
        for (QR_PolygonsP::const_iterator it = pNode->m_Polygons.begin();
                it != pNode->m_Polygons.end(); ++it)
        {
            // create left polygon
            std::auto_ptr<QR_PolygonP> pPolygon(new QR_PolygonP());

            // copy polygon
            for (QR_UInt8 i = 0; i < 3; ++i)
                pPolygon->SetVertex(i, (*it)->GetVertex(i));

            // add polygon to left list
            polygons.push_back(pPolygon.get());
            pPolygon.release();
        }

        return true;
    }

    // check if ray intersects the left box
    if (pNode->m_pLeft && QR_GeometryHelper::RayIntersectsBox(*pRay, *pNode->m_pLeft->m_pBox))
        // resolve left node
        leftResolved = Resolve(pRay, pNode->m_pLeft, polygons);

    // check if ray intersects the right box
    if (pNode->m_pRight && QR_GeometryHelper::RayIntersectsBox(*pRay, *pNode->m_pRight->m_pBox))
        // resolve right node
        rightResolved = Resolve(pRay, pNode->m_pRight, polygons);

    return (leftResolved || rightResolved);
}
//---------------------------------------------------------------------------
