/****************************************************************************
 * ==> QR_Light ------------------------------------------------------------*
 ****************************************************************************
 * Description : Classes to simulate lights                                 *
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

#ifndef QR_LightH
#define QR_LightH

// qr engine
#include "QR_Color.h"
#include "QR_Vector3D.h"

/**
* Basic light
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Light
{
    public:
        QR_Color m_Ambient;
        bool     m_Enabled;

        QR_Light();
        virtual ~QR_Light();

        /**
        * Clears light
        */
        virtual void Clear();
};

/**
* Directional light
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_DirectionalLight : public QR_Light
{
    public:
        QR_Color     m_Color;
        QR_Vector3DP m_Direction;

        QR_DirectionalLight();
        virtual ~QR_DirectionalLight();

        /**
        * Clears directional light
        */
        virtual void Clear();
};

#endif
