/****************************************************************************
 * ==> QR_Model ------------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a generic class to manage a 3D model              *
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

#include "QR_Model.h"

// qr engine
#include "QR_STDTools.h"
#include "QR_CollisionHelper.h"

//---------------------------------------------------------------------------
// QR_Model
//---------------------------------------------------------------------------
QR_Model::QR_Model() :
    m_Color(QR_Color(255, 255, 255, 255)),
    m_VertexFormat(QR_Vertex::IE_VF_None),
    m_DoConvertRHLH(false)
{}
//---------------------------------------------------------------------------
QR_Model::~QR_Model()
{}
//---------------------------------------------------------------------------
void QR_Model::Clear()
{
    m_Color         = QR_Color(255, 255, 255, 255);
    m_VertexFormat  = QR_Vertex::IE_VF_None;
    m_DoConvertRHLH = false;

    m_Light.Clear();
}
//---------------------------------------------------------------------------
void QR_Model::Copy(const QR_Model& other)
{
    m_Color         = other.m_Color;
    m_Light         = other.m_Light;
    m_VertexFormat  = other.m_VertexFormat;
    m_DoConvertRHLH = other.m_DoConvertRHLH;
}
//---------------------------------------------------------------------------
QR_Color QR_Model::CalculateLight(const QR_Vector3DP&        normal,
                                  const QR_DirectionalLight* pLight) const
{
    // calculate light angle
    M_Precision lightAngle = normal.Dot(pLight->m_Direction);

    // is light angle out of bounds?
    if (lightAngle < 0.0f)
        lightAngle = 0.0f;

    // calculate light color
    QR_UInt32 r = (QR_UInt32)(pLight->m_Color.GetRed()   * lightAngle) + pLight->m_Ambient.GetRed();
    QR_UInt32 g = (QR_UInt32)(pLight->m_Color.GetGreen() * lightAngle) + pLight->m_Ambient.GetGreen();
    QR_UInt32 b = (QR_UInt32)(pLight->m_Color.GetBlue()  * lightAngle) + pLight->m_Ambient.GetBlue();
    QR_UInt32 a = (QR_UInt32)(pLight->m_Color.GetAlpha() * lightAngle) + pLight->m_Ambient.GetAlpha();

    // is color red component out of bounds?
    if (r > 255)
        r = 255;

    // is color green component out of bounds?
    if (g > 255)
        g = 255;

    // is color blue component out of bounds?
    if (b > 255)
        b = 255;

    // is color alpha component out of bounds?
    if (a > 255)
        a = 255;

    return QR_Color(r, g, b, a);
}
//---------------------------------------------------------------------------
bool QR_Model::PopulateAABBTree(const QR_Mesh& mesh, QR_AABBTree* pAABBTree) const
{
    // no destination tree?
    if (!pAABBTree)
        // it's not an error so return true
        return true;

    // get mesh count
    const QR_SizeT meshCount = mesh.size();

    // no source mesh?
    if (!meshCount)
        // it's not an error so return true
        return true;

    QR_PolygonsP polygons;
    bool         success = false;

    try
    {
        // iterate through meshes
        for (QR_SizeT i = 0; i < meshCount; ++i)
            // get collide polygons
            if (!QR_CollisionHelper::GetPolygons(mesh[i], polygons))
                return false;

        // populate aligned-axis bounding box tree
        success = pAABBTree->Populate(polygons);
    }
    catch (...)
    {
        QR_STDTools::DelAndClear(polygons);
        throw;
    }

    QR_STDTools::DelAndClear(polygons);
    return success;
}
//---------------------------------------------------------------------------
