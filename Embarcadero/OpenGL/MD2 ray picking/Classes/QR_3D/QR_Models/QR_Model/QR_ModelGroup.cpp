/****************************************************************************
 * ==> QR_ModelGroup -------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a basic class to implement a model group. A model *
 *               group is generally composed by all the items (mesh,        *
 *               textures, ...) required to load and draw the whole model   *
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

#include "QR_ModelGroup.h"

// qr engine
#include "QR_Exception.h"
#include "QR_STDTools.h"

//---------------------------------------------------------------------------
// QR_ModelGroup::IState::IAnimationStateItem
//---------------------------------------------------------------------------
QR_ModelGroup::IState::IAnimationStateItem::IAnimationStateItem() :
    m_Gesture(0),
    m_FrameIndex(0),
    m_InterpolationFrameIndex(0),
    m_InterpolationFactor(0),
    m_Loop(false)
{}
//---------------------------------------------------------------------------
QR_ModelGroup::IState::IAnimationStateItem::~IAnimationStateItem()
{}
//---------------------------------------------------------------------------
// QR_ModelGroup::IState
//---------------------------------------------------------------------------
QR_ModelGroup::IState::IState() :
    m_RotationX(0.0f),
    m_RotationY(0.0f),
    m_RotationZ(0.0f),
    m_CombinationType(IE_CT_Scale_Rotate_Translate),
    m_pInitialMatrix(NULL)
{}
//---------------------------------------------------------------------------
QR_ModelGroup::IState::~IState()
{
    QR_STDTools::DelAndClear(m_AnimDict);

    if (m_pInitialMatrix)
        delete m_pInitialMatrix;
}
//---------------------------------------------------------------------------
// QR_ModelGroup::IRotation
//---------------------------------------------------------------------------
QR_ModelGroup::IRotation::IRotation() :
    m_Angle(0.0f)
{}
//---------------------------------------------------------------------------
QR_ModelGroup::IRotation::IRotation(const M_Precision& angle, const QR_Vector3DP& axis) :
    m_Angle(angle),
    m_Axis(axis)
{}
//---------------------------------------------------------------------------
QR_ModelGroup::IRotation::~IRotation()
{}
//---------------------------------------------------------------------------
// QR_ModelGroup
//---------------------------------------------------------------------------
QR_ModelGroup::QR_ModelGroup(const QR_Renderer* pRenderer) :
    m_pRenderer(pRenderer),
    m_Scaling(QR_Vector3DP(1.0f, 1.0f, 1.0f)),
    m_Translation(QR_Vector3DP(1.0f, 1.0f, 1.0f)),
    m_RotationX(IRotation(0.0f, QR_Vector3DP(1.0f, 0.0f, 0.0f))),
    m_RotationY(IRotation(0.0f, QR_Vector3DP(0.0f, 1.0f, 0.0f))),
    m_RotationZ(IRotation(0.0f, QR_Vector3DP(0.0f, 0.0f, 1.0f))),
    m_CombinationType(IE_CT_Scale_Rotate_Translate),
    m_pInitialMatrix(NULL),
    m_fOnHashCodeChanged(NULL),
    m_fOnLoadTexture(NULL),
    m_fOnDrawItem(NULL),
    m_fOnDetectCollision(NULL)
{
    M_ASSERT(m_pRenderer);
}
//---------------------------------------------------------------------------
QR_ModelGroup::~QR_ModelGroup()
{
    if (m_pInitialMatrix)
        delete m_pInitialMatrix;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::Clear()
{
    if (m_pInitialMatrix)
    {
        delete m_pInitialMatrix;
        m_pInitialMatrix = NULL;
    }

    m_Scaling         = QR_Vector3DP(1.0f, 1.0f, 1.0f);
    m_Translation     = QR_Vector3DP(1.0f, 1.0f, 1.0f);
    m_RotationX       = IRotation(0.0f, QR_Vector3DP(1.0f, 0.0f, 0.0f));
    m_RotationY       = IRotation(0.0f, QR_Vector3DP(0.0f, 1.0f, 0.0f));
    m_RotationZ       = IRotation(0.0f, QR_Vector3DP(0.0f, 0.0f, 1.0f));
    m_CombinationType = IE_CT_Scale_Rotate_Translate;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::Copy(const QR_ModelGroup& other)
{
    // copy the model attributes. NOTE the renderer is the only exception and should never be copied
    SetScaling(other.m_Scaling);
    SetTranslation(other.m_Translation);
    SetRotationX(other.m_RotationX.m_Angle);
    SetRotationY(other.m_RotationY.m_Angle);
    SetRotationZ(other.m_RotationZ.m_Angle);
    SetMatrixCombinationType(other.m_CombinationType);
    SetInitialMatrix(other.m_pInitialMatrix);

    // copy the events
    m_fOnLoadTexture     = other.m_fOnLoadTexture;
    m_fOnDrawItem        = other.m_fOnDrawItem;
    m_fOnDetectCollision = other.m_fOnDetectCollision;
}
//---------------------------------------------------------------------------
QR_Vector3DP QR_ModelGroup::GetTranslation() const
{
    return m_Translation;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::SetTranslation(const QR_Vector3DP& translation)
{
    m_Translation = translation;
}
//---------------------------------------------------------------------------
M_Precision QR_ModelGroup::GetRotationX() const
{
    return m_RotationX.m_Angle;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::SetRotationX(const M_Precision& angle)
{
    m_RotationX.m_Angle = angle;
}
//---------------------------------------------------------------------------
M_Precision QR_ModelGroup::GetRotationY() const
{
    return m_RotationY.m_Angle;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::SetRotationY(const M_Precision& angle)
{
    m_RotationY.m_Angle = angle;
}
//---------------------------------------------------------------------------
M_Precision QR_ModelGroup::GetRotationZ() const
{
    return m_RotationZ.m_Angle;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::SetRotationZ(const M_Precision& angle)
{
    m_RotationZ.m_Angle = angle;
}
//---------------------------------------------------------------------------
QR_Vector3DP QR_ModelGroup::GetScaling() const
{
    return m_Scaling;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::SetScaling(const QR_Vector3DP& scaling)
{
    m_Scaling = scaling;
}
//---------------------------------------------------------------------------
QR_ModelGroup::IECombinationType QR_ModelGroup::GetMatrixCombinationType() const
{
    return m_CombinationType;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::SetMatrixCombinationType(IECombinationType type)
{
    m_CombinationType = type;
}
//---------------------------------------------------------------------------
QR_Matrix16P* QR_ModelGroup::GetInitialMatrix() const
{
    return m_pInitialMatrix;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::SetInitialMatrix(const QR_Matrix16P* pMatrix)
{
    // previous initial matrix was set?
    if (m_pInitialMatrix)
        delete m_pInitialMatrix;

    // no new initial matrix to set?
    if (!pMatrix)
    {
        m_pInitialMatrix = NULL;
        return;
    }

    // copy initial matrix
    m_pInitialMatrix = new QR_Matrix16P(*pMatrix);
}
//---------------------------------------------------------------------------
void QR_ModelGroup::GetState(IState* pState) const
{
    if (!pState)
        return;

    // copy the actual values to state
    pState->m_Scaling         = m_Scaling;
    pState->m_Translation     = m_Translation;
    pState->m_RotationX       = m_RotationX.m_Angle;
    pState->m_RotationY       = m_RotationY.m_Angle;
    pState->m_RotationZ       = m_RotationZ.m_Angle;
    pState->m_CombinationType = m_CombinationType;

    // copy the initial matrix state
    if (m_pInitialMatrix)
    {
        if (pState->m_pInitialMatrix)
            *pState->m_pInitialMatrix = *m_pInitialMatrix;
        else
            pState->m_pInitialMatrix = new QR_Matrix16P(*m_pInitialMatrix);
    }
    else
    if (pState->m_pInitialMatrix)
    {
        delete pState->m_pInitialMatrix;
        pState->m_pInitialMatrix = NULL;
    }
}
//---------------------------------------------------------------------------
void QR_ModelGroup::SetState(const IState* pState)
{
    if (!pState)
        return;

    // restore actual values from state
    m_Scaling           = pState->m_Scaling;
    m_Translation       = pState->m_Translation;
    m_RotationX.m_Angle = pState->m_RotationX;
    m_RotationY.m_Angle = pState->m_RotationY;
    m_RotationZ.m_Angle = pState->m_RotationZ;
    m_CombinationType   = pState->m_CombinationType;

    // restore the initial matrix state
    if (pState->m_pInitialMatrix)
    {
        if (m_pInitialMatrix)
            *m_pInitialMatrix = *pState->m_pInitialMatrix;
        else
            m_pInitialMatrix = new QR_Matrix16P(*pState->m_pInitialMatrix);
    }
    else
    if (m_pInitialMatrix)
    {
        delete m_pInitialMatrix;
        m_pInitialMatrix = NULL;
    }
}
//---------------------------------------------------------------------------
void QR_ModelGroup::Set_OnHashCodeChanged(ITfOnHashCodeChanged fHandler)
{
    m_fOnHashCodeChanged = fHandler;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::Set_OnLoadTexture(ITfOnLoadTexture fHandler)
{
    m_fOnLoadTexture = fHandler;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::Set_OnDrawItem(ITfOnDrawItem fHandler)
{
    m_fOnDrawItem = fHandler;
}
//---------------------------------------------------------------------------
QR_ModelGroup::ITfOnDetectCollision QR_ModelGroup::Get_OnDetectCollision() const
{
    return m_fOnDetectCollision;
}
//---------------------------------------------------------------------------
void QR_ModelGroup::Set_OnDetectCollision(ITfOnDetectCollision fHandler)
{
    m_fOnDetectCollision = fHandler;
}
//---------------------------------------------------------------------------
QR_Matrix16P QR_ModelGroup::GetMatrix() const
{
    // initialize matrix
    QR_Matrix16P scaleMatrix     = QR_Matrix16P::Identity();
    QR_Matrix16P rotateXMatrix   = QR_Matrix16P::Identity();
    QR_Matrix16P rotateYMatrix   = QR_Matrix16P::Identity();
    QR_Matrix16P rotateZMatrix   = QR_Matrix16P::Identity();
    QR_Matrix16P translateMatrix = QR_Matrix16P::Identity();
    QR_Matrix16P modelMatrix     = m_pInitialMatrix ? *m_pInitialMatrix : QR_Matrix16P::Identity();

    // build scaling, rotation and translation matrix
    scaleMatrix.Scale(m_Scaling);
    rotateXMatrix.Rotate(m_RotationX.m_Angle, m_RotationX.m_Axis);
    rotateYMatrix.Rotate(m_RotationY.m_Angle, m_RotationY.m_Axis);
    rotateZMatrix.Rotate(m_RotationZ.m_Angle, m_RotationZ.m_Axis);
    translateMatrix.Translate(m_Translation);

    // build model matrix
    switch (m_CombinationType)
    {
        case IE_CT_Scale_Rotate_Translate:
            modelMatrix = modelMatrix.Multiply(scaleMatrix);
            modelMatrix = modelMatrix.Multiply(rotateXMatrix);
            modelMatrix = modelMatrix.Multiply(rotateYMatrix);
            modelMatrix = modelMatrix.Multiply(rotateZMatrix);
            modelMatrix = modelMatrix.Multiply(translateMatrix);
            break;

        case IE_CT_Scale_Translate_Rotate:
            modelMatrix = modelMatrix.Multiply(scaleMatrix);
            modelMatrix = modelMatrix.Multiply(translateMatrix);
            modelMatrix = modelMatrix.Multiply(rotateXMatrix);
            modelMatrix = modelMatrix.Multiply(rotateYMatrix);
            modelMatrix = modelMatrix.Multiply(rotateZMatrix);
            break;

        case IE_CT_Rotate_Translate_Scale:
            modelMatrix = modelMatrix.Multiply(rotateXMatrix);
            modelMatrix = modelMatrix.Multiply(rotateYMatrix);
            modelMatrix = modelMatrix.Multiply(rotateZMatrix);
            modelMatrix = modelMatrix.Multiply(translateMatrix);
            modelMatrix = modelMatrix.Multiply(scaleMatrix);
            break;

        case IE_CT_Rotate_Scale_Translate:
            modelMatrix = modelMatrix.Multiply(rotateXMatrix);
            modelMatrix = modelMatrix.Multiply(rotateYMatrix);
            modelMatrix = modelMatrix.Multiply(rotateZMatrix);
            modelMatrix = modelMatrix.Multiply(scaleMatrix);
            modelMatrix = modelMatrix.Multiply(translateMatrix);
            break;

        case IE_CT_Translate_Rotate_Scale:
            modelMatrix = modelMatrix.Multiply(translateMatrix);
            modelMatrix = modelMatrix.Multiply(rotateXMatrix);
            modelMatrix = modelMatrix.Multiply(rotateYMatrix);
            modelMatrix = modelMatrix.Multiply(rotateZMatrix);
            modelMatrix = modelMatrix.Multiply(scaleMatrix);
            break;

        case IE_CT_Translate_Scale_Rotate:
            modelMatrix = modelMatrix.Multiply(translateMatrix);
            modelMatrix = modelMatrix.Multiply(scaleMatrix);
            modelMatrix = modelMatrix.Multiply(rotateXMatrix);
            modelMatrix = modelMatrix.Multiply(rotateYMatrix);
            modelMatrix = modelMatrix.Multiply(rotateZMatrix);
            break;

        default:
            M_THROW_EXCEPTION("Unknown model matrix combination type");
    }

    return modelMatrix;
}
//---------------------------------------------------------------------------
