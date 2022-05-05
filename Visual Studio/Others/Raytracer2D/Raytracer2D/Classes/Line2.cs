/****************************************************************************
 * ==> Line2 ---------------------------------------------------------------*
 ****************************************************************************
 * Description : 2D line                                                    *
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

namespace Raytracer2D.Classes
{
    /**
    * 2D line
    *@author Jean-Milost Reymond
    */
    class Line2
    {
        /**
        * Gets or sets line start point
        */
        public Vector2 Start { get; set; } = new Vector2();

        /**
        * Gets or sets line end point
        */
        public Vector2 End   { get; set; } = new Vector2();

        /**
        * Constructor
        */
        public Line2()
        {}

        /**
        * Constructor
        *@param start - line start position
        *@param end - line end position
        */
        public Line2(Vector2 start, Vector2 end)
        {
            Start = start;
            End   = end;
        }

        /**
        * Copy constructor
        *@param other - other line to copy from
        */
        public Line2(Line2 other)
        {
            Start = other.Start;
            End   = other.End;
        }
    }
}
