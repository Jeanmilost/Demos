/****************************************************************************
 * ==> QR_MD2 --------------------------------------------------------------*
 ****************************************************************************
 * Description : MD2 model                                                  *
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

#ifndef QR_MD2H
#define QR_MD2H

// std
#include <vector>

// qr engine
#include "QR_Types.h"
#include "QR_Vector3D.h"
#include "QR_Vertex.h"
#include "QR_FramedModel.h"
#include "QR_MD2Parser.h"

/**
* MD2 model
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_MD2 : public QR_FramedModel
{
    public:
        /**
        * Constructor
        */
        QR_MD2();

        /**
        * Destructor
        */
        virtual ~QR_MD2();

        /**
        * Clears model
        */
        virtual void Clear();

        /**
        * Copies content from another model
        *@param other - other model to copy from
        */
        virtual void Copy(const QR_Model& other);

        /**
        * Loads MD2 from file
        *@param fileName - file name
        *@return true on success, otherwise false
        */
        virtual bool Load(const std::string&  fileName);
        virtual bool Load(const std::wstring& fileName);

        /**
        * Loads MD2 from buffer
        *@param buffer - buffer
        *@param length - length to read in buffer, in bytes
        *@return true on success, otherwise false
        *@note Read will begin from current offset
        */
        virtual bool Load(      QR_Buffer&            buffer,
                          const QR_Buffer::ISizeType& length);

        /**
        * Loads normals table from file
        *@param fileName - file name
        *@return true on success, otherwise false
        */
        virtual bool LoadNormals(const std::string&  fileName);
        virtual bool LoadNormals(const std::wstring& fileName);

        /**
        * Loads normals table from buffer
        *@param buffer - buffer
        *@param length - length to read in buffer, in bytes (not used here, can be 0)
        *@return true on success, otherwise false
        */
        virtual bool LoadNormals(      QR_Buffer&            buffer,
                                 const QR_Buffer::ISizeType& length);

        /**
        * Gets MD2 parser
        *@return MD2 parser
        */
        const QR_MD2Parser* GetParser() const;

        /**
        * Gets mesh
        *@param index - mesh index to get
        *@param[out] mesh - mesh
        *@param[out] pAABBTree - aligned-axis bounding box tree
        *@return true on success, otherwise false
        *@note vertex buffer content is organized as follow:
        *      [1]x [2]y [3]z [4]nx [5]ny [6]nz [7]tu [8]tv [9]r [10]g [11]b [12]a
        *      where:
        *      x/y/z    - vertex coordinates
        *      nx/ny/nz - vertex normal (if includeNormal is activated)
        *      tu/tv    - vertex texture coordinates(if includeTexture is activated)
        *      r/g/b/a  - vertex color(if includeColor is activated)
        *@note Mesh content should be deleted when useless. WARNING, the output mesh should
        *      be deleted even if function failed
        */
        virtual bool GetMesh(QR_SizeT index, QR_Mesh& mesh, QR_AABBTree* pAABBTree) const;

        /**
        * Gets mesh count
        *@return mesh count
        */
        virtual QR_SizeT GetMeshCount() const;

    protected:
        /**
        * Uncompress MD2 vertex
        *@param pFrame - MD2 frame
        *@param pVertex - MD2 vertex to uncompress
        *@return uncompressed vertex
        */
        virtual QR_Vector3DP UncompressVertex(const QR_MD2Parser::IFrame*  pFrame,
                                              const QR_MD2Parser::IVertex* pVertex) const;

    private:
        typedef std::vector<QR_Vector3DP> INormals;

        QR_MD2Parser m_Model;
        INormals     m_Normals;
};

#endif
