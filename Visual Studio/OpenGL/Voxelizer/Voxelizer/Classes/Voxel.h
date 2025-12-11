/****************************************************************************
 * ==> Voxel ---------------------------------------------------------------*
 ****************************************************************************
 * Description: Voxel                                                       *
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

#pragma once

// std
#include <memory.h>

// classes
#include "Matrix4x4.h"
#include "Box.h"
#include "Mesh.h"
#include "Shader.h"
#include "Renderer.h"

/**
* Voxel
*@author Jean-Milost Reymond
*/
class Voxel
{
    public:
        /**
        * Constructor
        *@param pMesh - mesh to use to draw the Voxel
        *@param pShader - shader to use to draw the Voxel
        *@param pRenderer - renderer to use to draw the Voxel
        */
        Voxel(Mesh* pMesh, Shader* pShader, Renderer* pRenderer);

        virtual ~Voxel();

        /**
        * Gets the Voxel position (absolute, from [0, 0, 0] coordinates)
        *@return position
        */
        virtual Vector3F GetPos() const;

        /**
        * Sets the Voxel position (absolute, from [0, 0, 0] coordinates)
        *@param pos - position
        */
        virtual void SetPos(const Vector3F& pos);

        /**
        * Gets the Voxel bounding box
        *@param matrix - Voxel matrix
        *@return the Voxel bounding box
        *@note The returned box should be deleted when useless
        */
        virtual Box* GetBoundingBox(const Matrix4x4F& matrix) const;

        /**
        * Draws the Voxel
        *@param matrix - Voxel matrix
        */
        virtual void Draw(const Matrix4x4F& matrix) const;

    private:
        std::unique_ptr<Box> m_pBox;
        Mesh*                m_pMesh     = nullptr;
        Shader*              m_pShader   = nullptr;
        Renderer*            m_pRenderer = nullptr;
        Vector3F             m_Pos;
};
