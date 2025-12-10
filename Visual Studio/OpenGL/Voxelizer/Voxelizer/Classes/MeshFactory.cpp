/****************************************************************************
 * ==> MeshFactory ---------------------------------------------------------*
 ****************************************************************************
 * Description: Mesh factory                                                *
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

#include "MeshFactory.h"

// std
#include <memory>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

// classes
#include "MemoryBuffer.h"
#include "Plane.h"

//---------------------------------------------------------------------------
// MeshFactory
//---------------------------------------------------------------------------
void MeshFactory::GetBox(      float                             width,
                               float                             height,
                               float                             depth,
                               bool                              repeatTexOnEachFace,
                         const VertexBuffer::IFormat&            format,
                         const VertexBuffer::ICulling&           culling,
                         const Material&                         material,
                               Mesh&                             mesh,
                         const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    // calculate half values
    const float halfX = width  / 2.0f;
    const float halfY = height / 2.0f;
    const float halfZ = depth  / 2.0f;

    // iterate through each edges
    for (std::size_t i = 0; i < 6; ++i)
    {
        // create a vertex buffer for each box edges
        std::unique_ptr<VertexBuffer> pVB(new VertexBuffer());

        // apply the user wished vertex format, culling and material
        pVB->m_Format   = format;
        pVB->m_Culling  = culling;
        pVB->m_Material = material;

        // set the vertex format type
        pVB->m_Format.m_Type = VertexBuffer::IFormat::IEType::IE_VT_TriangleStrip;

        // calculate the stride
        pVB->m_Format.CalculateStride();

        mesh.m_VBs.push_back(pVB.get());
        pVB.release();
    }

    Vector3F vertices[8];

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
            vertices[i].m_X = halfX;

        // generate 2 vertices on the front, then 2 vertices on the back
        if (!((i / 2) % 2))
            vertices[i].m_Z = -halfZ;
        else
            vertices[i].m_Z = halfZ;

        // for each vertices, generates one on the top, and one on the bottom
        if (!(i % 2))
            vertices[i].m_Y = -halfY;
        else
            vertices[i].m_Y = halfY;
    }

    Vector3F normals[6];

    // calculate normals
    normals[0].m_X = -1.0; normals[0].m_Y =  0.0; normals[0].m_Z =  0.0;
    normals[1].m_X =  1.0; normals[1].m_Y =  0.0; normals[1].m_Z =  0.0;
    normals[2].m_X =  0.0; normals[2].m_Y = -1.0; normals[2].m_Z =  0.0;
    normals[3].m_X =  0.0; normals[3].m_Y =  1.0; normals[3].m_Z =  0.0;
    normals[4].m_X =  0.0; normals[4].m_Y =  0.0; normals[4].m_Z = -1.0;
    normals[5].m_X =  0.0; normals[5].m_Y =  0.0; normals[5].m_Z =  1.0;

    Vector2F texCoords[24];

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
    mesh.m_VBs[0]->Add(&vertices[1], &normals[0], &texCoords[4], 0, fOnGetVertexColor);
    mesh.m_VBs[0]->Add(&vertices[0], &normals[0], &texCoords[5], 1, fOnGetVertexColor);
    mesh.m_VBs[0]->Add(&vertices[3], &normals[0], &texCoords[6], 2, fOnGetVertexColor);
    mesh.m_VBs[0]->Add(&vertices[2], &normals[0], &texCoords[7], 3, fOnGetVertexColor);

    // create box edge 2
    mesh.m_VBs[1]->Add(&vertices[3], &normals[5], &texCoords[8],  0, fOnGetVertexColor);
    mesh.m_VBs[1]->Add(&vertices[2], &normals[5], &texCoords[9],  1, fOnGetVertexColor);
    mesh.m_VBs[1]->Add(&vertices[7], &normals[5], &texCoords[10], 2, fOnGetVertexColor);
    mesh.m_VBs[1]->Add(&vertices[6], &normals[5], &texCoords[11], 3, fOnGetVertexColor);

    // create box edge 3
    mesh.m_VBs[2]->Add(&vertices[7], &normals[1], &texCoords[12], 0, fOnGetVertexColor);
    mesh.m_VBs[2]->Add(&vertices[6], &normals[1], &texCoords[13], 1, fOnGetVertexColor);
    mesh.m_VBs[2]->Add(&vertices[5], &normals[1], &texCoords[14], 2, fOnGetVertexColor);
    mesh.m_VBs[2]->Add(&vertices[4], &normals[1], &texCoords[15], 3, fOnGetVertexColor);

    // create box edge 4
    mesh.m_VBs[3]->Add(&vertices[5], &normals[4], &texCoords[16], 0, fOnGetVertexColor);
    mesh.m_VBs[3]->Add(&vertices[4], &normals[4], &texCoords[17], 1, fOnGetVertexColor);
    mesh.m_VBs[3]->Add(&vertices[1], &normals[4], &texCoords[18], 2, fOnGetVertexColor);
    mesh.m_VBs[3]->Add(&vertices[0], &normals[4], &texCoords[19], 3, fOnGetVertexColor);

    // create box edge 5
    mesh.m_VBs[4]->Add(&vertices[1], &normals[3], &texCoords[0], 0, fOnGetVertexColor);
    mesh.m_VBs[4]->Add(&vertices[3], &normals[3], &texCoords[1], 1, fOnGetVertexColor);
    mesh.m_VBs[4]->Add(&vertices[5], &normals[3], &texCoords[2], 2, fOnGetVertexColor);
    mesh.m_VBs[4]->Add(&vertices[7], &normals[3], &texCoords[3], 3, fOnGetVertexColor);

    // create box edge 6
    mesh.m_VBs[5]->Add(&vertices[2], &normals[2], &texCoords[20], 0, fOnGetVertexColor);
    mesh.m_VBs[5]->Add(&vertices[0], &normals[2], &texCoords[21], 1, fOnGetVertexColor);
    mesh.m_VBs[5]->Add(&vertices[6], &normals[2], &texCoords[22], 2, fOnGetVertexColor);
    mesh.m_VBs[5]->Add(&vertices[4], &normals[2], &texCoords[23], 3, fOnGetVertexColor);
}
//---------------------------------------------------------------------------
void MeshFactory::GetSphere(      float                             radius,
                                  std::size_t                       slices,
                                  std::size_t                       stacks,
                            const VertexBuffer::IFormat&            format,
                            const VertexBuffer::ICulling&           culling,
                            const Material&                         material,
                                  Mesh&                             mesh,
                            const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    if (!slices || !stacks)
        return;

    // initialize global values
    const float majorStep = ((float)M_PI         / slices);
    const float minorStep = ((float)(2.0 * M_PI) / stacks);

    // iterate through vertex slices
    for (std::size_t i = 0; i < slices; ++i)
    {
        // create a new vertex buffer to contain the next slice
        std::unique_ptr<VertexBuffer> pVB(new VertexBuffer());

        // apply the user wished vertex format, culling and material
        pVB->m_Format   = format;
        pVB->m_Culling  = culling;
        pVB->m_Material = material;

        // set the vertex format type
        pVB->m_Format.m_Type = VertexBuffer::IFormat::IEType::IE_VT_TriangleStrip;

        // calculate the stride
        pVB->m_Format.CalculateStride();

        // calculate next slice values
        const float a  = i * majorStep;
        const float b  = a + majorStep;
        const float r0 = radius * std::sinf(a);
        const float r1 = radius * std::sinf(b);
        const float z0 = radius * std::cosf(a);
        const float z1 = radius * std::cosf(b);

        // iterate through vertex stacks
        for (std::size_t j = 0; j <= stacks; ++j)
        {
            const float c = j * minorStep;
            const float x = std::cosf(c);
            const float y = std::sinf(c);

            Vector3F vertex;

            // calculate vertex
            vertex.m_X = x * r0;
            vertex.m_Y = y * r0;
            vertex.m_Z =     z0;

            Vector3F normal;

            // vertex has a normal?
            if (((std::size_t)pVB->m_Format.m_Format & (std::size_t)VertexBuffer::IFormat::IEFormat::IE_VF_Normals) != 0)
            {
                normal.m_X = (x * r0) / radius;
                normal.m_Y = (y * r0) / radius;
                normal.m_Z =      z0  / radius;
            }

            Vector2F uv;

            // vertex has UV texture coordinates?
            if (((std::size_t)pVB->m_Format.m_Format & (std::size_t)VertexBuffer::IFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                uv.m_X = ((float)j / (float)stacks);
                uv.m_Y = ((float)i / (float)slices);
            }

            // add the vertex to the buffer
            pVB->Add(&vertex,
                     &normal,
                     &uv,
                     j * 2,
                     fOnGetVertexColor);

            // calculate vertex
            vertex.m_X = x * r1;
            vertex.m_Y = y * r1;
            vertex.m_Z =     z1;

            // vertex has a normal?
            if (((std::size_t)pVB->m_Format.m_Format & (std::size_t)VertexBuffer::IFormat::IEFormat::IE_VF_Normals) != 0)
            {
                normal.m_X = (x * r1) / radius;
                normal.m_Y = (y * r1) / radius;
                normal.m_Z =      z1  / radius;
            }

            // vertex has UV texture coordinates?
            if (((std::size_t)pVB->m_Format.m_Format & (std::size_t)VertexBuffer::IFormat::IEFormat::IE_VF_TexCoords) != 0)
            {
                uv.m_X = ((float)j / (float)stacks);
                uv.m_Y = (((float)i + 1.0f) / (float)slices);
            }

            // add the vertex to the buffer
            pVB->Add(&vertex,
                     &normal,
                     &uv,
                     (j * 2) + 1,
                     fOnGetVertexColor);
        }

        mesh.m_VBs.push_back(pVB.get());
        pVB.release();
    }
}
//---------------------------------------------------------------------------
void MeshFactory::GetCapsule(      float                             height,
                                   float                             radius,
                                   float                             resolution,
                             const VertexBuffer::IFormat&            format,
                             const VertexBuffer::ICulling&           culling,
                             const Material&                         material,
                                   Mesh&                             mesh,
                             const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    if (radius == 0.0f)
        return;

    const float third     = 1.0f / 3.0f;
    const float twoThirds = 2.0f / 3.0f;

    const Vector3F capsuleTop   (0.0f, height, 0.0f);
    const Vector3F capsuleBottom(0.0f, 0.0f, 0.0f);

    // this capsule
    const Vector3F lineDir       = (capsuleTop - capsuleBottom).Normalize();
    const Vector3F lineEndOffset = lineDir * radius;
    const Vector3F top           = capsuleTop    - lineEndOffset;
    const Vector3F bottom        = capsuleBottom + lineEndOffset;

    const Vector3F axis   = bottom - top;
    const float    length = axis.Length();
    const Vector3F localZ = axis / (length ? length : 1.0f);
    const Vector3F localX = GetAnyPerpendicularUnitVector(localZ);
    const Vector3F localY = localZ.Cross(localX);

    const Vector3F start(0.0f, 0.0f, 0.0f);
    const Vector3F end  (1.0f, 1.0f, 1.0f);

    const Vector3F step = (end - start) / resolution;

    std::unique_ptr<VertexBuffer> pVB(new VertexBuffer());
    pVB->m_Material = material;
    pVB->m_Culling  = culling;
    pVB->m_Format   = format;
    pVB->m_Format.CalculateStride();

    Vector3F p0, p1, p2, p3, normal0, normal1, normal2, normal3;
    Vector2F uv0, uv1, uv2, uv3;

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
            p0 = GetCylinderVertex(top, localX, localY, localZ, radius, length, u, v);
            p1 = GetCylinderVertex(top, localX, localY, localZ, radius, length, u, vn);
            p2 = GetCylinderVertex(top, localX, localY, localZ, radius, length, un, v);
            p3 = GetCylinderVertex(top, localX, localY, localZ, radius, length, un, vn);

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexBuffer::IFormat::IEFormat::IE_VF_Normals) != 0)
            {
                // calculate the vertex normals
                normal0 = p0 / radius;
                normal1 = p1 / radius;
                normal2 = p2 / radius;
                normal3 = p3 / radius;
            }

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexBuffer::IFormat::IEFormat::IE_VF_TexCoords) != 0)
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
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexBuffer::IFormat::IEFormat::IE_VF_Normals) != 0)
            {
                // calculate the vertex normals
                normal0 = p0 / radius;
                normal1 = p1 / radius;
                normal2 = p2 / radius;
                normal3 = p3 / radius;
            }

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexBuffer::IFormat::IEFormat::IE_VF_TexCoords) != 0)
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

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexBuffer::IFormat::IEFormat::IE_VF_Normals) != 0)
            {
                // calculate the vertex normals
                normal0 = p0 / radius;
                normal1 = p1 / radius;
                normal2 = p2 / radius;
                normal3 = p3 / radius;
            }

            // vertex has UV texture coordinates?
            if (((std::uint32_t)pVB->m_Format.m_Format & (std::uint32_t)VertexBuffer::IFormat::IEFormat::IE_VF_TexCoords) != 0)
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

    mesh.m_VBs.push_back(pVB.get());
    pVB.release();
}
//---------------------------------------------------------------------------
bool MeshFactory::GetLandscape(const PixelBuffer*                      pPixelBuffer,
                                     float                             height,
                                     float                             scale,
                               const VertexBuffer::IFormat&            format,
                               const VertexBuffer::ICulling&           culling,
                               const Material&                         material,
                                     Mesh&                             mesh,
                               const VertexBuffer::ITfOnGetVertexColor fOnGetVertexColor)
{
    // validate the inputs
    if (!pPixelBuffer || height <= 0.0f || scale == 0.0f)
        return false;

    // create a new vertex buffer to contain the landscape
    std::unique_ptr<VertexBuffer> pVB(new VertexBuffer());

    // apply the user wished vertex format
    pVB->m_Format = format;

    // apply the user wished vertex culling
    pVB->m_Culling = culling;

    // apply the user wished material
    pVB->m_Material = material;

    // set the vertex format type
    pVB->m_Format.m_Type = VertexBuffer::IFormat::IEType::IE_VT_Triangles;

    // calculate the stride
    pVB->m_Format.CalculateStride();

    IVertices vertices;

    // generate landscape XYZ vertex from grayscale image
    if (!GenerateLandscapeVertices(pPixelBuffer, height, scale, vertices))
        return false;

    PlaneF p1;
    PlaneF p2;

    // loop through landscape XYZ vertices and generate mesh polygons
    for (std::size_t z = 0; z < (std::size_t)pPixelBuffer->m_Height - 1; ++z)
        for (std::size_t x = 0; x < (std::size_t)pPixelBuffer->m_Width - 1; ++x)
        {
            // polygons are generated in the following order:
            // v1 -- v2
            //     /
            //    /
            // v3 -- v4

            // calculate vertex index
            std::size_t index = (z * pPixelBuffer->m_Width) + x;

            const Vector3F& v1 = vertices[index];
            const Vector3F& v2 = vertices[index + 1];

            const std::size_t i1 = index;
            const std::size_t i2 = index + 1;

            // calculate next vertex index
            index = ((z + 1) * pPixelBuffer->m_Width) + x;

            const Vector3F& v3 = vertices[index];
            const Vector3F& v4 = vertices[index + 1];

            const std::size_t i3 = index;
            const std::size_t i4 = index + 1;

            Vector3F n1;
            Vector3F n2;

            // do include normals?
            if ((unsigned)pVB->m_Format.m_Format & (unsigned)VertexBuffer::IFormat::IEFormat::IE_VF_Normals)
            {
                // calculate polygons planes
                p1.FromPoints(v1, v2, v3);
                p2.FromPoints(v2, v3, v4);

                // get first normal
                n1.m_X = p1.m_A;
                n1.m_Y = p1.m_B;
                n1.m_Z = p1.m_C;

                // get second normal
                n2.m_X = p2.m_A;
                n2.m_Y = p2.m_B;
                n2.m_Z = p2.m_C;
            }

            Vector2F uv1;
            Vector2F uv2;
            Vector2F uv3;
            Vector2F uv4;

            // do include colors?
            if ((unsigned)pVB->m_Format.m_Format & (unsigned)VertexBuffer::IFormat::IEFormat::IE_VF_TexCoords)
            {
                // generate texture coordinates
                uv1.m_X = (float)(x)     / (float)(pPixelBuffer->m_Width);
                uv1.m_Y = (float)(z)     / (float)(pPixelBuffer->m_Height);
                uv2.m_X = (float)(x + 1) / (float)(pPixelBuffer->m_Width);
                uv2.m_Y = (float)(z)     / (float)(pPixelBuffer->m_Height);
                uv3.m_X = (float)(x)     / (float)(pPixelBuffer->m_Width);
                uv3.m_Y = (float)(z + 1) / (float)(pPixelBuffer->m_Height);
                uv4.m_X = (float)(x + 1) / (float)(pPixelBuffer->m_Width);
                uv4.m_Y = (float)(z + 1) / (float)(pPixelBuffer->m_Height);
            }

            // add first polygon first vertex to buffer
            pVB->Add(&v1, &n1, &uv1, i1, fOnGetVertexColor);

            // add first polygon second vertex to buffer
            pVB->Add(&v2, &n1, &uv2, i2, fOnGetVertexColor);

            // add first polygon third vertex to buffer
            pVB->Add(&v3, &n1, &uv3, i3, fOnGetVertexColor);

            // add second polygon first vertex to buffer
            pVB->Add(&v2, &n2, &uv2, i2, fOnGetVertexColor);

            // add second polygon second vertex to buffer
            pVB->Add(&v3, &n2, &uv3, i3, fOnGetVertexColor);

            // add second polygon third vertex to buffer
            pVB->Add(&v4, &n2, &uv4, i4, fOnGetVertexColor);
        }

    mesh.m_VBs.push_back(pVB.get());
    pVB.release();

    return true;
}
//---------------------------------------------------------------------------
Vector3F MeshFactory::GetCylinderVertex(const Vector3F& top,
                                        const Vector3F& localX,
                                        const Vector3F& localY,
                                        const Vector3F& localZ,
                                              float     radius,
                                              float     length,
                                              float     u,
                                              float     v)
{
    return top                                                 +
           localX * std::cosf(2.0f * (float)M_PI * u) * radius +
           localY * std::sinf(2.0f * (float)M_PI * u) * radius +
           localZ * v * length;

}
//---------------------------------------------------------------------------
Vector3F MeshFactory::GetSphereStartVertex(const Vector3F& top,
                                           const Vector3F& localX,
                                           const Vector3F& localY,
                                           const Vector3F& localZ,
                                                 float     radius,
                                                 float     u,
                                                 float     v)
{
    const float latitude = (float)(M_PI / 2.0) * (v - 1);

    return top                                                                       +
           localX * std::cosf(2.0f * (float)M_PI * u) * std::cosf(latitude) * radius +
           localY * std::sinf(2.0f * (float)M_PI * u) * std::cosf(latitude) * radius +
           localZ * std::sinf(latitude) * radius;
}
//---------------------------------------------------------------------------
Vector3F MeshFactory::GetSphereEndVertex(const Vector3F& bottom,
                                         const Vector3F& localX,
                                         const Vector3F& localY,
                                         const Vector3F& localZ,
                                               float     radius,
                                               float     u,
                                               float     v)
{
    const float latitude = (float)(M_PI / 2.0) * v;

    return bottom                                                                  +
           localX * std::cos(2.0f * (float)M_PI * u) * std::cos(latitude) * radius +
           localY * std::sin(2.0f * (float)M_PI * u) * std::cos(latitude) * radius +
           localZ * std::sin(latitude) * radius;
}
//---------------------------------------------------------------------------
Vector3F MeshFactory::GetAnyPerpendicularUnitVector(const Vector3F& vec)
{
    if (vec.m_Y != 0.0f || vec.m_Z != 0.0f)
        return Vector3F(1.0f, 0.0f, 0.0f);

    return Vector3F(0.0f, 1.0f, 0.0f);
}
//---------------------------------------------------------------------------
bool MeshFactory::GenerateLandscapeVertices(const PixelBuffer* pPixelBuffer,
                                                  float            height,
                                                  float            scale,
                                                  IVertices&       vertices)
{
    // validate the inputs
    if (!pPixelBuffer || height <= 0.0f || scale == 0.0f)
        return false;

    // calculate landscape data size and reserve memory for landscape mesh
    vertices.resize((std::size_t)pPixelBuffer->m_Width * (std::size_t)pPixelBuffer->m_Height);

    // calculate scaling factor on x and z axis
    const float scaleX = -(((pPixelBuffer->m_Width  - 1) * scale) / 2.0f);
    const float scaleZ =  (((pPixelBuffer->m_Height - 1) * scale) / 2.0f);

    // loop through height field points and calculate coordinates for each point
    for (std::size_t z = 0; z < pPixelBuffer->m_Height; ++z)
        for (std::size_t x = 0; x < pPixelBuffer->m_Width; ++x)
        {
            // calculate vertex index
            size_t index = (z * pPixelBuffer->m_Width) + x;
            float  value = (float)(((unsigned char*)pPixelBuffer->m_pData)[index * 3]) / 255.0f;

            // calculate landscape vertex
            vertices[index].m_X = scaleX + ((float)x * scale);
            vertices[index].m_Y = value  * height;
            vertices[index].m_Z = scaleZ - ((float)z * scale);
        }

    return true;
}
//---------------------------------------------------------------------------
