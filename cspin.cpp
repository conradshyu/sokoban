/*
 * cspin.cpp
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

// precompiled header
#include "stdafx.h"
#include <commctrl.h>

#include "cspin.h"

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
CSpin::CSpin()
{
    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_UPDOWN_CLASS;

    if (!::InitCommonControlsEx(&iccx))
    {
        return;
    } // make sure the control can be initialized successfully
} // end of default constructor

/*
 * The DestroyWindow function destroys the specified window. The function sends
 * WM_DESTROY and WM_NCDESTROY messages to the window to deactivate it and
 * remove the keyboard focus from it. The function also destroys the window's
 * menu, flushes the thread message queue, destroys timers, removes clipboard
 * ownership, and breaks the clipboard viewer chain (if the window is at the top
 * of the viewer chain).
*/
CSpin::~CSpin()
{
    ::DestroyWindow(hWnd);
} // end of destructor

/*
 * Remarks from MSDN, Up down control Windows style:
 *
 * UDS_ALIGNLEFT
 * Positions the up-down control next to the left edge of the buddy window.
 * The buddy window is moved to the right, and its width is decreased to
 * accommodate the width of the up-down control.
 *
 * UDS_ALIGNRIGHT
 * Positions the up-down control next to the right edge of the buddy window.
 * The width of the buddy window is decreased to accommodate the width of the
 * up-down control.
 *
 * UDS_ARROWKEYS
 * Causes the up-down control to increment and decrement the position when the
 * UP ARROW and DOWN ARROW keys are pressed.
 *
 * UDS_AUTOBUDDY
 * Automatically selects the previous window in the z-order as the up-down
 * control's buddy window.
 *
 * UDS_HORZ
 * Causes the up-down control's arrows to point left and right instead of up
 * and down.
 *
 * UDS_HOTTRACK
 * Causes the control to exhibit "hot tracking" behavior. That is, it
 * highlights the UP ARROW and DOWN ARROW on the control as the pointer passes
 * over them. This style requires Windows 98 or Windows 2000. If the system is
 * running Windows 95 or Windows NT 4.0, the flag is ignored. To check whether
 * hot tracking is enabled, call SystemParametersInfo.
 *
 * UDS_NOTHOUSANDS
 * Does not insert a thousands separator between every three decimal digits.
 *
 * UDS_SETBUDDYINT
 * Causes the up-down control to set the text of the buddy window (using the
 * WM_SETTEXT message) when the position changes. The text consists of the
 * position formatted as a decimal or hexadecimal string.
 *
 * UDS_WRAP
 * Causes the position to "wrap" if it is incremented or decremented beyond
 * the ending or beginning of the range.
*/
bool CSpin::Init(const HWND _hwnd, const HWND _buddy, const int _min, const int _max) {
    hInstance = (HINSTANCE)(::GetWindowLongPtr(_hwnd, GWLP_HINSTANCE));
    hWnd = ::CreateWindowEx(NULL, UPDOWN_CLASS, NULL,
        WS_CHILD | WS_BORDER | WS_VISIBLE | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_SETBUDDYINT,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, _hwnd, NULL, hInstance, NULL);

    if (!hWnd) {
        return (false);
    } // make sure window can be created successfully

    SetRange(_min, _max);
    SetStatus(_min);
    SetBuddy(_buddy);
    return(::ShowWindow(hWnd, SW_SHOW) ? true : false);
} // end of Init()

/*
 * UDM_GETBASE
 * Retrieves acceleration information for an up-down control.
 *
 * wParam: Number of elements in the array specified by lParam.
 * lParam: Pointer to an array of UDACCEL structures that receive acceleration
 * information.
*/
int CSpin::GetBase() const
{
    return ((int)::SendMessage(hWnd, UDM_GETBASE, 0, 0));
} // end of GetAccel()

/*
 * UDM_GETBUDDY
 * Retrieves the handle to the current buddy window.
 *
 * wParam: Must be zero.
 * lParam: Must be zero. 
*/
HWND CSpin::GetBuddy() const
{
    return ((HWND)::SendMessage(hWnd, UDM_GETBUDDY, 0, 0));
} // end of GetBuddy()

/*
 * UDM_GETPOS32
 * Returns the 32-bit position of an up-down control.
 *
 * wParam: Must be zero.
 * lParam: Pointer to a BOOL value that is set to zero if the value is
 * successfully retrieved or nonzero if an error occurs. If this parameter is
 * set to NULL, errors are not reported. If UDM_GETPOS32 is used in a cross
 * process situation, this parameter must be NULL.
*/
int CSpin::GetStatus() const
{
    BOOL bStatus;
    int nReturn = (int)::SendMessage(hWnd, UDM_GETPOS32, 0, (LPARAM)&bStatus);

    return ((bStatus == 0) ? nReturn : GetMin());
} // end of GetStatus()

/*
 * UDM_GETRANGE32
 * Retrieves the 32-bit range of an up-down control.
 *
 * wParam: Pointer to a signed integer that receives the lower limit of the
 * up-down control range. This parameter may be NULL.
 * lParam: Pointer to a signed integer that receives the upper limit of the
 * up-down control range. This parameter may be NULL.
*/
int CSpin::GetMax() const
{
    int nMin, nMax;

    ::SendMessage(hWnd, UDM_GETRANGE32, (WPARAM)&nMin, (LPARAM)&nMax);

    return (nMax);
} // end of GetMax()

int CSpin::GetMin() const
{
    int nMin, nMax;

    ::SendMessage(hWnd, UDM_GETRANGE32, (WPARAM)&nMin, (LPARAM)&nMax);

    return (nMin);
} // end of GetMin()

/*
 * UDM_SETBASE
 * Sets the radix base for an up-down control. The base value determines
 * whether the buddy window displays numbers in decimal or hexadecimal digits.
 * Hexadecimal numbers are always unsigned, and decimal numbers are signed.
 *
 * wParam: New base value for the control. This parameter can be 10 for decimal
 * or 16 for hexadecimal.
 * lParam: Must be zero. 
*/
bool CSpin::SetBase(
    const int _base) const
{
    ::SendMessage(hWnd, UDM_SETBASE, _base, 0);

    return (true);
} // end of SetBase()

/*
 * UDM_SETBUDDY
 * Sets the buddy window for an up-down control.
 *
 * wParam: Handle to the new buddy window.
 * lParam: Must be zero. 
*/
bool CSpin::SetBuddy(
    const HWND _buddy) const
{
    ::SendMessage(hWnd, UDM_SETBUDDY, (WPARAM)_buddy, 0);

    return (true);
} // end of SetBuddy()

/*
 * UDM_SETPOS32
 * Sets the position of an up-down control with 32-bit precision.
 *
 * wParam: Must be zero.
 * lParam: Variable of type integer that specifies the new position for the
 * up-down control. If the parameter is outside the control's specified range,
 * lParam is set to the nearest valid value.
*/
bool CSpin::SetStatus(
    const int _i) const
{
    ::SendMessage(hWnd, UDM_SETPOS32, 0, (LPARAM)_i);

    return (true);
} // end of SetStatus()

/*
 * UDM_SETRANGE32
 * Sets the 32-bit range of an up-down control.
 *
 * wParam: Signed integer value that represents the new lower limit of the
 * up-down control range.
 * lParam: Signed integer value that represents the new upper limit of the
 * up-down control range.
*/
bool CSpin::SetRange(
    const int _min, const int _max) const
{
    ::SendMessage(hWnd, UDM_SETRANGE32, (WPARAM)_min, (WPARAM)_max);

    return (true);
} // end of SetRange()

HWND CSpin::GetHandle() const
{
    return (hWnd);
} // end of GetHandle()

HINSTANCE CSpin::GetInstance() const
{
    return (hInstance);
} // end of GetInstance()
