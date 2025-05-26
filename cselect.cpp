/*
 * cselect.cpp
 * Class implementation of common control dialog box
 *
 * Written by Conrad Shyu (shyu4751@yahoo.com)
 * Created on November 6, 2011
 * Revised on November 25, 2011
*/
#include "stdafx.h"
#include "resource.h"

#include "cselect.h"

INT_PTR CALLBACK OnSelect(const HWND, const UINT, const WPARAM, const LPARAM);

CSelect::CSelect()
{
} // end of default constructor

CSelect::~CSelect()
{
    //::EndDialog( hWnd, nResult );
} // end of destructor

int CSelect::GetStatus() const
{
    return (nSelect);
} // end of GetStatus()

int CSelect::SetStatus(
    const int _i)
{
    return (nSelect = _i);
} // end of SetStatus()

bool CSelect::Init(
    const HWND _hwnd, const int _id, const int _select)
{
    hWnd = _hwnd;
    nSelect = _select;
    hInstance = (HINSTANCE)(::GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
    nResult = ::DialogBoxParam(hInstance, MAKEINTRESOURCE(_id), hWnd, OnSelect, (long)this);

    return (!(nResult == -1) ? true : false);
} // end of Init()

bool CSelect::OnInitDialog(
    const HWND _hwnd, const WPARAM _wp, const LPARAM _lp)
{
    UNREFERENCED_PARAMETER(_wp);
    UNREFERENCED_PARAMETER(_lp);

    spin.Init(_hwnd, (HWND)::GetDlgItem(_hwnd, IDC_SELECT), 1, nMaxGAME);
    spin.SetStatus(nSelect);

    return (true);
} // end of OnInitDialog()

bool CSelect::OnCommand(
    const HWND _hwnd, const UINT _msg, const WPARAM _wp, const LPARAM _lp)
{
    UNREFERENCED_PARAMETER(_msg);
    UNREFERENCED_PARAMETER(_lp);

    switch (LOWORD(_wp))
    {
    case IDOK:
        nSelect = spin.GetStatus();
    case IDCANCEL:
        ::EndDialog(_hwnd, LOWORD(_wp));
        break;
    default: // unknown message
        return (false);
    }

    return (true);
} // end of OnCommand()

/*
 * Message handler for multithread control panel dialog box
 * return true if messages have been processed; otherwise false
*/
INT_PTR CALLBACK OnSelect(
    const HWND _hwnd, const UINT _msg, const WPARAM _wp, const LPARAM _lp)
{
    if (_msg == WM_INITDIALOG)
    {
        ::SetWindowLongPtr(_hwnd, DWLP_USER, (long)_lp);
    } // save the pointer to the structure

    // retrieve the pointer from the structure
    CSelect *q = (CSelect *)::GetWindowLongPtr(_hwnd, DWLP_USER);

    switch (_msg)
    {
    case WM_INITDIALOG:
        q->OnInitDialog(_hwnd, _wp, _lp);
        break;
    case WM_COMMAND:
        q->OnCommand(_hwnd, _msg, _wp, _lp);
        break;
    default:
        return ((INT_PTR)FALSE);
    }

    return ((INT_PTR)TRUE);
} // end of OnSelect()
