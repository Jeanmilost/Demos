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

#include "QR_FramedModelGroup.h"

//---------------------------------------------------------------------------
// QR_FramedModelGroup::IAnimation
//---------------------------------------------------------------------------
QR_FramedModelGroup::IAnimation::IAnimation() :
    m_FrameIndex(0),
    m_InterpolationFrameIndex(0),
    m_InterpolationFactor(0.0),
    m_Loop(false)
{}
//---------------------------------------------------------------------------
QR_FramedModelGroup::IAnimation::IAnimation(QR_SizeT  frameIndex,
                                            QR_SizeT  interpolationFrameIndex,
                                            QR_Double interpolationFactor,
                                            bool      loop) :
    m_FrameIndex(frameIndex),
    m_InterpolationFrameIndex(interpolationFrameIndex),
    m_InterpolationFactor(interpolationFactor),
    m_Loop(loop)
{}
//---------------------------------------------------------------------------
QR_FramedModelGroup::IAnimation::~IAnimation()
{}
//---------------------------------------------------------------------------
// QR_FramedModelGroup
//---------------------------------------------------------------------------
QR_FramedModelGroup::QR_FramedModelGroup(const QR_Renderer* pRenderer) :
    QR_ModelGroup(pRenderer),
    m_ForceLoop(true),
    m_fOnAnimationEnd(NULL)
{}
//---------------------------------------------------------------------------
QR_FramedModelGroup::~QR_FramedModelGroup()
{}
//---------------------------------------------------------------------------
void QR_FramedModelGroup::Clear()
{
    QR_ModelGroup::Clear();

    m_ForceLoop = false;
}
//---------------------------------------------------------------------------
void QR_FramedModelGroup::Copy(const QR_ModelGroup& other)
{
    QR_ModelGroup::Copy(other);

    // try to get source model
    const QR_FramedModelGroup* pSource = dynamic_cast<const QR_FramedModelGroup*>(&other);

    // found it?
    if (!pSource)
    {
        Clear();
        return;
    }

    m_ForceLoop       = pSource->m_ForceLoop;
    m_fOnAnimationEnd = pSource->m_fOnAnimationEnd;
}
//---------------------------------------------------------------------------
void QR_FramedModelGroup::ForceLoop(bool value)
{
    m_ForceLoop = value;
}
//---------------------------------------------------------------------------
void QR_FramedModelGroup::Set_OnAnimationEnd(ITfOnAnimationEnd fHandler)
{
    m_fOnAnimationEnd = fHandler;
}
//---------------------------------------------------------------------------
QR_FramedModelGroup::IAnimation QR_FramedModelGroup::GetAnimation(const QR_Double& elapsedTime,
                                                                        QR_SizeT   fps,
                                                                        QR_SizeT   start,
                                                                        QR_SizeT   end,
                                                                        bool       loop,
                                                                        QR_SizeT&  frameIndex,
                                                                        QR_Double& interpolationFactor) const
{
    // no animation info?
    if (!fps || start == end)
        return IAnimation(start, start, 0.0, false);

    // calculate time interval between each frames
    const QR_Double timeInterval = (1000.0 / (QR_Double)fps);

    // calculate how many frames must be incremented since the last rendering
    interpolationFactor += (elapsedTime / timeInterval);

    // should increment one frame or more?
    if (interpolationFactor >= 1.0)
    {
        // calculate number of frames to increment
        const QR_SizeT frameCount = std::floor(interpolationFactor);

        // calculate interpolation factor (should always be between 0 and 1)
        interpolationFactor -= frameCount;

        // move frame index to next frame to show
        frameIndex += frameCount;
    }

    // not looping?
    if (!loop && !m_ForceLoop)
    {
        QR_SizeT nextFrameIndex;

        // last frame reached?
        if (frameIndex > end)
        {
            // show last frame
            frameIndex          = end;
            nextFrameIndex      = end;
            interpolationFactor = 0.0;

            // animation end reached?
            if (m_fOnAnimationEnd)
                m_fOnAnimationEnd();
        }
        else
        {
            // show next frame
            nextFrameIndex = frameIndex + 1;

            if (nextFrameIndex > end)
                nextFrameIndex = end;
        }

        return IAnimation(frameIndex, nextFrameIndex, interpolationFactor, loop);
    }

    // calculate next indexes to render
                   frameIndex     = ValidateIndex(frameIndex,     start, end);
    const QR_SizeT nextFrameIndex = ValidateIndex(frameIndex + 1, start, end);

    // return animation data
    return IAnimation(frameIndex, nextFrameIndex, interpolationFactor, loop);
}
//---------------------------------------------------------------------------
