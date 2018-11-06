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

#ifndef QR_ModelH
#define QR_ModelH

// qr engine
#include "QR_Types.h"
#include "QR_Color.h"
#include "QR_Light.h"
#include "QR_Vertex.h"
#include "QR_AABBTree.h"

/**
* Basic 3D model
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Model
{
    public:
        /**
        * Constructor
        */
        QR_Model();

        /**
        * Destructor
        */
        virtual ~QR_Model();

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
        * Gets color
        *@return color
        */
        virtual inline QR_Color GetColor() const;

        /**
        * Sets color
        *@param color - color
        */
        virtual inline void SetColor(const QR_Color& color);

        /**
        * Gets pre-calculated light
        *@return pre-calculated light to apply
        */
        virtual inline QR_DirectionalLight GetLight() const;

        /**
        * Sets pre-calculated light
        *@param pLight - pre-calculated light to apply to this model
        */
        virtual inline void SetLight(const QR_DirectionalLight& light);

        /**
        * Enables or disables the pre-calculated light
        *@param value - if true, pre-calculated light is enabled, otherwise disabled
        */
        virtual inline void EnableLight(bool value);

        /**
        * Checks if left hand to right hand conversion mode is enabled
        *@return true if left hand to right hand conversion is enabled, otherwise false
        */
        virtual inline bool DoConvertRHLH() const;

        /**
        * Enables or disables left hand to right hand conversion mode
        *@param value - if true, conversion mode is enabled, disabled otherwise
        */
        virtual inline void SetConvertRHLH(bool value);

        /**
        * Gets vertex format
        *@return vertex format
        */
        virtual inline QR_Vertex::IEFormat GetVertexFormat() const;

        /**
        * Sets vertex format
        *@param format - vertex format
        */
        virtual inline void SetVertexFormat(QR_Vertex::IEFormat format);

    protected:
        QR_Color            m_Color;
        QR_DirectionalLight m_Light;
        QR_Vertex::IEFormat m_VertexFormat;
        bool                m_DoConvertRHLH;

        /**
        * Calculates vertex color based on pre-calculated light
        *@param normal - vertex normal
        *@param light - pre-calculated light
        *@return vertex color
        */
        virtual QR_Color CalculateLight(const QR_Vector3DP&        normal,
                                        const QR_DirectionalLight* pLight) const;

        /**
        * Populates aligned-axis bounding box tree from a mesh
        *@param mesh - source mesh from which aligned-axis bounding box tree should be populated
        *@param pAABBTree - aligned-axis bounding box tree to populate
        *@return true on success, otherwise false
        */
        virtual bool PopulateAABBTree(const QR_Mesh& mesh, QR_AABBTree* pAABBTree) const;
};

//---------------------------------------------------------------------------
// QR_Model
//---------------------------------------------------------------------------
QR_Color QR_Model::GetColor() const
{
    return m_Color;
}
//---------------------------------------------------------------------------
void QR_Model::SetColor(const QR_Color& color)
{
    m_Color = color;
}
//---------------------------------------------------------------------------
QR_DirectionalLight QR_Model::GetLight() const
{
    return m_Light;
}
//---------------------------------------------------------------------------
void QR_Model::SetLight(const QR_DirectionalLight& light)
{
    m_Light = light;
}
//---------------------------------------------------------------------------
void QR_Model::EnableLight(bool value)
{
    m_Light.m_Enabled = value;
}
//---------------------------------------------------------------------------
bool QR_Model::DoConvertRHLH() const
{
    return m_DoConvertRHLH;
}
//---------------------------------------------------------------------------
void QR_Model::SetConvertRHLH(bool value)
{
    m_DoConvertRHLH = value;
}
//---------------------------------------------------------------------------
QR_Vertex::IEFormat QR_Model::GetVertexFormat() const
{
    return m_VertexFormat;
}
//---------------------------------------------------------------------------
void QR_Model::SetVertexFormat(QR_Vertex::IEFormat format)
{
    m_VertexFormat = format;
}
//---------------------------------------------------------------------------

#endif
