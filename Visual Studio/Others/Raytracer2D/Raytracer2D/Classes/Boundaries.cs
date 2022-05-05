/****************************************************************************
 * ==> Boundaries ----------------------------------------------------------*
 ****************************************************************************
 * Description : Boundary set                                               *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - 2D Raytracer                                               *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sublicense, and/or sell copies of the Software, and to       *
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

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Raytracer2D.Classes
{
    /**
    * Boundary set
    *@author Jean-Milost Reymond
    */
    class Boundaries
    {
        private readonly List<Boundary> m_Boundaries = new List<Boundary>();

        /**
        * Gets boundary at index
        */
        public Boundary this[int index]
        {
            get
            {
                if (index < 0 || index >= m_Boundaries.Count)
                    throw new ArgumentOutOfRangeException();

                return m_Boundaries[index];
            }
        }

        /**
        * Gets boundary count
        */
        public int Count
        {
            get
            {
                return m_Boundaries.Count;
            }
        }

        /**
        * Constructor
        */
        public Boundaries()
        {}

        /**
        * Adds boundary
        *@param x1 - boundary start x position
        *@param y1 - boundary start y position
        *@param x2 - boundary end x position
        *@param y2 - boundary end y position
        */
        public void Add(float x1, float y1, float x2, float y2)
        {
            m_Boundaries.Add(new Boundary(x1, y1, x2, y2));
        }

        /**
        * Adds boundary
        *@param start - boundary start position
        *@param end - boundary end position
        */
        public void Add(Vector2 start, Vector2 end)
        {
            m_Boundaries.Add(new Boundary(start, end));
        }

        /**
        * Adds boundary
        *@param boundary - boundary to add
        */
        public void Add(Boundary boundary)
        {
            m_Boundaries.Add(boundary);
        }

        /**
        * Draws the boundaries
        *@param gfx - graphic context
        */
        public void Draw(Graphics gfx)
        {
            foreach (Boundary boundary in m_Boundaries)
                boundary.Draw(gfx);
        }

        /**
        * Draws the boundaries
        *@param graphicPath - graphic path in which the boundaries should be drawn
        */
        public void Draw(GraphicsPath graphicPath)
        {
            foreach (Boundary boundary in m_Boundaries)
                boundary.Draw(graphicPath);
        }
    }
}
