/*
 * cstatus.cpp
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

#include "stdafx.h"
#include <commctrl.h>

#include "cstatus.h"

/*
 * Ensures that the common control DLL (Comctl32.dll) is loaded, and registers
 * specific common control classes from the DLL. An application must call this
 * function before creating a common control.
 *
 * ICC_ANIMATE_CLASS        Load animate control class.
 * ICC_BAR_CLASSES          Load toolbar, status bar, trackbar, and tooltip
 *                          control classes.
 * ICC_COOL_CLASSES         Load rebar control class.
 * ICC_DATE_CLASSES         Load date and time picker control class.
 * ICC_HOTKEY_CLASS         Load hot key control class.
 * ICC_INTERNET_CLASSES     Load IP address class.
 * ICC_LINK_CLASS           Load a hyperlink control class.
 * ICC_LISTVIEW_CLASSES     Load list-view and header control classes.
 * ICC_NATIVEFNTCTL_CLASS   Load a native font control class.
 * ICC_PAGESCROLLER_CLASS   Load pager control class.
 * ICC_PROGRESS_CLASS       Load progress bar control class.
 * ICC_STANDARD_CLASSES     Load one of the intrinsic User32 control classes.
 *                          The user controls include button, edit, static,
 *                          listbox, combobox, and scroll bar.
 * ICC_TAB_CLASSES          Load tab and tooltip control classes.
 * ICC_TREEVIEW_CLASSES     Load tree-view and tooltip control classes.
 * ICC_UPDOWN_CLASS         Load up-down control class.
 * ICC_USEREX_CLASSES       Load ComboBoxEx class.
 * ICC_WIN95_CLASSES        Load animate control, header, hot key, list-view,
 *                          progress bar, status bar, tab, tooltip, toolbar,
 *                          trackbar, tree-view, and up-down control classes.
*/
CStatus::CStatus()
{
    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_BAR_CLASSES;

    if (!::InitCommonControlsEx(&iccx))
    {
        return;
    } // make sure the control can be initialized successfully
} // end of default constructor

CStatus::~CStatus()
{
    ::DestroyWindow(hWnd);
} // end of destructor

/*
 * This styles, in addition to standard window styles, supported by status bar
 * controls.
 *
 * SBARS_SIZEGRIP
 * The status bar control will include a sizing grip at the right end of the
 * status bar. A sizing grip is similar to a sizing border; it is a rectangular
 * area that the user can click and drag to resize the parent window.
 *
 * SBT_TOOLTIPS
 * Version 4.71. Use this style to enable tooltips.
 *
 * SBARS_TOOLTIPS
 * Version 5.80. Identical to SBT_TOOLTIPS. Use this flag for versions 5.00 or
 * later.
*/
bool CStatus::Init(
    const HWND _hwnd)
{
    hInstance = (HINSTANCE)::GetWindowLongPtr(_hwnd, GWLP_HINSTANCE);
    hWnd = ::CreateWindowEx(NULL, STATUSCLASSNAME, NULL,
        WS_CHILD | WS_BORDER | WS_VISIBLE | SBT_TOOLTIPS,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        _hwnd, NULL, hInstance, NULL);

    return (::ShowWindow(hWnd, SW_SHOW) ? true : false);
} // end of Init()

/*
 * SB_GETBORDERS
 * Retrieves the current widths of the horizontal and vertical borders of a
 * status window.
 *
 * wParam: Must be zero.
 * lParam: Pointer to an integer array that has three elements. The first
 * element receives the width of the horizontal border, the second receives the
 * width of the vertical border, and the third receives the width of the border
 * between rectangles.
*/
const int *CStatus::GetBorders(
    int *_a) const
{
    ::SendMessage(hWnd, SB_GETBORDERS, 0, (LPARAM)&_a);

    return (_a);
} // end of GetBorders

/*
 * SB_GETICON
 * Retrieves the icon for a part in a status bar.
 *
 * wParam: Zero-based index of the part that contains the icon to be retrieved.
 * If this parameter is -1, the status bar is assumed to be a Simple Mode
 * status bar.
 * lParam: Must be zero. 
*/
HICON CStatus::GetIcon(
    const int _i) const
{
    return ((HICON)::SendMessage(hWnd, SB_GETICON, _i, 0));
} // end of GetIcon()

/*
 * SB_GETPARTS
 * Retrieves a count of the parts in a status window. The message also
 * retrieves the coordinate of the right edge of the specified number of parts.
 *
 * wParam: Number of parts for which to retrieve coordinates. If this parameter
 * is greater than the number of parts in the window, the message retrieves
 * coordinates for existing parts only.
 * lParam: Pointer to an integer array that has the same number of elements as
 * parts specified by wParam. Each element in the array receives the client
 * coordinate of the right edge of the corresponding part. If an element is set
 * to -1, the position of the right edge for that part extends to the right
 * edge of the window. To retrieve the current number of parts, set this
 * parameter to zero.
 *
 * Return value
 * Returns the number of parts in the window. 
*/
const int *CStatus::GetParts(
    int *_lp, const int _wp) const
{
    ::SendMessage(hWnd, SB_GETPARTS, _wp, (LPARAM)_lp);

    return (_lp);
} // end of GetParts()

/*
 * SB_GETRECT
 * Retrieves the bounding rectangle of a part in a status window.
 *
 * wParam: Zero-based index of the part whose bounding rectangle is to be
 * retrieved.
 * lParam: Pointer to a RECT structure that receives the bounding rectangle.
*/
const RECT &CStatus::GetSize(
    RECT &_box, const int _i) const
{
    ::SendMessage(hWnd, SB_GETRECT, _i, (LPARAM)&_box);

    return (_box);
} // end of GetSize()

/*
 * SB_GETTEXTLENGTH
 * Retrieves the length, in characters, of the text from the specified part of
 * a status window.
 *
 * wParam: Zero-based index of the part from which to retrieve text.
 * lParam: Must be zero. 
 *
 * Return value
 * Returns a 32-bit value that consists of two 16-bit values. The low word
 * specifies the length, in characters, of the text. The high word specifies
 * the type of operation used to draw the text. The type can be one of the
 * following values:
 *
 * 0                The text is drawn with a border to appear lower than the
 *                  plane of the window.
 * SBT_NOBORDERS    The text is drawn without borders.
 * SBT_OWNERDRAW    The text is drawn by the parent window.
 * SBT_POPOUT       The text is drawn with a border to appear higher than the
 *                  plane of the window.
 * SBT_RTLREADING   The text will be displayed in the opposite direction to the
 *                  text in the parent window.
*/
int CStatus::GetTextLength(
    const int _i) const
{
    return (LOWORD((int)::SendMessage(hWnd, SB_GETTEXTLENGTH, _i, 0)));
} // end of GetTextLength()

int CStatus::GetDrawStyle(
    const int _i) const
{
    return (HIWORD((int)::SendMessage(hWnd, SB_GETTEXTLENGTH, _i, 0)));
} // end of GetDrawStyle()

/*
 * SB_GETTIPTEXT
 * Retrieves the tooltip text for a part in a status bar. The status bar must
 * be created with the SBT_TOOLTIPS style to enable tooltips.
 *
 * wParam: The LOWORD specifies the zero-based index of the part that receives
 * the tooltip text. The HIWORD specifies the size of the buffer at lParam, in
 * characters.
 * lParam: Pointer to a character buffer that receives the tooltip text.
*/
const CString &CStatus::GetTipText(
    ATL::CString &_t, const int _i)
{
    ::SendMessage(hWnd, SB_GETTIPTEXT, MAKELONG(_i, 256), (LPARAM)_t.GetBuffer());

    return (_t);
} // end of GetTipText()

/*
 * SB_GETTEXT
 * Retrieves the text from the specified part of a status window.
 *
 * wParam: Zero-based index of the part from which to retrieve text.
 * lParam: Pointer to the buffer that receives the text as a null-terminated
 * string. Use the SB_GETTEXTLENGTH message to determine the required size of
 * the buffer.
*/
const CString &CStatus::GetText(
    ATL::CString &_t, const int _i)
{
    ::SendMessage(hWnd, SB_GETTEXT, _i, (LPARAM)_t.GetBuffer());

    return (_t);
} // end of GetText()

/*
 * SB_ISSIMPLE
 * Checks a status bar control to determine if it is in simple mode.
 *
 * wParam: Must be zero.
 * lParam: Must be zero. 
*/
bool CStatus::IsSimple() const
{
    return (::SendMessage(hWnd, SB_ISSIMPLE, 0, 0) ? true : false);
} // end of IsSimple()

/*
 * SB_SETBKCOLOR
 * Sets the background color in a status bar.
 *
 * wParam: Must be zero.
 * lParam: COLORREF value that specifies the new background color. Specify the
 * CLR_DEFAULT value to cause the status bar to use its default background
 * color.
*/
bool CStatus::SetBkColor(
    const COLORREF _c) const
{
    ::SendMessage(hWnd, SB_SETBKCOLOR, 0, (LPARAM)_c);

    return (true);
} // end of SetBkColor()

/*
 * SB_SETICON
 * Sets the icon for a part in a status bar.
 *
 * wParam: Zero-based index of the part that will receive the icon. If this
 * parameter is -1, the status bar is assumed to be a simple status bar.
 * lParam: Handle to the icon to be set. If this value is NULL, the icon is
 * removed from the part.
*/
bool CStatus::SetIcon(
    const HICON _icon, const int _i) const
{
    return (::SendMessage(hWnd, SB_SETICON, _i, (LPARAM)_icon) ? true : false);
} // end of SetIcon()

/*
 * SB_SETMINHEIGHT
 * Sets the minimum height of a status window's drawing area.
 *
 * wParam: Minimum height, in pixels, of the window.
 * lParam: Must be zero. 
*/
bool CStatus::SetHeight(
    const int _h) const
{
    ::SendMessage(hWnd, SB_SETMINHEIGHT, _h, 0);

    return (true);
} // end of SetHeight()

/*
 * SB_SETPARTS
 * Sets the number of parts in a status window and the coordinate of the right
 * edge of each part.
 *
 * wParam: Number of parts to set (cannot be greater than 256).
 * lParam: Pointer to an integer array. The number of elements is specified in
 * wParam. Each element specifies the position, in client coordinates, of the
 * right edge of the corresponding part. If an element is -1, the right edge of
 * the corresponding part extends to the border of the window.
*/
bool CStatus::SetParts(
    const int *_p, const int _s) const
{
    return (::SendMessage(hWnd, SB_SETPARTS, _s, (LPARAM)_p) ? true : false);
} // end of SetParts()

/*
 * SB_SETTIPTEXT
 * Sets the tooltip text for a part in a status bar. The status bar must have
 * been created with the SBT_TOOLTIPS style to enable tooltips.
 *
 * wParam: Zero-based index of the part that will receive the tooltip text.
 * lParam: Pointer to a character buffer that contains the new tooltip text.
*/
bool CStatus::SetTipText(
    ATL::CString &_t, const int _i) const
{
    ::SendMessage(hWnd, SB_SETTIPTEXT, _i, (LPARAM)_t.GetBuffer());

    return (true);
} // end of SetTipText()

/*
 * SB_SETTEXT
 * Sets the text in the specified part of a status window.
 *
 * wParam: The LOBYTE of the low-order word specifies the zero-based index of
 * the part to set.
 * lParam: Pointer to a null-terminated string that specifies the text to set.
 * If wParam is SBT_OWNERDRAW, this parameter represents 32 bits of data. The
 * parent window must interpret the data and draw the text when it receives the
 * WM_DRAWITEM message. In Windows XP and earlier, the text for each part is
 * limited to 127 characters. This limitation has been removed in Windows
 * Vista.
*/
bool CStatus::SetText(
    ATL::CString &_t, const int _i) const
{
    ::SendMessage(hWnd, SB_SETTEXT, _i, (LPARAM)_t.GetBuffer());

    return (true);
} // end of SetText()

/*
 * SB_SIMPLE
 * Specifies whether a status window displays simple text or displays all
 * window parts set by a previous SB_SETPARTS message.
 *
 * wParam: Display type flag. If this parameter is TRUE, the window displays
 * simple text. If it is FALSE, it displays multiple parts.
 * lParam: Must be zero.
 *
 * Remarks
 * If the status window is being changed from nonsimple to simple, or vice
 * versa, the window is immediately redrawn. 
*/
bool CStatus::SetSimple(
    const bool _set) const
{
    ::SendMessage(hWnd, SB_SIMPLE, (_set) ? TRUE : FALSE, 0);

    return (true);
} // end of SetSimple()

HWND CStatus::GetHandle() const
{
    return (hWnd);
} // end of GetHandle()

HINSTANCE CStatus::GetInstance() const
{
    return (hInstance);
} // end of GetInstance()
