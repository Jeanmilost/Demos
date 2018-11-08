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

#ifndef QR_FramedModelH
#define QR_FramedModelH

// qr engine
#include "QR_Model.h"

/**
* Base class for models based on frames, e.g. MD2, MD3, ...
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_FramedModel : public QR_Model
{
    public:
        /**
        * Constructor
        */
        QR_FramedModel();

        /**
        * Destructor
        */
        virtual ~QR_FramedModel();

        /**
        * Gets mesh
        *@param index - mesh index to get
        *@param[out] mesh - mesh
        *@param[out] pAABBTree - aligned-axis bounding box tree
        *@return true on success, otherwise false
        */
        virtual bool GetMesh(QR_SizeT index, QR_Mesh& mesh, QR_AABBTree* pAABBTree) const = 0;

        /**
        * Gets mesh count
        *@return mesh count
        */
        virtual QR_SizeT GetMeshCount() const = 0;

        /**
        * Interpolates mesh
        *@param position - interpolation position, in percent (between 0.0f and 1.0f)
        *@param mesh1 - first mesh to interpolate
        *@param mesh2 - second mesh to interpolate
        *@param[out] result -resulting interpolated mesh
        *@return 0 on success, otherwise warning (positive value) or error (negative value) code
        *@note This function should only be used for compatibility with old OpenGL 1.x versions, as
        *      normally interpolation should be done in vertex shader
        */
        static bool Interpolate(const QR_Float& position,
                                const QR_Mesh&  mesh1,
                                const QR_Mesh&  mesh2,
                                      QR_Mesh&  result);
};

#endif
