/*
 * canvas.h
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

// Windows Header Files:
#include "windows.h"

#ifndef _CANVAS_H

#define _CANVAS_H

class Canvas
{
public:
    Canvas();
    ~Canvas();

    bool Init(const HWND, const int = WHITE_BRUSH, const bool = true);
    bool Clear() const;
    bool OnPaint() const;
    bool BitBlt(const HBITMAP, const int, const int, const DWORD = SRCCOPY) const;

    HWND GetHandle() const;
    HINSTANCE GetInstance() const;

private:
    HDC hMem;
    HWND hWnd;
    HBRUSH hBrush;
    HBITMAP hCanvas;
    HINSTANCE hInstance;
    SIZE mSize;

    HDC SetMemDC(const HDC);
    HDC GetMemDC() const;
    bool Invalidate(const RECT &, bool = false) const;
};

#endif // _CANVAS_H
