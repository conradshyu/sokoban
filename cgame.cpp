/*
 * cgame.cpp
 *
 * Sokoban (warehouse keeper) is a type of transport puzzle, in which the
 * player pushes boxes or crates around in a warehouse, trying to get them
 * to storage locations. The puzzle is usually implemented as a video game.
 *
 * Written by Conrad Shyu (shyu4751@yahoo.com)
 * Created on October 21, 2011
 * Revised on October 26, 2011
*/

// precompiled header
#include "stdafx.h"
#include <atlstr.h> // for cstring
#include "strsafe.h"

#include "cgame.h"

CGame::CGame()
{
} // end of default constructor

CGame::~CGame()
{
    ::DeleteDC(hMem);
} // end of destructor

HDC CGame::GetMemDC() const
{
    return (hMem);
} // end of GetDC()

HDC CGame::SetMemDC(
    const HDC _dc)
{
    return (hMem = _dc);
} // end of SetDC()

bool CGame::Clear() const
{
    RECT box;
    ::GetClientRect(hWnd, &box);

    HBRUSH brush = (HBRUSH)::GetStockObject(nBrush);
    ::SelectObject(GetMemDC(), brush);
    ::PatBlt(GetMemDC(), 0, 0, box.right, box.bottom, PATCOPY);

    ::InvalidateRect(hWnd, &box, FALSE);
    ::DeleteObject(brush);

    return (true);
} // end of Clear()

bool CGame::Init(
    const HWND _hwnd,
    const int _color)
{
    hWnd = _hwnd;
    nBrush = _color;
    hInstance = (HINSTANCE)::GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    RECT box;
    HDC hdc = ::GetDC(hWnd);
    SetMemDC(::CreateCompatibleDC(hdc));
    ::GetClientRect(hWnd, &box);

    HBITMAP hmem = ::CreateCompatibleBitmap(hdc, box.right, box.bottom);
    ::SelectObject(GetMemDC(), hmem);

    HBRUSH brush = (HBRUSH)::GetStockObject(nBrush);
    ::SelectObject(GetMemDC(), brush);
    ::PatBlt(GetMemDC(), 0, 0, box.right, box.bottom, PATCOPY);

    ::DeleteObject(hmem);
    ::DeleteObject(brush);
    ::ReleaseDC(hWnd, hdc);

    SetPuzzle();
    return (true);
} // end of Init()

bool CGame::SetBlock(
    const int _h,
    const int _w,
    const HBITMAP _bmp)
{
    RECT box;
    box.left = (_w * nBlockWIDTH);
    box.right = box.left + nBlockWIDTH;
    box.top = (_h * nBlockHEIGHT);
    box.bottom = box.top + nBlockHEIGHT;

    HDC hdc = ::GetDC(hWnd);
    HDC hbmp = ::CreateCompatibleDC(hdc);

    ::SelectObject(hbmp, _bmp);
    ::BitBlt(GetMemDC(), box.left, box.top, nBlockWIDTH, nBlockHEIGHT,
             hbmp, 0, 0, SRCCOPY);

    ::DeleteDC(hbmp);
    ::ReleaseDC(hWnd, hdc);
    ::InvalidateRect(hWnd, &box, FALSE);

    return (true);
} // end of SetBlock()
