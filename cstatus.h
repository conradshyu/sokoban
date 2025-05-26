/*
 * cstatus.h
 *
 * Remark from MSDN
 * A status bar control is a horizontal window, usually displayed at the bottom
 * of a parent window, in which an application can display various kinds of
 * status information. The status bar control can be divided into parts to
 * display more than one type of information.
 *
 * Written by Conrad Shyu (shyu4751@yahoo.com)
 * Created on October 29, 2011
 * Revised on December 6, 2011
 * Revised on December 9, 2011
*/

// Windows Header Files:
#include "windows.h"
#include <commctrl.h>
#include <atlstr.h> // for cstring
#include "strsafe.h"

#ifndef _CSTATUS_H

#define _CSTATUS_H

class CStatus
{
public:
    CStatus();
    ~CStatus();

    bool Init(const HWND);
    bool IsSimple() const;
    bool SetHeight(const int) const;
    bool SetBkColor(const COLORREF) const;
    bool SetSimple(const bool = true) const;
    bool SetParts(const int *, const int) const;
    bool SetIcon(const HICON, const int = 0) const;
    bool SetText(ATL::CString &, const int = 0) const;
    bool SetTipText(ATL::CString &, const int = 0) const;

    int GetDrawStyle(const int = 0) const;
    int GetTextLength(const int = 0) const;

    const int *GetBorders(int *) const;
    const int *GetParts(int *, const int = 0) const;
    const RECT &GetSize(RECT &, const int = 0) const;
    const ATL::CString &GetText(ATL::CString &, const int = 0);
    const ATL::CString &GetTipText(ATL::CString &, const int = 0);

    HWND GetHandle() const;
    HINSTANCE GetInstance() const;
    HICON GetIcon(const int = 0) const;

private:
    HWND hWnd;
    HINSTANCE hInstance;
};

#endif // _CSTATUS_H
