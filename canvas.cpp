/*
 * canvas.cpp
 *
 * Windows GDI
 * The Microsoft Windows graphics device interface (GDI) enables applications to
 * use graphics and formatted text on both the video display and the printer.
 * Windows-based applications do not access the graphics hardware directly.
 * Instead, GDI interacts with device drivers on behalf of applications.
 *
 * Written by Conrad Shyu (shyu4751@yahoo.com)
 * Created on October 21, 2011
 * Revised on December 11, 2011
*/

// precompiled header
#include "stdafx.h"
#include <atlstr.h> // for cstring
#include "strsafe.h"

#include "canvas.h"

Canvas::Canvas()
{
} // end of default constructor

Canvas::~Canvas()
{
    ::DeleteObject(hCanvas);
    ::DeleteDC(hMem);
} // end of destructor

/*
 * setup a virtual screen as memory dc
*/
bool Canvas::Init(
    const HWND _hwnd, const int _brush, const bool _full)
{
    hWnd = _hwnd;
    hBrush = (HBRUSH)::GetStockObject(_brush);
    hInstance = (HINSTANCE)::GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    RECT box;

    if (_full) {
        mSize.cx = ::GetSystemMetrics(SM_CXSCREEN);
        mSize.cy = ::GetSystemMetrics(SM_CYSCREEN);
        // for the entire screen
    } else {
        ::GetClientRect(hWnd, &box);
        mSize.cx = box.right;
        mSize.cy = box.bottom;
    } // only the client area

    HDC hdc = ::GetDC(hWnd);
    SetMemDC(::CreateCompatibleDC(hdc));
    hCanvas = ::CreateCompatibleBitmap(hdc, mSize.cx, mSize.cy);
    ::SelectObject(GetMemDC(), hCanvas);
    ::SelectObject(GetMemDC(), hBrush);
    ::PatBlt(GetMemDC(), 0, 0, mSize.cx, mSize.cy, PATCOPY);

    return (::ReleaseDC(hWnd, hdc) ? true : false);
} // end of Init()

/*
 * The PatBlt function paints the specified rectangle using the brush that is
 * currently selected into the specified device context. The brush color and the
 * surface color or colors are combined by using the specified raster operation.
 *
 * The raster operation code. This code can be one of the following values.
 *
 * PATCOPY
 * Copies the specified pattern into the destination bitmap.
 *
 * PATINVERT
 * Combines the colors of the specified pattern with the colors of the
 * destination rectangle by using the Boolean XOR operator.
 *
 * DSTINVERT
 * Inverts the destination rectangle.
 *
 * BLACKNESS
 * Fills the destination rectangle using the color associated with index 0 in
 * the physical palette. (This color is black for the default physical palette.)
 *
 * WHITENESS
 * Fills the destination rectangle using the color associated with index 1 in
 * the physical palette. (This color is white for the default physical palette.)
*/
bool Canvas::Clear() const
{
    RECT box;
    box.left = 0;
    box.right = mSize.cx;
    box.top = 0;
    box.bottom = mSize.cy;

    ::SelectObject(GetMemDC(), hBrush);
    ::PatBlt(GetMemDC(), 0, 0, mSize.cx, mSize.cy, PATCOPY);

    return (Invalidate(box, false));
} // end of Clear()

bool Canvas::BitBlt(
    const HBITMAP _bmp, const int _x, const int _y, const DWORD _op) const
{
    RECT box;
    BITMAP block;
    HDC hdc = ::GetDC(hWnd);
    HDC hbmp = ::CreateCompatibleDC(hdc);

    ::GetObject(_bmp, sizeof(block), &block);
    ::SelectObject(hbmp, _bmp); // select the bitmap into device
    ::BitBlt(GetMemDC(), _x, _y, block.bmWidth, block.bmHeight, hbmp, 0, 0, _op);
    ::DeleteDC(hbmp);
    ::ReleaseDC(hWnd, hdc);

    box.left = _x;
    box.right = box.left + block.bmWidth;
    box.top = _y;
    box.bottom = box.top + block.bmHeight;

    return (Invalidate(box, false));
} // end of BitBlt()

/*
 * The InvalidateRect function adds a rectangle to the specified window's update
 * region. The update region represents the portion of the window's client area
 * that must be redrawn.
*/
bool Canvas::Invalidate(
    const RECT &_r, const bool _b) const
{
    return (::InvalidateRect(hWnd, &_r, _b ? TRUE : FALSE) ? true : false);
} // end of Invalidate()

bool Canvas::OnPaint() const
{
    PAINTSTRUCT ps;

    HDC hdc = ::BeginPaint(hWnd, &ps);
    ::BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top,
             (ps.rcPaint.right - ps.rcPaint.left),
             (ps.rcPaint.bottom - ps.rcPaint.top),
             GetMemDC(), ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

    return (::EndPaint(hWnd, &ps) ? true : false);
} // end of OnPaint()

HWND Canvas::GetHandle() const
{
    return (hWnd);
} // end of GetHandle()

HINSTANCE Canvas::GetInstance() const
{
    return (hInstance);
} // end of GetInstance()

HDC Canvas::SetMemDC(
    const HDC _mem)
{
    return (hMem = _mem);
} // end of SetMemDC()

HDC Canvas::GetMemDC() const
{
    return (hMem);
} // end of GetDC()
