/*
 * cabout.h
 * Class implementation for the about dialog box
 *
 * Remark from MSDN Library
 * The DialogBox macro uses the CreateWindowEx function to create the dialog
 * box. DialogBox then sends a WM_INITDIALOG message (and a WM_SETFONT message
 * if the template specifies the DS_SETFONT or DS_SHELLFONT style) to the dialog
 * box procedure. The function displays the dialog box (regardless of whether
 * the template specifies the WS_VISIBLE style), disables the owner window, and
 * starts its own message loop to retrieve and dispatch messages for the dialog
 * box.
 *
 * When the dialog box procedure calls the EndDialog function, DialogBox destroys
 * the dialog box, ends the message loop, enables the owner window (if previously
 * enabled), and returns the nResult parameter specified by the dialog box
 * procedure when it called EndDialog.
 *
 * Written by Conrad Shyu (shyu4751@yahoo.com)
 * Created on October 13, 2011
 * Revised on October 14, 2011
 * Revised on October 20, 2011
*/

// Windows Header Files:
#include "windows.h"
#include "resource.h"

#ifndef _CABOUT_H

#define _CABOUT_H

class CAbout
{
public:
    CAbout();
    ~CAbout();

    bool Init(const HWND, const int);
    bool OnCommand(const HWND, const UINT, const WPARAM, const LPARAM);

    HWND GetHandle() const;
    HINSTANCE GetInstance() const;

private:
    HWND hWnd;
    HINSTANCE hInstance;
    INT_PTR nResult;
}; // class implementation of about box

#endif // _CABOUT_H
