#include "PlayerGDI.h"

// windows
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "msimg32.lib") // AlphaBlend

//------------------------------------------------------------------------------
// PlayerGDI
//------------------------------------------------------------------------------
PlayerGDI::PlayerGDI() :
    Player()
{
    Gdiplus::GdiplusStartupInput si;
    Gdiplus::GdiplusStartup(&m_Token, &si, nullptr);
}
//------------------------------------------------------------------------------
PlayerGDI::~PlayerGDI()
{
    if (m_hBitmap)
        ::DeleteObject(m_hBitmap);

    Gdiplus::GdiplusShutdown(m_Token);
}
//------------------------------------------------------------------------------
bool PlayerGDI::Load(const std::wstring& fileName)
{
    // load via IStream (avoids GDI+ path-validation quirks). Read the whole file into an HGLOBAL, wrap it in an IStream,
    // then let GDI+ decode from the stream – this sidesteps InvalidParameter errors that the Bitmap(path) constructor
    // can raise on some runtime
    HANDLE hFile = ::CreateFileW(fileName.c_str(),
                                 GENERIC_READ,
                                 FILE_SHARE_READ,
                                 nullptr,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    LARGE_INTEGER fileSize = {};

    if (!::GetFileSizeEx(hFile, &fileSize) || fileSize.QuadPart == 0)
    {
        ::CloseHandle(hFile);
        return false;
    }

    HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, static_cast<SIZE_T>(fileSize.QuadPart));

    if (!hMem)
    {
        ::CloseHandle(hFile);
        return false;
    }

    void*      pMem      = ::GlobalLock(hMem);
    DWORD      bytesRead = 0;
    const BOOL readOk    = ::ReadFile(hFile,
                                      pMem,
                                      static_cast<DWORD>(fileSize.QuadPart),
                                      &bytesRead,
                                      nullptr);

    ::GlobalUnlock(hMem);
    ::CloseHandle(hFile);

    if (!readOk || bytesRead != static_cast<DWORD>(fileSize.QuadPart))
    {
        ::GlobalFree(hMem);
        return false;
    }

    IStream* pStream = nullptr;

    if (FAILED(::CreateStreamOnHGlobal(hMem, TRUE, &pStream)))
    {
        ::GlobalFree(hMem);
        return false;
    }

    Gdiplus::Bitmap bmp(pStream);
    pStream->Release();

    const Gdiplus::Status result = bmp.GetLastStatus();

    if (result != Gdiplus::Ok)
        return false;

    const UINT w = bmp.GetWidth();
    const UINT h = bmp.GetHeight();

    if (w == 0 || h == 0)
        return false;

    // create a 32-bpp DIB section
    BITMAPINFOHEADER bih =  {};
    bih.biSize           =  sizeof(bih);
    bih.biWidth          =  static_cast<LONG>(w);
    bih.biHeight         = -static_cast<LONG>(h); // top-down
    bih.biPlanes         =  1;
    bih.biBitCount       =  32;
    bih.biCompression    =  BI_RGB;               // GDI calls it BI_RGB but we store BGRA

    void* pBits = nullptr;
    m_hBitmap   = ::CreateDIBSection(nullptr,
                                     reinterpret_cast<BITMAPINFO*>(&bih),
                                     DIB_RGB_COLORS,
                                     &pBits,
                                     nullptr,
                                     0);

    if (!m_hBitmap || !pBits)
        return false;

    // lock GDI+ pixels directly into the DIB bits. PixelFormat32bppARGB matches BGRA memory
    // layout on little-endian x86
    Gdiplus::Rect       rc(0, 0, static_cast<INT>(w), static_cast<INT>(h));
    Gdiplus::BitmapData bd;

    if (bmp.LockBits(&rc, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bd) != Gdiplus::Ok)
    {
        ::DeleteObject(m_hBitmap);
        m_hBitmap = nullptr;
        return false;
    }

    // copy row-by-row (stride may differ from w*4 if GDI+ padded the scan lines)
    const BYTE* pSrc     = static_cast<const BYTE*>(bd.Scan0);
    BYTE*       pDst     = static_cast<BYTE*>(pBits);
    const UINT  rowBytes = w * 4;

    for (UINT y = 0; y < h; ++y)
    {
        const BYTE* pSrcRow = pSrc + static_cast<ptrdiff_t>(y) * bd.Stride;
        BYTE*       pDstRow = pDst + static_cast<ptrdiff_t>(y) * rowBytes;

        // pre-multiply RGB by alpha – required by AlphaBlend (AC_SRC_ALPHA)
        for (UINT x = 0; x < w; ++x)
        {
            const BYTE b = pSrcRow[x * 4 + 0];
            const BYTE g = pSrcRow[x * 4 + 1];
            const BYTE r = pSrcRow[x * 4 + 2];
            const BYTE a = pSrcRow[x * 4 + 3];

            // Fast integer pre-multiply: (c * a + 127) / 255
            pDstRow[x * 4 + 0] = static_cast<BYTE>((b * a + 127) / 255);
            pDstRow[x * 4 + 1] = static_cast<BYTE>((g * a + 127) / 255);
            pDstRow[x * 4 + 2] = static_cast<BYTE>((r * a + 127) / 255);
            pDstRow[x * 4 + 3] = a;
        }
    }

    bmp.UnlockBits(&bd);

    return true;
}
//------------------------------------------------------------------------------
void PlayerGDI::Draw(double elapsedTime, HDC hDC, const RECT& clientRect, const RECT& towerRect) const
{
    if (!m_hBitmap)
        return;

    if (!hDC)
        return;

    // is player turning or moving?
    if (m_Turning)
    {
        const_cast<double&>(m_CurTime) += elapsedTime;

        // animation frame update
        while (m_CurTime >= m_AnimTime)
        {
            const_cast<double&>(m_CurTime) -= m_AnimTime;
            ++const_cast<int&>(m_Count);

            if (m_Count >= 7)
            {
                const_cast<double&>(m_CurTime) = 0;
                const_cast<int&>(m_Index)      = 0;
                break;
            }
        }

        if (m_Count >= 7)
        {
            const_cast<double&>(m_CurTime) = 0;
            const_cast<int&>(m_Index)      = 0;
            const_cast<int&>(m_Count)      = 0;
            const_cast<bool&>(m_Turning)   = false;
        }
        else
        if (m_Count >= 4)
        {
            const_cast<int&>(m_Index) = (6 - m_Count);
        }
        else
            const_cast<int&>(m_Index) = m_Count;
    }
    else
    if (!m_Stopped)
    {
        const_cast<double&>(m_CurTime) += elapsedTime;

        // animation frame update
        while (m_CurTime >= m_AnimTime)
        {
            const_cast<double&>(m_CurTime) -= m_AnimTime;
            const_cast<int&>(m_Index)       = (m_Index + 1) % 9;
        }
    }
    else
    {
        const_cast<int&>(m_Index)      = 0;
        const_cast<double&>(m_CurTime) = 0.0;
    }

    // calculate the source frame to copy
    const int srcX = 4 + (m_Turning ? 24 * m_Index : 28 * m_Index);
    const int srcY =      m_Turning ? 28 : 4;
    const int srcW = 24;
    const int srcH = 20;

    // calculate the destination frame
    const int dstW = 48;
    const int dstH = 40;
    const int dstX = towerRect.left   + ((towerRect.right - towerRect.left) >> 1) - (dstW >> 1);
    const int dstY = towerRect.bottom -  (dstH + 100);

    // configure blending operation
    BLENDFUNCTION bf       = {};
    bf.BlendOp             = AC_SRC_OVER;
    bf.BlendFlags          = 0;
    bf.SourceConstantAlpha = 255;
    bf.AlphaFormat         = AC_SRC_ALPHA;

    // is moving on the right?
    if ((!m_Turning && !m_MovingLeft) || (m_Turning && ((m_MovingLeft && m_Count < 4) || (m_MovingRight && m_Count > 3))))
    {
        HDC hDcSrc = ::CreateCompatibleDC(hDC);

        if (!hDcSrc)
            return;

        HGDIOBJ hOld = ::SelectObject(hDcSrc, m_hBitmap);

        // default dir, blit directly
        ::AlphaBlend(hDC,    dstX, dstY, dstW, dstH,
                     hDcSrc, srcX, srcY, srcW, srcH,
                     bf);

        ::SelectObject(hDcSrc, hOld);
        ::DeleteDC(hDcSrc);
    }
    else
    {
        // mirrored path, StretchBlt the sprite frame (flipped) into a temporary 32-bpp DIB
        // that matches the destination size. StretchBlt with a negative srcW mirrors on the
        // X axis but does NOT preserve alpha, so blit into an off-screen DC first, then
        // AlphaBlend from there
        BITMAPINFOHEADER bih =  {};
        bih.biSize           =  sizeof(bih);
        bih.biWidth          =  dstW;
        bih.biHeight         = -dstH; // top-down
        bih.biPlanes         =  1;
        bih.biBitCount       =  32;
        bih.biCompression    =  BI_RGB;

        void*   pBits  = nullptr;
        HBITMAP hbmTmp = ::CreateDIBSection(hDC,
                                            reinterpret_cast<BITMAPINFO*>(&bih),
                                            DIB_RGB_COLORS,
                                            &pBits,
                                            nullptr,
                                            0);

        if (!hbmTmp)
            return;

        HDC hDcTmp = ::CreateCompatibleDC(hDC);
        HDC hDcSrc = ::CreateCompatibleDC(hDC);

        if (!hDcTmp || !hDcSrc)
        {
            if (hDcTmp)
                ::DeleteDC(hDcTmp);

            if (hDcSrc)
                ::DeleteDC(hDcSrc);

            ::DeleteObject(hbmTmp);
            return;
        }

        HGDIOBJ hOldTmp = ::SelectObject(hDcTmp, hbmTmp);
        HGDIOBJ hOldSrc = ::SelectObject(hDcSrc, m_hBitmap);

        // flip source, starts at srcX + srcW and has width - srcW
        ::SetStretchBltMode(hDcTmp, COLORONCOLOR);
        ::StretchBlt(hDcTmp, 0, 0, dstW, dstH,
                     hDcSrc, srcX + srcW, srcY, -srcW, srcH, SRCCOPY);

        // alpha blend from the flipped temporary bitmap
        ::AlphaBlend(hDC,    dstX, dstY, dstW, dstH,
                     hDcTmp, 0,    0,    dstW, dstH,
                     bf);

        ::SelectObject(hDcTmp, hOldTmp);
        ::SelectObject(hDcSrc, hOldSrc);
        ::DeleteDC(hDcTmp);
        ::DeleteDC(hDcSrc);
        ::DeleteObject(hbmTmp);
    }
}
//------------------------------------------------------------------------------
