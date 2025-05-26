/*
 * cgame.h
 *
 * Sokoban (warehouse keeper) is a type of transport puzzle, in which the
 * player pushes boxes or crates around in a warehouse, trying to get them
 * to storage locations. The puzzle is usually implemented as a video game.
 *
 * Written by Conrad Shyu (shyu4751@yahoo.com)
 * Created on October 21, 2011
*/

// Windows Header Files:
#include "windows.h"
#include "resource.h"
#include "constant.h"

#ifndef _CGAME_H

#define _CGAME_H

class CGame
{
public:
    CGame();
    CGame(const HWND, const int = BLACK_BRUSH);
    ~CGame();

    HDC GetMemDC() const;
    HDC SetMemDC(const HDC);

    bool Init(const HWND, const int = BLACK_BRUSH);

private:
    HDC hMem;
    HWND hWnd;
    HINSTANCE hInstance;

    bool Clear() const;
    bool SetBlock(const int, const int, const HBITMAP);
};

#endif // _CGAME_H
