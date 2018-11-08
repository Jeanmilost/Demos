/****************************************************************************
 * ==> Main ----------------------------------------------------------------*
 ****************************************************************************
 * Description : This module contains the demo main form                    *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************/

#include <vcl.h>
#pragma hdrstop

#include "Main.h"

// classes
#include "Rasterizer.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* pOwner) :
    TForm(pOwner)
{
    // create an overlay bitmap to contain the rendered pixel buffer
    m_pBitmap.reset(new TBitmap());
    m_pBitmap->PixelFormat = pf24bit;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::aeEventsIdle(TObject* pSender, bool& done)
{
    // create a frame and initialize it with the z far clipping plane
    Raster::Frame frame(ClientWidth, ClientHeight, true, 2.0f);

    // create a first polygon. NOTE as no transformation matrix are used here, the z values needs to
    // remain small, to avoid to bias the ray collision detection process (normally a revert model
    // transformation should be applied to the ray to put it in the polygon coordinate system, but
    // as said before, a such transformation isn't used here)
    Polygon3DF polygon1(Vector3DF( 0.13f,  0.34f, 1.0f),
                        Vector3DF( 0.29f, -0.15f, 1.1f),
                        Vector3DF(-0.48f, -0.1f,  1.2f));

    // check if the mouse ray intersects the polygon, and draw the polygon in the correct color
    if (RayCollidePolygon(polygon1))
        Raster::Rasterize(Raster::Vertex(polygon1.GetVertex1(), 1.0f, 0.0f, 0.0f),
                          Raster::Vertex(polygon1.GetVertex2(), 1.0f, 0.0f, 0.0f),
                          Raster::Vertex(polygon1.GetVertex3(), 1.0f, 0.0f, 0.0f),
                         &frame);
    else
        Raster::Rasterize(Raster::Vertex(polygon1.GetVertex1(), 0.0f, 0.0f, 1.0f),
                          Raster::Vertex(polygon1.GetVertex2(), 0.0f, 1.0f, 0.0f),
                          Raster::Vertex(polygon1.GetVertex3(), 1.0f, 0.0f, 0.0f),
                         &frame);

    // create a second polygon. NOTE as no transformation matrix are used here, the z values needs
    // to remain small, to avoid to bias the ray collision detection process (normally a revert model
    // transformation should be applied to the ray to put it in the polygon coordinate system, but
    // as said before, a such transformation isn't used here)
    Polygon3DF polygon2(Vector3DF(-0.13f,  0.34f, 1.2f),
                        Vector3DF( 0.29f, -0.3f,  1.1f),
                        Vector3DF(-0.68f,  0.25f, 1.0f));

    // check if the mouse ray intersects the polygon, and draw the polygon in the correct color
    if (RayCollidePolygon(polygon2))
        Raster::Rasterize(Raster::Vertex(polygon2.GetVertex1(), 1.0f, 0.0f, 0.0f),
                          Raster::Vertex(polygon2.GetVertex2(), 1.0f, 0.0f, 0.0f),
                          Raster::Vertex(polygon2.GetVertex3(), 1.0f, 0.0f, 0.0f),
                         &frame);
    else
        Raster::Rasterize(Raster::Vertex(polygon2.GetVertex1(), 0.0f, 1.0f, 1.0f),
                          Raster::Vertex(polygon2.GetVertex2(), 0.0f, 1.0f, 1.0f),
                          Raster::Vertex(polygon2.GetVertex3(), 1.0f, 1.0f, 0.0f),
                         &frame);

    // resize the overlay bitmap
    m_pBitmap->SetSize(ClientWidth, ClientHeight);

    // iterate through overlay bitmap lines
    for (int y = 0; y < m_pBitmap->Height; ++y)
    {
        // get the next line
        TRGBTriple* pLine = reinterpret_cast<TRGBTriple*>(m_pBitmap->ScanLine[y]);

        // iterate through the overlay bitmap pixels
        for (int x = 0; x < m_pBitmap->Width; ++x)
        {
            // calculate the offset in source pixel buffer
            const std::size_t offset = (y * m_pBitmap->Width) + x;

            // copy the pixel from source frame pixel buffer to overlay
            pLine[x].rgbtRed   = frame.m_pPixels[offset][0];
            pLine[x].rgbtGreen = frame.m_pPixels[offset][1];
            pLine[x].rgbtBlue  = frame.m_pPixels[offset][2];
        }
    }

    // draw the overlay buffer on the form background
    Canvas->Draw(0, 0, m_pBitmap.get());
}
//---------------------------------------------------------------------------
bool TMainForm::RayCollidePolygon(const Polygon3DF& polygon)
{
    TRectF viewport(-1.0f, -1.0f, 1.0f, 1.0f);
    RayF   ray;

    // get the mouse position
    TPoint mousePos = Mouse->CursorPos;

    // convert to client coordinate system
    if (!::ScreenToClient(Handle, &mousePos))
        return false;

    // place the ray coming out the mouse position to the viewport coordinate system
    ray.SetPos(Vector3DF(-1.0f + ((mousePos.X * viewport.Width())  / ClientWidth),
                          1.0f - ((mousePos.Y * viewport.Height()) / ClientHeight),
                          0.0f));
    ray.SetDir(Vector3DF(0.0f, 0.0f, -1.0f));

    // check if a ray collision happens with the polygon
    return polygon.Intersect(ray);
}
//---------------------------------------------------------------------------
