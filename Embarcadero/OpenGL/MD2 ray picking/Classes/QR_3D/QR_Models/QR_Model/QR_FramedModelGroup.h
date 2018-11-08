/****************************************************************************
 * ==> QR_FramedModelGroup -------------------------------------------------*
 ****************************************************************************
 * Description : Framed model group, contains all items and functions       *
 *               needed to manage a complete model based on frames          *
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

#ifndef QR_FramedModelGroupH
#define QR_FramedModelGroupH

// qr engine
#include "QR_Model.h"
#include "QR_ModelGroup.h"

/**
* Framed model group, contains all items and functions needed to manage a complete framed model
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_FramedModelGroup : public QR_ModelGroup
{
    public:
        /**
        * Called when model animation ends
        */
        #if defined(CP_EMBARCADERO)
            typedef void (__closure *ITfOnAnimationEnd)();
        #else
            typedef void (*ITfOnAnimationEnd)();
        #endif

        /**
        * Constructor
        *@param pRenderer - renderer to use to draw the model
        */
        QR_FramedModelGroup(const QR_Renderer* pRenderer);

        /**
        * Destructor
        */
        virtual ~QR_FramedModelGroup();

        /**
        * Clears group
        */
        virtual void Clear();

        /**
        * Copies content from another group
        *@param other - other group to copy from
        */
        virtual void Copy(const QR_ModelGroup& other);

        /**
        * Sets force loop
        *@param value - if true, animation will always loop
        */
        virtual void ForceLoop(bool value);

        /**
        * Sets OnAnimationEnd callback
        *@param fHandler - function handler
        */
        virtual void Set_OnAnimationEnd(ITfOnAnimationEnd fHandler);

    protected:
        /**
        * Animation data structure
        */
        struct IAnimation
        {
            QR_SizeT  m_FrameIndex;
            QR_SizeT  m_InterpolationFrameIndex;
            QR_Double m_InterpolationFactor;
            bool      m_Loop;

            IAnimation();

            /**
            * Constructor
            *@param frameIndex - frame index to render
            *@param interpolationFrameIndex - frame index to use for interpolation
            *@param interpolationFactor - interpolation factor in percent (between 0.0f and 1.0f)
            *@param loop - if true, animation should loop at end
            */
            IAnimation(QR_SizeT  frameIndex,
                       QR_SizeT  interpolationFrameIndex,
                       QR_Double interpolationFactor,
                       bool      loop);

            virtual ~IAnimation();
        };

        bool              m_ForceLoop;
        ITfOnAnimationEnd m_fOnAnimationEnd;

        /**
        * Gets animation
        *@param elapsedTime - elapsed time since last frame was drawn
        *@param fps - number of frames per seconds
        *@param start - animation start index
        *@param end - animation end index
        *@aram loop - if true, animation should loop at end
        *@param[in, out] frameIndex - frame index, next frame index to draw when function ends
        *@param[in, out] interpolationFactor - interpolation factor between 2 frames
        *@return animation
        */
        virtual IAnimation GetAnimation(const QR_Double& elapsedTime,
                                              QR_SizeT   fps,
                                              QR_SizeT   start,
                                              QR_SizeT   end,
                                              bool       loop,
                                              QR_SizeT&  frameIndex,
                                              QR_Double& interpolationFactor) const;

        /**
        * Checks and returns an index that is always within the range delimited by start and end
        *@param index - index to check
        *@param start - range start
        *@param end - range end
        *@return an index that is always within the range delimited by start and end
        */
        virtual inline QR_SizeT ValidateIndex(QR_SizeT index, QR_SizeT start, QR_SizeT end) const;
};
//---------------------------------------------------------------------------
// QR_FramedModelGroup
//---------------------------------------------------------------------------
QR_SizeT QR_FramedModelGroup::ValidateIndex(QR_SizeT index, QR_SizeT start, QR_SizeT end) const
{
    // is index out of bounds?
    if (index >= start && index <= end)
        return index;

    // calculate range
    const QR_SizeT range = (end - start) + 1;

    // calculate and return index within the range delimited by start and end
    return (start + ((index - start) % range));
}
//---------------------------------------------------------------------------

#endif
