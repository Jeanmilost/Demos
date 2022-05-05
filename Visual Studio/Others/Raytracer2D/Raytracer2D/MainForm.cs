using System;
using System.Drawing;
using System.Windows.Forms;

using Raytracer2D.Classes;

namespace Raytracer2D
{
    public partial class MainForm : Form
    {
        private Layer   m_Layer = new Layer();
        private Vector2 m_Pos   = new Vector2(150.0f, 150.0f);

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            m_Layer.ClientRect = ClientRectangle;
            m_Layer.Boundaries.Add(new Vector2(10.0f, 100.0f), new Vector2(350.0f, 50.0f));
            m_Layer.Boundaries.Add(new Vector2(450.0f, 10.0f), new Vector2(320.0f, 350.0f));
            m_Layer.RaySources.Add(new RaySource(new Vector2(150.0f, 150.0f), 1750));

        }

        private void MainForm_MouseMove(object sender, MouseEventArgs e)
        {
            m_Pos = new Vector2(e.X, e.Y);
            Invalidate();
        }

        private void MainForm_SizeChanged(object sender, EventArgs e)
        {
            m_Layer.ClientRect = ClientRectangle;
        }

        private void MainForm_Paint(object sender, PaintEventArgs e)
        {
            SolidBrush blackBrush = new SolidBrush(Color.FromArgb(255, 0, 0, 0));
            e.Graphics.FillRectangle(blackBrush, ClientRectangle);

            m_Layer.Draw(e.Graphics, m_Pos);

            /*REM
            Boundary boundary = new Boundary(10, 100, 350, 200);
            boundary.Draw(e.Graphics);

            Ray2 ray = new Ray2(new Vector2(150.0f, 300.0f), new Vector2(0.1f, -0.9f));
            Pen limitPen = new Pen(Color.FromArgb(255, 0, 0, 255));
            e.Graphics.DrawLine(limitPen, ray.Pos.X, ray.Pos.Y, ray.Pos.X * ray.Dir.X * 1, ray.Pos.Y * ray.Dir.Y * 1);

            Vector2 point;
            if (ray.Cast(boundary.Limit, out point))
            {
                Pen limitPen2 = new Pen(Color.FromArgb(255, 255, 0, 0));
                e.Graphics.DrawEllipse(limitPen2, point.X, point.Y, 5.0f, 5.0f);
            }
            */

            //public void Draw(Graphics gfx, Pen rayPen, Boundaries boundaries)
            //{
            //    if (m_Rays.Count == 0)
            //        return;

            //    GraphicsPath rayPath   = new GraphicsPath();
            //    Vector2      rayCenter = m_Rays[0].Pos;

            //    foreach (Ray2 ray in m_Rays)
            //    {
            //        Vector2 shortestPoint = new Vector2();
            //        bool    resolved      = false;

            //        for (uint i = 0; i < boundaries.Count; ++i)
            //        {
            //            Vector2 curPoint;

            //            if (ray.Cast(boundaries[(int)i].Limit, out curPoint))
            //            {
            //                if (!resolved || (curPoint - ray.Pos).Length() < (shortestPoint - ray.Pos).Length())
            //                    shortestPoint = curPoint;

            //                resolved = true;
            //            }
            //        }

            //        if (resolved)
            //        {
            //            /*REM
            //            LinearGradientBrush rayGradientBrush =
            //                    new LinearGradientBrush(new Point((int)ray.Pos.X, (int)ray.Pos.Y),
            //                                            new Point((int)(ray.Pos.X + (2000.0f * ray.Dir.X)), (int)(ray.Pos.Y + (2000.0f * ray.Dir.Y))),
            //                                            Color.FromArgb(255, 255, 255, 255),
            //                                            Color.FromArgb(0,   255, 255, 255));
            //            */
            //            /*
            //            // Create a path that consists of a single ellipse
            //            GraphicsPath rayGradientPath = new GraphicsPath();
            //            rayGradientPath.AddEllipse((int)ray.Pos.X - 250, (int)ray.Pos.Y - 250, 500, 500);

            //            // Use the path to construct a brush
            //            PathGradientBrush rayGradientBrush = new PathGradientBrush(rayGradientPath);

            //            // Set the color at the center of the path
            //            rayGradientBrush.CenterColor = Color.FromArgb(255, 255, 255, 255);

            //            // set the color along the entire boundary of the path
            //            Color[] colors = { Color.FromArgb(0, 255, 255, 255) };
            //            rayGradientBrush.SurroundColors = colors;

            //            Pen rayPen = new Pen(rayGradientBrush);
            //            gfx.DrawLine(rayPen, ray.Pos.X, ray.Pos.Y, shortestPoint.X, shortestPoint.Y);

            //            DrawHitPoint(gfx, shortestPoint, 5.0f);

            //            rayPen.Dispose();
            //            rayGradientBrush.Dispose();
            //            */
            //            rayPath.AddLine(ray.Pos.X, ray.Pos.Y, shortestPoint.X, shortestPoint.Y);
            //        }
            //    }

            //    // Create a path that consists of a single ellipse
            //    GraphicsPath rayGradientPath = new GraphicsPath();
            //    rayGradientPath.AddEllipse((int)rayCenter.X - 250, (int)rayCenter.Y - 250, 480, 500);

            //    // use the path to construct a brush
            //    PathGradientBrush rayGradientBrush = new PathGradientBrush(rayGradientPath);

            //    // set the color at the center of the path
            //    rayGradientBrush.CenterColor = Color.FromArgb(255, 255, 255, 255);

            //    // set the color along the entire boundary of the path
            //    Color[] colors = { Color.FromArgb(0, 255, 255, 255) };
            //    rayGradientBrush.SurroundColors = colors;

            //    Pen rayPen = new Pen(rayGradientBrush);
            //    gfx.DrawPath(rayPen, rayPath);
            //}
        }
    }
}
