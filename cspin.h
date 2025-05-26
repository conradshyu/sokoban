/*
 * cspin.h
 * Class implementation of common control spin box in windows
 *
 * Remarks from MSDN
 * An up-down control is a pair of arrow buttons that the user can click to
 * increment or decrement a value, such as a scroll position or a number
 * displayed in a companion control (called a buddy window).
 *
 * Written by Conrad Shyu (shyu4751@yahoo.com)
 * Created on November 6, 2011
 * Revised on November 24, 2011
 * Revised on December 6, 2011
 * Revised on December 10, 2011
*/

// Windows Header Files:
#include "windows.h"

#ifndef _CSPIN_H

#define _CSPIN_H

class CSpin
{
public:
    CSpin();
    ~CSpin();

    bool Init(const HWND, const HWND, const int = 0, const int = 100);
    bool SetBase(const int = 10) const;
    bool SetBuddy(const HWND) const;
    bool SetStatus(const int) const;
    bool SetRange(const int, const int) const;

    int GetMax() const;
    int GetMin() const;
    int GetBase() const;
    int GetStatus() const;

    HWND GetBuddy() const;
    HWND GetHandle() const;
    HINSTANCE GetInstance() const;

private:
    HWND hWnd;           // handles of the window and parent window
    HINSTANCE hInstance; // instance of the main window
};

#endif
