/****************************************************************************
 * ==> QR_FramedModel ------------------------------------------------------*
 ****************************************************************************
 * Description : Base class for models based on frames, e.g. MD2, MD3, ...  *
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

#include "QR_FramedModel.h"

//---------------------------------------------------------------------------
// QR_FramedModel
//---------------------------------------------------------------------------
QR_FramedModel::QR_FramedModel() :
    QR_Model()
{}
//---------------------------------------------------------------------------
QR_FramedModel::~QR_FramedModel()
{}
//---------------------------------------------------------------------------
bool QR_FramedModel::Interpolate(const QR_Float& position,
                                 const QR_Mesh&  mesh1,
                                 const QR_Mesh&  mesh2,
                                       QR_Mesh& result)
{
    // get vertice count
    const QR_SizeT count = mesh1.size();

    // are mesh compatible?
    if (count != mesh2.size())
        return false;

    // iterate through mesh to interpolate
    for (QR_SizeT i = 0; i < count; ++i)
    {
        // are frame compatibles?
        if (!mesh1[i]->CompareFormat(*mesh2[i]))
            return false;

        // not a 3D coordinate?
        if (mesh1[i]->m_CoordType != QR_Vertex::IE_VC_XYZ)
            return false;

        // create and populate new interpolation vertex
        std::auto_ptr<QR_Vertex> pVertex(new QR_Vertex());
        pVertex->m_Name      = mesh1[i]->m_Name;
        pVertex->m_Stride    = mesh1[i]->m_Stride;
        pVertex->m_Type      = mesh1[i]->m_Type;
        pVertex->m_Format    = mesh1[i]->m_Format;
        pVertex->m_CoordType = mesh1[i]->m_CoordType;

        // get vertex buffer data count
        const QR_SizeT bufferCount = mesh1[i]->m_Buffer.size();

        pVertex->m_Buffer.reserve(bufferCount);

        // iterate through vertex buffer content
        for (QR_SizeT j = 0; j < bufferCount; j += mesh1[i]->m_Stride)
        {
            QR_UInt32 index = 3;

            // get positions
            QR_Vector3DP srcVec(mesh1[i]->m_Buffer[j],
                                mesh1[i]->m_Buffer[j + 1],
                                mesh1[i]->m_Buffer[j + 2]);
            QR_Vector3DP dstVec(mesh2[i]->m_Buffer[j],
                                mesh2[i]->m_Buffer[j + 1],
                                mesh2[i]->m_Buffer[j + 2]);

            // interpolate positions
            QR_Vector3DP vec = srcVec.Interpolate(dstVec, position);

            // set interpolated positions in destination buffer
            pVertex->m_Buffer.push_back(vec.m_X);
            pVertex->m_Buffer.push_back(vec.m_Y);
            pVertex->m_Buffer.push_back(vec.m_Z);

            // do include normals?
            if (mesh1[i]->m_Format & QR_Vertex::IE_VF_Normals)
            {
                // get normals
                QR_Vector3DP srcNormal(mesh1[i]->m_Buffer[j + index],
                                       mesh1[i]->m_Buffer[j + index + 1],
                                       mesh1[i]->m_Buffer[j + index + 2]);
                QR_Vector3DP dstNormal(mesh2[i]->m_Buffer[j + index],
                                       mesh2[i]->m_Buffer[j + index + 1],
                                       mesh2[i]->m_Buffer[j + index + 2]);

                // interpolate normals
                QR_Vector3DP normal = srcNormal.Interpolate(dstNormal, position);

                // set interpolated normals in destination buffer
                pVertex->m_Buffer.push_back(normal.m_X);
                pVertex->m_Buffer.push_back(normal.m_Y);
                pVertex->m_Buffer.push_back(normal.m_Z);

                index += 3;
            }

            // do include texture coordinates?
            if (mesh1[i]->m_Format & QR_Vertex::IE_VF_TexCoords)
            {
                // copy texture coordinates from source
                pVertex->m_Buffer.push_back(mesh1[i]->m_Buffer[j + index]);
                pVertex->m_Buffer.push_back(mesh1[i]->m_Buffer[j + index + 1]);

                index += 2;
            }

            // do include colors?
            if (mesh1[i]->m_Format & QR_Vertex::IE_VF_Colors)
            {
                // copy color from source
                pVertex->m_Buffer.push_back(mesh1[i]->m_Buffer[j + index]);
                pVertex->m_Buffer.push_back(mesh1[i]->m_Buffer[j + index + 1]);
                pVertex->m_Buffer.push_back(mesh1[i]->m_Buffer[j + index + 2]);
                pVertex->m_Buffer.push_back(mesh1[i]->m_Buffer[j + index + 3]);
            }
        }

        // add interpolated mesh to output list
        result.push_back(pVertex.get());
        pVertex.release();
    }

    return true;
}
//---------------------------------------------------------------------------
