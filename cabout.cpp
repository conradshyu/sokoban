/*
 * cabout.cpp
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

#include "stdafx.h" // precompiled header
#include "cabout.h"

INT_PTR CALLBACK OnAbout(const HWND, const UINT, const WPARAM, const LPARAM);

CAbout::CAbout() {
    hWnd = 0;
    nResult = 0;
    hInstance = 0;
} // end of default constructor

CAbout::~CAbout() {
    //::EndDialog( hWnd, nResult );
} // end of destructor

/*
 * This function typically fails for one of the following reasons:
 *
 * 1. an invalid parameter value
 * 2. the system class was registered by a different module
 * 3. the WH_CBT hook is installed and returns a failure code
 * 4. if one of the controls in the dialog template is not registered, or its
 *    window window procedure fails WM_CREATE or WM_NCCREATE
*/
bool CAbout::Init(
    const HWND _hwnd, const int _id) {
    hWnd = _hwnd;
    hInstance = (HINSTANCE)(::GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
    nResult = ::DialogBox(hInstance, MAKEINTRESOURCE(_id), hWnd, OnAbout);

    return ((nResult == -1) ? false : true);
} // end of Init()

HWND CAbout::GetHandle() const {
    return (hWnd);
} // end of GetHandle()

HINSTANCE CAbout::GetInstance() const {
    return (hInstance);
} // end of GetInstance()

bool CAbout::OnCommand(
    const HWND _hwnd, const UINT _msg, const WPARAM _wp, const LPARAM _lp) {
    UNREFERENCED_PARAMETER(_msg);
    UNREFERENCED_PARAMETER(_lp);

    switch (LOWORD(_wp)) {
    case IDOK:
    case IDCANCEL:
        ::EndDialog(_hwnd, LOWORD(_wp));
        break;
    default: // unknown message
        return (false);
    }

    return (true);
} // end of OnCommand()

// Message handler for about box.
// return true if messages have been processed; otherwise false
INT_PTR CALLBACK OnAbout(
    const HWND _hwnd, const UINT _msg, const WPARAM _wp, const LPARAM _lp) {

    if (_msg == WM_INITDIALOG) {
        ::SetWindowLongPtr(_hwnd, DWLP_USER, (long)_lp);
    } // save the pointer to the structure

    // retrieve the pointer from the structure
    CAbout *q = (CAbout *)::GetWindowLongPtr(_hwnd, DWLP_USER);

    switch (_msg) {
    case WM_INITDIALOG:
        break;
    case WM_COMMAND:
        q->OnCommand(_hwnd, _msg, _wp, _lp);
        break;
    default:
        return ((INT_PTR)FALSE);
    } // process the messages

    return ((INT_PTR)TRUE);
} // end of OnMessage()
