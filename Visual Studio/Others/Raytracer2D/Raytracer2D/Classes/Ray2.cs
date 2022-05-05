/****************************************************************************
 * ==> Ray2 ----------------------------------------------------------------*
 ****************************************************************************
 * Description : 2D ray                                                     *
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

namespace Raytracer2D.Classes
{
    /**
    * 2D Ray
    *@author Jean-Milost Reymond
    */
    class Ray2
    {
        private Vector2 m_Dir = new Vector2();

        /**
        * Gets or sets ray position
        */
        public Vector2 Pos { get; set; } = new Vector2();

        /**
        * Gets or sets ray direction
        */
        public Vector2 Dir
        {
            get
            {
                return m_Dir;
            }

            set
            {
                m_Dir = value;

                // calculate the invert direction
                InvDir = new Vector2((value.X == 0.0f) ? Single.PositiveInfinity : (1.0f / value.X),
                                     (value.Y == 0.0f) ? Single.PositiveInfinity : (1.0f / value.Y));
            }
        }

        /**
        * Gets ray inverted direction (i.e 1 / dir)
        */
        public Vector2 InvDir { get; private set; }

        /**
        * Constructor
        */
        public Ray2()
        {
            InvDir = new Vector2(Single.PositiveInfinity, Single.PositiveInfinity);
        }

        /**
        * Constructor
        *@param pos - ray position
        *@param dir - ray direction
        */
        public Ray2(Vector2 pos, Vector2 dir)
        {
            Pos = pos;
            Dir = dir;
        }

        /**
        * Copy constructor
        *@param other - other ray to copy from
        */
        public Ray2(Ray2 other)
        {
            Pos = other.Pos;
            Dir = other.Dir;
        }

        /**
        * Casts the ray and check if it hits a limit
        *@param line - line representing the limit
        *@param[out] point - intersection point, in case the cast returns true
        *@return true if ray intersects the limit, otherwise false
        */
        public bool Cast(Line2 line, out Vector2 point)
        {
            point = new Vector2();

            float x1 = line.Start.X;
            float y1 = line.Start.Y;
            float x2 = line.End.X;
            float y2 = line.End.Y;

            float x3 = Pos.X;
            float y3 = Pos.Y;
            float x4 = Pos.X + Dir.X;
            float y4 = Pos.Y + Dir.Y;

            float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

            if (den == 0.0f)
                return false;

            float t =  ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
            float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

            if (t > 0.0f && t < 1.0f && u > 0.0f)
            {
                point.X = x1 + t * (x2 - x1);
                point.Y = y1 + t * (y2 - y1);
                return true;
            }

            return false;
        }
    }
}
