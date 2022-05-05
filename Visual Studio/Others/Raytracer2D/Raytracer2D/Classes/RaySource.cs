/****************************************************************************
 * ==> RaySource -----------------------------------------------------------*
 ****************************************************************************
 * Description : Ray source                                                 *
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
    * Ray source
    *@author Jean-Milost Reymond
    */
    class RaySource
    {
        private readonly List<Ray2> m_Rays = new List<Ray2>();
        private          Vector2    m_Pos  = new Vector2();

        /**
        * Gets ray at index
        *@throw exception if index is out of bounds
        */
        public Ray2 this[int index]
        {
            get
            {
                if (index < 0 ||index >= m_Rays.Count)
                    throw new ArgumentOutOfRangeException();

                return m_Rays[index];
            }
        }

        /**
        * Gets ray count
        */
        public int Count
        {
            get
            {
                return m_Rays.Count;
            }
        }

        /**
        * Gets or sets ray source position
        */
        public Vector2 Pos
        {
            get
            {
                return m_Pos;
            }

            set
            {
                m_Pos = value;

                foreach (Ray2 ray in m_Rays)
                    ray.Pos = value;
            }
        }

        /**
        * Gets or sets ray pen
        */
        public Pen RayPen { get; set; } = new Pen(Color.FromArgb(255, 255, 255, 255), 1.0f);

        /**
        * Gets or sets hit pen
        */
        public Pen HitPen { get; set; } = new Pen(Color.FromArgb(255, 255, 0, 0), 1.0f);

        /**
        * Constructor
        */
        public RaySource()
        {}

        /**
        * Constructor
        *@param pos - ray source position
        *@param count - ray count
        */
        public RaySource(Vector2 pos, uint count)
        {
            Initialize(pos, count);
        }

        /**
        * Draws the ray source
        *@param gfx - graphic context
        *@param boundaries - boundaries contained in the scene
        *@param drawHitPoints - if true, the hit points will be drawn
        */
        public void Draw(Graphics gfx, Boundaries boundaries, bool drawHitPoints)
        {
            // no rays?
            if (m_Rays.Count == 0)
                return;

            // iterate through rays to draw
            foreach (Ray2 ray in m_Rays)
            {
                Vector2 shortestPoint = new Vector2();
                bool    resolved      = false;

                // iterate through boundaries
                for (uint i = 0; i < boundaries.Count; ++i)
                {
                    // is the ray hitting a boundary?
                    if (ray.Cast(boundaries[(int)i].Limit, out Vector2 curPoint))
                    {
                        // first found boundary, or found a hit point closest to the boundary?
                        if (!resolved || (curPoint - ray.Pos).Length() < (shortestPoint - ray.Pos).Length())
                            shortestPoint = curPoint;

                        resolved = true;
                    }
                }

                // ray cast was resolved (i.e the ray has a start and end point and may be drawn)?
                if (resolved)
                {
                    // draw the ray
                    gfx.DrawLine(RayPen, ray.Pos.X, ray.Pos.Y, shortestPoint.X, shortestPoint.Y);

                    if (drawHitPoints)
                        DrawHitPoint(gfx, shortestPoint, 5.0f);
                }
            }
        }

        /**
        * Draws the ray source
        *@param graphicPath - graphic path in which the ray should be drawn
        *@param boundaries - boundaries contained in the scene
        *@param drawHitPoints - if true, the hit points will be drawn
        */
        public void Draw(GraphicsPath graphicPath, Boundaries boundaries, bool drawHitPoints)
        {
            // no rays?
            if (m_Rays.Count == 0)
                return;

            // iterate through rays to draw
            foreach (Ray2 ray in m_Rays)
            {
                Vector2 shortestPoint = new Vector2();
                bool    resolved      = false;

                // iterate through boundaries
                for (uint i = 0; i < boundaries.Count; ++i)
                {
                    Vector2 curPoint;

                    // is the ray hitting a boundary?
                    if (ray.Cast(boundaries[(int)i].Limit, out curPoint))
                    {
                        // first found boundary, or found a hit point closest to the boundary?
                        if (!resolved || (curPoint - ray.Pos).Length() < (shortestPoint - ray.Pos).Length())
                            shortestPoint = curPoint;

                        resolved = true;
                    }
                }

                // ray cast was resolved (i.e the ray has a start and end point and may be drawn)?
                if (resolved)
                {
                    // draw the ray
                    graphicPath.AddLine(ray.Pos.X, ray.Pos.Y, shortestPoint.X, shortestPoint.Y);

                    if (drawHitPoints)
                        DrawHitPoint(graphicPath, shortestPoint, 5.0f);
                }
            }
        }

        /**
        * Initializes the ray source
        *@param pos - ray source position
        *@param count - ray count
        */
        void Initialize(Vector2 pos, uint count)
        {
            // no rays, or ray count not changed?
            if (count == 0 || count == m_Rays.Count)
                return;

            // clear the previous rays
            m_Rays.Clear();

            float angle = 0.0f;
            float step  = (float)(Math.PI * 2.0) / count;

            // iterate through rays to create
            for (uint i = 0; i < count; ++i)
            {
                // add ray
                m_Rays.Add(new Ray2(pos, new Vector2((float)Math.Cos(angle), (float)Math.Sin(angle))));

                // calculate next angle
                angle += step;
            }
        }

        /**
        * Draws the hit point
        *@param gfx - graphic context
        *@param point - hit point
        *@param radius - hit radius
        */
        void DrawHitPoint(Graphics gfx, Vector2 point, float radius)
        {
            gfx.DrawEllipse(HitPen, point.X - (radius / 2.0f), point.Y - (radius / 2.0f), radius, radius);
        }

        /**
        * Draws the hit point
        *@param graphicPath - graphic path in which the ray should be drawn
        *@param point - hit point
        *@param radius - hit radius
        */
        void DrawHitPoint(GraphicsPath graphicPath, Vector2 point, float radius)
        {
            graphicPath.AddEllipse(point.X - (radius / 2.0f), point.Y - (radius / 2.0f), radius, radius);
        }
    }
}
