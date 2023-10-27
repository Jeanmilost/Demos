/****************************************************************************
 * ==> DWF_ModelFactory ----------------------------------------------------*
 ****************************************************************************
 * Description: Model factory                                               *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - DwarfStar Game Engine                                      *
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

#include "DWF_ModelFactory.h"

//std
#include <memory>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

using namespace DWF_Model;

//---------------------------------------------------------------------------
// Factory
//---------------------------------------------------------------------------
Model* Factory::GetSurface(float                             width,
                           float                             height,
                     const VertexFormat&                     format,
                     const VertexCulling&                    culling,
                     const Material&                         material,
                     const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    // create a buffer template: 0 for negative values, 1 for positive
    const int bufferTemplate[] =
    {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    };

    // create a mesh to contain the shape
    std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

    // create a new vertex buffer to contain the mesh vertices
    std::unique_ptr<VertexBuffer> pVB = std::make_unique<VertexBuffer>();
    pVB->m_Material                   = material;
    pVB->m_Culling                    = culling;
    pVB->m_Format                     = format;

    // set the vertex format type
    pVB->m_Format.m_Type = DWF_Model::VertexFormat::IEType::IE_VT_TriangleStrip;

    // calculate the stride
    pVB->m_Format.CalculateStride();

    // iterate through vertex to create
    for (std::size_t i = 0; i < 4; ++i)
    {
        // calculate template buffer index
        const std::size_t index = i * 2;

        DWF_Math::Vector3F vertex;

        // populate vertex buffer
        if (bufferTemplate[index])
            vertex.m_X =  width / 2.0f;
        else
            vertex.m_X = -width / 2.0f;

        if (bufferTemplate[index + 1])
            vertex.m_Y =  height / 2.0f;
        else
            vertex.m_Y = -height / 2.0f;

        vertex.m_Z = 0.0f;

        DWF_Math::Vector3F normal;

        // vertex has a normal?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
        {
            normal.m_X =  0.0f;
            normal.m_Y =  0.0f;
            normal.m_Z = -1.0f;
        }

        DWF_Math::Vector2F uv;

        // vertex has UV texture coordinates?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
        {
            // calculate texture u coordinate
            if (bufferTemplate[index])
                uv.m_X = 1.0f;
            else
                uv.m_X = 0.0f;

            // calculate texture v coordinate
            if (bufferTemplate[index + 1])
                uv.m_Y = 1.0f;
            else
                uv.m_Y = 0.0f;
        }

        // add the vertex to the buffer
        pVB->Add(&vertex, &normal, &uv, i, fOnGetVertexColor);
    }

    // add the vertex buffer to the mesh
    pMesh->m_VB.push_back(pVB.get());
    pVB.release();

    // create a model and add the newly created mesh to it
    std::unique_ptr<Model> pModel = std::make_unique<Model>();
    pModel->m_Mesh.push_back(pMesh.get());
    pMesh.release();

    return pModel.release();
}
//---------------------------------------------------------------------------
Model* Factory::GetBox(float                             width,
                       float                             height,
                       float                             depth,
                       bool                              repeatTexOnEachFace,
                 const VertexFormat&                     format,
                 const VertexCulling&                    culling,
                 const Material&                         material,
                 const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    // create a mesh to contain the shape
    std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

    // calculate half values
    const float halfX = width  / 2.0f;
    const float halfY = height / 2.0f;
    const float halfZ = depth  / 2.0f;

    pMesh->m_VB.reserve(6);

    // iterate through each edges
    for (std::size_t i = 0; i < 6; ++i)
    {
        // create a new vertex buffer to contain the next face
        std::unique_ptr<VertexBuffer> pVB = std::make_unique<VertexBuffer>();
        pVB->m_Material                   = material;
        pVB->m_Culling                    = culling;
        pVB->m_Format                     = format;

        // set the vertex format type
        pVB->m_Format.m_Type = DWF_Model::VertexFormat::IEType::IE_VT_TriangleStrip;

        // calculate the stride
        pVB->m_Format.CalculateStride();

        // add the vertex buffer to the mesh
        pMesh->m_VB.push_back(pVB.get());
        pVB.release();
    }

    DWF_Math::Vector3F vertices[8];

    // iterate through vertices to create. Vertices are generated as follow:
    //     v2 *--------* v6
    //      / |      / |
    // v4 *--------* v8|
    //    |   |    |   |
    //    |v1 *----|---* v5
    //    | /      | /
    // v3 *--------* v7
    for (std::size_t i = 0; i < 8; ++i)
    {
        // generate the 4 first vertices on the left, and 4 last on the right
        if (!(i / 4))
            vertices[i].m_X = -halfX;
        else
            vertices[i].m_X =  halfX;

        // generate 2 vertices on the front, then 2 vertices on the back
        if (!((i / 2) % 2))
            vertices[i].m_Z = -halfZ;
        else
            vertices[i].m_Z =  halfZ;

        // for each vertices, generates one on the top, and one on the bottom
        if (!(i % 2))
            vertices[i].m_Y = -halfY;
        else
            vertices[i].m_Y =  halfY;
    }

    DWF_Math::Vector3F normals[6];

    // vertex has normals?
    if (((std::uint32_t)format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
    {
        // calculate normals
        normals[0].m_X = -1.0; normals[0].m_Y =  0.0; normals[0].m_Z =  0.0;
        normals[1].m_X =  1.0; normals[1].m_Y =  0.0; normals[1].m_Z =  0.0;
        normals[2].m_X =  0.0; normals[2].m_Y = -1.0; normals[2].m_Z =  0.0;
        normals[3].m_X =  0.0; normals[3].m_Y =  1.0; normals[3].m_Z =  0.0;
        normals[4].m_X =  0.0; normals[4].m_Y =  0.0; normals[4].m_Z = -1.0;
        normals[5].m_X =  0.0; normals[5].m_Y =  0.0; normals[5].m_Z =  1.0;
    }

    DWF_Math::Vector2F texCoords[24];

    // vertex has UV texture coordinates?
    if (((std::uint32_t)format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
        // do repeat texture on each faces?
        if (repeatTexOnEachFace)
        {
            // calculate texture positions
            texCoords[0].m_X  = 0.0; texCoords[0].m_Y  = 0.0;
            texCoords[1].m_X  = 0.0; texCoords[1].m_Y  = 1.0;
            texCoords[2].m_X  = 1.0; texCoords[2].m_Y  = 0.0;
            texCoords[3].m_X  = 1.0; texCoords[3].m_Y  = 1.0;
            texCoords[4].m_X  = 0.0; texCoords[4].m_Y  = 0.0;
            texCoords[5].m_X  = 0.0; texCoords[5].m_Y  = 1.0;
            texCoords[6].m_X  = 1.0; texCoords[6].m_Y  = 0.0;
            texCoords[7].m_X  = 1.0; texCoords[7].m_Y  = 1.0;
            texCoords[8].m_X  = 0.0; texCoords[8].m_Y  = 0.0;
            texCoords[9].m_X  = 0.0; texCoords[9].m_Y  = 1.0;
            texCoords[10].m_X = 1.0; texCoords[10].m_Y = 0.0;
            texCoords[11].m_X = 1.0; texCoords[11].m_Y = 1.0;
            texCoords[12].m_X = 0.0; texCoords[12].m_Y = 0.0;
            texCoords[13].m_X = 0.0; texCoords[13].m_Y = 1.0;
            texCoords[14].m_X = 1.0; texCoords[14].m_Y = 0.0;
            texCoords[15].m_X = 1.0; texCoords[15].m_Y = 1.0;
            texCoords[16].m_X = 0.0; texCoords[16].m_Y = 0.0;
            texCoords[17].m_X = 0.0; texCoords[17].m_Y = 1.0;
            texCoords[18].m_X = 1.0; texCoords[18].m_Y = 0.0;
            texCoords[19].m_X = 1.0; texCoords[19].m_Y = 1.0;
            texCoords[20].m_X = 0.0; texCoords[20].m_Y = 0.0;
            texCoords[21].m_X = 0.0; texCoords[21].m_Y = 1.0;
            texCoords[22].m_X = 1.0; texCoords[22].m_Y = 0.0;
            texCoords[23].m_X = 1.0; texCoords[23].m_Y = 1.0;
        }
        else
        {
            // calculate texture offset
            const float texOffset = 1.0f / 3.0f;

            // calculate texture positions. They are distributed as follow:
            // -------------------
            // |     |     |     |
            // |  1  |  2  |  3  |
            // |     |     |     |
            // |-----------------|
            // |     |     |     |
            // |  4  |  5  |  6  |
            // |     |     |     |
            // -------------------
            // |  This texture   |
            // |  area is not    |
            // |  used           |
            // -------------------
            texCoords[0].m_X  = 0.0f;             texCoords[0].m_Y  = texOffset;
            texCoords[1].m_X  = 0.0f;             texCoords[1].m_Y  = 0.0f;
            texCoords[2].m_X  = texOffset;        texCoords[2].m_Y  = texOffset;
            texCoords[3].m_X  = texOffset;        texCoords[3].m_Y  = 0.0f;
            texCoords[4].m_X  = texOffset;        texCoords[4].m_Y  = texOffset;
            texCoords[5].m_X  = texOffset;        texCoords[5].m_Y  = 0.0f;
            texCoords[6].m_X  = texOffset * 2.0f; texCoords[6].m_Y  = texOffset;
            texCoords[7].m_X  = texOffset * 2.0f; texCoords[7].m_Y  = 0.0f;
            texCoords[8].m_X  = texOffset * 2.0f; texCoords[8].m_Y  = texOffset;
            texCoords[9].m_X  = texOffset * 2.0f; texCoords[9].m_Y  = 0.0f;
            texCoords[10].m_X = 1.0f;             texCoords[10].m_Y = texOffset;
            texCoords[11].m_X = 1.0f;             texCoords[11].m_Y = 0.0f;
            texCoords[12].m_X = 0.0f;             texCoords[12].m_Y = texOffset * 2.0f;
            texCoords[13].m_X = 0.0f;             texCoords[13].m_Y = texOffset;
            texCoords[14].m_X = texOffset;        texCoords[14].m_Y = texOffset * 2.0f;
            texCoords[15].m_X = texOffset;        texCoords[15].m_Y = texOffset;
            texCoords[16].m_X = texOffset;        texCoords[16].m_Y = texOffset * 2.0f;
            texCoords[17].m_X = texOffset;        texCoords[17].m_Y = texOffset;
            texCoords[18].m_X = texOffset * 2.0f; texCoords[18].m_Y = texOffset * 2.0f;
            texCoords[19].m_X = texOffset * 2.0f; texCoords[19].m_Y = texOffset;
            texCoords[20].m_X = texOffset * 2.0f; texCoords[20].m_Y = texOffset * 2.0f;
            texCoords[21].m_X = texOffset * 2.0f; texCoords[21].m_Y = texOffset;
            texCoords[22].m_X = 1.0f;             texCoords[22].m_Y = texOffset * 2.0f;
            texCoords[23].m_X = 1.0f;             texCoords[23].m_Y = texOffset;
        }

    // create box edge 1
    pMesh->m_VB[0]->Add(&vertices[1], &normals[0], &texCoords[4], 0, fOnGetVertexColor);
    pMesh->m_VB[0]->Add(&vertices[0], &normals[0], &texCoords[5], 1, fOnGetVertexColor);
    pMesh->m_VB[0]->Add(&vertices[3], &normals[0], &texCoords[6], 2, fOnGetVertexColor);
    pMesh->m_VB[0]->Add(&vertices[2], &normals[0], &texCoords[7], 3, fOnGetVertexColor);

    // create box edge 2
    pMesh->m_VB[1]->Add(&vertices[3], &normals[5], &texCoords[8], 0, fOnGetVertexColor);
    pMesh->m_VB[1]->Add(&vertices[2], &normals[5], &texCoords[9], 1, fOnGetVertexColor);
    pMesh->m_VB[1]->Add(&vertices[7], &normals[5], &texCoords[10], 2, fOnGetVertexColor);
    pMesh->m_VB[1]->Add(&vertices[6], &normals[5], &texCoords[11], 3, fOnGetVertexColor);

    // create box edge 3
    pMesh->m_VB[2]->Add(&vertices[7], &normals[1], &texCoords[12], 0, fOnGetVertexColor);
    pMesh->m_VB[2]->Add(&vertices[6], &normals[1], &texCoords[13], 1, fOnGetVertexColor);
    pMesh->m_VB[2]->Add(&vertices[5], &normals[1], &texCoords[14], 2, fOnGetVertexColor);
    pMesh->m_VB[2]->Add(&vertices[4], &normals[1], &texCoords[15], 3, fOnGetVertexColor);

    // create box edge 4
    pMesh->m_VB[3]->Add(&vertices[5], &normals[4], &texCoords[16], 0, fOnGetVertexColor);
    pMesh->m_VB[3]->Add(&vertices[4], &normals[4], &texCoords[17], 1, fOnGetVertexColor);
    pMesh->m_VB[3]->Add(&vertices[1], &normals[4], &texCoords[18], 2, fOnGetVertexColor);
    pMesh->m_VB[3]->Add(&vertices[0], &normals[4], &texCoords[19], 3, fOnGetVertexColor);

    // create box edge 5
    pMesh->m_VB[4]->Add(&vertices[1], &normals[3], &texCoords[0], 0, fOnGetVertexColor);
    pMesh->m_VB[4]->Add(&vertices[3], &normals[3], &texCoords[1], 1, fOnGetVertexColor);
    pMesh->m_VB[4]->Add(&vertices[5], &normals[3], &texCoords[2], 2, fOnGetVertexColor);
    pMesh->m_VB[4]->Add(&vertices[7], &normals[3], &texCoords[3], 3, fOnGetVertexColor);

    // create box edge 6
    pMesh->m_VB[5]->Add(&vertices[2], &normals[2], &texCoords[20], 0, fOnGetVertexColor);
    pMesh->m_VB[5]->Add(&vertices[0], &normals[2], &texCoords[21], 1, fOnGetVertexColor);
    pMesh->m_VB[5]->Add(&vertices[6], &normals[2], &texCoords[22], 2, fOnGetVertexColor);
    pMesh->m_VB[5]->Add(&vertices[4], &normals[2], &texCoords[23], 3, fOnGetVertexColor);

    // create a model and add the newly created mesh to it
    std::unique_ptr<Model> pModel = std::make_unique<Model>();
    pModel->m_Mesh.push_back(pMesh.get());
    pMesh.release();

    return pModel.release();
}
//---------------------------------------------------------------------------
Model* Factory::GetSphere(float                             radius,
                          std::size_t                       slices,
                          std::size_t                       stacks,
                    const VertexFormat&                     format,
                    const VertexCulling&                    culling,
                    const Material&                         material,
                    const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    // create a mesh to contain the shape
    std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

    // initialize global values
    const float majorStep = ((float)       M_PI  / slices);
    const float minorStep = ((float)(2.0 * M_PI) / stacks);

    // iterate through vertex slices
    for (std::size_t i = 0; i < slices; ++i)
    {
        // create a new vertex buffer to contain the next slice
        std::unique_ptr<VertexBuffer> pVB = std::make_unique<VertexBuffer>();
        pVB->m_Material                   = material;
        pVB->m_Culling                    = culling;
        pVB->m_Format                     = format;

        // set the vertex format type
        pVB->m_Format.m_Type = DWF_Model::VertexFormat::IEType::IE_VT_TriangleStrip;

        // calculate the stride
        pVB->m_Format.CalculateStride();

        // calculate next slice values
        const float a  = i * majorStep;
        const float b  = a + majorStep;
        const float r0 = radius * sinf(a);
        const float r1 = radius * sinf(b);
        const float z0 = radius * cosf(a);
        const float z1 = radius * cosf(b);

        // iterate through vertex stacks
        for (std::size_t j = 0; j <= stacks; ++j)
        {
            const float c = j * minorStep;
            const float x = cosf(c);
            const float y = sinf(c);

            DWF_Math::Vector3F vertex;

            // calculate vertex
            vertex.m_X = x * r0;
            vertex.m_Y = y * r0;
            vertex.m_Z =     z0;

            DWF_Math::Vector3F normal;

            // vertex has a normal?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
            {
                normal.m_X = (x * r0) / radius;
                normal.m_Y = (y * r0) / radius;
                normal.m_Z =      z0  / radius;
            }

            DWF_Math::Vector2F uv;

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                uv.m_X = ((float)j / (float)stacks);
                uv.m_Y = ((float)i / (float)slices);
            }

            // add the vertex to the buffer
            pVB->Add(&vertex, &normal, &uv, j * 2, fOnGetVertexColor);

            // calculate vertex
            vertex.m_X = x * r1;
            vertex.m_Y = y * r1;
            vertex.m_Z =     z1;

            // vertex has a normal?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
            {
                normal.m_X = (x * r1) / radius;
                normal.m_Y = (y * r1) / radius;
                normal.m_Z =      z1  / radius;
            }

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                uv.m_X =  ((float)j         / (float)stacks);
                uv.m_Y = (((float)i + 1.0f) / (float)slices);
            }

            // add the vertex to the buffer
            pVB->Add(&vertex, &normal, &uv, (j * 2) + 1, fOnGetVertexColor);
        }

        // add the vertex buffer to the mesh
        pMesh->m_VB.push_back(pVB.get());
        pVB.release();
    }

    // create a model and add the newly created mesh to it
    std::unique_ptr<Model> pModel = std::make_unique<Model>();
    pModel->m_Mesh.push_back(pMesh.get());
    pMesh.release();

    return pModel.release();
}
//---------------------------------------------------------------------------
Model* Factory::GetCylinder(float                             minRadius,
                            float                             maxRadius,
                            float                             height,
                            std::size_t                       faces,
                      const VertexFormat&                     format,
                      const VertexCulling&                    culling,
                      const Material&                         material,
                      const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    // create a mesh to contain the shape
    std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

    // create a new vertex buffer to contain the mesh vertices
    std::unique_ptr<VertexBuffer> pVB = std::make_unique<VertexBuffer>();
    pVB->m_Material                   = material;
    pVB->m_Culling                    = culling;
    pVB->m_Format                     = format;

    // set the vertex format type
    pVB->m_Format.m_Type = DWF_Model::VertexFormat::IEType::IE_VT_TriangleStrip;

    // calculate the stride
    pVB->m_Format.CalculateStride();

    // calculate step to apply between faces
    const float step = (float)(2.0 * M_PI) / (float)faces;

    // iterate through vertices to create
    for (std::size_t i = 0; i < faces + 1; ++i)
    {
        // calculate angle
        const float angle = step * i;

        DWF_Math::Vector3F vertex;

        // set vertex data
        vertex.m_X = minRadius * cosf(angle);
        vertex.m_Y = -(height / 2.0f);
        vertex.m_Z = minRadius * sinf(angle);

        DWF_Math::Vector3F normal;

        // vertex has normals?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
        {
            // set normals
            normal.m_X = cosf(angle);
            normal.m_Y = 0.0f;
            normal.m_Z = sinf(angle);
        }

        DWF_Math::Vector2F uv;

        // vertex has UV texture coordinates?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
        {
            // is the first point to calculate?
            if (!i)
            {
                // add texture coordinates data to buffer
                uv.m_X = 0.0f;
                uv.m_Y = 0.0f;
            }
            else
            {
                // add texture coordinates data to buffer
                uv.m_X = 1.0f / (float)i;
                uv.m_Y = 0.0f;
            }
        }

        // add the vertex to the buffer
        pVB->Add(&vertex, &normal, &uv, (size_t)i * 2, fOnGetVertexColor);

        // set vertex data
        vertex.m_X = maxRadius * cosf(angle);
        vertex.m_Y = (height / 2.0f);
        vertex.m_Z = maxRadius * sinf(angle);

        // vertex has normals?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
        {
            // set normals
            normal.m_X = cosf(angle);
            normal.m_Y = 0.0f;
            normal.m_Z = sinf(angle);
        }

        // vertex has UV texture coordinates?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
        {
            // is the first point to calculate?
            if (!i)
            {
                // add texture coordinates data to buffer
                uv.m_X = 0.0f;
                uv.m_Y = 1.0f;
            }
            else
            {
                // add texture coordinates data to buffer
                uv.m_X = 1.0f / (float)i;
                uv.m_Y = 1.0f;
            }
        }

        // add the vertex to the buffer
        pVB->Add(&vertex, &normal, &uv, ((size_t)i * 2) + 1, fOnGetVertexColor);
    }

    // add the vertex buffer to the mesh
    pMesh->m_VB.push_back(pVB.get());
    pVB.release();

    // create a model and add the newly created mesh to it
    std::unique_ptr<Model> pModel = std::make_unique<Model>();
    pModel->m_Mesh.push_back(pMesh.get());
    pMesh.release();

    return pModel.release();
}
//---------------------------------------------------------------------------
Model* Factory::GetCapsule(float                             height,
                           float                             radius,
                           float                             resolution,
                     const VertexFormat&                     format,
                     const VertexCulling&                    culling,
                     const Material&                         material,
                     const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    if (radius == 0.0f)
        return nullptr;

    std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

    const float third     = 1.0f / 3.0f;
    const float twoThirds = 2.0f / 3.0f;

    const DWF_Math::Vector3F capsuleTop   (0.0f, height, 0.0f);
    const DWF_Math::Vector3F capsuleBottom(0.0f, 0.0f, 0.0f);

    // this capsule
    const DWF_Math::Vector3F lineDir       = (capsuleTop    - capsuleBottom).Normalize();
    const DWF_Math::Vector3F lineEndOffset =  lineDir       * radius;
    const DWF_Math::Vector3F top           =  capsuleTop    - lineEndOffset;
    const DWF_Math::Vector3F bottom        =  capsuleBottom + lineEndOffset;

    const DWF_Math::Vector3F axis   = bottom - top;
    const float              length = axis.Length();
    const DWF_Math::Vector3F localZ = axis / (length ? length : 1.0f);
    const DWF_Math::Vector3F localX = GetAnyPerpendicularUnitVector(localZ);
    const DWF_Math::Vector3F localY = localZ.Cross(localX);

    const DWF_Math::Vector3F start(0.0f, 0.0f, 0.0f);
    const DWF_Math::Vector3F end  (1.0f, 1.0f, 1.0f);

    const DWF_Math::Vector3F step = (end - start) / resolution;

    std::unique_ptr<VertexBuffer> pVB = std::make_unique<VertexBuffer>();
    pVB->m_Material                   = material;
    pVB->m_Culling                    = culling;
    pVB->m_Format                     = format;
    pVB->m_Format.CalculateStride();

    DWF_Math::Vector3F p0,  p1,  p2,  p3, normal0, normal1, normal2, normal3;
    DWF_Math::Vector2F uv0, uv1, uv2, uv3;

    // iterate through latitude and longitude
    for (float i = 0.0f; i < resolution; ++i)
        for (float j = 0.0f; j < resolution; ++j)
        {
            // calculate the current slice
            const float u = i * step.m_X + start.m_X;
            const float v = j * step.m_Y + start.m_Y;

            // calculate the next slice
            const float un = (i + 1 == resolution) ? end.m_X : (i + 1) * step.m_X + start.m_X;
            const float vn = (j + 1 == resolution) ? end.m_Y : (j + 1) * step.m_Y + start.m_Y;

            // create next cylinder face
            p0 = GetCylinderVertex(top, localX, localY, localZ, radius, length, u,  v);
            p1 = GetCylinderVertex(top, localX, localY, localZ, radius, length, u,  vn);
            p2 = GetCylinderVertex(top, localX, localY, localZ, radius, length, un, v);
            p3 = GetCylinderVertex(top, localX, localY, localZ, radius, length, un, vn);

            // vertex has normals?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
            {
                // calculate the vertex normals
                normal0 = p0 / radius;
                normal1 = p1 / radius;
                normal2 = p2 / radius;
                normal3 = p3 / radius;
            }

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                uv0.m_X =           (i      / resolution);
                uv0.m_Y = third +  ((j      / resolution) * third);
                uv1.m_X =           (i      / resolution);
                uv1.m_Y = third + (((j + 1) / resolution) * third);
                uv2.m_X =          ((i + 1) / resolution);
                uv2.m_Y = third +  ((j      / resolution) * third);
                uv3.m_X =          ((i + 1) / resolution);
                uv3.m_Y = third + (((j + 1) / resolution) * third);
            }

            // add face to vertex buffer
            pVB->Add(&p0, &normal0, &uv0, 0, fOnGetVertexColor);
            pVB->Add(&p1, &normal1, &uv1, 0, fOnGetVertexColor);
            pVB->Add(&p2, &normal2, &uv2, 0, fOnGetVertexColor);
            pVB->Add(&p3, &normal3, &uv3, 0, fOnGetVertexColor);
            pVB->Add(&p1, &normal1, &uv1, 0, fOnGetVertexColor);
            pVB->Add(&p2, &normal2, &uv2, 0, fOnGetVertexColor);

            // create next sphere start face
            p0 = GetSphereStartVertex(top, localX, localY, localZ, radius, u,  v);
            p1 = GetSphereStartVertex(top, localX, localY, localZ, radius, u,  vn);
            p2 = GetSphereStartVertex(top, localX, localY, localZ, radius, un, v);
            p3 = GetSphereStartVertex(top, localX, localY, localZ, radius, un, vn);

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
            {
                // calculate the vertex normals
                normal0 = p0 / radius;
                normal1 = p1 / radius;
                normal2 = p2 / radius;
                normal3 = p3 / radius;
            }

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                uv0.m_X =   (i      / resolution);
                uv0.m_Y =  ((j      / resolution) * third);
                uv1.m_X =   (i      / resolution);
                uv1.m_Y = (((j + 1) / resolution) * third);
                uv2.m_X =  ((i + 1) / resolution);
                uv2.m_Y =  ((j      / resolution) * third);
                uv3.m_X =  ((i + 1) / resolution);
                uv3.m_Y = (((j + 1) / resolution) * third);
            }

            // add face to vertex buffer
            pVB->Add(&p0, &normal0, &uv0, 0, fOnGetVertexColor);
            pVB->Add(&p1, &normal1, &uv1, 0, fOnGetVertexColor);
            pVB->Add(&p2, &normal2, &uv2, 0, fOnGetVertexColor);
            pVB->Add(&p3, &normal3, &uv3, 0, fOnGetVertexColor);
            pVB->Add(&p1, &normal1, &uv1, 0, fOnGetVertexColor);
            pVB->Add(&p2, &normal2, &uv2, 0, fOnGetVertexColor);

            // create next sphere end face
            p0 = GetSphereEndVertex(bottom, localX, localY, localZ, radius, u,  v);
            p1 = GetSphereEndVertex(bottom, localX, localY, localZ, radius, u,  vn);
            p2 = GetSphereEndVertex(bottom, localX, localY, localZ, radius, un, v);
            p3 = GetSphereEndVertex(bottom, localX, localY, localZ, radius, un, vn);

            // vertex has normal coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
            {
                // calculate the vertex normals
                normal0 = p0 / radius;
                normal1 = p1 / radius;
                normal2 = p2 / radius;
                normal3 = p3 / radius;
            }

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                uv0.m_X =               (i      / resolution);
                uv0.m_Y = twoThirds +  ((j      / resolution) * third);
                uv1.m_X =               (i      / resolution);
                uv1.m_Y = twoThirds + (((j + 1) / resolution) * third);
                uv2.m_X =              ((i + 1) / resolution);
                uv2.m_Y = twoThirds +  ((j      / resolution) * third);
                uv3.m_X =              ((i + 1) / resolution);
                uv3.m_Y = twoThirds + (((j + 1) / resolution) * third);
            }

            // add face to vertex buffer
            pVB->Add(&p0, &normal0, &uv0, 0, fOnGetVertexColor);
            pVB->Add(&p1, &normal1, &uv1, 0, fOnGetVertexColor);
            pVB->Add(&p2, &normal2, &uv2, 0, fOnGetVertexColor);
            pVB->Add(&p3, &normal3, &uv3, 0, fOnGetVertexColor);
            pVB->Add(&p1, &normal1, &uv1, 0, fOnGetVertexColor);
            pVB->Add(&p2, &normal2, &uv2, 0, fOnGetVertexColor);
        }

    pMesh->m_VB.push_back(pVB.get());
    pVB.release();

    // create a model and add the newly created mesh to it
    std::unique_ptr<Model> pModel = std::make_unique<Model>();
    pModel->m_Mesh.push_back(pMesh.get());
    pMesh.release();

    return pModel.release();
}
//---------------------------------------------------------------------------
Model* Factory::GetDisk(float                             centerX,
                        float                             centerY,
                        float                             radius,
                        std::size_t                       slices,
                  const VertexFormat&                     format,
                  const VertexCulling&                    culling,
                  const Material&                         material,
                  const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    // create a mesh to contain the shape
    std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

    // create a new vertex buffer to contain the mesh vertices
    std::unique_ptr<VertexBuffer> pVB = std::make_unique<VertexBuffer>();
    pVB->m_Material                   = material;
    pVB->m_Culling                    = culling;
    pVB->m_Format                     = format;

    // set the vertex format type
    pVB->m_Format.m_Type = DWF_Model::VertexFormat::IEType::IE_VT_TriangleFan;

    // calculate the stride
    pVB->m_Format.CalculateStride();

    // calculate the slice step
    const float step = (float)(2.0 * M_PI) / (float)slices;

    // iterate through disk slices to create
    for (std::size_t i = 0; i <= slices + 1; ++i)
    {
        float angle = 0.0f;
        float x;
        float y;

        // is the first point to calculate?
        if (!i)
        {
            // get the center
            x = centerX;
            y = centerY;
        }
        else
        {
            // calculate the current slice angle
            angle = step * (float)(i - 1);

            // calculate the slice point
            x = centerX + radius * cosf(angle);
            y = centerY + radius * sinf(angle);
        }

        DWF_Math::Vector3F vertex;

        // add min point in buffer
        vertex.m_X = x;
        vertex.m_Y = y;
        vertex.m_Z = 0.0f;

        DWF_Math::Vector3F normal;

        // vertex has normal coordinates?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
        {
            // set normal data
            normal.m_X = 0.0f;
            normal.m_Y = 0.0f;
            normal.m_Z = 1.0f;
        }

        DWF_Math::Vector2F uv;

        // vertex has UV texture coordinates?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
            // set texture data
            if (!i)
            {
                uv.m_X = 0.5f;
                uv.m_Y = 0.5f;
            }
            else
            {
                uv.m_X = 0.5f + (cosf(angle) * 0.5f);
                uv.m_Y = 0.5f + (sinf(angle) * 0.5f);
            }

        // add the vertex to the buffer
        pVB->Add(&vertex, &normal, &uv, i, fOnGetVertexColor);
    }

    pMesh->m_VB.push_back(pVB.get());
    pVB.release();

    // create a model and add the newly created mesh to it
    std::unique_ptr<Model> pModel = std::make_unique<Model>();
    pModel->m_Mesh.push_back(pMesh.get());
    pMesh.release();

    return pModel.release();
}
//---------------------------------------------------------------------------
Model* Factory::GetRing(float                             centerX,
                        float                             centerY,
                        float                             minRadius,
                        float                             maxRadius,
                        std::size_t                       slices,
                  const VertexFormat&                     format,
                  const VertexCulling&                    culling,
                  const Material&                         material,
                  const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    // create a mesh to contain the shape
    std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

    // create a new vertex buffer to contain the mesh vertices
    std::unique_ptr<VertexBuffer> pVB = std::make_unique<VertexBuffer>();
    pVB->m_Material                   = material;
    pVB->m_Culling                    = culling;
    pVB->m_Format                     = format;

    // set the vertex format type
    pVB->m_Format.m_Type = DWF_Model::VertexFormat::IEType::IE_VT_TriangleStrip;

    // calculate the stride
    pVB->m_Format.CalculateStride();

    // calculate the slice step
    const float step = (float)(2.0 * M_PI) / (float)slices;

    // iterate through ring slices to create
    for (std::size_t i = 0; i <= slices; ++i)
    {
        // calculate the current slice angle
        const float angle = step * (float)i;

        // calculate the slice min point
        const float xA = centerX + minRadius * cosf(angle);
        const float yA = centerY - minRadius * sinf(angle);

        // calculate the slice max point
        const float xB = centerX + maxRadius * cosf(angle);
        const float yB = centerY - maxRadius * sinf(angle);

        float texU;

        // calculate texture u coordinate
        if (!i)
            texU = 0.0f;
        else
        if (i == slices)
            texU = 1.0f;
        else
            texU = (float)i / (float)slices;

        DWF_Math::Vector3F vertex;

        // add min point in buffer
        vertex.m_X = xA;
        vertex.m_Y = yA;
        vertex.m_Z = 0.0f;

        DWF_Math::Vector3F normal;

        // vertex has normal coordinates?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
        {
            // set normal data
            normal.m_X = 0.0f;
            normal.m_Y = 0.0f;
            normal.m_Z = 1.0f;
        }

        DWF_Math::Vector2F uv;

        // vertex has UV texture coordinates?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
        {
            // set texture data
            uv.m_X = texU;
            uv.m_Y = 0.0f;
        }

        // add the vertex to the buffer
        pVB->Add(&vertex, &normal, &uv, (size_t)i * 2, fOnGetVertexColor);

        // add max point in the buffer
        vertex.m_X = xB;
        vertex.m_Y = yB;
        vertex.m_Z = 0.0f;

        // vertex has normal coordinates?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
        {
            // set normal data
            normal.m_X = 0.0f;
            normal.m_Y = 0.0f;
            normal.m_Z = 1.0f;
        }

        // vertex has UV texture coordinates?
        if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
        {
            // set texture data
            uv.m_X = texU;
            uv.m_Y = 1.0f;
        }

        // add the vertex to the buffer
        pVB->Add(&vertex, &normal, &uv, ((size_t)i * 2) + 1, fOnGetVertexColor);
    }

    pMesh->m_VB.push_back(pVB.get());
    pVB.release();

    // create a model and add the newly created mesh to it
    std::unique_ptr<Model> pModel = std::make_unique<Model>();
    pModel->m_Mesh.push_back(pMesh.get());
    pMesh.release();

    return pModel.release();
}
//---------------------------------------------------------------------------
Model* Factory::GetTorus(float                             centerX,
                         float                             centerY,
                   const DWF_Math::Vector2F&               innerRadius,
                   const DWF_Math::Vector2F&               outerRadius,
                         std::size_t                       slices,
                         std::size_t                       facesPerSlices,
                   const VertexFormat&                     format,
                   const VertexCulling&                    culling,
                   const Material&                         material,
                   const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    // no slice count?
    if (!slices)
        return nullptr;

    // no faces per slice count?
    if (!facesPerSlices)
        return nullptr;

    // create a mesh to contain the shape
    std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

    // calculate the steps to perform to achieve the torus
    const float totalAngle = 2.0f * (float)M_PI;
    const float uStep      = totalAngle / slices;
    const float vStep      = totalAngle / facesPerSlices;
    const float uTexStep   = 1.0f       / slices;
    const float vTexStep   = 1.0f       / facesPerSlices;

    // iterate through slices to create
    for (std::size_t u = 0; u < slices; ++u)
    {
        // create a new vertex buffer to contain the mesh vertices
        std::unique_ptr<VertexBuffer> pVB = std::make_unique<VertexBuffer>();
        pVB->m_Material                   = material;
        pVB->m_Culling                    = culling;
        pVB->m_Format                     = format;

        // set the vertex format type
        pVB->m_Format.m_Type = DWF_Model::VertexFormat::IEType::IE_VT_TriangleStrip;

        // calculate the stride
        pVB->m_Format.CalculateStride();

        const float theta     =  (u      * uStep);
        const float nextTheta = ((u + 1) * uStep);

        // iterate through faces to create
        for (std::size_t v = 0; v < facesPerSlices; ++v)
        {
            const float phi = (v * vStep);

            DWF_Math::Vector3F vertex;

            // calculate vertex
            vertex.m_X = std::cos(theta) * (outerRadius.m_X + innerRadius.m_X * std::cos(phi));
            vertex.m_Y = std::sin(theta) * (outerRadius.m_Y + innerRadius.m_X * std::cos(phi));
            vertex.m_Z =                                      innerRadius.m_Y * std::sin(phi);

            DWF_Math::Vector3F normal;

            // vertex has normal coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
            {
                // calculate normal
                normal.m_X = std::cos(theta) * std::cos(phi);
                normal.m_Y = std::sin(theta) * std::cos(phi);
                normal.m_Z =                   std::sin(phi);
            }

            DWF_Math::Vector2F texCoord;

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                texCoord.m_X = v * vTexStep;
                texCoord.m_Y = u * uTexStep;
            }

            // build slice start vertices
            pVB->Add(&vertex, &normal, &texCoord, pMesh->m_VB.size(), fOnGetVertexColor);

            // calculate next vertex
            vertex.m_X = std::cos(nextTheta) * (outerRadius.m_X + innerRadius.m_X * std::cos(phi));
            vertex.m_Y = std::sin(nextTheta) * (outerRadius.m_Y + innerRadius.m_X * std::cos(phi));
            vertex.m_Z =                                          innerRadius.m_Y * std::sin(phi);

            // vertex has normal coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
            {
                // calculate next normal
                normal.m_X = std::cos(nextTheta) * std::cos(phi);
                normal.m_Y = std::sin(nextTheta) * std::cos(phi);
                normal.m_Z =                       std::sin(phi);
            }

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                texCoord.m_X =  v      * vTexStep;
                texCoord.m_Y = (u + 1) * uTexStep;
            }

            // build slice end vertices
            pVB->Add(&vertex, &normal, &texCoord, pMesh->m_VB.size(), fOnGetVertexColor);
        }

        pMesh->m_VB.push_back(pVB.get());
        pVB.release();
    }

    // create a model and add the newly created mesh to it
    std::unique_ptr<Model> pModel = std::make_unique<Model>();
    pModel->m_Mesh.push_back(pMesh.get());
    pMesh.release();

    return pModel.release();
}
//---------------------------------------------------------------------------
Model* Factory::GetSpiral(float                             centerX,
                          float                             centerY,
                          float                             minRadius,
                          float                             maxRadius,
                          float                             deltaMin,
                          float                             deltaMax,
                          float                             deltaZ,
                          std::size_t                       slices,
                          std::size_t                       stacks,
                    const VertexFormat&                     format,
                    const VertexCulling&                    culling,
                    const Material&                         material,
                    const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    // create a mesh to contain the shape
    std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

    // calculate the slice step
    float step = (float)(2.0 * M_PI) / (float)slices;
    float z    = 0.0f;

    // iterate through spiral stacks to create
    for (std::size_t i = 0; i < stacks; ++i)
    {
        // create a new vertex buffer to contain the next slice
        std::unique_ptr<VertexBuffer> pVB = std::make_unique<VertexBuffer>();
        pVB->m_Material                   = material;
        pVB->m_Culling                    = culling;
        pVB->m_Format                     = format;

        // set the vertex format type
        pVB->m_Format.m_Type = DWF_Model::VertexFormat::IEType::IE_VT_TriangleStrip;

        // calculate the stride
        pVB->m_Format.CalculateStride();

        // iterate through spiral slices to create
        for (std::size_t j = 0; j <= slices; ++j)
        {
            // calculate the current slice angle
            const float angle = step * (float)j;

            // calculate the slice min point
            const float xA = centerX + minRadius * cosf(angle);
            const float yA = centerY + minRadius * sinf(angle);

            // calculate the slice max point
            const float xB = centerX + maxRadius * cosf(angle);
            const float yB = centerY + maxRadius * sinf(angle);

            // calculate the spiral curve
            minRadius += deltaMin;
            maxRadius += deltaMax;

            // calculate the z position
            z -= deltaZ;

            float texU;

            // calculate texture u coordinate
            if (!j)
                texU = 0.0f;
            else
            if (j == slices)
                texU = 1.0f;
            else
                texU = (float)j / (float)slices;

            DWF_Math::Vector3F vertex;

            // add min point in buffer
            vertex.m_X = xA;
            vertex.m_Y = yA;
            vertex.m_Z = z;

            DWF_Math::Vector3F normal;

            // vertex has a normal?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
            {
                // set normal data
                normal.m_X = 0.0f;
                normal.m_Y = 0.0f;
                normal.m_Z = 1.0f;
            }

            DWF_Math::Vector2F uv;

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                // set texture data
                uv.m_X = texU;
                uv.m_Y = 0.0f;
            }

            // add the vertex to the buffer
            pVB->Add(&vertex, &normal, &uv, j * 2, fOnGetVertexColor);

            // add max point in the buffer
            vertex.m_X = xB;
            vertex.m_Y = yB;
            vertex.m_Z = z;

            // vertex has a normal?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_Normals) != 0)
            {
                // set normal data
                normal.m_X = 0.0f;
                normal.m_Y = 0.0f;
                normal.m_Z = 1.0f;
            }

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                // set texture data
                uv.m_X = texU;
                uv.m_Y = 1.0f;
            }

            // add the vertex to the buffer
            pVB->Add(&vertex, &normal, &uv, (j * 2) + 1, fOnGetVertexColor);
        }

        // add the vertex buffer to the mesh
        pMesh->m_VB.push_back(pVB.get());
        pVB.release();

        // correct the last values otherwise the spiral will appears broken
        minRadius -= deltaMin;
        maxRadius -= deltaMax;
        z         += deltaZ;
    }

    // create a model and add the newly created mesh to it
    std::unique_ptr<Model> pModel = std::make_unique<Model>();
    pModel->m_Mesh.push_back(pMesh.get());
    pMesh.release();

    return pModel.release();
}
//---------------------------------------------------------------------------
DWF_Math::Vector3F Factory::GetCylinderVertex(const DWF_Math::Vector3F& top,
                                              const DWF_Math::Vector3F& localX,
                                              const DWF_Math::Vector3F& localY,
                                              const DWF_Math::Vector3F& localZ,
                                                    float               radius,
                                                    float               length,
                                                    float               u,
                                                    float               v)
{
    return top                                                 +
           localX * std::cosf(2.0f * (float)M_PI * u) * radius +
           localY * std::sinf(2.0f * (float)M_PI * u) * radius +
           localZ * v * length;

}
//---------------------------------------------------------------------------
DWF_Math::Vector3F Factory::GetSphereStartVertex(const DWF_Math::Vector3F& top,
                                                 const DWF_Math::Vector3F& localX,
                                                 const DWF_Math::Vector3F& localY,
                                                 const DWF_Math::Vector3F& localZ,
                                                       float               radius,
                                                       float               u,
                                                       float               v)
{
    const float latitude = (float)(M_PI / 2.0) * (v - 1);

    return top                                                                       +
           localX * std::cosf(2.0f * (float)M_PI * u) * std::cosf(latitude) * radius +
           localY * std::sinf(2.0f * (float)M_PI * u) * std::cosf(latitude) * radius +
           localZ * std::sinf(latitude) * radius;
}
//---------------------------------------------------------------------------
DWF_Math::Vector3F Factory::GetSphereEndVertex(const DWF_Math::Vector3F& bottom,
                                               const DWF_Math::Vector3F& localX,
                                               const DWF_Math::Vector3F& localY,
                                               const DWF_Math::Vector3F& localZ,
                                                     float               radius,
                                                     float               u,
                                                     float               v)
{
    const float latitude = (float)(M_PI / 2.0) * v;

    return bottom                                                                  +
           localX * std::cos(2.0f * (float)M_PI * u) * std::cos(latitude) * radius +
           localY * std::sin(2.0f * (float)M_PI * u) * std::cos(latitude) * radius +
           localZ * std::sin(latitude) * radius;
}
//---------------------------------------------------------------------------
DWF_Math::Vector3F Factory::GetAnyPerpendicularUnitVector(const DWF_Math::Vector3F& vec)
{
    if (vec.m_Y != 0.0f || vec.m_Z != 0.0f)
        return DWF_Math::Vector3F(1.0f, 0.0f, 0.0f);

    return DWF_Math::Vector3F(0.0f, 1.0f, 0.0f);
}
//---------------------------------------------------------------------------
