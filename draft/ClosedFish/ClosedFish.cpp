// ClosedFish.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ClosedFish.h"
#include <iostream>
#include <fstream>

#define MAX_LOADSTRING 100

#define ARGB(A, R, G, B) (COLORREF)(((unsigned int)A<<24) + (unsigned int)(B<<16) + (unsigned int)(G<<8) + (unsigned int)(R))

#pragma region globalVariables //these are my global variables, which i will not mix with the windows objects
bmpClass bc = NULL;
timer tmrTransOnTop, tmrCheckMove;
bool started = false;
#pragma endregion globalVariables



// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWndTrans;
LPCWSTR TClassName = L"TransparentOveray";
HRGN hRgn;
LPCSTR curStatus = "Engine started"; //The engine starts when the UI starts
LPCTSTR consoleName = "Executable.exe"; 
HWND consoleH;
STARTUPINFO si;
PROCESS_INFORMATION pi;
TCHAR szFilePath[MAX_PATH];
DWORD processId;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcT(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK StartBtnMsg(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

#pragma region testFunc

std::thread t1;

bool cont = true;

void consoleTest()//testing different stuff using the console
{
    AllocConsole();
    //opening the standard streams for use
    FILE* stream;
    freopen_s(&stream, "CONIN$", "r", stdin);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    freopen_s(&stream, "CONOUT$", "w", stdout);
    std::cout << std::thread::hardware_concurrency() << "\n";
    int nr = -1;
    while (cont)
    {
        std::cin >> nr;
        switch (nr)
        {
        case -18:
            keybd_event(0x41, 0, KEYEVENTF_EXTENDEDKEY | 0, NULL);
            keybd_event(0x41, 0, KEYEVENTF_KEYUP, NULL);
            //bc = bmpClass(true);
            //bc.send_input("g8f6");
            //bc.send_input("c1f4");
            break;
        case -19:
            std::cout<<" "<<bc.castleLeft()<<" " << bc.castleRight() << "\n";
            break;
        case -1:
            FreeConsole();
            break;
        case -9:
            bc.split_the_board();
            break;
        case -7:
        {
            int c1, c2;
            bc.getBoardColours(c1, c2);
            break;
        }
        case -3:
            POINT p;
            GetCursorPos(&p);
            bc.sendClick(p.x, p.y);
            break;
        case -21:
        {
            char* path = (char*)bc._saveScreenToFileWithType((LPCSTR)"Test.png", 1);
            //HarrisCorner::addCorners();
            //auto dpair = giveBoard(path, bc.giveW(), bc.giveH());
            //std::cout << dpair.first.first << " " << dpair.first.second << "<-\n";
            //std::cout << dpair.second.first << " " << dpair.second.second << "<-\n"; 
        }
        break;
        case -14:
            tmrTransOnTop.shutdown = true;
            break;
        case -4:
            bc.testRemoveBg();
            break;
        case -33:
            bc.findAllPieces();
            break;
        default:
            bc.printSq(nr);
            std::cout << nr << "<-\n";
            break;
            break;
        }
        nr = -1;//Safety measure so external calls to FreeConsole don't call printSq on some deleted HBITMAP object
    }
}

void displayError()
{
#if _DEBUG
    DWORD dw = GetLastError();

    LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK);
    //ExitProcess(dw);
#endif
}

void SetWindowOnTop(HWND hWnd)
{
    if (!SetWindowPos(hWndTrans, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
    {
        std::cout << "->" << tmrTransOnTop.shutdown << "\n";
        displayError();
    }
}

BOOL DisplayTransparent(HINSTANCE hInst, int nCmdShow)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcT;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = TClassName;
    wcex.hIconSm = NULL;

    RegisterClassExW(&wcex);

    hWndTrans = CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        TClassName, NULL, WS_POPUP,
        GetSystemMetrics(SM_CXSCREEN) * 0.02, GetSystemMetrics(SM_CYSCREEN) * 0.02, 350, 150,
        NULL, NULL, hInst, NULL);

    if (!hWndTrans)
    {
        return FALSE;
    }

    SetLayeredWindowAttributes(hWndTrans, RGB(11, 12, 13), 150, LWA_COLORKEY | LWA_ALPHA);
    
    hRgn = CreateRoundRectRgn(GetSystemMetrics(SM_CXSCREEN) * 0.02, GetSystemMetrics(SM_CYSCREEN) * 0.02,
        350 - GetSystemMetrics(SM_CXSCREEN) * 0.02, 150 - GetSystemMetrics(SM_CYSCREEN) * 0.02,
        10, 10);

    SetWindowRgn(hWndTrans, hRgn, true);

    ShowWindow(hWndTrans, SW_SHOW);
    UpdateWindow(hWndTrans);

    //I want this window the be always on top while the app is running
    if (!SetWindowPos(hWndTrans, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
    {
        displayError();
    }

    //This puts the window back on top
    //It s needed for when something pops up above the text window
    timers::start_timer(tmrTransOnTop, SetWindowOnTop, hWndTrans, 500);

    return TRUE;
}
#pragma endregion testFunc

void startConsole()
{

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    PathRemoveFileSpec(szFilePath);
    PathAppend(szFilePath, _T("Executable.exe"));

    CreateProcessA(szFilePath,
        NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    consoleH = (HWND)pi.hProcess;
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    std::cout << "AA\n";
    DWORD windowProcessId;
    GetWindowThreadProcessId(hwnd, &windowProcessId);
    if (windowProcessId == processId)
    {
        consoleH = hwnd;
        return false;
    }
    return true;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    //t1 = std::thread(consoleTest);

    ofstream fout("out.txt");

    fout << "a0a0";

    fout.close();

    Sleep(100);

    startConsole();

    processId = GetProcessId(pi.hProcess);

    std::cout<<EnumWindows(EnumWindowsProc, 0);

    Sleep(100);

    MessageBox(NULL, (LPCTSTR)"Please note that when you press start, make sure that nothing covers the chessboard. Enjoy!", TEXT("Keep in Mind!!!"), MB_OK);

    ShowWindow((HWND)pi.hProcess, SW_MINIMIZE);

    // Initialize global strings
    if (LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0)
    {
        displayError();
    }
    if (LoadStringW(hInstance, IDC_CLOSEDFISH, szWindowClass, MAX_LOADSTRING) == 0)
    {
        displayError();
    }
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        displayError();
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLOSEDFISH));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLOSEDFISH));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLOSEDFISH);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable
    HWND hWnd = CreateWindowExW(0L, (LPCWSTR)szWindowClass, (LPCWSTR)szTitle, (DWORD)(WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU), //removing thickframe makes the window not resizeable
        GetSystemMetrics(SM_CXSCREEN) * 0.05, GetSystemMetrics(SM_CYSCREEN) * 0.05, 350, 250,
        nullptr, nullptr, hInstance, nullptr);

    //Get info about the main window
    //_CONSOLE_SCREEN_BUFFER_INFO scrInf;
    //GetConsoleScreenBufferInfo(hWnd, &scrInf);

    //CREATE Start button
    HWND hwndStartButton = CreateWindow(
        (LPCSTR)"BUTTON", (LPCSTR)"Start",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT,
        245, 156,
        80, 30,
        hWnd,
        NULL,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    DisplayTransparent(hInstance, nCmdShow);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            TerminateProcess(pi.hProcess, 1);
            CloseHandle(consoleH);
            TerminateProcess(GetCurrentProcess(), 0);
            break;
        case BN_CLICKED://the one button was clicked
        {
            if (started == false) {
                ShowWindow((HWND)GetCurrentProcess(), SW_MINIMIZE);
                std::thread t([&]() {
                    bc = new bmpClass(true);
                    //bc.send_input("a6e2");
                    tmrCheckMove.delay = 2000;
                    timers::moveDetection(tmrCheckMove, &bc, consoleH);
                    std::cout << "Cox";
                });
                t.detach();
                started = true;
            }

        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(consoleH);
        TerminateProcess(GetCurrentProcess(), 0);
        WndProcT(hWndTrans, WM_DESTROY, NULL, NULL);
        FreeConsole();
        //t1.join();
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(consoleH);
        TerminateProcess(GetCurrentProcess(), 0);
        DestroyWindow(hWnd);
        DestroyWindow(hWndTrans);
        FreeConsole();
        //t1.join();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//
//  FUNCTION: WndProcT(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the transparent window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProcT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_EXIT:
            TerminateProcess(pi.hProcess, 1);
            CloseHandle(consoleH);
            TerminateProcess(GetCurrentProcess(), 0);
            tmrTransOnTop.shutdown = true;
            FreeConsole();
            t1.join();
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        //std::cout << "paint\n";
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // TODO: Add any drawing code that uses hdc here...
        RECT rect;
        GetClientRect(hWnd, &rect);

        //The order of drawing is important!!!

        // Draw a transparent rectangle on the window
        SetBkMode(hdc, TRANSPARENT); // set the background mode to transparent

        HBRUSH hBrush2 = CreateSolidBrush(RGB(28, 28, 28)); //create a brush
        SelectObject(hdc, hBrush2); //select the brush
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom); //Draw the rectangle
        DeleteObject(hBrush2); //delete the brush

        HBRUSH hBrush = CreateSolidBrush(RGB(86, 150, 118)); //create a brush
        SelectObject(hdc, hBrush); //select the brush
        Rectangle(hdc, rect.left + 53, rect.top + 26, rect.right - 53, rect.bottom - 26); //Draw the rectangle
        DeleteObject(hBrush); //delete the brush

        //Adding the status text
        SetBkMode(hdc, OPAQUE);

        HFONT hFont = CreateFontW(0, 0, 0, 0, 600,
            TRUE, FALSE, FALSE, EASTEUROPE_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
            , PROOF_QUALITY, VARIABLE_PITCH | FF_MODERN, (LPCWSTR)L"Courier New");

        SelectObject(hdc, hFont);
        //SetBkColor(hdc, RGB(86, 150, 118));
        SetBkColor(hdc, RGB(86, 150, 118));
        SetTextColor(hdc, RGB(1, 1, 5));
        DrawTextA(hdc, curStatus, -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        DeleteObject(hFont);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(consoleH);
        TerminateProcess(GetCurrentProcess(), 0);
        tmrTransOnTop.shutdown = true;
        DestroyWindow(hWnd);
        break;
    case WM_CLOSE:
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(consoleH);
        TerminateProcess(GetCurrentProcess(), 0);
        tmrTransOnTop.shutdown = true;
        DestroyWindow(hWnd);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
