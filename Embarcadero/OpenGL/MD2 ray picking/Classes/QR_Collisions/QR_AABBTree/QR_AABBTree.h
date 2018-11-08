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

#ifndef QR_AABBTreeH
#define QR_AABBTreeH

// qr engine
#include "QR_Types.h"
#include "QR_Vector3D.h"
#include "QR_Plane.h"
#include "QR_Polygon.h"
#include "QR_Box.h"
#include "QR_Ray.h"

/**
* Aligned-axis bounding box tree node
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
struct QR_AABBNode
{
    QR_AABBNode* m_pParent;
    QR_AABBNode* m_pLeft;
    QR_AABBNode* m_pRight;
    QR_BoxP*     m_pBox;
    QR_PolygonsP m_Polygons;

    /**
    * Constructor
    *@param pParent - parent node
    */
    QR_AABBNode(QR_AABBNode* pParent);

    virtual ~QR_AABBNode();
};

/**
*Aligned-axis bounding box tree
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_AABBTree
{
    public:
        /**
        * Constructor
        */
        QR_AABBTree();

        /**
        * Destructor
        */
        virtual ~QR_AABBTree();

        /**
        * Gets the model bounding box
        *@return the model bounding box, NULL if not found or on error
        */
        virtual QR_BoxP* GetBoundingBox() const;

        /**
        * Populates AABB tree
        *@param polygons - source polygon array
        *@return true on success, otherwise false
        */
        virtual bool Populate(const QR_PolygonsP& polygons);

        /**
        * Resolves AABB tree
        *@param pRay - ray against which tree boxes will be tested
        *@param[in, out] polygons - polygons belonging to boxes hit by ray
        *@return true on success, otherwise false
        *@note Polygon list content should be deleted when useless
        */
        virtual bool Resolve(const QR_RayP* pRay, QR_PolygonsP& polygons) const;

    protected:
        /**
        * Checks if a value is between 2 values
        *@param value - value to test
        *@param valueStart - start value
        *@param valueEnd - end value
        *@param epsilon - epsilon value for tolerance
        *@return true if value is between values, otherwise false
        */
        virtual bool ValueIsBetween(M_Precision value,
                                    M_Precision valueStart,
                                    M_Precision valueEnd,
                                    M_Precision epsilon) const;

        /**
        * Checks if a point is between 2 vertices
        *@param point - point to test
        *@param pointStart - start vertex
        *@param pointEnd - end vertex
        *@param epsilon - epsilon value for tolerance
        *@return true if value is between points, otherwise false
        */
        virtual bool VectorIsBetween(const QR_Vector3DP& point,
                                     const QR_Vector3DP& pointStart,
                                     const QR_Vector3DP& pointEnd,
                                           M_Precision   epsilon) const;

        /**
        * Adds a polygon inside an existing bounding box
        *@param polygon - polygon to add
        *@param box - bounding box in which polygon should be added
        *@param[in, out] empty - if true, box is empty an still no contains any polygon
        */
        virtual void AddPolygonToBoundingBox(const QR_PolygonP& polygon,
                                                   QR_BoxP*     pBox,
                                                   bool&        empty) const;

        /**
        * Populates AABB tree
        *@param pNode - root or parent node to create from
        *@param polygons - source polygon array
        *@return true on success, otherwise false
        */
        virtual bool Populate(QR_AABBNode* pNode, const QR_PolygonsP& polygons);

        /**
        * Resolves AABB tree
        *@param pRay - ray against which tree boxes will be tested
        *@param pNode - root or parent node to resolve
        *@param[in, out] polygons - polygons belonging to boxes hit by ray
        *@return true on success, otherwise false
        *@note Polygon list content should be deleted when useless
        */
        virtual bool Resolve(const QR_RayP*      pRay,
                             const QR_AABBNode*  pNode,
                                   QR_PolygonsP& polygons) const;

    private:
        QR_AABBNode* m_pRoot;
};

#endif
