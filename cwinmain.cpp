/*
 * cwinmain.cpp
 * class implementation for the main window
 *
 * Written by Conrad Shyu (shyu4751@yahoo.com)
 * Created on October 13, 2011
 */

// precompiled header
#include "stdafx.h"
#include "resource.h"

#include "layout.h"
#include "cabout.h"
#include "cselect.h"
#include "cwinmain.h"

const int nBOX[] = {IDB_BOXRED, IDB_BOXYELLOW};
const int nBLOCK[] = {IDB_BRICKGRAY, IDB_BRICKBLUE, IDB_BALL};
const int nDirFRONT = 0;
const int nDirBACK = 1;
const int nDirLEFT = 2;
const int nDirRIGHT = 3;
const int nBrickGRAY = 1;
const int nBrickBLUE = 2;
const int nBall = 3;

const int ID_TIMER = 1;

LRESULT CALLBACK OnMessage(const HWND, const UINT, const WPARAM, const LPARAM);

CWinMain::CWinMain(
    const ATL::CString &_title,
    const ATL::CString &_class,
    const HINSTANCE _hinst) : szTitle(_title), szClass(_class), hInstance(_hinst)
{
    WNDCLASSEX wcex;

    // specify details of window class
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = OnMessage;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOKOBAN));
    wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(WHITE_BRUSH);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SOKOBAN);
    wcex.lpszClassName = szClass;
    wcex.hIconSm = ::LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    ::RegisterClassEx(&wcex); // register the window class
} // end of class constructor

CWinMain::~CWinMain() {
    ::DeleteObject(hBox[0]);
    ::DeleteObject(hBox[1]);
    ::DeleteObject(hBlock[0]);
    ::DeleteObject(hBlock[1]);
    ::DeleteObject(hBlock[2]);
    ::DeleteObject(hAvatar[nDirFRONT][0]);
    ::DeleteObject(hAvatar[nDirFRONT][1]);
    ::DeleteObject(hAvatar[nDirBACK][0]);
    ::DeleteObject(hAvatar[nDirBACK][1]);
    ::DeleteObject(hAvatar[nDirLEFT][0]);
    ::DeleteObject(hAvatar[nDirLEFT][1]);
    ::DeleteObject(hAvatar[nDirRIGHT][0]);
    ::DeleteObject(hAvatar[nDirRIGHT][1]);

    ::KillTimer(hWnd, ID_TIMER);
    ::DestroyWindow(hWnd);
} // end of calss destructor

/*
 * The CreateWindow function creates an overlapped, pop-up, or child window.
 * It specifies the window class, window title, window style, and (optionally)
 * the initial position and size of the window. The function also specifies the
 * window's parent or owner, if any, and the window's menu.
 */
bool CWinMain::Init() {
    RECT box = {0, 0, nMaxWIDTH * nBlockWIDTH, nMaxHEIGHT * nBlockHEIGHT};
    nCurrent = 0, nSuccess = 0, nMove = 0;

    ::AdjustWindowRectEx(&box, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE, NULL);
    hWnd = ::CreateWindowEx(NULL, szClass, szTitle,
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
        box.right - box.left, box.bottom - box.top + nStatusHEIGHT,
        NULL, NULL, hInstance, (LPVOID) this);
    hAccelTable = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOKOBAN));

    return (hWnd) ? true : false;
} // end of Init()

/*
 * The WM_CREATE message is sent when an application requests that a window be
 * created by calling the CreateWindowEx or CreateWindow function. (The message
 * is sent before the function returns.) The window procedure of the new window
 * receives this message after the window is created, but before the window
 * becomes visible.
 */
bool CWinMain::OnCreate(
    const HWND _hwnd) {
    hWnd = _hwnd;
    int box[3] = {118, 400, -1};

    canvas.Init(_hwnd, DKGRAY_BRUSH, false); // virtual screen

    status.Init(_hwnd); // status bar
    status.SetParts(box, 3);
    ShowTime();

    SetTimer(nMaxELAPSE); // timer for animiation
    ::srand(::GetTickCount());

    hBox[0] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(nBOX[0]));
    hBox[1] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(nBOX[1]));
    hBlock[0] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(nBLOCK[0]));
    hBlock[1] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(nBLOCK[1]));
    hBlock[2] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(nBLOCK[2]));

    hAvatar[nDirFRONT][0] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_FRONT1));
    hAvatar[nDirFRONT][1] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_FRONT2));
    hAvatar[nDirBACK][0] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BACK1));
    hAvatar[nDirBACK][1] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BACK2));
    hAvatar[nDirLEFT][0] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LEFT1));
    hAvatar[nDirLEFT][1] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LEFT2));
    hAvatar[nDirRIGHT][0] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RIGHT1));
    hAvatar[nDirRIGHT][1] = ::LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RIGHT2));

    return SetPuzzle();
} // end of OnCreate()

/*
 *  : black background
 * .: blue background
 * #: wall
 * *: ball
 * @: avatar
 * $: empty box
 * &: box with ball
 *
 * the array index ranges from 0 to max - 1, while the rest program
 * uses the range from 1 to max
 */
bool CWinMain::SetPuzzle() {
    nSuccess = 0, nMove = 0;
    lsUndo.clear();
    canvas.Clear();
    nCurrent = ((nCurrent > nMaxGAME) || (nCurrent < 1)) ? 1 : nCurrent;
    --nCurrent; // adjust for the 0 based index

    for (int h = 0; h < nMaxHEIGHT; ++h) {
        for (int w = 0; w < nMaxWIDTH; ++w) {
            switch (szLAYOUT[nCurrent][h * nMaxWIDTH + w]) {
            case '.': // blue background
                nLayout[h][w] = nBrickBLUE;
                nBoxMap[h][w] = 0;
                break;
            case '#': // gray wall
                nLayout[h][w] = nBrickGRAY;
                nBoxMap[h][w] = 0;
                break;
            case '*': // yellow ball
                nLayout[h][w] = nBall;
                nBoxMap[h][w] = 0;
                break;
            case '@': // position of avatar
                nW = w;
                nH = h;
                nLayout[h][w] = nBrickBLUE;
                nBoxMap[h][w] = 0;
                break;
            case '$': // empty box (yellow)
                nLayout[h][w] = nBrickBLUE;
                nBoxMap[h][w] = 1;
                break;
            case '&': // box with ball (red)
                nLayout[h][w] = nBall;
                nBoxMap[h][w] = 1;
                break;
            default: // black background
                nLayout[h][w] = 0;
                nBoxMap[h][w] = 0;
            } // construct the map

            if (nLayout[h][w] == 0) {
                continue;
            } // skip the black background

            SetBlock(w, h, hBlock[(nLayout[h][w] - 1)]);

            if (nBoxMap[h][w] == 1) {
                ++nSuccess;
                SetBlock(w, h, hBox[(nLayout[h][w] == nBall) ? 0 : 1]);
            } // draw the yellow ball
        }     // for each block on the x-axis
    }         // for each block on the y-axis

    ATL::CString szCaption;

    szCaption.Format(_T("Sokoban: %d"), nCurrent + 1);
    ::SetWindowText(hWnd, szCaption.GetBuffer());
    SetStatus(), SetUndo();
    ++nCurrent; // adjust for the 1 based index

    return Move(nDirFRONT, 0, 0);
} // end of SetPuzzle()

bool CWinMain::DoUndo() {
    // restore the position of avatar
    SetBlock(nW, nH, hBlock[(nLayout[nH][nW] - 1)]);
    nW = (lsUndo.back()).avatar.x;
    nH = (lsUndo.back()).avatar.y;
    nGoDir = (lsUndo.back()).dir;
    SetBlock(nW, nH, hAvatar[nGoDir][0]);

    POINT pt;

    // restore the position of box
    pt.x = (lsUndo.back()).box2.x;
    pt.y = (lsUndo.back()).box2.y;
    SetBlock(pt.x, pt.y, hBlock[(nLayout[pt.y][pt.x] - 1)]);
    nBoxMap[pt.y][pt.x] = 0;

    pt.x = (lsUndo.back()).box1.x;
    pt.y = (lsUndo.back()).box1.y;
    SetBlock(pt.x, pt.y, hBox[(nLayout[pt.y][pt.x] == nBall) ? 0 : 1]);
    nBoxMap[pt.y][pt.x] = 1;
    lsUndo.pop_back();

    return SetUndo();
} // end of DoUndo()

bool CWinMain::ShowTime() const {
    SYSTEMTIME time;
    ATL::CString szTime;

    ::GetLocalTime(&time);
    szTime.Format(_T( "%02d:%02d:%02d" ), time.wHour, time.wMinute, time.wSecond);

    return status.SetText(szTime, 2);
} // end of ShowTime()

bool CWinMain::IsWin() {
    int win = 0;

    for (int w = 0; w < nMaxWIDTH; ++w) {
        for (int h = 0; h < nMaxHEIGHT; ++h) {
            win = ((nLayout[h][w] == nBall) && (nBoxMap[h][w] == 1)) ? win + 1 : win;
        } // check the position of box and ball
    }

    return win == nSuccess;
} // end of CheckWin()

bool CWinMain::Move(
    // current direction, height, width
    const int _dir, const int _h, const int _w) {
    stTRACK undo;
    int bw = nW + _w;
    int bh = nH + _h;

    nGoDir = _dir;
    SetBlock(nW, nH, hBlock[(nLayout[nH][nW] - 1)]);
    SetBlock(bw, bh, hBlock[(nLayout[bh][bw] - 1)]);

    if (!nBoxMap[bh][bw]) {
        nW = (nLayout[nH][(nW + _w)] == nBrickGRAY) ? nW : nW + _w;
        nH = (nLayout[(nH + _h)][nW] == nBrickGRAY) ? nH : nH + _h;
    } // there is no box in the way
    else {
        ++nMove;
        undo.avatar.x = nW;
        undo.avatar.y = nH;
        undo.box1.x = bw;
        undo.box1.y = bh;

        nBoxMap[bh][bw] = false;
        bw = ((nLayout[bh][bw + _w] == nBrickGRAY) || (nBoxMap[bh][bw + _w] == nBrickGRAY)) ? bw : bw + _w;
        bh = ((nLayout[bh + _h][bw] == nBrickGRAY) || (nBoxMap[bh + _h][bw] == nBrickGRAY)) ? bh : bh + _h;

        nBoxMap[bh][bw] = true;
        nW = (nBoxMap[nH][nW + _w] == nBrickGRAY) ? nW : nW + _w;
        nH = (nBoxMap[nH + _h][nW] == nBrickGRAY) ? nH : nH + _h;

        SetBlock(bw, bh, hBox[(nLayout[bh][bw] == nBall) ? 0 : 1]);

        undo.box2.x = bw;
        undo.box2.y = bh;
        undo.dir = nGoDir;
        lsUndo.push_back(undo);
        SetUndo(); // set the undo menu
    }              // there is a box in the way

    return SetBlock(nW, nH, hAvatar[nGoDir][0]);
} // end of Move()

bool CWinMain::SetBlock(
    const int _x, // x position of the block
    const int _y, // y position of the block
    const HBITMAP _bmp) const
{
    return canvas.BitBlt(_bmp, _x * nBlockWIDTH, _y * nBlockHEIGHT);
} // end of SetBlock()

bool CWinMain::SetUndo() {
    ::EnableMenuItem(::GetMenu(hWnd), IDM_UNDO, (lsUndo.empty()) ? MF_GRAYED : MF_ENABLED | MF_BYCOMMAND);

    return lsUndo.empty();
} // end of IsUndo()

bool CWinMain::SetTimer(
    const UINT _elapse) const
{
    return ::SetTimer(hWnd, ID_TIMER, _elapse, NULL) ? true : false;
} // end of SetTimer()

const ATL::CString &CWinMain::GetTitle() const
{
    return szTitle;
} // end of GetTitle()

const ATL::CString &CWinMain::GetClass() const
{
    return szClass;
} // end of GetClass()

bool CWinMain::SetStatus() const {
    ATL::CString szText;
    szText.Format(_T( "Your moves: %d" ), nMove);

    return status.SetText(szText);
} // end of SetStatus()

HWND CWinMain::GetHandle() const {
    return hWnd;
} // end of GetHandle()

HACCEL CWinMain::GetAccelHandle() const {
    return hAccelTable;
} // end of GetAccelHandle()

HINSTANCE CWinMain::GetInstance() const {
    return hInstance;
} // end of GetInstance()

bool CWinMain::ShowWindow(
    const int _cmd) const {
    return ::ShowWindow(hWnd, _cmd) ? true : false;
} // end of ShowWindow()

bool CWinMain::UpdateWindow() const {
    return ::UpdateWindow(hWnd) ? true : false;
} // end of UpdateWindow()

bool CWinMain::OnTimer() {
    bTimer = !bTimer;
    SetBlock(nW, nH, hAvatar[nGoDir][(bTimer) ? 1 : 0]);

    return ShowTime();
} // end of OnTimer()

bool CWinMain::OnPaint(
    const HWND _hwnd) const {
    UNREFERENCED_PARAMETER(_hwnd);

    return canvas.OnPaint();
} // end of OnPaint()

/*
 * The WM_ERASEBKGND message is sent when the window background must be erased
 * (for example, when a window is resized). The message is sent to prepare an
 * invalidated portion of a window for painting.
 */
bool CWinMain::OnEraseBKGND() const {
    return true;
} // end of OnEraseBKND()

bool CWinMain::OnExit() const {
    int nUserCmd = ::MessageBox(
        hWnd, _T( "Are you sure you want to exit the game?" ), _T( "Exit" ), MB_ICONQUESTION | MB_YESNO);

    if (nUserCmd == IDYES) {
        ::PostQuitMessage(0);
    } // check the user response

    return true;
} // end of OnExit()

bool CWinMain::OnKeyboard(
    const UINT _msg, const WPARAM _wp, const LPARAM _lp) {
    UNREFERENCED_PARAMETER(_msg);
    UNREFERENCED_PARAMETER(_lp);

    bTimer = false;

    switch ((char)_wp) {
    case VK_UP:
        Move(nDirBACK, -1, 0);
        break;
    case VK_DOWN:
        Move(nDirFRONT, 1, 0);
        break;
    case VK_LEFT:
        Move(nDirLEFT, 0, -1);
        break;
    case VK_RIGHT:
        Move(nDirRIGHT, 0, 1);
        break;
    default:
        return false;
    }

    if (IsWin()) {
        ::MessageBox(hWnd, _T("You Win! Good Job!"), _T("You Win"), MB_ICONINFORMATION | MB_OK);
        ++nCurrent; SetPuzzle();    // next game
    }   // check if there is a win

    return SetStatus();
} // end of OnKeyboard()

LRESULT CALLBACK CWinMain::OnCommand(
    const UINT _msg, const WPARAM _wp, const LPARAM _lp) {
    int wmId = LOWORD(_wp);
    int wmEvent = HIWORD(_wp);

    CAbout about;   // create an instance of about box
    CSelect select; // create an instance of selection box

    switch (wmId) {
    case IDM_NEWGAME:
        nCurrent = ::rand() % nMaxGAME;
    case IDM_RESTART:
        SetPuzzle();
        break;
    case IDM_SELECTGAME:
        select.Init(hWnd, IDD_SELECT, nCurrent);
        nCurrent = select.GetStatus();
        SetPuzzle();
        break;
    case IDM_UNDO:
        DoUndo();
        break;
    case IDM_ABOUT:
        about.Init(hWnd, IDD_ABOUTBOX);
        break;
    case IDM_EXIT:
        OnExit();
        break;
    default:
        return ::DefWindowProc(hWnd, _msg, _wp, _lp);
    }

    SetStatus(); return TRUE;
} // end of OnCommand()

LRESULT CALLBACK OnMessage(
    const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
    CWinMain *q;

    if (message == WM_NCCREATE) {
        q = (CWinMain *)((LPCREATESTRUCT)lParam)->lpCreateParams;
        ::SetWindowLongPtr(hWnd, GWLP_USERDATA, (long) q);
    } // save pointer to the message processing function
    else {
        q = (CWinMain *)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    } // retrieve pointer to the message processing function

    switch (message) {
    case WM_CREATE:
        q->OnCreate(hWnd);
        break;
    case WM_COMMAND:
        q->OnCommand(message, wParam, lParam);
        break;
    case WM_PAINT:
        q->OnPaint(hWnd);
        break;
    case WM_ERASEBKGND: // don't redraw the background
        q->OnEraseBKGND();
        break;
    case WM_KEYDOWN:
        q->OnKeyboard(message, wParam, lParam);
        break;
    case WM_TIMER:
        q->OnTimer();
        break;
    case WM_QUIT:
        ::DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    } // process events generated by windows

    return 0;
} // end of WndProc()
