/*
 * cselect.h
 * Class implementation of common control dialog box
 *
 * Written by Conrad Shyu (shyu4751@yahoo.com)
 * Created on November 6, 2011
 * Revised on November 25, 2011
*/

// Windows Header Files:
#include "windows.h"
#include "constant.h"

#include "cspin.h"

#ifndef _CSELECT_H

#define _CSELECT_H

class CSelect
{
public:
    CSelect();
    ~CSelect();

    bool Init(const HWND, const int, const int = 1);
    bool OnInitDialog(const HWND, const WPARAM, const LPARAM);
    bool OnCommand(const HWND, const UINT, const WPARAM, const LPARAM);

    int GetStatus() const;
    int SetStatus(const int);

private:
    HWND hWnd;
    HINSTANCE hInstance;
    INT_PTR nResult;
    int nSelect;

    CSpin spin;
};

#endif // _CSELECT_H
