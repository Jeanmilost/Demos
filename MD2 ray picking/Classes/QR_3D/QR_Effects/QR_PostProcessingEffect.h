/****************************************************************************
 * ==> QR_PostProcessingEffect ---------------------------------------------*
 ****************************************************************************
 * Description : Post-processing effect interface                           *
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

#ifndef QR_PostProcessingEffectH
#define QR_PostProcessingEffectH

// qr engine
#include "QR_Types.h"
#include "QR_Effect.h"

/**
* Post-processing effect interface
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_PostProcessingEffect : public QR_Effect
{
    public:
        /**
        * Constructor
        */
        QR_PostProcessingEffect();

        /**
        * Destructor
        */
        virtual ~QR_PostProcessingEffect();

        /**
        * Begins effect
        */
        virtual void Begin() = 0;

        /**
        * Ends effect
        */
        virtual void End() = 0;

        /**
        * Sets the effect size
        *@param width - new width to set
        *@param height - new height to set
        */
        virtual void SetSize(QR_UInt32 width, QR_UInt32 height) = 0;
};

#endif
