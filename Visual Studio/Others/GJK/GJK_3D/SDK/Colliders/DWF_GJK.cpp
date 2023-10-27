/****************************************************************************
 * ==> DWF_GJK -------------------------------------------------------------*
 ****************************************************************************
 * Description : Gilbert-Johnson-Keerthi intersection algorithm             *
 *               implementation                                             *
 * Developer   : Jean-Milost Reymond                                        *
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

#include "DWF_GJK.h"

#define GJK_MAX_NUM_ITERATIONS  64
#define EPA_TOLERANCE           0.0001f
#define EPA_MAX_NUM_FACES       64
#define EPA_MAX_NUM_LOOSE_EDGES 32
#define EPA_MAX_NUM_ITERATIONS  64

using namespace DWF_Collider;

//---------------------------------------------------------------------------
// GJK
//---------------------------------------------------------------------------
GJK::GJK()
{}
//---------------------------------------------------------------------------
GJK::~GJK()
{}
//---------------------------------------------------------------------------
bool GJK::Resolve(const Collider& c1, const Collider& c2, DWF_Math::Vector3F& mtv)
{
    // simplex, it's just a set of points (a is always most recently added)
    DWF_Math::Vector3F a, b, c, d;

    // initial search direction between colliders
    DWF_Math::Vector3F searchDir = c1.m_Pos - c2.m_Pos;

    // get initial point for simplex
    c = c2.Support(searchDir) - c1.Support(-searchDir);

    // search in direction of origin
    searchDir = -c;

    // get second point for a line segment simplex
    b = c2.Support(searchDir) - c1.Support(-searchDir);

    //we didn't reach the origin, won't enclose it
    if (b.Dot(searchDir) < 0.0f)
        return false;

    // search perpendicular to line segment towards origin
    searchDir = ((c - b).Cross(-b)).Cross(c - b);

    // origin is on this line segment
    if (searchDir == DWF_Math::Vector3F())
    {
        // apparently any normal search vector will do? Perform the normal with x-axis
        searchDir = (c - b).Cross(DWF_Math::Vector3F(1.0f, 0.0f, 0.0f));

        // normal with z-axis
        if (searchDir == DWF_Math::Vector3F())
            searchDir = (c - b).Cross(DWF_Math::Vector3F(0.0f, 0.0f, -1.0f));
    }

    // simplex dimension
    std::size_t simpDim = 2;

    for (std::size_t iterations = 0; iterations < GJK_MAX_NUM_ITERATIONS; ++iterations)
    {
        a = c2.Support(searchDir) - c1.Support(-searchDir);

        // we didn't reach the origin, won't enclose it
        if (a.Dot(searchDir) < 0.0f)
            return false;

        ++simpDim;

        if (simpDim == 3)
            UpdateSimplex3(a, b, c, d, simpDim, searchDir);
        else
        if (UpdateSimplex4(a, b, c, d, simpDim, searchDir))
        {
            mtv = FindMinTranslationVec(a, b, c, d, c1, c2);
            return true;
        }
    }

    return false;
}
//---------------------------------------------------------------------------
void GJK::UpdateSimplex3(DWF_Math::Vector3F& a,
                         DWF_Math::Vector3F& b,
                         DWF_Math::Vector3F& c,
                         DWF_Math::Vector3F& d,
                         std::size_t&        simpDim,
                         DWF_Math::Vector3F& searchDir)
{
    // below is the required winding order
    //
    //  b
    //  |\
    //  | \
    //  |  \ a
    //  |  /
    //  | /
    //  |/
    //  c

    // triangle normal
    const DWF_Math::Vector3F n = (b - a).Cross(c - a);

    // direction to origin
    const DWF_Math::Vector3F AO = -a;

    // determine which feature is closest to origin, make that the new simplex
    simpDim = 2;

    // closest to edge AB
    if (((b - a).Cross(n)).Dot(AO) > 0.0f)
    {
        c         = a;
        searchDir = ((b - a).Cross(AO)).Cross(b - a);
        return;
    }

    // closest to edge AC
    if ((n.Cross(c - a)).Dot(AO) > 0.0f)
    {
        b         = a;
        searchDir = ((c - a).Cross(AO)).Cross(c - a);
        return;
    }

    simpDim = 3;

    // above triangle
    if (n.Dot(AO) > 0.0f)
    {
        d         = c;
        c         = b;
        b         = a;
        searchDir = n;
        return;
    }

    // below triangle
    d         = b;
    b         = a;
    searchDir = -n;
}
//---------------------------------------------------------------------------
bool GJK::UpdateSimplex4(DWF_Math::Vector3F& a,
                         DWF_Math::Vector3F& b,
                         DWF_Math::Vector3F& c,
                         DWF_Math::Vector3F& d,
                         std::size_t&        simpDim,
                         DWF_Math::Vector3F& searchDir)
{
    // a is peak/tip of pyramid, BCD is the base (counterclockwise winding order)
    // we know a priori that origin is above BCD and below a

    // get new faces normals
    const DWF_Math::Vector3F ABC = (b - a).Cross(c - a);
    const DWF_Math::Vector3F ACD = (c - a).Cross(d - a);
    const DWF_Math::Vector3F ADB = (d - a).Cross(b - a);

    // dir to origin
    const DWF_Math::Vector3F AO = -a;

    // hoisting this just cause
    simpDim = 3;

    // plane-test origin with 3 faces. NOTE kind of primitive approach used here. If origin is
    // in front of a face, just use it as the new simplex. Otherwise iterate through the faces
    // sequentially and exit at the first one which satisfies dot product. Perhaps this isn't
    // optimal or edges should be considered as possible simplexes,, but it seems that this
    // method is good enough. Makes no difference for AABBS, should test with more complex
    // colliders

    // in front of ABC
    if (ABC.Dot(AO) > 0.0f)
    {
        d         = c;
        c         = b;
        b         = a;
        searchDir = ABC;

        return false;
    }

    // in front of ACD
    if (ACD.Dot(AO) > 0.0f)
    {
        b         = a;
        searchDir = ACD;

        return false;
    }

    // in front of ADB
    if (ADB.Dot(AO) > 0.0f)
    {
        c         = d;
        d         = b;
        b         = a;
        searchDir = ADB;

        return false;
    }

    // inside tetrahedron, enclosed! NOTE in case where two faces have similar normals,
    // the origin could eventually be close to an edge of the tetrahedron. It would
    // probably not make a difference to limit new simplexes to just one of the faces,
    // maybe test it later
    return true;
}
//---------------------------------------------------------------------------
DWF_Math::Vector3F GJK::FindMinTranslationVec(const DWF_Math::Vector3F& a,
                                              const DWF_Math::Vector3F& b,
                                              const DWF_Math::Vector3F& c,
                                              const DWF_Math::Vector3F& d,
                                              const Collider&           c1,
                                              const Collider&           c2)
{
    // array of faces, each with 3 vertices and a normal
    DWF_Math::Vector3F faces[EPA_MAX_NUM_FACES][4];

    // init with final simplex from GJK
    faces[0][0] = a;
    faces[0][1] = b;
    faces[0][2] = c;
    faces[0][3] = ((b - a).Cross(c - a)).Normalize(); // ABC

    faces[1][0] = a;
    faces[1][1] = c;
    faces[1][2] = d;
    faces[1][3] = ((c - a).Cross(d - a)).Normalize(); // ACD

    faces[2][0] = a;
    faces[2][1] = d;
    faces[2][2] = b;
    faces[2][3] = ((d - a).Cross(b - a)).Normalize(); // ADB

    faces[3][0] = b;
    faces[3][1] = d;
    faces[3][2] = c;
    faces[3][3] = ((d - b).Cross(c - b)).Normalize(); // BDC

    std::size_t numFaces = 4;
    std::size_t closestFace;

    for (std::size_t iterations = 0; iterations < EPA_MAX_NUM_ITERATIONS; ++iterations)
    {
        // find face that's closest to origin
        float minDist = faces[0][0].Dot(faces[0][3]);
        closestFace   = 0;

        for (std::size_t i = 1; i < numFaces; ++i)
        {
            float dist = faces[i][0].Dot(faces[i][3]);

            if (dist < minDist)
            {
                minDist     = dist;
                closestFace = i;
            }
        }

        //search normal to face that's closest to origin
        const DWF_Math::Vector3F searchDir = faces[closestFace][3];
        const DWF_Math::Vector3F p         = c2.Support(searchDir) - c1.Support(-searchDir);

        if (p.Dot(searchDir) - minDist < EPA_TOLERANCE)
        {
            // convergence (new point is not significantly further from origin), dot vertex
            // with normal to resolve collision along normal!
            return faces[closestFace][3] * p.Dot(searchDir);
        }

        // keep track of edges we need to fix after removing faces
        DWF_Math::Vector3F looseEdges[EPA_MAX_NUM_LOOSE_EDGES][2];
        std::size_t        numLooseEdges = 0;

        // find all triangles that are facing p
        for (std::size_t i = 0; i < numFaces; ++i)
        {
            // triangle i faces p, remove it
            if (faces[i][3].Dot(p - faces[i][0]) > 0.0f)
            {
                // add removed triangle's edges to loose edge list. If it's already there, remove it
                // (both triangles it belonged to are gone)
                for (std::size_t j = 0; j < 3; ++j) //Three edges per face
                {
                    DWF_Math::Vector3F currentEdge[2] =
                    {
                        faces[i][j],
                        faces[i][(j + 1) % 3]
                    };

                    bool foundEdge = false;

                    // check if current edge is already in list
                    for (std::size_t k = 0; k < numLooseEdges; ++k)
                        if (looseEdges[k][1] == currentEdge[0] && looseEdges[k][0] == currentEdge[1])
                        {
                            // edge is already in the list, remove it
                            // THIS ASSUMES EDGE CAN ONLY BE SHARED BY 2 TRIANGLES (which should be true)
                            // THIS ALSO ASSUMES SHARED EDGE WILL BE REVERSED IN THE TRIANGLES (which 
                            // should be true provided every triangle is wound CCW)
                            looseEdges[k][0] = looseEdges[numLooseEdges - 1][0]; //Overwrite current edge
                            looseEdges[k][1] = looseEdges[numLooseEdges - 1][1]; //with last edge in list
                            --numLooseEdges;

                            foundEdge = true;

                            // exit loop because edge can only be shared once
                            k = numLooseEdges;
                        }

                    if (!foundEdge)
                    {
                        // add current edge to list
                        // assert(num_loose_edges<EPA_MAX_NUM_LOOSE_EDGES);
                        if (numLooseEdges >= EPA_MAX_NUM_LOOSE_EDGES)
                            break;

                        looseEdges[numLooseEdges][0] = currentEdge[0];
                        looseEdges[numLooseEdges][1] = currentEdge[1];
                        ++numLooseEdges;
                    }
                }

                // remove triangle i from list
                faces[i][0] = faces[numFaces - 1][0];
                faces[i][1] = faces[numFaces - 1][1];
                faces[i][2] = faces[numFaces - 1][2];
                faces[i][3] = faces[numFaces - 1][3];

                --numFaces;
                --i;
            }
        }

        // reconstruct polytope with p added
        for (std::size_t i = 0; i < numLooseEdges; ++i)
        {
            // assert(num_faces<EPA_MAX_NUM_FACES);
            if (numFaces >= EPA_MAX_NUM_FACES)
                break;

            faces[numFaces][0] = looseEdges[i][0];
            faces[numFaces][1] = looseEdges[i][1];
            faces[numFaces][2] = p;
            faces[numFaces][3] = ((looseEdges[i][0] - looseEdges[i][1]).Cross(looseEdges[i][0] - p)).Normalize();

            // check for wrong normal to maintain CCW winding
            const float bias = 0.000001f; //in case dot result is only slightly < 0 (because origin is on face)

            if (faces[numFaces][0].Dot(faces[numFaces][3]) + bias < 0)
            {
                DWF_Math::Vector3F temp =  faces[numFaces][0];
                faces[numFaces][0]      =  faces[numFaces][1];
                faces[numFaces][1]      =  temp;
                faces[numFaces][3]      = -faces[numFaces][3];
            }

            ++numFaces;
        }
    }

    // return most recent closest point
    return faces[closestFace][3] * faces[closestFace][0].Dot(faces[closestFace][3]);
}
//---------------------------------------------------------------------------
