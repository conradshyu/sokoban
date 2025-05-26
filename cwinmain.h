/*
 * cwinmain.h
 * class implementation for the main window
 *
 * Written by Conrad Shyu (shyu4751@yahoo.com)
 * Created on October 13, 2011
*/

// Windows Header Files:
#include "windows.h"
#include <atlstr.h> // for cstring
#include "strsafe.h"

#include <list>
#include "canvas.h"
#include "cstatus.h"
#include "constant.h"

#ifndef _CWINMAIN_H

#define _CWINMAIN_H

typedef struct _TRACK
{
    POINT box1;
    POINT box2;
    POINT avatar;
    int dir;
} stTRACK;

class CWinMain
{
public:
    CWinMain(const ATL::CString &, const ATL::CString &, const HINSTANCE);
    ~CWinMain();

    bool Init();
    bool OnTimer();
    bool OnExit() const;
    bool UpdateWindow() const;
    bool OnEraseBKGND() const;
    bool OnCreate(const HWND);
    bool OnPaint(const HWND) const;
    bool ShowWindow(const int = SW_SHOW) const;
    bool SetTimer(const UINT = nMaxELAPSE) const;
    bool OnKeyboard(const UINT, const WPARAM, const LPARAM);

    HWND GetHandle() const;        // get the window handle
    HACCEL GetAccelHandle() const; // get the short cut handle
    HINSTANCE GetInstance() const;
    const ATL::CString &GetClass() const;
    const ATL::CString &GetTitle() const;
    LRESULT CALLBACK OnCommand(const UINT, const WPARAM, const LPARAM);

private:
    HWND hWnd;
    HACCEL hAccelTable;
    HINSTANCE hInstance;
    HBITMAP hBox[2];
    HBITMAP hBlock[3];
    HBITMAP hAvatar[4][2]; // handle to the bitmap images

    bool bTimer;
    std::list<stTRACK> lsUndo;
    ATL::CString szTitle, szClass;

    CStatus status;
    Canvas canvas;

    int nBrush;
    int nW, nH;
    int nCurrent;
    int nMove, nGoDir, nSuccess;
    int nLayout[nMaxHEIGHT][nMaxWIDTH];
    int nBoxMap[nMaxHEIGHT][nMaxWIDTH];

    bool IsWin();
    bool DoUndo();
    bool SetUndo();
    bool SetPuzzle();
    bool ShowTime() const;
    bool SetStatus() const;
    bool Move(const int, const int, const int);
    bool SetBlock(const int, const int, const HBITMAP) const;
};

#endif
