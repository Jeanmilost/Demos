/****************************************************************************
 * ==> VoxelAsset ----------------------------------------------------------*
 ****************************************************************************
 * Description: Voxel asset                                                 *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - Capsule collision demo                                     *
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

#include "VoxelAsset.h"

// classes
#include "MeshFactory.h"

//---------------------------------------------------------------------------
// VoxelAsset
//---------------------------------------------------------------------------
VoxelAsset::VoxelAsset(Shader* pShader, Renderer* pRenderer) :
    Voxels(pShader, pRenderer)
{}
//---------------------------------------------------------------------------
VoxelAsset::~VoxelAsset()
{}
//---------------------------------------------------------------------------
bool VoxelAsset::Load(const PixelBuffer& bitmap)
{
    std::map<std::uint32_t, Mesh*> meshDictionary;

    // iterate through bitmap pixels
    for (std::size_t y = 0; y < bitmap.m_Height; ++y)
        for (std::size_t x = 0; x < bitmap.m_Width; ++x)
        {
            const std::size_t i = (y * bitmap.m_Stride) + (x * bitmap.m_BytePerPixel);

            const std::uint8_t b = (std::uint8_t)(static_cast<unsigned char*>(bitmap.m_pData)[i]);
            const std::uint8_t g = (std::uint8_t)(static_cast<unsigned char*>(bitmap.m_pData)[i + 1]);
            const std::uint8_t r = (std::uint8_t)(static_cast<unsigned char*>(bitmap.m_pData)[i + 2]);

            // color key to ignore?
            if (r == 255 && g == 0 && b == 255)
                continue;

            // calculate the color
            const std::uint32_t color = (r << 16) | (g << 8) | b;

            Mesh* pMesh;

            // Search for an already created matching mesh
            auto it = meshDictionary.find(color);

            // a matching mesh already exists?
            if (it == meshDictionary.end())
            {
                // no, create a new mesh and add it to the list
                Material material;
                material.m_Color = ColorF((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);

                VertexBuffer::ICulling culling;

                VertexBuffer::IFormat format;
                format.m_Format = (VertexBuffer::IFormat::IEFormat)((std::int32_t)VertexBuffer::IFormat::IEFormat::IE_VF_Colors);
                format.m_Type   = VertexBuffer::IFormat::IEType::IE_VT_TriangleStrip;

                std::unique_ptr<Mesh> pBoxMesh = std::make_unique<Mesh>();
                MeshFactory::GetBox(1.0f, 1.0f, 1.0f, false, format, culling, material, *pBoxMesh);

                m_Meshes.push_back(pBoxMesh.get());

                meshDictionary[color] = pBoxMesh.get();
                pMesh                 = pBoxMesh.release();
            }
            else
                // yes, get it
                pMesh = it->second;

            // calculate x and y position
            const float xPos = (float)(bitmap.m_Width  / 2.0f) - x - 0.5f;
            const float yPos = (float)(bitmap.m_Height / 2.0f) - y - 0.5f;

            // create the Voxel
            std::unique_ptr<Voxel> pVoxel = std::make_unique<Voxel>(pMesh, m_pShader, m_pRenderer);
            pVoxel->SetPos(Vector3F(xPos, yPos, 0.0f));
            m_Voxels.push_back(pVoxel.get());
            pVoxel.release();

            // calculate the min bounding box edge
            m_pBox->m_Min.m_X = std::min(m_pBox->m_Min.m_X, xPos - 0.5f);
            m_pBox->m_Min.m_Y = std::min(m_pBox->m_Min.m_Y, yPos - 0.5f);

            // calculate the max bounding box edge
            m_pBox->m_Max.m_X = std::max(m_pBox->m_Max.m_X, xPos + 0.5f);
            m_pBox->m_Max.m_Y = std::max(m_pBox->m_Max.m_Y, yPos + 0.5f);
        }

    return true;
}
//---------------------------------------------------------------------------
