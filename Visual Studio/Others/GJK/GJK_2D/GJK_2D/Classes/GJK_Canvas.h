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

#pragma once

// libraries
#include <windows.h>

// classes
#include "GJK_PolygonCollider.h"
#include "GJK_Algorithm.h"

/**
* GJK canvas
*@author Jean-Milost Reymond
*/
class GJK_Canvas
{
    public:
        struct IPolygon
        {
            GJK_PolygonCollider m_Collider;

            IPolygon();
            virtual ~IPolygon();
        };

        /**
        * Constructor
        *@param hWnd - windows handle
        */
        GJK_Canvas(HWND m_hWnd);

        virtual ~GJK_Canvas();

        /**
        * Draws the canvas content
        */
        void Draw();

        /**
        * Called when a key is down
        *@param key - pressed key code
        */
        void OnKeyDown(DWORD key);

    private:
        typedef std::vector<IPolygon> IPolygons;

        HWND          m_hWnd = nullptr;
        IPolygons     m_Polygons;
        GJK_Algorithm m_Algorithm;
        std::size_t   m_ZoomFactorX;
        std::size_t   m_ZoomFactorY;
        float         m_LeftAngle  = 0.025f;
        float         m_RightAngle = 0.025f;

        /**
        * Add polygons to canvas
        *@param polygonCount - polygon count
        *@param verticesCount - per polygon vertices count
        *@param vertices - vertices to put in each polygon
        */
        void AddPolygons(std::size_t polygonCount, std::size_t verticesCount, const std::vector<GJK_Vector2F>& vertices);
};
