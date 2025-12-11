/****************************************************************************
 * ==> Voxels --------------------------------------------------------------*
 ****************************************************************************
 * Description: Voxels                                                      *
 * Developer:   Jean-Milost Reymond                                         *
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

#include "Voxels.h"

//---------------------------------------------------------------------------
// Voxels
//---------------------------------------------------------------------------
Voxels::Voxels(Shader* pShader, Renderer* pRenderer) :
    m_pShader(pShader),
    m_pRenderer(pRenderer)
{
    m_pBox = std::make_unique<Box>(Vector3F(0.0f, 0.0f, -0.5f), Vector3F(0.0f, 0.0f, 0.5f));
}
//---------------------------------------------------------------------------
Voxels::~Voxels()
{
    for (std::size_t i = 0; i < m_Voxels.size(); ++i)
        delete m_Voxels[i];

    for (std::size_t i = 0; i < m_Meshes.size(); ++i)
        delete m_Meshes[i];
}
//---------------------------------------------------------------------------
Vector3F Voxels::GetPos() const
{
    return m_Pos;
}
//---------------------------------------------------------------------------
void Voxels::SetPos(const Vector3F& pos)
{
    m_Pos = pos;
}
//---------------------------------------------------------------------------
Box* Voxels::GetBoundingBox(const Matrix4x4F& matrix) const
{
    return m_pBox->ApplyMatrix(matrix);
}
//---------------------------------------------------------------------------
void Voxels::Draw(const Matrix4x4F& matrix) const
{
    // iterate through Voxels to draw
    for (std::size_t i = 0; i < m_Voxels.size(); ++i)
    {
        // get the Voxel position
        const Vector3F voxelPos = m_Voxels[i]->GetPos();

        // create the Voxel matrix
        Matrix4x4F voxelMatrix     = Matrix4x4F::Identity();
        voxelMatrix.m_Table[3][0] -= voxelPos.m_X;
        voxelMatrix.m_Table[3][1] -= voxelPos.m_Y;
        voxelMatrix.m_Table[3][2] -= voxelPos.m_Z;

        // build the final Voxel matrix and draw the Voxel
        m_Voxels[i]->Draw(voxelMatrix.Multiply(matrix));
    }
}
//---------------------------------------------------------------------------
