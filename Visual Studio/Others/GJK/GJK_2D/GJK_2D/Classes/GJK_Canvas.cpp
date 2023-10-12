/****************************************************************************
 * ==> GJK_Canvas ----------------------------------------------------------*
 ****************************************************************************
 * Description : A canvas on which the figures used for show and test the   *
 *               GJK algorithm can be drawn                                 *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - GJK 2D                                                     *
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

#include "GJK_Canvas.h"

//------------------------------------------------------------------------------
// GJK_Canvas::IPolygon
//------------------------------------------------------------------------------
GJK_Canvas::IPolygon::IPolygon()
{}
//------------------------------------------------------------------------------
GJK_Canvas::IPolygon::~IPolygon()
{}
//------------------------------------------------------------------------------
// GJK_Canvas
//------------------------------------------------------------------------------
GJK_Canvas::GJK_Canvas(HWND hWnd) :
    m_hWnd(hWnd)
{
    // triangular polygons
    std::vector<GJK_Vector2F> vertices;
    vertices.push_back(GJK_Vector2F(-7.0f, 5.0f));
    vertices.push_back(GJK_Vector2F(-5.0f,  3.0f));
    vertices.push_back(GJK_Vector2F(-7.0f, 1.0f));
    vertices.push_back(GJK_Vector2F( 5.0f,  4.0f));
    vertices.push_back(GJK_Vector2F( 4.0f,  3.0f));
    vertices.push_back(GJK_Vector2F( 5.0f,  2.0f));

    AddPolygons(2, 3, vertices);
}
//------------------------------------------------------------------------------
GJK_Canvas::~GJK_Canvas()
{}
//------------------------------------------------------------------------------
void GJK_Canvas::Draw()
{
    // get the window client rect
    RECT clientRect;
    ::GetClientRect(m_hWnd, &clientRect);

    const std::size_t gridCount = 20;

    m_ZoomFactorX = (clientRect.right  - clientRect.left) / gridCount;
    m_ZoomFactorY = (clientRect.bottom - clientRect.top)  / gridCount;

    // get the window device context
    HDC hDC = ::GetDC(m_hWnd);

    // fill background
    HBRUSH hBgBrush = ::CreateSolidBrush(RGB(20, 30, 43));
    ::FillRect(hDC, &clientRect, hBgBrush);
    ::DeleteObject(hBgBrush);

    HPEN hGridPen    = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HPEN hOldGridPen = (HPEN)::SelectObject(hDC, hGridPen);

    // draw grid, x axis
    for (std::size_t i = 0; i < gridCount + 1; ++i)
    {
        ::MoveToEx(hDC, i * m_ZoomFactorX, clientRect.top, (LPPOINT)nullptr);
        ::LineTo  (hDC, i * m_ZoomFactorX, clientRect.bottom);
    }

    // draw grid, y axis
    for (std::size_t i = 0; i < gridCount + 1; ++i)
    {
        ::MoveToEx(hDC, clientRect.left,  i * m_ZoomFactorY, (LPPOINT)nullptr);
        ::LineTo  (hDC, clientRect.right, i * m_ZoomFactorY);
    }

    ::SelectObject(hDC, hOldGridPen);
    ::DeleteObject(hGridPen);

    const std::size_t centerX = (gridCount / 2) * m_ZoomFactorX;
    const std::size_t centerY = (gridCount / 2) * m_ZoomFactorY;

    HPEN hAxisPen    = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN hOldAxisPen = (HPEN)::SelectObject(hDC, hAxisPen);

    // draw red axis
    ::MoveToEx(hDC, centerX,          clientRect.top, (LPPOINT)nullptr);
    ::LineTo  (hDC, centerX,          clientRect.bottom);
    ::MoveToEx(hDC, clientRect.left,  centerY,        (LPPOINT)nullptr);
    ::LineTo  (hDC, clientRect.right, centerY);

    ::SelectObject(hDC, hOldAxisPen);
    ::DeleteObject(hAxisPen);

    COLORREF col;

    // polygons are in collision? If yes, change the polygons color
    if (m_Algorithm.Resolve(m_Polygons[0].m_Collider, m_Polygons[1].m_Collider))
        col = RGB(255, 0, 255);
    else
        col = RGB(0, 255, 0);

    HPEN hPolygonPen    = ::CreatePen(PS_SOLID, 2, col);
    HPEN hOldPolygonPen = (HPEN)::SelectObject(hDC, hPolygonPen);

    // iterate through polygons
    for (std::size_t i = 0; i < m_Polygons.size(); ++i)
        // iterate through polygon vertices
        for (std::size_t j = 0; j < m_Polygons[i].m_Collider.m_Vertices.size(); ++j)
        {
            GJK_Vector2F coords[2];

            // get polygon vertex and place it in its final position
            coords[0] = m_Polygons[i].m_Collider.m_Matrix.Transform(m_Polygons[i].m_Collider.m_Vertices[j]);

            // get polygon next vertex and place it in its final position
            if (j == m_Polygons[i].m_Collider.m_Vertices.size() - 1)
                coords[1] = m_Polygons[i].m_Collider.m_Matrix.Transform(m_Polygons[i].m_Collider.m_Vertices[0]);
            else
                coords[1] = m_Polygons[i].m_Collider.m_Matrix.Transform(m_Polygons[i].m_Collider.m_Vertices[j + 1]);

            // draw the next polygon line
            ::MoveToEx(hDC, centerX + coords[0].m_X * m_ZoomFactorX, centerY + coords[0].m_Y * m_ZoomFactorY, (LPPOINT)nullptr);
            ::LineTo  (hDC, centerX + coords[1].m_X * m_ZoomFactorX, centerY + coords[1].m_Y * m_ZoomFactorY);
        }

    ::SelectObject(hDC, hOldPolygonPen);
    ::DeleteObject(hAxisPen);
}
//------------------------------------------------------------------------------
void GJK_Canvas::OnKeyDown(DWORD key)
{
    const DWORD virtKey = ::GetKeyState(VK_SHIFT);

    // dispatch key
    switch (key)
    {
        case VK_LEFT:
            // rotate polygon
            if (virtKey & 0x8000)
            {
                m_Polygons[1].m_Collider.m_Matrix.RotateCenter(m_LeftAngle, m_Polygons[1].m_Collider.AveragePoint(false));
                break;
            }

            // move polygon
            m_Polygons[1].m_Collider.m_Matrix.m_Table[2][0] -= 0.1f;
            break;

        case VK_UP:
            // rotate polygon
            if (virtKey & 0x8000)
            {
                m_Polygons[1].m_Collider.m_Matrix.RotateCenter(m_LeftAngle, m_Polygons[1].m_Collider.AveragePoint(false));
                break;
            }

            // move polygon
            m_Polygons[1].m_Collider.m_Matrix.m_Table[2][1] -= 0.1f;
            break;

        case VK_RIGHT:
            // rotate polygon
            if (virtKey & 0x8000)
            {
                m_Polygons[1].m_Collider.m_Matrix.RotateCenter(-m_LeftAngle, m_Polygons[1].m_Collider.AveragePoint(false));
                break;
            }

            // move polygon
            m_Polygons[1].m_Collider.m_Matrix.m_Table[2][0] += 0.1f;
            break;

        case VK_DOWN:
            // rotate polygon
            if (virtKey & 0x8000)
            {
                m_Polygons[1].m_Collider.m_Matrix.RotateCenter(-m_LeftAngle, m_Polygons[1].m_Collider.AveragePoint(false));
                break;
            }

            // move polygon
            m_Polygons[1].m_Collider.m_Matrix.m_Table[2][1] += 0.1f;
            break;

        // A key
        case 0x41:
            // rotate polygon
            if (virtKey & 0x8000)
            {
                m_Polygons[0].m_Collider.m_Matrix.RotateCenter(m_LeftAngle, m_Polygons[0].m_Collider.AveragePoint(false));
                break;
            }

            // move polygon
            m_Polygons[0].m_Collider.m_Matrix.m_Table[2][0] -= 0.1f;
            break;

        // W key
        case 0x57:
            // rotate polygon
            if (virtKey & 0x8000)
            {
                m_Polygons[0].m_Collider.m_Matrix.RotateCenter(m_LeftAngle, m_Polygons[0].m_Collider.AveragePoint(false));
                break;
            }

            // move polygon
            m_Polygons[0].m_Collider.m_Matrix.m_Table[2][1] -= 0.1f;
            break;

        // D key
        case 0x44:
            // rotate polygon
            if (virtKey & 0x8000)
            {
                m_Polygons[0].m_Collider.m_Matrix.RotateCenter(-m_LeftAngle, m_Polygons[0].m_Collider.AveragePoint(false));
                break;
            }

            // move polygon
            m_Polygons[0].m_Collider.m_Matrix.m_Table[2][0] += 0.1f;
            break;

        // S key
        case 0x53:
            // rotate polygon
            if (virtKey & 0x8000)
            {
                m_Polygons[0].m_Collider.m_Matrix.RotateCenter(-m_LeftAngle, m_Polygons[0].m_Collider.AveragePoint(false));
                break;
            }

            // move polygon
            m_Polygons[0].m_Collider.m_Matrix.m_Table[2][1] += 0.1f;
            break;

        case VK_F1:
        {
            m_Polygons.clear();

            // triangular polygons
            std::vector<GJK_Vector2F> vertices;
            vertices.push_back(GJK_Vector2F(-7.0f, 5.0f));
            vertices.push_back(GJK_Vector2F(-5.0f, 3.0f));
            vertices.push_back(GJK_Vector2F(-7.0f, 1.0f));
            vertices.push_back(GJK_Vector2F(5.0f, 4.0f));
            vertices.push_back(GJK_Vector2F(4.0f, 3.0f));
            vertices.push_back(GJK_Vector2F(5.0f, 2.0f));

            AddPolygons(2, 3, vertices);
            break;
        }

        case VK_F2:
        {
            m_Polygons.clear();

            // quadratic polygons
            std::vector<GJK_Vector2F> vertices;
            vertices.push_back(GJK_Vector2F(-7.0f, 5.0f));
            vertices.push_back(GJK_Vector2F(-5.0f, 3.0f));
            vertices.push_back(GJK_Vector2F(-5.0f, 2.0f));
            vertices.push_back(GJK_Vector2F(-7.0f, 1.0f));
            vertices.push_back(GJK_Vector2F(5.0f, 4.0f));
            vertices.push_back(GJK_Vector2F(4.0f, 3.0f));
            vertices.push_back(GJK_Vector2F(4.0f, 1.0f));
            vertices.push_back(GJK_Vector2F(5.0f, 2.0f));

            AddPolygons(2, 4, vertices);
            break;
        }
    }
}
//------------------------------------------------------------------------------
void GJK_Canvas::AddPolygons(std::size_t polygonCount, std::size_t verticesCount, const std::vector<GJK_Vector2F>& vertices)
{
    m_Polygons.resize(polygonCount);

    for (std::size_t i = 0; i < polygonCount; ++i)
    {
        m_Polygons[i].m_Collider.m_Vertices.resize(verticesCount);

        for (std::size_t j = 0; j < verticesCount; ++j)
            m_Polygons[i].m_Collider.m_Vertices[j] = vertices[(i * verticesCount) + j];
    }
}
//------------------------------------------------------------------------------
