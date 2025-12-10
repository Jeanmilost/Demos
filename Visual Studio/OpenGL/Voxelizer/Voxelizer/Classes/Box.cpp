/****************************************************************************
 * ==> Box -----------------------------------------------------------------*
 ****************************************************************************
 * Description : Geometric box                                              *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License                                                              *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sub-license, and/or sell copies of the Software, and to      *
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

#include "Box.h"

//---------------------------------------------------------------------------
// Box
//---------------------------------------------------------------------------
Box::Box()
{}
//---------------------------------------------------------------------------
Box::Box(const Vector3F& min, const Vector3F& max) :
    m_Min(min),
    m_Max(max)
{}
//---------------------------------------------------------------------------
Box::~Box()
{}
//---------------------------------------------------------------------------
bool Box::Collide(const Box& box) const
{
    return !(m_Min.m_X <= box.m_Max.m_X &&
             m_Max.m_X >= box.m_Min.m_X &&
             m_Min.m_Y <= box.m_Max.m_Y &&
             m_Max.m_Y >= box.m_Min.m_Y &&
             m_Min.m_Z <= box.m_Max.m_Z &&
             m_Max.m_Z >= box.m_Min.m_Z);
}
//---------------------------------------------------------------------------
Box* Box::ApplyMatrix(const Matrix4x4F& matrix) const
{
    // build a new box transforming all vertices of the box using
    // given matrix, and return new built box
    return new Box(matrix.Transform(m_Min), matrix.Transform(m_Max));
}
//---------------------------------------------------------------------------
