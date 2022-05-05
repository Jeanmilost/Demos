/****************************************************************************
 * ==> Boundary ------------------------------------------------------------*
 ****************************************************************************
 * Description : Boundary                                                   *
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

using System.Drawing;
using System.Drawing.Drawing2D;

namespace Raytracer2D.Classes
{
    /**
    * Boundary
    *@author Jean-Milost Reymond
    */
    class Boundary
    {
        /**
        * Gets or sets boundary limit
        */
        public Line2 Limit { get; set; } = new Line2();

        /**
        * Gets or sets limit pen
        */
        public Pen LimitPen { get; set; } = new Pen(Color.FromArgb(255, 255, 255, 255), 1.0f);

        /**
        * Constructor
        */
        public Boundary()
        {}

        /**
        * Constructor
        *@param start - boundary start position
        *@param end - boundary end position
        */
        public Boundary(Vector2 start, Vector2 end)
        {
            Limit.Start = start;
            Limit.End   = end;
        }

        /**
        * Constructor
        *@param x1 - boundary start x position
        *@param y1 - boundary start y position
        *@param x2 - boundary end x position
        *@param y2 - boundary end y position
        */
        public Boundary(float x1, float y1, float x2, float y2)
        {
            Limit.Start.X = x1;
            Limit.Start.Y = y1;
            Limit.End.X   = x2;
            Limit.End.Y   = y2;
        }

        /**
        * Draws the boundary
        *@param gfx - graphic context
        */
        public void Draw(Graphics gfx)
        {
            gfx.DrawLine(LimitPen, Limit.Start.X, Limit.Start.Y, Limit.End.X, Limit.End.Y);
        }

        /**
        * Draws the boundary
        *@param graphicPath - graphic path in which the boundary should be drawn
        */
        public void Draw(GraphicsPath graphicPath)
        {
            graphicPath.AddLine(Limit.Start.X, Limit.Start.Y, Limit.End.X, Limit.End.Y);
        }
    }
}
