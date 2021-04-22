/****************************************************************************
 * ==> Model ---------------------------------------------------------------*
 ****************************************************************************
 * Description : 3D model                                                   *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - fbx model reader                                           *
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

#include "Model.h"

//---------------------------------------------------------------------------
// Model::IBone
//---------------------------------------------------------------------------
Model::IBone::IBone() :
    m_Matrix(Matrix4x4F::Identity()),
    m_pParent(nullptr)
{}
//---------------------------------------------------------------------------
Model::IBone::~IBone()
{
    const std::size_t count = m_Children.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Children[i];
}
//---------------------------------------------------------------------------
// Model::IWeightInfluence
//---------------------------------------------------------------------------
Model::IWeightInfluence::IWeightInfluence()
{}
//---------------------------------------------------------------------------
Model::IWeightInfluence::~IWeightInfluence()
{}
//---------------------------------------------------------------------------
// Model::ISkinWeights
//---------------------------------------------------------------------------
Model::ISkinWeights::ISkinWeights() :
    m_pBone(nullptr),
    m_Matrix(Matrix4x4F::Identity())
{}
//---------------------------------------------------------------------------
Model::ISkinWeights::~ISkinWeights()
{
    const std::size_t count = m_WeightInfluences.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_WeightInfluences[i];
}
//---------------------------------------------------------------------------
// Model::IMeshSkinWeights
//---------------------------------------------------------------------------
Model::IMeshSkinWeights::IMeshSkinWeights()
{}
//---------------------------------------------------------------------------
Model::IMeshSkinWeights::~IMeshSkinWeights()
{
    const std::size_t count = m_SkinWeights.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_SkinWeights[i];
}
//---------------------------------------------------------------------------
// Model::IAnimationKey
//---------------------------------------------------------------------------
Model::IAnimationKey::IAnimationKey() :
    m_Frame(0)
{}
//---------------------------------------------------------------------------
Model::IAnimationKey::~IAnimationKey()
{}
//---------------------------------------------------------------------------
// Model::IAnimationKeys
//---------------------------------------------------------------------------
Model::IAnimationKeys::IAnimationKeys() :
    m_Type(IE_KT_Unknown)
{}
//---------------------------------------------------------------------------
Model::IAnimationKeys::~IAnimationKeys()
{
    const std::size_t count = m_Keys.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Keys[i];
}
//---------------------------------------------------------------------------
// Model::IAnimation
//---------------------------------------------------------------------------
Model::IAnimation::IAnimation() :
    m_pBone(nullptr)
{}
//---------------------------------------------------------------------------
Model::IAnimation::~IAnimation()
{
    const std::size_t count = m_Keys.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Keys[i];
}
//---------------------------------------------------------------------------
// Model::IAnimationSet
//---------------------------------------------------------------------------
Model::IAnimationSet::IAnimationSet()
{}
//---------------------------------------------------------------------------
Model::IAnimationSet::~IAnimationSet()
{
    const std::size_t count = m_Animations.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Animations[i];
}
//---------------------------------------------------------------------------
// Model
//---------------------------------------------------------------------------
Model::Model() :
    m_pSkeleton(nullptr),
    m_MeshOnly(false),
    m_PoseOnly(false)
{}
//---------------------------------------------------------------------------
Model::~Model()
{
    const std::size_t meshCount = m_Mesh.size();

    for (std::size_t i = 0; i < meshCount; ++i)
        delete m_Mesh[i];
}
//---------------------------------------------------------------------------
