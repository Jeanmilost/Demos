/****************************************************************************
 * ==> VoxelAsset ----------------------------------------------------------*
 ****************************************************************************
 * Description: Voxel asset                                                 *
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

// classes
#include "Voxels.h"
#include "PixelBuffer.h"

/**
* Voxel asset
*@author Jean-Milost Reymond
*/
class VoxelAsset : public Voxels
{
    public:
        /**
        * Constructor
        *@param pShader - shader to use to draw the Voxel
        *@param pRenderer - renderer to use to draw the Voxel
        */
        VoxelAsset(Shader* pShader, Renderer* pRenderer);

        virtual ~VoxelAsset();

        /**
        * Loads Voxel asset from bitmap file
        *@param bitmap - source bitmap
        *@return true on success, otherwise false
        */
        virtual bool Load(const PixelBuffer& bitmap);
};
