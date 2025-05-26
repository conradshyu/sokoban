/*
 * Sokoban.cpp : Defines the entry point for the application.
 *
 * Written by Conrad Shyu, September 18, 2011
 * Revised on October 18, 2011
*/

#include "stdafx.h"

#pragma once

#include "windows.h"
#include <atlstr.h>         // for cstring
#include "strsafe.h"
#include "resource.h"

// additional include files
#include "cwinmain.h"

/*
 *  windows program entry point
*/
int APIENTRY _tWinMain(
    const HINSTANCE hInstance,      // handle to this instance
    const HINSTANCE	hPrevInstance,  // handle to previous instance
    const LPTSTR lpCmdLine,         // command line parameters
    const int nCmdShow = SW_SHOW)   // how to show window
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // activate Microsoft FreeType support
    ::SystemParametersInfo(SPI_SETFONTSMOOTHING, TRUE, 0,
        SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
    ::SystemParametersInfo(SPI_GETFONTSMOOTHINGTYPE, 0,
        (PVOID) FE_FONTSMOOTHINGCLEARTYPE, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

    ATL::CString szTitle; szTitle.LoadString(IDS_APP_TITLE);
    ATL::CString szClass; szClass.LoadString(IDC_SOKOBAN);
    CWinMain tMain(szTitle, szClass, hInstance);

    if (!tMain.Init()) {
        return 0;
    }    // make sure window can be created successfully

    tMain.ShowWindow(nCmdShow);
    tMain.UpdateWindow();
    tMain.SetTimer();

    MSG msg;

    while (::GetMessage(&msg, NULL, 0, 0)) {
        if (::TranslateAccelerator(msg.hwnd, tMain.GetAccelHandle(), &msg)) {
            continue;
        }   // skip dialog and short cut messages

        ::TranslateMessage(&msg);   // translates virtual-key messages
        ::DispatchMessage(&msg);    // dispatches message to window procedure
    }   // main message loop

    return  (int) msg.wParam;
}   // end of _tWinMain()
